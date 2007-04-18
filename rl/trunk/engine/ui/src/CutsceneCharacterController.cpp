/*
* This source file is part of Rastullahs Lockenpracht.
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

#include "CutsceneCharacterController.h"
#include "Exception.h"
#include "Actor.h"
#include "CameraObject.h"
#include "CommandMapper.h"
#include "PhysicalThing.h"

using namespace Ogre;

namespace rl {

	CutsceneCharacterController::CutsceneCharacterController(CommandMapper* cmdMapper,
        Actor* camera) : CharacterController(cmdMapper, camera, NULL)
	{
		mCameraActor->getPhysicalThing()->freeze();
		Camera* ogreCam = static_cast<Camera*>(mCameraActor->_getMovableObject());
		ogreCam->setFixedYawAxis(true);
		mCameraActor->_getSceneNode()->setFixedYawAxis(true);
	}

	CutsceneCharacterController::~CutsceneCharacterController()
	{
		mCameraActor->getPhysicalThing()->unfreeze();
	}

    void CutsceneCharacterController::pause()
    {
    }

    void CutsceneCharacterController::resume()
    {
    }

	void CutsceneCharacterController::run(Ogre::Real elapsedTime)
	{
	}

	void CutsceneCharacterController::toggleViewMode()
	{
	}

    void CutsceneCharacterController::resetCamera()
	{
	}

    void CutsceneCharacterController::setCameraPosition(const Ogre::Vector3& pos)
    {
        mCameraActor->setPosition(pos);
    }

    void CutsceneCharacterController::setCameraOrientation(const Ogre::Quaternion& orient)
    {
        mCameraActor->setOrientation(orient);
    }

    void CutsceneCharacterController::lookAt(const Ogre::Vector3& point)
    {
        dynamic_cast<Ogre::Camera*>(mCameraActor->getControlledObject()->getMovableObject())
            ->lookAt(point);
    }

    bool CutsceneCharacterController::injectKeyUp(int keycode)
	{
		return startAction(mCommandMapper->getControlStateAction(keycode, CST_CUTSCENE));		
	}
}
