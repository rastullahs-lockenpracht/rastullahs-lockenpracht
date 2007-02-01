/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include "VanityModeCharacterController.h"

#include "Actor.h"
#include "CameraObject.h"
#include "CommandMapper.h"


namespace rl {

    VanityModeCharacterController::VanityModeCharacterController(Actor* camera, Actor* character)
        : CharacterController(camera, character)
    {
    }

    VanityModeCharacterController::~VanityModeCharacterController(void)
    {
    }

	void VanityModeCharacterController::run(Ogre::Real elapsedTime)
	{
        static Ogre::Real angle(0);
        angle += Ogre::Real(elapsedTime * 30);
        while (angle > 360)
        {
            angle -= 360;
        }

        static Ogre::Real radius = 5;

        Vector3 charPos = mCharacterActor->getWorldPosition();
        Vector3 targetPos = charPos + radius * Vector3(sin(angle), 0, cos(angle));

        mCameraActor->setPosition(targetPos);
        dynamic_cast<Ogre::Camera*>(mCameraActor->getControlledObject()->getMovableObject())
            ->lookAt(charPos);
        //mCameraActor->getPhysicalThing()->addForce();
	}

    CharacterController::ControllerType VanityModeCharacterController::getType() const
    {
        return CharacterController::CTRL_VANITY_MODE;
    }

    void VanityModeCharacterController::toggleViewMode()
    {
    }

    void VanityModeCharacterController::resetCamera()
    {
    }

	bool VanityModeCharacterController::injectKeyUp(int keycode)
    {
		return startAction(mCommandMapper->getAction(keycode, CMDMAP_KEYMAP_OFF_COMBAT));		
	}


} // namespace rl
