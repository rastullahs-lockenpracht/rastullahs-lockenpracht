/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Perl Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Perl Artistic License for more details.
*
*  You should have received a copy of the Perl Artistic License
*  along with this program; if not you can get it here
*  http://www.perldoc.com/perl5.6/Artistic.html.
*/

#include "FreeFlightCharacterController.h"
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

#include <OgreSceneManager.h>

using namespace Ogre;

namespace rl {

	FreeFlightCharacterController::FreeFlightCharacterController(Actor* camera, Actor* character)
		: CharacterController(camera, character),
		mMovementSpeed(5.0f),
		mSpeedRange(0.03f, 90.0f),
		mSpeedIncrement(0.02f),
		mRotationSpeed(4.0f),
		mCurrentMovementState(0),
        mDesiredVelocity(Vector3::ZERO),
        mCollisionsEnabled(false),
        mPitchRange(Degree(-89), Degree(89)),
        mCameraUpConstraint(Vector3::ZERO)
	{
		mCameraActor->getPhysicalThing()->freeze();
        mCameraUpConstraint = mCameraActor->getPhysicalThing()->getUpConstraint();
		mCharacterActor->getPhysicalThing()->freeze();
        mMouseSensitivity = ConfigurationManager::getSingleton().getIntSetting(ConfigurationManager::CS_INPUT, "Mouse Sensitivity");
        mInvertedMouse = ConfigurationManager::getSingleton().getBoolSetting(ConfigurationManager::CS_INPUT, "Mouse Invert");


		resetCamera();


        // The actor should be controlled manually,
        // so let the PM prepare it accordingly
        PhysicsManager::getSingleton().setPhysicsController(
            mCameraActor->getPhysicalThing(), this);
        // We also handle char<->level, char<->default collision from now on (camera=char!)
        PhysicsManager::getSingleton().setCharLevelContactCallback(this);
        PhysicsManager::getSingleton().setCharDefaultContactCallback(this);

		MeshObject* mesh = dynamic_cast<MeshObject*>(mCharacterActor->getControlledObject());
        if( mesh != NULL )
        {
		    mesh->stopAllAnimations();
		    mesh->startAnimation("idle");
        }
	}

	FreeFlightCharacterController::~FreeFlightCharacterController()
	{
		mCameraActor->getPhysicalThing()->unfreeze();
        if(mCameraUpConstraint != Vector3::ZERO)
            mCameraActor->getPhysicalThing()->setUpConstraint(mCameraUpConstraint);
		mCharacterActor->getPhysicalThing()->unfreeze();
	}

	CharacterController::ControllerType FreeFlightCharacterController::getType() const
	{
		return CTRL_FREEFLIGHT;
	}

	void FreeFlightCharacterController::run(Real elapsedTime)
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

		mMovementSpeed += im->getMouseRelativeZ() * mSpeedIncrement;
		if (mMovementSpeed < mSpeedRange.first)
		{
			mMovementSpeed = mSpeedRange.first;
		}
		if (mMovementSpeed > mSpeedRange.second)
		{
			mMovementSpeed = mSpeedRange.second;
		}


        Radian yaw;
        if (movement & TURN_LEFT)
            yaw = elapsedTime * Degree(120.0f);
        if (movement & TURN_RIGHT)
            yaw = -elapsedTime * Degree(120.0f);

        // mouse
        if( !(movement & TURN_LEFT || movement & TURN_RIGHT) )
        {
            yaw = -mMouseSensitivity * Degree(im->getMouseRelativeX() / 10);
        }
        while (yaw.valueDegrees() > 360.0f) yaw -= Degree(360.0f);
        while (yaw.valueDegrees() < -360.0f) yaw += Degree(360.0f);

        Radian pitch;
        if (mInvertedMouse)
            pitch = mMouseSensitivity * Degree(im->getMouseRelativeY() / 4);
        else
            pitch = -mMouseSensitivity * Degree(im->getMouseRelativeY() / 4);

        while (pitch.valueDegrees() > 360.0f) pitch -= Degree(360.0f);
        while (pitch.valueDegrees() < -360.0f) pitch += Degree(360.0f);
        if (pitch < mPitchRange.first) pitch = mPitchRange.first;
        if (pitch > mPitchRange.second) pitch = mPitchRange.second;

        mCameraActor->getPhysicalThing()->clearUpConstraint();
        mCameraActor->yaw(yaw.valueDegrees());
        mCameraActor->pitch(pitch.valueDegrees());
    }

	void FreeFlightCharacterController::toggleViewMode()
	{
		// with or without collision?
        // be careful to enable collision if beeing in another collision
        mCollisionsEnabled = !mCollisionsEnabled;
	}

	void FreeFlightCharacterController::resetCamera()
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
	}

	bool FreeFlightCharacterController::injectKeyDown(int keycode)
	{
        int scancode;
        mCommandMapper->decodeKey(keycode, &scancode, NULL);
        int movement = mCommandMapper->getMovement(scancode);


		if (movement != MOVE_NONE)
		{
			mCurrentMovementState |= movement;
			return true;
		}
		return false;
	}

	bool FreeFlightCharacterController::injectKeyUp(int keycode)
	{
        int scancode;
        mCommandMapper->decodeKey(keycode, &scancode, NULL);
        int movement = mCommandMapper->getMovement(scancode);

		if (movement != MOVE_NONE)
		{
			mCurrentMovementState &= ~movement;
			return true;
		}
        else
        {
            startAction(mCommandMapper->getAction(keycode, CMDMAP_KEYMAP_OFF_COMBAT));
        }
			
		return false;
	}


    int FreeFlightCharacterController::userProcess()
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



    void FreeFlightCharacterController::OnApplyForceAndTorque(PhysicalThing* thing)
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
