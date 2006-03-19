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

#include "CharacterController.h"
#include "CoreSubsystem.h"
#include "World.h"
#include "InputManager.h"
#include "CommandMapper.h"
#include "DebugWindow.h"
#include "Exception.h"
#include "Actor.h"
#include "CameraObject.h"
#include "MeshObject.h"
#include "PhysicsManager.h"
#include "PhysicalThing.h"
#include "MeshObject.h"
#include "ActorManager.h"
#include "Logger.h"

#include <OgreSceneManager.h>
#include <OgreAxisAlignedBox.h>
#include <OgreStringConverter.h>
#include <OgreMath.h>

#include <numeric>

using namespace Ogre;

namespace rl {

	CharacterController::CharacterController(Actor* camera, Actor* character)
		: GameTask(),
		mCamera(camera),
		mCharacter(character),
		mCamBody(0),
		mCharBody(0)
	{
		if (mCamera == 0 || mCharacter == 0)
		{
			Throw(NullPointerException, "Character and Camera must not be NULL.");
		}

		if (!mCharacter->_getSceneNode())
		{
			Throw(InvalidArgumentException,
				"character has to be placed in the scene beforehand");
		}

		mCharBody = mCharacter->getPhysicalThing()->_getBody();
		mCamBody = mCamera->getPhysicalThing()->_getBody();

		if (!mCamera->_getSceneNode())
		{
			mCamera->placeIntoNode(CoreSubsystem::getSingleton().
				getWorld()->getSceneManager()->getRootSceneNode());
		}

		// Reset camera position/orientation, as it is now controlled via scene nodes.
		Camera* ogreCam = dynamic_cast<CameraObject*>(mCamera->getControlledObject())->getCamera();
		ogreCam->setPosition(Vector3::UNIT_Z);
		ogreCam->setOrientation(Quaternion::IDENTITY);

		mCamera->_getSceneNode()->setFixedYawAxis(true);
	}
}
