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
#include "stdinc.h" //precompiled header

#include "ControlState.h"

#include <OgreSceneManager.h>

#include "AbstractWindow.h"
#include "Action.h"
#include "ActionManager.h"
#include "Actor.h"
#include "CameraObject.h"
#include "CommandMapper.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "Exception.h"
#include "InputManager.h"
#include "Person.h"
#include "PhysicalThing.h"
#include "WindowManager.h"
#include "World.h"

using namespace Ogre;

namespace rl {

	ControlState::ControlState(CommandMapper* commandMapper,
        Actor* camera, Person* character, ControlStateType type)
		: mCharacter(character),
        mCameraActor(camera),
		mCharacterActor(NULL),
		mCamBody(NULL),
		mCharBody(NULL),
        mCommandMapper(commandMapper),
        mType(type)
	{
		if (mCameraActor == NULL)
		{
			Throw(NullPointerException, "Camera must not be NULL.");
		}

		if (mCharacter != NULL)
		{
            mCharacterActor = mCharacter->getActor();

			if (mCharacter != NULL && mCharacterActor->_getSceneNode() == NULL)
			{
				Throw(IllegalArgumentException,
					"character has to be placed in the scene beforehand");
			}
            if (mCharacter != NULL && mCharacterActor->getPhysicalThing() == NULL)
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

        PhysicsManager::getSingleton().createMaterialID("camera");

        PhysicsManager::getSingleton().createMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("default"))->setDefaultCollidable(0);

        PhysicsManager::getSingleton().createMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("level"))->setDefaultCollidable(0);

        PhysicsManager::getSingleton().createMaterialPair(
            PhysicsManager::getSingleton().getMaterialID("camera"),
            PhysicsManager::getSingleton().getMaterialID("character"))->setDefaultCollidable(0);
	}

    ControlState::~ControlState()
    {

    }

	bool ControlState::startAction(const CeGuiString& actionName, Creature* character)
	{
		if (actionName.length() == 0)
		{
			return false;
		}

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

    bool ControlState::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        if (WindowManager::getSingleton().getWindowInputMask() & AbstractWindow::WIT_MOUSE_INPUT)
        {
            CEGUI::System::getSingleton().injectMouseButtonDown(
                static_cast<CEGUI::MouseButton>(id));
            return true;
        }
        else
        {
            return false;
        }
    }

    bool ControlState::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        if (WindowManager::getSingleton().getWindowInputMask() & AbstractWindow::WIT_MOUSE_INPUT)
        {
            CEGUI::System::getSingleton().injectMouseButtonUp(static_cast<CEGUI::MouseButton>(id));
            return true;
        }
        else
        {
            return false;
        }
    }

    bool ControlState::mouseMoved(const OIS::MouseEvent& evt)
    {
        if (WindowManager::getSingleton().getWindowInputMask() & AbstractWindow::WIT_MOUSE_INPUT)
        {
            CEGUI::Renderer* renderer = CEGUI::System::getSingleton().getRenderer();
            CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool ControlState::keyPressed(const OIS::KeyEvent& evt)
    {
        InputManager* im = InputManager::getSingletonPtr();

        if (sendKeyToCeGui(evt))
        {
            CEGUI::System& cegui = CEGUI::System::getSingleton();
            cegui.injectKeyDown(evt.key);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool ControlState::keyReleased(const OIS::KeyEvent& evt)
    {
        InputManager* im = InputManager::getSingletonPtr();

        if (sendKeyToCeGui(evt))
        {
            CEGUI::System& cegui = CEGUI::System::getSingleton();
            cegui.injectKeyUp(evt.key);
            cegui.injectChar(im->getKeyChar(evt.key, im->getModifierCode()));
            return true;
        }
        else
        {
            int code = CommandMapper::encodeKey(evt.key, im->getModifierCode());

            // First see, if a control state action is defined
		    CeGuiString action = mCommandMapper->getControlStateAction(code, mType);
            if (action == "")
            {
                // No. So try global actions.
                action = mCommandMapper->getGlobalAction(code);
            }
            return startAction(action, mCharacter);
        }
    }

    bool ControlState::isCeguiActive() const
    {
        return WindowManager::getSingleton().getWindowInputMask() != 0;
    }

    bool ControlState::sendKeyToCeGui(const OIS::KeyEvent& evt) const
    {
        InputManager* im = InputManager::getSingletonPtr();

        // Wenn kein Fenster mit Tastatureingabe aktiv ist, kriegt CEGUI keine KeyEvents
        if ((WindowManager::getSingleton().getWindowInputMask()
            & AbstractWindow::WIT_KEYBOARD_INPUT) == 0)
        {
            return false;
        }

        // ---- Tastatureingabe gefordert ----

        // Tasten, die Zeichen liefern sollen an CEGUI gesendet werden
        if (im->getKeyChar(evt.key, im->getModifierCode()) != 0)
        {
            return true;
        }

        if (evt.key == OIS::KC_RETURN
            || evt.key == OIS::KC_HOME || evt.key == OIS::KC_END
            || evt.key == OIS::KC_LEFT || evt.key == OIS::KC_RIGHT
            || evt.key == OIS::KC_BACK || evt.key == OIS::KC_DELETE
            || evt.key == OIS::KC_UP   || evt.key == OIS::KC_DOWN
            || evt.key == OIS::KC_RMENU
            || evt.key == OIS::KC_LCONTROL)
        {
            return true;
        }

        return false;
    }
}
