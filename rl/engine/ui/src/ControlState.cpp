/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#include "AbstractWindow.h"
#include "Action.h"
#include "ActionManager.h"
#include "Actor.h"
#include "CameraObject.h"
#include "CommandMapper.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "Exception.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "PhysicalThing.h"
#include "RulesMessages.h"
#include "WindowManager.h"
#include "World.h"

using namespace Ogre;

namespace rl
{

    const Ogre::String ControlState::PROPERTY_CHARACTERID = "characterid";

    ControlState::ControlState(CommandMapper* commandMapper, Actor* camera, Creature* character, ControlStateType type)
        : mCharacter(character)
        , mCameraActor(camera)
        , mCharacterActor(NULL)
        , mCamBody(NULL)
        , mCharBody(NULL)
        , mCommandMapper(commandMapper)
        , mType(type)
    {
        if (mCameraActor == NULL)
        {
            Throw(NullPointerException, "Camera must not be NULL.");
        }

        if (mCharacter != NULL)
        {
            mCharacterId = mCharacter->getId();

            mMessageType_GameObjectsLoaded_Handler
                = MessagePump::getSingleton().addMessageHandler<MessageType_GameObjectsLoaded>(
                    boost::bind(&ControlState::refetchCharacter, this));

            mCharacterActor = mCharacter->getActor();

            if (mCharacter != NULL && mCharacterActor->_getSceneNode() == NULL)
            {
                Throw(IllegalArgumentException, "character has to be placed in the scene beforehand");
            }
            if (mCharacter != NULL && mCharacterActor->getPhysicalThing() == NULL)
            {
                Throw(IllegalArgumentException, "character must have a physics proxy");
            }

            mCharBody = mCharacterActor->getPhysicalThing()->_getBody();
        }

        mCamBody = mCameraActor->getPhysicalThing()->_getBody();

        if (!mCameraActor->_getSceneNode())
        {
            mCameraActor->placeIntoNode(
                CoreSubsystem::getSingleton().getWorld()->getSceneManager()->getRootSceneNode());
        }

        // Reset camera position/orientation, as it is now controlled via scene nodes.
        Camera* ogreCam = static_cast<CameraObject*>(mCameraActor->getControlledObject())->getCamera();
        ogreCam->setPosition(Vector3::ZERO);
        ogreCam->setOrientation(Quaternion::IDENTITY);

        mCameraActor->_getSceneNode()->setFixedYawAxis(true);

        PhysicsManager::getSingleton().createMaterialID("camera");

        PhysicsManager::getSingleton()
            .createMaterialPair(PhysicsManager::getSingleton().getMaterialID("camera"),
                PhysicsManager::getSingleton().getMaterialID("default"))
            ->setDefaultCollidable(0);

        PhysicsManager::getSingleton()
            .createMaterialPair(PhysicsManager::getSingleton().getMaterialID("camera"),
                PhysicsManager::getSingleton().getMaterialID("level"))
            ->setDefaultCollidable(0);

        PhysicsManager::getSingleton()
            .createMaterialPair(PhysicsManager::getSingleton().getMaterialID("camera"),
                PhysicsManager::getSingleton().getMaterialID("character"))
            ->setDefaultCollidable(0);

        PhysicsManager::getSingleton().getNewtonDebugger()->setMaterialColor(
            PhysicsManager::getSingleton().getMaterialID("camera"), Ogre::ColourValue::Black);
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

    bool ControlState::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id, bool handled)
    {
        return false;
    }

    bool ControlState::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id, bool handled)
    {
        return false;
    }

    bool ControlState::mouseMoved(const OIS::MouseEvent& evt, bool handled)
    {
        return false;
    }

    bool ControlState::keyPressed(const OIS::KeyEvent& evt, bool handled)
    {
        int code = CommandMapper::encodeKey(evt.key, InputManager::getSingleton().getModifierCode());

        // First see, if a control state action is defined
        CeGuiString action = mCommandMapper->getControlStateAction(code, mType);
        if (action == "")
        {
            // No. So try global actions.
            action = mCommandMapper->getGlobalAction(code);
        }
        if (!handled)
            return startAction(action, mCharacter);

        // hack to enable tab to hide the console-window if opened
        if (action == "toggleconsole") // we start toggleconsole always, even if handled, because cegui handles tab
            return startAction(action, mCharacter);

        return false;
    }

    bool ControlState::keyReleased(const OIS::KeyEvent& evt, bool handled)
    {
        return false;
    }

    bool ControlState::isMouseUsedByCegui() const
    {
        return WindowManager::getSingleton().getWindowInputMask() & AbstractWindow::WIT_MOUSE_INPUT;
    }

    bool ControlState::refetchCharacter()
    {
        mCharacter = static_cast<Creature*>(GameObjectManager::getSingleton().getGameObject(mCharacterId));

        if (mCharacter != NULL)
        {
            mCharacterActor = mCharacter->getActor();

            if (mCharacter != NULL && mCharacterActor->_getSceneNode() == NULL)
            {
                Throw(IllegalArgumentException, "character has to be placed in the scene beforehand");
            }
            if (mCharacter != NULL && mCharacterActor->getPhysicalThing() == NULL)
            {
                Throw(IllegalArgumentException, "character must have a physics proxy");
            }

            mCharBody = mCharacterActor->getPhysicalThing()->_getBody();
        }
        return false;
    }
}
