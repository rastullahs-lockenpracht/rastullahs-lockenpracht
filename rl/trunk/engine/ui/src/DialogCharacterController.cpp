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

#include "DialogCharacterController.h"
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

	DialogCharacterController::DialogCharacterController(Actor* camera, Actor* character)
		: CharacterController(camera, character),
		mOgreCam(0),
		mText("")
	{
		mCamera->getPhysicalThing()->freeze();
		mCharacter->getPhysicalThing()->freeze();
		mOgreCam = static_cast<Camera*>(mCamera->_getMovableObject());
	}

	DialogCharacterController::~DialogCharacterController()
	{
		mCamera->getPhysicalThing()->unfreeze();
		mCharacter->getPhysicalThing()->unfreeze();
	}

	void DialogCharacterController::setDialogPartner(Actor* partner)
	{
		mDialogPartner = partner;

		resetCamera();
		mOgreCam->setPosition(mCamera->_getSceneNode()->_getDerivedPosition());
		mOgreCam->setOrientation(Quaternion::IDENTITY);
		mOgreCam->setFixedYawAxis(true);

		MeshObject* mesh = dynamic_cast<MeshObject*>(mCharacter->getControlledObject());
		mesh->stopAllAnimations();
		mesh->startAnimation("Idle");
	}

	void DialogCharacterController::run(Real elapsedTime)
	{
		if (mTime > 0)
		{
			mTime -= elapsedTime;
			if (mTime <= 0)
			{
				mTime = 0;
				//mDialogWindow->timeOver();
			}
		}
	}

	void DialogCharacterController::toggleViewMode()
	{
		// Gibbet keine.
	}

	void DialogCharacterController::resetCamera()
	{
		// Position camera at position between char and dialog partner
		Vector3 camPosition = (mCharacter->getWorldPosition() + mDialogPartner->getWorldPosition()) / 2;
		camPosition.y += 1;
		mCamera->_getSceneNode()->setPosition(camPosition);
		mCamera->_getSceneNode()->setOrientation(Quaternion::IDENTITY);
	}

	void DialogCharacterController::response(
		Actor* actor, const CeGuiString& text, const Ogre::String& soundFile)
	{
		mOgreCam->lookAt(actor->getWorldPosition());
		mText = text;
		mTime = text.length() * 0.1;
	}
}
