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

#include <OgreSceneManager.h>

#include "Action.h"
#include "ActionManager.h"
#include "Actor.h"
#include "CameraObject.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "Exception.h"
#include "PhysicalThing.h"
#include "World.h"

using namespace Ogre;

namespace rl {

	CharacterController::CharacterController(Actor* camera, Actor* character)
		: GameTask(),
		mCameraActor(camera),
		mCharacterActor(character),
		mCamBody(0),
		mCharBody(0),
        mCommandMapper(NULL)
	{
		if (mCameraActor == 0)
		{
			Throw(NullPointerException, "Camera must not be NULL.");
		}

		if (mCharacterActor != NULL)
		{
			if (mCharacterActor->_getSceneNode() == NULL)
			{
				Throw(IllegalArgumentException,
					"character has to be placed in the scene beforehand");
			}
            if (mCharacterActor->getPhysicalThing() == NULL)
			{
				Throw(IllegalArgumentException,
					"character must have a physics proxy");
			}

			mCharBody = mCharacterActor->getPhysicalThing()->_getBody();
		}

		mCamBody = mCameraActor->getPhysicalThing()->_getBody();

		if (!mCameraActor->_getSceneNode())
		{
			mCameraActor->placeIntoNode(CoreSubsystem::getSingleton().
				getWorld()->getSceneManager()->getRootSceneNode());
		}

		// Reset camera position/orientation, as it is now controlled via scene nodes.
		Camera* ogreCam = static_cast<CameraObject*>(
            mCameraActor->getControlledObject())->getCamera();
		ogreCam->setPosition(Vector3::ZERO);
		ogreCam->setOrientation(Quaternion::IDENTITY);

		mCameraActor->_getSceneNode()->setFixedYawAxis(true);
	}
    
    CharacterController::~CharacterController()
    {
    
    }

	void CharacterController::setCommandMapper(CommandMapper* commandMapper)
	{
		mCommandMapper = commandMapper;
	}

	bool CharacterController::startAction(const CeGuiString& actionName, Creature* character)
	{
		if (actionName.length() == 0)
		{
			return false;
		}

		try
		{
            Action* action = ActionManager::getSingleton().getInGameGlobalAction(actionName);
            if (action != NULL)
            {
                action->doAction(NULL, NULL, NULL);
            }
            else if (character != NULL)
			{
				character->doAction(actionName, character, character);
			}
			return true;
		}
		catch( ScriptInvocationFailedException& sife )
		{
			LOG_ERROR(Logger::UI, sife.toString() );
		}

		return false;
	}

    const Ogre::String& CharacterController::getName() const
    {
        static String NAME = "CharacterController";

        return NAME;
    }
}
