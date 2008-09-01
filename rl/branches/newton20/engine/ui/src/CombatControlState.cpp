/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */
#include "stdinc.h"

#include "CombatControlState.h"

#include "Actor.h"
#include "CameraObject.h"
#include "Combat.h"
#include "CombatGui.h"
#include "CombatManager.h"
#include "CommandMapper.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "CreatureController.h"
#include "CreatureControllerManager.h"
#include "InputManager.h"
#include "MeshObject.h"
#include "PhysicalThing.h"
#include "RulesMessages.h"
#include "Selector.h"
#include "UiMessages.h"
#include "World.h"

using namespace Ogre;

namespace rl {
    CombatControlState::CombatControlState(CommandMapper* cmdMapper,
        Actor* camera, Creature* character)
        : ControlState(cmdMapper, camera, character, CST_COMBAT),
          Combatant(CombatManager::getSingleton().startCombat(),
			CreatureControllerManager::getSingleton().getCreatureController(character)),
		  mAttackedOpponent(NULL),
		  mParriedOpponent(NULL),
          mCombatManager(CombatManager::getSingletonPtr()),
		  mCombatGui(NULL),
          mEnemySelector(CoreSubsystem::getSingleton().getWorld()->getSceneManager(),
            QUERYFLAG_CREATURE),
          mCamera(NULL),
		  mState(ROUND_EXECUTION),
          mCameraYaw(0),
          mCameraPitch(60), // the same value as in resetCamera
          mMovementState(0),
          mCameraLinearSpringK(100.0f),
          mCameraLinearDampingK(Math::NEG_INFINITY),
          mMaxCameraDistance(30.0f),
          mMinCameraDistance(5.0f)
    {
        CreatureSelectionFilter* filter = new CreatureSelectionFilter();
        filter->setAlignmentMask(Creature::ALIGNMENT_ENEMY);
        mEnemySelector.setFilter(filter);

        // Message handlers
		mCombatIoAttackOpponentConnection =
            MessagePump::getSingleton().addMessageHandler<MessageType_CombatIoAttackOpponent>(
			    boost::bind(&CombatControlState::userRequestAttackOpponent, this, _1));
		mCombatIoParryOpponentConnection =
            MessagePump::getSingleton().addMessageHandler<MessageType_CombatIoParryOpponent>(
			    boost::bind(&CombatControlState::userRequestParryOpponent, this, _1));
		mCombatIoEndTurnRequestedConnection =
            MessagePump::getSingleton().addMessageHandler<MessageType_CombatIoEndTurnRequested>(
			    boost::bind(&CombatControlState::userRequestEndTurn, this));
		mEnemyLeftCombatConnection =
            MessagePump::getSingleton().addMessageHandler<MessageType_CombatOpponentLeft>(
			    boost::bind(&CombatControlState::enemyLeftCombat, this, _1));
		mCombatEndConnection =
            MessagePump::getSingleton().addMessageHandler<MessageType_CombatEnded>(
			    boost::bind(&CombatControlState::combatEnded, this, _1));

        mCamera = static_cast<CameraObject*>(mCameraActor->getControlledObject());
		mCombatGui = new CombatGui(mCombat, mCamera);


        // calculate camera spring-damping system coefficients
        Real relationCoefficient = 0.8f;
        mCameraLinearDampingK = relationCoefficient * 2.0 * Math::Sqrt(mCameraLinearSpringK);
    }

	CombatControlState::~CombatControlState()
    {
		delete mCombatGui;
        delete mEnemySelector.getFilter();
		CombatManager::getSingleton().stopCombat();
    }

    void CombatControlState::resume()
    {
        // control camera
        mCameraActor->getPhysicalThing()->setMaterialID(
        PhysicsManager::getSingleton().getMaterialID("camera"));
        mCameraActor->getPhysicalThing()->unfreeze();
        mCameraActor->getPhysicalThing()->setPhysicsController(this);
        mCameraActor->getPhysicalThing()->setUpConstraint(Vector3::ZERO);
        
        // We also handle cam<->level, cam<->default cam<->char collision from now on
        OgreNewt::MaterialPair* mat_pair = NULL;
        mat_pair = PhysicsManager::getSingleton().createMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("default"));
        mat_pair->setContactCallback(this);
        mat_pair->setDefaultCollidable(1);
        mat_pair->setDefaultFriction(0,0);
        mat_pair->setDefaultFriction(0,1);
        mat_pair = PhysicsManager::getSingleton().createMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("level"));
        mat_pair->setContactCallback(this);
        mat_pair->setDefaultCollidable(1);
        mat_pair->setDefaultFriction(0,0);
        mat_pair->setDefaultFriction(0,1);
        mat_pair = PhysicsManager::getSingleton().createMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("character"));
        mat_pair->setContactCallback(this);
        mat_pair->setDefaultCollidable(1);
        mat_pair->setDefaultFriction(0,0);
        mat_pair->setDefaultFriction(0,1);

        // reset camera
        //resetCamera(); // if you don't call this here, the camera should smoothly move to the new position -> looks nicer
        // perhaps this should be handled differently!
        mMovementState = 0;


        ///\todo Richtig machen, nur temporär Ani hier setzen.
        static_cast<MeshObject*>(mCharacterActor->getControlledObject())
            ->startAnimation("kampf_schwerter_idle");

        // Set reference to character
        mEnemySelector.setCheckVisibility(true, mCharacter);
        mEnemySelector.track(mCharacter);
        mEnemySelector.setRadius(10.0);

        mEnemySelector.updateSelection();
        const Selector::GameObjectVector& enemies = mEnemySelector.getAllSelectedObjects();
        if (!enemies.empty())
        {
            for (size_t i = 0; i < enemies.size(); ++i)
            {
                mCombat->addOpponent(static_cast<Creature*>(enemies[i]));
            }
        }
        else
        {
            // Oops. Nothing to fight. Pop self.
            InputManager::getSingleton().popControlState();
            return;
        }

		mCombatGui->show();

        // We want to play too..
        mCombat->addAlly(this);

        mCombat->start();
    }

    void CombatControlState::pause()
    {
        mCombatGui->hide();


        // stop controlling camera actor
        mCameraActor->getPhysicalThing()->setPhysicsController(NULL);
        mCameraActor->getPhysicalThing()->freeze();
        // cam<->Level collision back to default
        PhysicsManager::getSingleton().resetMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("default"));
        // cam<->Default collision back to default
        PhysicsManager::getSingleton().resetMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("level"));
        PhysicsManager::getSingleton().resetMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("character"));

        static_cast<MeshObject*>(mCharacterActor->getControlledObject())->stopAllAnimations();

        mCombat->pause();
    }

    bool CombatControlState::keyPressed(const OIS::KeyEvent& evt, bool handled)
    {
        bool retval = false;
        if( !handled )
        {
            int movement = mCommandMapper->getMovement(evt.key);
            mMovementState |= movement;
            
            if( movement != MOVE_NONE )
                retval = true;
        }

        if( ControlState::keyPressed(evt, handled || retval) )
            retval = true;

        return retval;
    }
     
    bool CombatControlState::keyReleased(const OIS::KeyEvent& evt, bool handled)
    {
        bool retval = false;
        int movement = mCommandMapper->getMovement(evt.key);
        if( movement != MOVE_NONE )
        {
            mMovementState &= (~movement);
            retval = true;
        }

        if( ControlState::keyReleased(evt, handled || retval) )
            retval = true;
        return retval;
    }

    void CombatControlState::run(Ogre::Real elapsedTime)
    {
        // update CombatGui
        mCombatGui->update();



        // update camera look-at position
        updateCameraLookAt(elapsedTime);

        if( mMovementState & MOVE_RIGHT )
            mCameraYaw += Degree( 360.0/10.0 * elapsedTime );
        if( mMovementState & MOVE_LEFT )
            mCameraYaw -= Degree( 360.0/10.0 * elapsedTime );

    }

    void CombatControlState::resetCamera(void)
    {
        mCameraYaw = Degree(0);
        mCameraPitch = Degree(50);
        mCameraActor->setPosition(calculateOptimalCameraPosition());
    }

    void CombatControlState::updateCameraLookAt(Real elapsedTime)
    {
        Vector3 combatCenter = getCombatCenterPosition();
        SceneNode* cameraNode = mCameraActor->_getSceneNode();

        cameraNode->lookAt(combatCenter + 2*Vector3::UNIT_Y, Node::TS_WORLD);
    }

    Vector3 CombatControlState::getCombatCenterPosition()
    {
        // get the center of all persons (allies and opponents)
        int n = 0;
        Vector3 pos = Vector3::ZERO;

        const Combat::CombatantSet &allies (mCombat->getAllAllies() );
        for(Combat::CombatantSet::iterator it = allies.begin(); it != allies.end(); it++)
        {
            pos += (*it)->getCreature()->getPosition();
            n++;
        }

        const Combat::CombatantSet &opponents (mCombat->getAllOpponents());
        for(Combat::CombatantSet::iterator it = opponents.begin(); it != opponents.end(); it++)
        {
            pos += (*it)->getCreature()->getPosition();
            n++;
        }

        if( n > 0 )
            pos /= n;
        else
        {
            pos = mCharacter->getPosition();
        }

        return pos;
    }
    
    Vector3 CombatControlState::calculateOptimalCameraPosition()
    {
        Vector3 center = getCombatCenterPosition();

        // get the greatest distance from center from all persons
        Real distance = 0;
        const Combat::CombatantSet &allies (mCombat->getAllAllies() );
        for(Combat::CombatantSet::iterator it = allies.begin(); it != allies.end(); it++)
        {
            distance = std::max( ( (*it)->getCreature()->getPosition() - center ).length(), distance );
        }
        const Combat::CombatantSet &opponents = mCombat->getAllOpponents();
        for(Combat::CombatantSet::iterator it = opponents.begin(); it != opponents.end(); it++)
        {
            distance = std::max( ( (*it)->getCreature()->getPosition() - center ).length(), distance );
        }
        distance+=2;


        // put camera on the line player-center far enough to see all persons
        // if player is "exactly" in the center, use player orientation as fallback orientation
        // + rotation from mCameraYaw
        Vector3 playerPos = mCharacter->getPosition();
        Vector3 diff = playerPos - center;
        diff.y = 0;
        Quaternion camYaw;
        camYaw.FromAngleAxis(mCameraYaw, Vector3::UNIT_Y);
        if( diff.squaredLength() < 0.01 )
        {
            diff = mCharacterActor->getOrientation()*Vector3::UNIT_Z;
        }
        diff.y = 0;
        diff.normalise();
        diff = camYaw*diff;
        diff.y = Math::Tan(mCameraPitch.valueRadians());

        distance = std::max( distance,  mMinCameraDistance*Math::Cos(mCameraPitch.valueRadians()) );
        distance = std::min( distance,  mMaxCameraDistance*Math::Cos(mCameraPitch.valueRadians()) );
            
        Vector3 camPos = center + distance*diff;
        return camPos;
    }

    void CombatControlState::OnApplyForceAndTorque(PhysicalThing* thing, float timestep)
    {
        Vector3 camPos;
        Quaternion camOri;
        mCamBody->getPositionOrientation(camPos, camOri);

        Vector3 optimalCamPos = calculateOptimalCameraPosition();


        Vector3 diff = camPos - optimalCamPos;
        Vector3 cameraVelocity;
        cameraVelocity = mCamBody->getVelocity();
        // spring velocity
        Vector3 springAcc = -mCameraLinearSpringK*diff - mCameraLinearDampingK * cameraVelocity;
                                            
        // get the camera mass
        Real mass;
        Vector3 inertia;
        mCamBody->getMassMatrix(mass, inertia);
                                                
        mCamBody->setForce(springAcc * mass);
    }

    int CombatControlState::onAABBOverlap(int threadIndex)
    {
        // TODO handle camera collisions here
        return 0;
    }
   
    int CombatControlState::userProcess(Ogre::Real timestep, int threadIndex)
    {
        // TODO handle camera collisions here
        return 0;
    }

    Ogre::String CombatControlState::getCombatantTypeName() const
    {
        return "CombatControlState";
    }

    void CombatControlState::requestCombatantAction()
    {
        // Change state to allow user to choose actions for next round.
		mState = REQUEST_USER_INPUT;
		mCombatGui->enableUserInput(true);
    }

	bool CombatControlState::userRequestAttackOpponent(Combatant* opponent)
	{
		mAttackedOpponent = opponent;
		return true;
	}

	bool CombatControlState::userRequestParryOpponent(Combatant* opponent)
	{
		mParriedOpponent = opponent;
		return true;
	}

	bool CombatControlState::userRequestEndTurn()
	{
		// Only handle, if we accept user input now.
		if (mState != REQUEST_USER_INPUT)
		{
			return true;
		}

		// Do we want to attack someone?
		if (mAttackedOpponent)
		{
			// Are we in weapon range to opponent
			if (mCombat->canAttack(this, mAttackedOpponent))
			{
				// Ok, we can attack
				mCombat->registerAttacke(this, mAttackedOpponent);
			}
			else
			{
				// We can't attack from here, so go to opponent.
				mCombat->registerFolgen(this, mAttackedOpponent);
			}
		}
		if (mParriedOpponent)
		{
			mCombat->registerParade(this);
		}

		mCombat->registerCombatantRoundDone(this);

		mState = ROUND_EXECUTION;

		return true;
	}

	bool CombatControlState::enemyLeftCombat(Combatant* opponent)
	{
		// Make sure we don't have any dangling pointers on invalid Combatants.

		if (opponent == mAttackedOpponent)
		{
			mAttackedOpponent = NULL;
		}
		if (opponent == mParriedOpponent)
		{
			mParriedOpponent = NULL;
		}
		return true;
	}

    bool CombatControlState::combatEnded(bool alliesWon)
    {
        ///@todo some feedback, Abenteuerpunkte, anything? 
        InputManager::getSingleton().popControlState();
        return true;
    }
}
