/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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
#include "UiPrerequisites.h"

#include "CoreSubsystem.h"
#include "World.h"

#include "InputManager.h"
#include "DebugWindow.h"
#include "Console.h"
#include "CameraActor.h"
#include "Exception.h"

#include "FlyingCameraController.h"

namespace rl {

float FlyingCameraController::TIME_DECAY = 0.04;
float FlyingCameraController::MOUSE_SENSITIVITY = 200.0;

FlyingCameraController::FlyingCameraController() 
{
    // Take about 20 seconds for full rotation
	mTurningSpeed = 100.0;
    // Move about 150 units per second,
	mMovementSpeed = 150.0; 

	mActor = 0;

	//InputManager::getSingleton().addMouseMotionListener(this);
	//Ogre::Root::getSingleton().addFrameListener(this);
}

FlyingCameraController::FlyingCameraController(CameraActor* pCameraActor)
{
	// Take about 20 seconds for full rotation
	mTurningSpeed = 100.0;
    // Move about 150 units per second,
	mMovementSpeed = 150.0; 

	mActor = pCameraActor;
	//InputManager::getSingleton().addMouseMotionListener(this);
	//Ogre::Root::getSingleton().addFrameListener(this);
}

FlyingCameraController::~FlyingCameraController()
{
	//InputManager::getSingleton().removeMouseMotionListener(this);
	//Ogre::Root::getSingleton().removeFrameListener(this);
}

void FlyingCameraController::setControlledActor(CameraActor* controlledActor)
{  
    mActor = controlledActor;
}

CameraActor* FlyingCameraController::getControlledActor(void)
{
    return mActor;
}

void FlyingCameraController::run( Ogre::Real elapsedTime )
{
    mActor = CoreSubsystem::getSingleton().getWorld()->getActiveCamera();

    if( mActor != 0 )
    {
	    if (mLastEventSince <= 0)
		{
			mRotX = mRotY = 0;
		}
		else 
			mLastEventSince -= elapsedTime;
		
		calculateScalingFactors(elapsedTime);

	    //if( !Console::getSingleton().isOpen() )
		    
    	
		if (!InputManager::getSingleton().isCeguiActive())
		{
			processKeys();
			processMouse();
		}

        moveCamera();
    }

	if( InputManager::getSingleton().isKeyDown(Ogre::KC_ESCAPE) )
	{
		delete &Ogre::Root::getSingleton();
		exit(0);
	}
}

void FlyingCameraController::moveCamera()
{
    RlAssert( mActor != 0, "mActor darf nicht null sein");

	// Make all the changes to the camera
	// Note that YAW direction is around a fixed axis (freelook style) rather than a natural YAW (e.g. airplane)
	mActor->yaw(mRotX.valueDegrees());
	mActor->pitch(mRotY.valueDegrees());
	mActor->translate(mTranslateVector);	
}

void FlyingCameraController::calculateScalingFactors(Ogre::Real timePassed)
{
	if (timePassed == 0)
	{
		mMoveScale = 0.5;
		mRotScale = 0.1;
	}
	// Otherwise scale movement units by time passed since last frame
	else
	{
		mMoveScale = mMovementSpeed * timePassed;
		mRotScale = mTurningSpeed * timePassed;
	}

	mTranslateVector = Vector3::ZERO;
}

void FlyingCameraController::processMouse()
{	
	if( InputManager::getSingleton().isMouseButtonDown( 1 ) )
    {
		mTranslateVector.x += InputManager::getSingleton().getMouseRelativeX() * 0.13;
		mTranslateVector.y -= InputManager::getSingleton().getMouseRelativeY() * 0.13;
    }
    else
    {
		mRotX = -InputManager::getSingleton().getMouseRelativeX() * 0.13;
		mRotY = -InputManager::getSingleton().getMouseRelativeY() * 0.13;
	}
}


void FlyingCameraController::processKeys()
{
    RlAssert( mActor != 0, "mActor darf nicht null sein" );


	if (InputManager::getSingleton().isKeyDown(KC_A))
	{
		// Move camera left
		mTranslateVector.x = -mMoveScale;
	}

	if (InputManager::getSingleton().isKeyDown(KC_D))
	{
		// Move camera RIGHT
		mTranslateVector.x = mMoveScale;
	}

	/* Move camera forward by keypress. */
	if (InputManager::getSingleton().isKeyDown(KC_UP) || InputManager::getSingleton().isKeyDown(KC_W) )
	{
		mTranslateVector.z = -mMoveScale;
	}

	/* Move camera backward by keypress. */
	if (InputManager::getSingleton().isKeyDown(KC_DOWN) || InputManager::getSingleton().isKeyDown(KC_S) )
	{
		mTranslateVector.z = mMoveScale;
	}

	if (InputManager::getSingleton().isKeyDown(KC_PGUP))
	{
		// Move camera up
		mTranslateVector.y = mMoveScale;
	}

	if (InputManager::getSingleton().isKeyDown(KC_PGDOWN))
	{
		// Move camera down
		mTranslateVector.y = -mMoveScale;
	}

	if (InputManager::getSingleton().isKeyDown(KC_RIGHT))
	{
		mActor->yaw(-(mRotScale.valueDegrees()));
	}

	if (InputManager::getSingleton().isKeyDown(KC_LEFT))
	{
		mActor->yaw(mRotScale.valueDegrees());
	}

    if (InputManager::getSingleton().isKeyDown(KC_SYSRQ))
	{
        CoreSubsystem::getSingleton().makeScreenshot( "rastullah" );
	}
}


}
