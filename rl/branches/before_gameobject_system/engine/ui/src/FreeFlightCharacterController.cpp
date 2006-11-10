/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
		mMovementSpeed(30.0f),
		mSpeedRange(0.03f, 90.0f),
		mSpeedIncrement(0.02f),
		mRotationSpeed(4.0f),
		mOgreCam(0),
		mCurrentMovementState(0)
	{
		mCameraActor->getPhysicalThing()->freeze();
		mCharacterActor->getPhysicalThing()->freeze();

		resetCamera();
		mOgreCam = static_cast<Camera*>(mCameraActor->_getMovableObject());
        mOgreCam->setPosition(Vector3::ZERO);
		mOgreCam->setOrientation(Quaternion::IDENTITY);
		mOgreCam->setFixedYawAxis(true);

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
		Vector3 translation = Vector3::ZERO;

		int movement = mCurrentMovementState;

		// Determine character's control state based on user input
		if (movement & MOVE_FORWARD)
			translation.z = -mMovementSpeed;

		if (movement & MOVE_BACKWARD)
			translation.z = mMovementSpeed;

		if (movement & MOVE_RIGHT)
			translation.x = mMovementSpeed;

		if (movement & MOVE_LEFT)
			translation.x = -mMovementSpeed;

		if (movement & MOVE_RUN)
			translation *= 4.0;

		mMovementSpeed += im->getMouseRelativeZ() * mSpeedIncrement;
		if (mMovementSpeed < mSpeedRange.first)
		{
			mMovementSpeed = mSpeedRange.first;
		}
		if (mMovementSpeed > mSpeedRange.second)
		{
			mMovementSpeed = mSpeedRange.second;
		}

		Radian pitch = Degree(-im->getMouseRelativeY() * 30.0 * elapsedTime);
		Radian yaw = Degree(-im->getMouseRelativeX() * 30.0 * elapsedTime);

		mOgreCam->yaw(yaw);
		mOgreCam->pitch(pitch);
		mOgreCam->moveRelative(translation*elapsedTime);
	}

	void FreeFlightCharacterController::toggleViewMode()
	{
		// Gibbet keine.
	}

	void FreeFlightCharacterController::resetCamera()
	{
		// Position camera at char position
        if( mCharacterActor != NULL )
        {
            mCameraActor->_getSceneNode()->setOrientation( mCharacterActor->getWorldOrientation() );
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
		    mCameraActor->_getSceneNode()->setOrientation( Quaternion::IDENTITY );
            mCameraActor->_getSceneNode()->setPosition( Vector3::ZERO );
        }
	}

	bool FreeFlightCharacterController::injectKeyDown(int keycode)
	{
		int movement = mCommandMapper->getMovement(keycode);

		if (movement != MOVE_NONE)
		{
			mCurrentMovementState |= movement;
			return true;
		}
		return false;
	}

	bool FreeFlightCharacterController::injectKeyUp(int keycode)
	{
		int movement = mCommandMapper->getMovement(keycode);

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
}
