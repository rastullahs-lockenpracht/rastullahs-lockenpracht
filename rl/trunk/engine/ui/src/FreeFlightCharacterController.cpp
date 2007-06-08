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

	FreeFlightCharacterController::FreeFlightCharacterController(CommandMapper* cmdMapper,
        Actor* camera, Person* character)
		: CharacterController(cmdMapper, camera, character, CST_FREEFLIGHT),
		mMovementSpeed(5.0f),
		mSpeedRange(0.03f, 90.0f),
		mSpeedIncrement(0.02f),
		mRotationSpeed(4.0f),
		mCurrentMovementState(0),
        mDesiredVelocity(Vector3::ZERO),
        mCollisionsEnabled(false),
        mPitchRange(Degree(-89), Degree(89)),
        mCameraUpConstraint(Vector3::ZERO),
        mYaw(Degree(0)),
        mPitch(Degree(0))
	{
        mMouseSensitivity = ConfigurationManager::getSingleton().getIntSetting("Input", "Mouse Sensitivity");
        mInvertedMouse = ConfigurationManager::getSingleton().getBoolSetting("Input", "Mouse Invert");
	}

	FreeFlightCharacterController::~FreeFlightCharacterController()
	{
	}

    void FreeFlightCharacterController::pause()
    {
		mCameraActor->getPhysicalThing()->unfreeze();
        if(mCameraUpConstraint != Vector3::ZERO)
            mCameraActor->getPhysicalThing()->setUpConstraint(mCameraUpConstraint);
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

    void FreeFlightCharacterController::resume()
    {
        mCameraActor->getPhysicalThing()->freeze();
        mCameraUpConstraint = mCameraActor->getPhysicalThing()->getUpConstraint();
		mCharacterActor->getPhysicalThing()->freeze();

        resetCamera();

        // The actor should be controlled manually,
        // so let the PM prepare it accordingly
        mCameraActor->getPhysicalThing()->setPhysicsController(this);
        // We also handle char<->level, char<->default collision from now on (camera=char!)
        PhysicsManager::getSingleton().getMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("default"))->setContactCallback(this);
        PhysicsManager::getSingleton().getMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("level"))->setContactCallback(this);
        PhysicsManager::getSingleton().getMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("character"))->setContactCallback(this);

        MeshObject* mesh = dynamic_cast<MeshObject*>(mCharacterActor->getControlledObject());
        if( mesh != NULL )
        {
		    mesh->stopAllAnimations();
		    mesh->startAnimation("idle");
        }
    }

	void FreeFlightCharacterController::run(Real elapsedTime)
	{
        if (isCeguiActive()) return;

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


        if (movement & TURN_LEFT)
            mYaw += elapsedTime * Degree(120.0f);
        if (movement & TURN_RIGHT)
            mYaw -= elapsedTime * Degree(120.0f);

        // mouse
        if( !(movement & TURN_LEFT || movement & TURN_RIGHT) )
        {
            mYaw -= mMouseSensitivity * Degree(im->getMouseRelativeX() / 10);
        }
        while (mYaw.valueDegrees() > 360.0f) mYaw -= Degree(360.0f);
        while (mYaw.valueDegrees() < -360.0f) mYaw += Degree(360.0f);


        if (mInvertedMouse)
            mPitch += mMouseSensitivity * Degree(im->getMouseRelativeY() / 4);
        else
            mPitch -= mMouseSensitivity * Degree(im->getMouseRelativeY() / 4);

        while (mPitch.valueDegrees() > 360.0f) mPitch -= Degree(360.0f);
        while (mPitch.valueDegrees() < -360.0f) mPitch += Degree(360.0f);
        if (mPitch < mPitchRange.first) mPitch = mPitchRange.first;
        if (mPitch > mPitchRange.second) mPitch = mPitchRange.second;

        mCameraActor->setOrientation(Quaternion::IDENTITY);
        mCameraActor->getPhysicalThing()->clearUpConstraint();
        mCameraActor->yaw(mYaw.valueDegrees());
        mCameraActor->pitch(mPitch.valueDegrees());
    }

	void FreeFlightCharacterController::toggleCameraCollision()
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
        mYaw = Degree(0);
        mPitch = Degree(0);
	}

    bool FreeFlightCharacterController::keyPressed(const OIS::KeyEvent& evt)
	{
        int movement = mCommandMapper->getMovement(evt.key);

		if (movement != MOVE_NONE)
		{
			mCurrentMovementState |= movement;
			return true;
		}

		return false;
	}

    bool FreeFlightCharacterController::keyReleased(const OIS::KeyEvent& evt)
	{
        int movement = mCommandMapper->getMovement(evt.key);

		if (movement != MOVE_NONE)
		{
			mCurrentMovementState &= ~movement;
			return true;
		}
        else
        {
            InputManager* im = InputManager::getSingletonPtr();
            int keycode = CommandMapper::encodeKey(evt.key, im->getModifierCode());
            CeGuiString command = mCommandMapper->getControlStateAction(keycode, CST_FREEFLIGHT);
            if (command == "back_to_character_movement")
            {
                InputManager::getSingleton().popControlState();
                return true;
            }
            if (command == "toggle_camera_collision")
            {
                toggleCameraCollision();
                return true;
            }
            else 
            {
                return CharacterController::keyReleased(evt);
            }
        }
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
