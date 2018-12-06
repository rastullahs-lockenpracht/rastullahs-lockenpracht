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
#include "stdinc.h" //precompiled header

#include "AbstractMovement.h"
#include "Actor.h"
#include "Creature.h"
#include "DsaManager.h"
#include "MeshAnimation.h"
#include "MeshObject.h"
#include "CreatureController.h"
#include "CreatureControllerManager.h"
#include "PhysicsManager.h"
#include "PhysicalThing.h"
#include "TimeSource.h"
#include "GameObjectManager.h"
#include "RulesMessages.h"

// included Movements
#include "AbstractMovement.h"
#include "FallDownMovement.h"
#include "JogBackwardsMovement.h"
#include "JogFastMovement.h"
#include "JogMovement.h"
#include "JumpHighMovement.h"
#include "JumpLongMovement.h"
#include "JumpToTargetMovement.h"
#include "LieDownMovement.h"
#include "RotateMovement.h"
#include "SneakMovement.h"
#include "SprintMovement.h"
#include "StandAroundMovement.h"
#include "StepRecognitionMovement.h"
#include "StrafeMovement.h"
#include "WalkBackwardsMovement.h"
#include "WalkMovement.h"


using namespace Ogre;
using namespace std;

namespace rl
{
    const Ogre::String CreatureController::PROPERTY_CREATUREID = "creature_id";


    CreatureController::CreatureController(Creature *creature) :
        mCreature(creature),
        mAbstractLocation(AL_AIRBORNE),
        mMovement(NULL),
        mLastMovementType(MT_NONE),
        mLastMovementChange(0),
        mDirection(Vector3::ZERO),
        mRotation(Vector3::ZERO),
        mLastAnimationName(""),
        mLastCollisionName(""),
        mLastAnimationSpeed(1),
        mStillWeightedAnimationName(""),
        mLastFloorContact(TimeSourceManager::getSingleton().getTimeSource(TimeSource::REALTIME_INTERRUPTABLE)->getClock()),
        mLastFloorContact2(mLastFloorContact)
    {
        mGameObjectId = mCreature->getId();

        mOldMaterialId = mCreature->getActor()->getPhysicalThing()->_getBody()->getMaterialGroupID();
        const OgreNewt::MaterialID *material = PhysicsManager::getSingleton().getMaterialID("character");
        mCreature->getActor()->getPhysicalThing()->setMaterialID(material);

        mCreature->getActor()->getPhysicalThing()->setPhysicsController(this);

        //mCreature->getActor()->getPhysicalThing()->_getBody()->setAngularDamping(0*Vector3::UNIT_SCALE);
        //mCreature->getActor()->getPhysicalThing()->_getBody()->setContinuousCollisionMode(1);


        std::pair<MovementType, AbstractMovement*> movementPair;
        movementPair.first = MT_NONE;
        movementPair.second = NULL;
        mMovementMap.insert(movementPair);
        movementPair.first = MT_LIEGEN;
        movementPair.second = new LieDownMovement (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_FALLEN;
        movementPair.second = new FallDownMovement (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_DREHEN;
        movementPair.second = new RotateMovement (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_STEHEN;
        movementPair.second = new StandAroundMovement (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_GEHEN;
        movementPair.second = new WalkMovement (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_JOGGEN;
        movementPair.second = new JogMovement (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_LAUFEN;
        movementPair.second = new JogFastMovement (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_RENNEN;
        movementPair.second = new SprintMovement (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_RUECKWAERTS_GEHEN;
        movementPair.second = new WalkBackwardsMovement (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_RUECKWAERTS_JOGGEN;
        movementPair.second = new JogBackwardsMovement (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_SEITWAERTS_GEHEN;
        movementPair.second = new StrafeMovement (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_SCHLEICHEN;
        movementPair.second = new SneakMovement (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_HOCHSPRUNG;
        movementPair.second = new JumpHighMovement (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_WEITSPRUNG;
        movementPair.second = new JumpLongMovement (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_ZIELSPRUNG;
        movementPair.second = new JumpToTargetMovement (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_STUFENERKENNUNG;
        movementPair.second = new StepRecognitionMovement (this);
        mMovementMap.insert(movementPair);

        mMessageType_GameObjectsLoaded_Handler = MessagePump::getSingleton().addMessageHandler<MessageType_GameObjectsLoaded>(
                boost::bind(&CreatureController::refetchCreature, this));

        LOG_DEBUG("CreatureController", "Added CreatureController for " + creature->getName());
    }

    CreatureController::~CreatureController()
    {
        MovementMap::iterator iter;
        for(iter = mMovementMap.begin(); iter != mMovementMap.end(); iter++)
            delete iter->second;
        mMovementMap.erase(mMovementMap.begin(), mMovementMap.end());

        if( mCreature->getActor() != NULL )
        {
            if( mCreature->getActor()->getPhysicalThing() != NULL )
            {
                mCreature->getActor()->getPhysicalThing()->setPhysicsController(NULL);

                mCreature->getActor()->getPhysicalThing()->setMaterialID(mOldMaterialId);
        
//                mCreature->getActor()->getPhysicalThing()->_getBody()->setAngularDamping(0.1*Vector3::UNIT_SCALE);
            }
        }
    }

    CreatureController::MovementType CreatureController::getMovementId() const
    {
        if (mMovement == NULL)
            return MT_NONE;
        else
            return mMovement->getId();
    }

    int CreatureController::getCurrentGS() const
    {
        int act_gs = mCreature->getWert(Creature::WERT_GS);
        ///@todo wy does this not work
        act_gs -= mCreature->getWert(Creature::WERT_BE);
        return max(act_gs,1);
    }

    Ogre::Real CreatureController::getMaximumSpeed() const
    {
        return getCurrentGS() * 1.2; // GS = m/KR
    }

    MeshAnimation *CreatureController::setAnimation(const Ogre::String &name, Ogre::Real speed, unsigned int timesToPlay, const Ogre::String &collisionName, Real weight)
    {
        MeshObject* mesh = dynamic_cast<MeshObject*>(mCreature->getActor()->getControlledObject());
        PhysicalThing* pt = mCreature->getActor()->getPhysicalThing();
        MeshAnimation *meshAnim(NULL);

        if( mLastAnimationName != name)
        {
            const Ogre::String *pCollisionName = &collisionName;
            if( collisionName == "" )
                pCollisionName = &name;

            if( *pCollisionName != mLastCollisionName )
            {
                pt->fitToPose(*pCollisionName);
                mLastCollisionName = *pCollisionName;
            }


            if ( mStillWeightedAnimationName != "" )
            {
                mesh->stopAnimation(mStillWeightedAnimationName);
                mStillWeightedAnimationName = "";
            }


            if( mesh->hasAnimation(mLastAnimationName) )
            {
                if ( weight == 1 )
                    mesh->stopAnimation(mLastAnimationName);
                else
                {
                    MeshAnimation *lastAnimation = mesh->getAnimation(mLastAnimationName);
                    if( lastAnimation != NULL )
                    {
                        mStillWeightedAnimationName = mLastAnimationName;
                        lastAnimation->setWeight(1-weight);
                    }
                }
            }

            meshAnim = mesh->startAnimation(name, speed, timesToPlay);
            if( meshAnim )
            {
                meshAnim->setWeight(weight);
                meshAnim->setPaused(false);
                mLastAnimationName = name;
                mLastAnimationSpeed = speed;
            }
        }
        else
        {
            meshAnim = mesh->getAnimation(name);
            if( mLastAnimationSpeed != speed )
            {
                meshAnim->setSpeed(speed);
                meshAnim->setPaused(false);
                mLastAnimationSpeed = speed;
                meshAnim->setWeight(weight);
            }
            if( mStillWeightedAnimationName != "" )
            {
                MeshAnimation *lastAnimation = mesh->getAnimation(mStillWeightedAnimationName);
                if( lastAnimation !=  NULL )
                {
                    lastAnimation->setWeight(1-weight);
                    if( weight == 1 )
                    {
                        mesh->stopAnimation(mStillWeightedAnimationName);
                        mStillWeightedAnimationName = "";
                    }
                }
                else
                {
                    mStillWeightedAnimationName = "";
                }
            }
        }

        LOG_DEBUG("CreatureController", "Set animation " + name + " at " + mCreature->getName());

        return meshAnim;
    }

    Ogre::Real CreatureController::getAnimationTimePlayed() const
    {
        MeshObject* mesh = dynamic_cast<MeshObject*>(mCreature->getActor()->getControlledObject());
        MeshAnimation *meshAnim = mesh->getAnimation(mLastAnimationName);
        if( meshAnim != NULL )
            return meshAnim->getTimePlayed()/meshAnim->getLength();
        return 0;
    }

    Ogre::Vector3 CreatureController::getVelocity() const
    {
        return
            mCreature->getActor()->getPhysicalThing()->getOrientation().Inverse() *
            mCreature->getActor()->getPhysicalThing()->_getBody()->getVelocity();
    }

    Ogre::Vector3 CreatureController::getOmega() const
    {
        return mCreature->getActor()->getPhysicalThing()->_getBody()->getOmega();
    }

    bool CreatureController::run(Real elapsedTime)
    {
        Time timeSinceLastFloorContact = 
            TimeSourceManager::getSingleton().getTimeSource(TimeSource::REALTIME_INTERRUPTABLE)->getClock()
            - mLastFloorContact;
        Time timeSinceLastFloorContact2 = 
            TimeSourceManager::getSingleton().getTimeSource(TimeSource::REALTIME_INTERRUPTABLE)->getClock()
            - mLastFloorContact2;
        Vector3 speed = getVelocity();
        if( getAbstractLocation() == AL_AIRBORNE )
        {
            // find a reason why we now are AL_FLOOR
            if( timeSinceLastFloorContact < Time(Date::ONE_SECOND)*0.19 &&
                abs(speed.y) < 0.6 )
            {
                setAbstractLocation(AL_FLOOR);
            }
        }
        else
        {
            // find a reason why we now are AL_AIRBORNE
            if( timeSinceLastFloorContact >= Time(Date::ONE_SECOND)*0.2 && // 2 seconds?
                abs(speed.y) > 0.6 )
            {
                //std::ostringstream oss;
                //oss << "Raycast for floor: speed.y: " << speed.y << "      timeSinceLastFloorContact: " << timeSinceLastFloorContact;



		// find the distance to the floor:
                // raycasts
                PhysicsMaterialRaycast::MaterialVector materialVector;
                materialVector.push_back(PhysicsManager::getSingleton().getMaterialID("camera")); // should we perhaps only use level here?
                materialVector.push_back(PhysicsManager::getSingleton().getMaterialID("character"));

                Vector3 start = getCreature()->getPosition();
                Vector3 end = start + Vector3::NEGATIVE_UNIT_Y*0.4;

                RaycastInfo info = mRaycast.execute(
                            PhysicsManager::getSingleton()._getNewtonWorld(),
                            &materialVector,
                            start, end, false);

		if( info.mBody == NULL )
		{
		    //oss << "   Now AL_AIRBORNE!";
		    setAbstractLocation(AL_AIRBORNE);
		}
		else
		{
		    //oss << "   floor to near, stay AL_FLOOR!";
		}

                //LOG_MESSAGE(Logger::RULES, oss.str());
            }
        }

        if(mMovement != NULL)
        {
            mLastMovementChange += elapsedTime;
            if( !mMovement->isPossible() )
            {
                setMovement(mMovement->getFallBackMovement(), mDirection, mRotation);
                if( mMovement == NULL )
                    return false;
            }

            Vector3 rotation(mRotation);
            mRotation = Vector3::ZERO;
            return mMovement->run(elapsedTime, mDirection, rotation);
        }

        return false;
    }

    void CreatureController::OnApplyForceAndTorque(PhysicalThing* thing, float timestep)
    {
        Vector3 force, torque;
        OgreNewt::Body *body = thing->_getBody();
        force = Vector3::ZERO;
        torque = Vector3::ZERO;
        OgreNewt::World *world = PhysicsManager::getSingleton()._getNewtonWorld();
        Real mass;
        Vector3 inertia;
        body->getMassMatrix(mass, inertia);

        if(mMovement != NULL)
        {
            mMovement->calculateForceAndTorque(force, torque, timestep);
            force = thing->getOrientation() * force;
        }
        else
        {
            // don't move
            force = - mass * body->getVelocity() / 1 * timestep;
            force.y = 0;
        }
        force += mass * PhysicsManager::getSingleton().getGravity();
        body->setForce(force);
        body->setTorque(torque);
    }

    void CreatureController::userProcess(OgreNewt::ContactJoint &contactJoint, Real timestep, int threadid)
    {
for(OgreNewt::Contact contact = contactJoint.getFirstContact(); contact; contact = contact.getNext() )
{
        // own collision handling (floor, in order to get information for mAbstractLocation)
        OgreNewt::Body* charBody = mCreature->getActor()->getPhysicalThing()->_getBody();

        Vector3 point;
        Vector3 normal;
        contact.getPositionAndNormal(charBody, point, normal);

        // determine if this contact is with the floor.
        // Meaning the contact normal has an angle to UNIT_Y of 20 or less.
        Degree angle = Math::ACos(normal.dotProduct(Vector3::UNIT_Y));

        Vector3 charPos;
        Quaternion charOri;
        charBody->getPositionOrientation(charPos, charOri);
        bool isFloorCollision(false);

        AxisAlignedBox charAab = charBody->getCollision()->getAABB();
        Real charHeight = charAab.getMaximum().y - charAab.getMinimum().y;
        Real stepHeight = point.y - charPos.y;

        if( stepHeight < charHeight/2 )
            isFloorCollision = true;

        if ( isFloorCollision )
        {
            Time time = TimeSourceManager::getSingleton().getTimeSource(TimeSource::REALTIME_INTERRUPTABLE)->getClock();
            if( time != mLastFloorContact )
            {
                mLastFloorContact2 = mLastFloorContact;
                mLastFloorContact = time;
            }
        }

        // set contact direction to point to the center of mass
        contact.setNormalDirection((charPos + charBody->getCenterOfMass() - point).normalisedCopy());
        contact.setNormalAcceleration(0);
        // no friction
        contact.setFrictionState(0,0);
        contact.setFrictionState(0,1);
        contact.setTangentAcceleration(0,0);
        contact.setTangentAcceleration(0,1);


/*
//        setContactNormalDirection(((Vector3::UNIT_Y.dotProduct(point-charPos)*Vector3::UNIT_Y + charPos) - point).normalisedCopy());
        setContactNormalDirection(point - (charPos + charHeight/2));
        setContactNormalAcceleration(0);
        setContactFrictionState(0,0);
        setContactFrictionState(0,1);
        setContactTangentAcceleration(0, 0);
        setContactTangentAcceleration(0, 1);
*/

//std::ostringstream oss;
//Vector3 vec1, vec2;
//oss << " Collision: Point: " << point-charPos;
//    << "  \t Normal: " << normal
//    << "  \t Force: " << getContactForce()
//    << "  \t Normal-Speed: " << getContactNormalSpeed()
//    << "  \t Contact-Speed: " << point
//    << "  \t Contact-Normal: " << normal;
//getContactTangentDirections(vec1, vec2);
//oss << "  \t Tangent-Directions: " << vec1 << " " << vec2;
//LOG_MESSAGE(Logger::RULES, oss.str());

//        contact.rotateTangentDirections(/*charOri*mDirection + */Vector3::UNIT_Y);
//        contact.setFrictionState(0,0);
//        contact.setFrictionState(0,1);
/*
        if( stepHeight < 0.4 )
        {
            if(stepHeight > 0.01f) // experimantal value, 
                                   // too low means the creature glides upwards on inclined planes, 
                                   // too high means the creature stops if moving slowly onto a step because of the friction
            {
                //setContactNormalAcceleration(5);
                contact.setTangentAcceleration(5,0);
                //setContactFrictionState(1,0);
                //setContactFrictionState(1,1);
            }
            else
            {
                //setContactFrictionState(1,0);
                //setContactFrictionState(1,1);
            }
            //setContactElasticity(0.0f);
        }
        else
        {
            Vector3 vel; // prevent going upwards if running against small things
            vel = mCreature->getActor()->getPhysicalThing()->getVelocity();
            if( vel.y >= 0 )
            {
                //setContactFrictionState(1,0);
                //setContactFrictionState(1,1);    
            }
            else
            {
                //setContactFrictionState(0,0);
                //setContactFrictionState(0,1);
            }
        }
*/
}

        if(mMovement != NULL)
        {
            // give the movement a chance to modify the contact
            mMovement->userProcess(contactJoint, timestep, threadid);
        }
    }

    AbstractMovement *CreatureController::getMovementFromId(CreatureController::MovementType id)
    {
        MovementMap::iterator iter = mMovementMap.find(id);
        if(iter == mMovementMap.end())
        {
            Throw(IllegalArgumentException, "Der angegebene Bewegungstyp wurde nicht gefunden.");
        }
        return iter->second;
    }


    bool CreatureController::setMovement(MovementType type, Vector3 direction, Vector3 rotation)
    {
        if( mMovement != NULL )
        {
            if( mMovement->getId() == type )
            {
                mDirection = direction;
                mRotation = rotation;
                return true;
            }
        }

        if( mMovement != NULL )
        {
            if( !mMovement->canChangeToMovement(type) )
            {
                mMovement->requestChangeToMovement(type);
                return false;
            }
        }

        AbstractMovement *movement = getMovementFromId(type);

        while(movement != NULL)
        {
            if(movement->isPossible())
            {
                if( movement != mMovement )
                {
                    // runs the old movement if idle!
                    if(mMovement == NULL)
                    {
                        mLastMovementType = MT_NONE;
                    }
                    else
                    {
                        mLastMovementType = mMovement->getId();
                        mMovement->deactivate();
                    }
                    mMovement = movement;
                    mMovement->activate();
                    mLastMovementChange = 0;
                }
                mDirection = direction;
                mRotation = rotation;
                
                return true;
            }

            movement = getMovementFromId(movement->getFallBackMovement());
        }

        if (!movement)
        {
            mMovement = NULL;
        }

        return false;
    }



    Ogre::Radian CreatureController::getYaw()
    {
        Radian yaw = mCreature->getActor()->getWorldOrientation().getYaw();

        AbstractMovement *drehen = getMovementFromId(MT_DREHEN);

        if( drehen == NULL )
            return yaw;

        if( !drehen->isActive() )
            return yaw;

        return Radian(drehen->getMovementDefinedValue());
    }

    bool CreatureController::refetchCreature()
    {
        mCreature = static_cast<Creature*>(GameObjectManager::getSingleton().getGameObject(mGameObjectId));

        //mOldMaterialId = mCreature->getActor()->getPhysicalThing()->_getBody()->getMaterialGroupID();

        const OgreNewt::MaterialID *material = PhysicsManager::getSingleton().getMaterialID("character");
        mCreature->getActor()->getPhysicalThing()->setMaterialID(material);

        mCreature->getActor()->getPhysicalThing()->setPhysicsController(this);

        return false;
    }

    void CreatureController::setAbstractLocation(CreatureController::AbstractLocation location)
    {
        if( mAbstractLocation != location )
        {
            if( location == AL_AIRBORNE )
            {
                LOG_MESSAGE(Logger::RULES, "CreatureController: Now AL_AIRBORNE");
            }
            else
            {
                LOG_MESSAGE(Logger::RULES, "CreatureController: Now AL_FLOOR");
            }
        }
	mAbstractLocation = location;
    }
}

