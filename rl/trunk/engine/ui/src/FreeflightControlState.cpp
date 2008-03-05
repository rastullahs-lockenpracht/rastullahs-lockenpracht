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

#include "FreeflightControlState.h"

#include "AbstractWindow.h"
#include "CoreSubsystem.h"
#include "ConfigurationManager.h"
#include "Exception.h"
#include "Actor.h"
#include "PhysicalThing.h"
#include "World.h"
#include "CameraObject.h"
#include "InputManager.h"
#include "CommandMapper.h"
#include "MeshObject.h"
#include "WindowManager.h"
#include "Person.h"


using namespace Ogre;

namespace rl {

	FreeflightControlState::FreeflightControlState(CommandMapper* cmdMapper,
        Actor* camera, Person* character)
		: ControlState(cmdMapper, camera, character, CST_FREEFLIGHT),
		mMovementSpeed(5.0f),
		mSpeedRange(0.03f, 90.0f),
		mSpeedIncrement(0.02f),
		mRotationSpeed(4.0f),
		mCurrentMovementState(0),
        mDesiredVelocity(Vector3::ZERO),
        mCollisionsEnabled(false),
        mPitchRange(Degree(-89), Degree(89)),
        mYaw(Degree(0)),
        mPitch(Degree(0))
	{
        mMouseSensitivity = ConfigurationManager::getSingleton().getIntSetting("Input", "Mouse Sensitivity");
        mInvertedMouse = ConfigurationManager::getSingleton().getBoolSetting("Input", "Mouse Invert");
	}

	FreeflightControlState::~FreeflightControlState()
	{
	}

    void FreeflightControlState::pause()
    {
		mCameraActor->getPhysicalThing()->freeze();
		mCharacterActor->getPhysicalThing()->unfreeze();
        mCameraActor->getPhysicalThing()->setPhysicsController(NULL);

        // Char<->Level collision back to default
        PhysicsManager::getSingleton().resetMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("default"));
        // Char<->Default collision back to default
        PhysicsManager::getSingleton().resetMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("level"));
        PhysicsManager::getSingleton().resetMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("character"));
    }

    void FreeflightControlState::resume()
    {
        mCameraActor->getPhysicalThing()->unfreeze();
		mCharacterActor->getPhysicalThing()->freeze();

        resetCamera();

        // The actor should be controlled manually,
        // so let the PM prepare it accordingly
        mCameraActor->getPhysicalThing()->setPhysicsController(this);
        mCameraActor->getPhysicalThing()->setUpConstraint(Vector3::ZERO);
        // We also handle camera<->level, camera<->default and camera<->char collision from now on!
        // we need continous collision mode for fast people :-P
        OgreNewt::MaterialPair *mat_pair;
        mat_pair = PhysicsManager::getSingleton().createMaterialPair(
                    PhysicsManager::getSingleton().getMaterialID("camera"),
                    PhysicsManager::getSingleton().getMaterialID("default"));
        mat_pair->setContactCallback(this);
        mat_pair->setContinuousCollisionMode(1);
        mat_pair->setDefaultCollidable(1);
        mat_pair = PhysicsManager::getSingleton().createMaterialPair(
                    PhysicsManager::getSingleton().getMaterialID("camera"),
                    PhysicsManager::getSingleton().getMaterialID("level"));
        mat_pair->setContactCallback(this);
        mat_pair->setContinuousCollisionMode(1);
        mat_pair->setDefaultCollidable(1);
        mat_pair = PhysicsManager::getSingleton().createMaterialPair(
                    PhysicsManager::getSingleton().getMaterialID("camera"),
                    PhysicsManager::getSingleton().getMaterialID("character"));
        mat_pair->setContactCallback(this);
        mat_pair->setContinuousCollisionMode(1);
        mat_pair->setDefaultCollidable(1);
    }

	void FreeflightControlState::run(Real elapsedTime)
	{
        InputManager* im = InputManager::getSingletonPtr();

		// Fetch current movement state
		mDesiredVelocity = Vector3::ZERO;

		int movement = mCurrentMovementState;

		// Determine character's control state based on user input
		if (movement & MOVE_FORWARD)
        {
			mDesiredVelocity.z = -mMovementSpeed;
        }

		if (movement & MOVE_BACKWARD)
        {
            mDesiredVelocity.z = mMovementSpeed;
        }

		if (movement & MOVE_RIGHT)
        {
			mDesiredVelocity.x = mMovementSpeed;
        }

		if (movement & MOVE_LEFT)
        {
			mDesiredVelocity.x = -mMovementSpeed;
        }

		if (movement & MOVE_RUN)
        {
			mDesiredVelocity *= 10.0;
        }

        if (movement & MOVE_JUMP)
        {
            // put character here
            if( mCharacterActor != NULL )
            {
                mCharacterActor->setPosition(
                    mCameraActor->getPosition()
                    + mCameraActor->getWorldOrientation() * Vector3::NEGATIVE_UNIT_Z * 2
                    - 1.5 * Vector3::UNIT_Y);
            }
        }

        bool useMouseInput = !isMouseUsedByCegui();

        if( useMouseInput )
		    mMovementSpeed += im->getMouseRelativeZ() * mSpeedIncrement;
		if (mMovementSpeed < mSpeedRange.first)
		{
			mMovementSpeed = mSpeedRange.first;
		}
		if (mMovementSpeed > mSpeedRange.second)
		{
			mMovementSpeed = mSpeedRange.second;
		}


        if (movement & TURN_LEFT)
            mYaw += elapsedTime * Degree(120.0f);
        if (movement & TURN_RIGHT)
            mYaw -= elapsedTime * Degree(120.0f);

        // mouse
        if( !(movement & TURN_LEFT || movement & TURN_RIGHT) )
        {
            if( useMouseInput )
                mYaw -= mMouseSensitivity * Degree(im->getMouseRelativeX() / 10);
        }
        while (mYaw.valueDegrees() > 360.0f) mYaw -= Degree(360.0f);
        while (mYaw.valueDegrees() < -360.0f) mYaw += Degree(360.0f);


        if (useMouseInput )
        {
            if (mInvertedMouse)
                mPitch += mMouseSensitivity * Degree(im->getMouseRelativeY() / 4);
            else
                mPitch -= mMouseSensitivity * Degree(im->getMouseRelativeY() / 4);
        }

        while (mPitch.valueDegrees() > 360.0f) mPitch -= Degree(360.0f);
        while (mPitch.valueDegrees() < -360.0f) mPitch += Degree(360.0f);
        if (mPitch < mPitchRange.first) mPitch = mPitchRange.first;
        if (mPitch > mPitchRange.second) mPitch = mPitchRange.second;

        mCameraActor->setOrientation(Quaternion::IDENTITY);
        mCameraActor->yaw(mYaw.valueDegrees());
        mCameraActor->pitch(mPitch.valueDegrees());
    }

	void FreeflightControlState::toggleCameraCollision()
	{
		// with or without collision?
        // be careful to enable collision if beeing in another collision
        mCollisionsEnabled = !mCollisionsEnabled;
	}

	void FreeflightControlState::resetCamera()
	{
		// Position camera at char position
        if( mCharacterActor != NULL )
        {
            mCameraActor->setOrientation(Quaternion::IDENTITY);
            Vector3 newPos = mCharacterActor->getWorldPosition();
            if( mCharacterActor->getControlledObject()->isMeshObject() )
            {
                MeshObject* mo = dynamic_cast<MeshObject*>(mCharacterActor->getControlledObject());
                newPos.y += mo->getDefaultSize().getMaximum().y;
            }
		    mCameraActor->_getSceneNode()->setPosition( newPos );
        }
        else
        {
		    mCameraActor->setOrientation( Quaternion::IDENTITY );
            mCameraActor->setPosition( Vector3::ZERO );
        }
        mYaw = Degree(0);
        mPitch = Degree(0);
	}

    bool FreeflightControlState::keyPressed(const OIS::KeyEvent& evt, bool handled)
	{
        bool retval = false;
        if( !handled )
        {
            int movement = mCommandMapper->getMovement(evt.key);

		    if (movement != MOVE_NONE)
		    {
			    mCurrentMovementState |= movement;
			    retval = true;
		    }

            int code = CommandMapper::encodeKey(evt.key, InputManager::getSingleton().getModifierCode());
            // First see, if a control state action is defined
	        CeGuiString command = mCommandMapper->getControlStateAction(code, mType);
            if (command == "")
            {
                // No. So try global actions.
                command = mCommandMapper->getGlobalAction(code);
            }
            else if (command == "back_to_character_movement")
            {
                InputManager::getSingleton().popControlState();
                retval = true;
            }
            else if (command == "toggle_camera_collision" )
            {
                mCollisionsEnabled = !mCollisionsEnabled;
                retval = true;
            }
        }

        if( ControlState::keyPressed(evt, retval || handled) )
            retval = true;
        return retval;
	}

    bool FreeflightControlState::keyReleased(const OIS::KeyEvent& evt, bool handled)
	{
        bool retval = false;
        int movement = mCommandMapper->getMovement(evt.key);
		if (movement != MOVE_NONE)
		{
			mCurrentMovementState &= ~movement;
			retval = true;
		}

        if( ControlState::keyReleased(evt, handled || retval) )
            retval = true;
        return retval;
	}

    int FreeflightControlState::userProcess()
    {
        if (m_body0 == mCamBody || m_body1 == mCamBody)
        {
            // this is camera collision

            if( !mCollisionsEnabled )
                return 0;

            setContactSoftness(1.0f);  // "weiche" Collision
            setContactElasticity(0.0f);

            return 1;
        }

        // return one to tell Newton we want to accept this contact
        return 1;
    }



    void FreeflightControlState::OnApplyForceAndTorque(PhysicalThing* thing)
    {
        OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();
        OgreNewt::Body* body = thing->_getBody();

        // Get the current world timestep
        Real timestep = world->getTimeStep();

        if (body == mCamBody)
        {
            // apply camera force
            Vector3 position;
            Quaternion orientation;
            body->getPositionOrientation(position, orientation);

            // get the camera mass
            Real mass;
            Vector3 inertia;
            body->getMassMatrix(mass, inertia);


            // Get the velocity vector
            Vector3 currentVel = body->getVelocity();
            Real delay = 2 * PhysicsManager::getSingleton().getMaxTimestep();
            Vector3 force = mass*(orientation * mDesiredVelocity - currentVel) / delay;

            body->setForce(force);
        }
    }

}
