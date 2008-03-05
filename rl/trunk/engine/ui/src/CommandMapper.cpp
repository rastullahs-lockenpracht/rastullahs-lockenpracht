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

#include "Action.h"
#include "ActionManager.h"
#include "CommandMapper.h"
#include "ConfigurationManager.h"
#include "Exception.h"
#include "InputManager.h"
#include "Person.h"
#include "UiSubsystem.h"

using namespace Ogre;

namespace rl {

    CommandMapper::CommandMapper()
        : mMovementCommands(),
        mKeyGlobalActions(),
        mKeyMovementControlState(),
        mKeyFreeflightControlState(),
        mKeyDialogControlState(),
        mKeyCombatControlState(),
        mKeyCutsceneControlState()
    {
        buildCommandMapping();
    }

    CommandMapper::~CommandMapper()
    {
    }

    const CeGuiString& CommandMapper::getGlobalAction(int keyCodeOrMouseButton) const
    {
        KeyAndMouseCommandMap::const_iterator command =
            mKeyGlobalActions.find(keyCodeOrMouseButton);

        if (command != mKeyGlobalActions.end())
        {
            return (*command).second;
        }

        static CeGuiString NO_ACTION = "";
        return NO_ACTION;
    }

    const CeGuiString& CommandMapper::getControlStateAction(int keyCodeOrMouseButton,
        ControlStateType type) const
    {
        const KeyAndMouseCommandMap& mapping = getControlStateMapping(type);

        KeyAndMouseCommandMap::const_iterator command = mapping.find(keyCodeOrMouseButton);
        if (command != mapping.end())
        {
            return (*command).second;
        }

        static CeGuiString NO_ACTION = "";
        return NO_ACTION;
    }

    const CommandMapper::KeyAndMouseCommandMap&
        CommandMapper::getControlStateMapping(ControlStateType type) const
    {
        switch (type)
        {
        case CST_MOVEMENT:
            return mKeyMovementControlState;
        case CST_FREEFLIGHT:
            return mKeyFreeflightControlState;
        case CST_CUTSCENE:
            return mKeyCutsceneControlState;
        case CST_DIALOG:
            return mKeyDialogControlState;
        case CST_COMBAT:
            return mKeyCombatControlState;
        default:
            RlAssert1(false && "Unknown ControlStateType");
        }
    }

    int CommandMapper::encodeKey(int scancode, int syskeys)
    {
        return scancode | syskeys << 16;
    }

    void CommandMapper::decodeKey(int combinedKeyCode, int* scancode, int* syskeys)
    {
        int syskeysTmp = combinedKeyCode >> 16;
        int scancodeTmp = combinedKeyCode & ~(syskeysTmp << 16);

        if (syskeys != NULL)
        {
            *syskeys = syskeysTmp;
        }
        if (scancode != NULL)
        {
            *scancode = scancodeTmp;
        }
    }

    const MovementState CommandMapper::getMovement(int keycode) const
    {
        MovementCommandMap::const_iterator mvcmd = mMovementCommands.find(keycode);

        if (mvcmd == mMovementCommands.end())
        {
            return MOVE_NONE;
        }
        else
        {
            return (*mvcmd).second;
        }
    }

    void CommandMapper::buildCommandMapping()
    {
        ConfigurationManager* cfgMgr = ConfigurationManager::getSingletonPtr();
        InputManager* inputMgr = InputManager::getSingletonPtr();

        // First get the movement commands
        const NameValuePairList& commands = cfgMgr->getSettings("Movement keys");

        for (NameValuePairList::const_iterator it = commands.begin(); it != commands.end(); it++)
        {
            // Split the path at the ',' character
            StringVector keys = Ogre::StringUtil::split(it->second, ",");

            for (size_t i = 0; i < keys.size(); i++)
            {
                mMovementCommands[inputMgr->getScanCode(keys[i])] = getMovement(it->first);
                LOG_MESSAGE(Logger::UI,
                    Ogre::String("Key ") + keys[i] + " ("
                    + StringConverter::toString(inputMgr->getScanCode(keys[i]))
                    + ") is assigned to movement " + it->first +" ("
                    + StringConverter::toString(getMovement(it->first))+")");
            }
        }

        buildCommandMap(mKeyGlobalActions, cfgMgr->getSettings("Action keys"));
        buildCommandMap(mKeyMovementControlState, cfgMgr->getSettings("MovementController keys"));
        buildCommandMap(mKeyFreeflightControlState, cfgMgr->getSettings("FreeflightController keys"));
        buildCommandMap(mKeyDialogControlState, cfgMgr->getSettings("DialogController keys"));
        buildCommandMap(mKeyCombatControlState, cfgMgr->getSettings("CombatController keys"));
        buildCommandMap(mKeyCutsceneControlState, cfgMgr->getSettings("CutsceneController keys"));
    }

    void CommandMapper::buildCommandMap(KeyAndMouseCommandMap& cmdMap,
        const NameValuePairList& values)
    {
        for (NameValuePairList::const_iterator it = values.begin(); it != values.end(); it++)
        {
            // Split the path at the ',' character
            StringVector keys = Ogre::StringUtil::split(it->second, ",");

            for (size_t i = 0; i < keys.size(); i++)
            {
                int keycode = getKeyCode(keys[i]);
                cmdMap[keycode] = CeGuiString(it->first);
                LOG_MESSAGE(Logger::UI,
                    Ogre::String("Key ") + keys[i] + " ("
                    + StringConverter::toString(keycode)
                    + ") is assigned to command " + it->first);
            }
        }
    }

    int CommandMapper::getKeyCode(const Ogre::String &keyDescription)
    {
        StringVector parts = StringUtil::split(keyDescription, "+");

        int modifiers = 0;
        for(size_t i = 0; i<parts.size()-1; i++)
        {
            modifiers |= InputManager::getSingleton().getSystemCode(parts[i]);
        }

        return encodeKey(InputManager::getSingleton().getScanCode(parts[parts.size()-1]), modifiers);
    }

    int CommandMapper::getMouseButtonCode(int buttonNum)
    {
        return buttonNum;
    }

    int CommandMapper::getMouseButtonCode(const Ogre::String &buttonDescription)
    {
        StringVector parts = StringUtil::split(buttonDescription, "+");

        int modifiers = 0;
        for(size_t i = 0; i<parts.size()-1; i++)
        {
            modifiers |= InputManager::getSingleton().getSystemCode(parts[i]);
        }

        int buttonNum = StringConverter::parseInt(parts[parts.size()-1].substr(6));
        return encodeKey(getMouseButtonCode(buttonNum), modifiers);
    }

    MovementState CommandMapper::getMovement(const Ogre::String &movementDescription)
    {
        if (movementDescription == "move_left")
        {
            return MOVE_LEFT;
        }
        else if (movementDescription == "move_right")
        {
            return MOVE_RIGHT;
        }
        else if (movementDescription == "move_forward")
        {
            return MOVE_FORWARD;
        }
        else if (movementDescription == "move_backward")
        {
            return MOVE_BACKWARD;
        }
        else if (movementDescription == "turn_left")
        {
            return TURN_LEFT;
        }
        else if (movementDescription == "turn_right")
        {
            return TURN_RIGHT;
        }
        else if (movementDescription == "run")
        {
            return MOVE_RUN;
        }
        else if (movementDescription == "sneak")
        {
            return MOVE_SNEAK;
        }
        else if (movementDescription == "jump")
        {
            return MOVE_JUMP;
        }
        else if (movementDescription == "run_lock")
        {
            return MOVE_RUN_LOCK;
        }

        return MOVE_NONE;
    }
}
