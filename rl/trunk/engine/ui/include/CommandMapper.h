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

#ifndef __CommandMapper_H__
#define __CommandMapper_H__

#include "UiPrerequisites.h"

#include <map>

namespace rl {

    enum MovementState
    {
        MOVE_NONE = 0,
        MOVE_LEFT = 1<<0,
        MOVE_RIGHT = 1<<1,
        MOVE_FORWARD = 1<<2,
        MOVE_BACKWARD = 1<<3,
        TURN_LEFT = 1<<4,
        TURN_RIGHT = 1<<5,
        MOVE_RUN = 1<<6,
        MOVE_SNEAK = 1<<7,
        MOVE_JUMP = 1<<8,
        MOVE_RUN_LOCK = 1<<9
    };

    const int CMDMAP_NO_MAPPING = -9999999;

    class _RlUiExport CommandMapper
    {
    public:

        CommandMapper();
        ~CommandMapper();

        std::map<CeGuiString, MovementState> getMovements();

        static int encodeKey(int scancode, int syskeys);
        static void decodeKey(int combinedKeyCode, int* scancode, int* syskeys);

        const MovementState getMovement(int keycode) const;

        /// Get the action name assigned to the key.
        const CeGuiString& getGlobalAction(int keyCodeOrMouseButton) const;
        const CeGuiString& getControlStateAction(int keyCodeOrMouseButton, ControlStateType) const;

    private:

        // KeyCode -> Name
        typedef std::map<int, CeGuiString > KeyAndMouseCommandMap;
        typedef std::map<int, MovementState> MovementCommandMap;

        MovementCommandMap mMovementCommands;
        KeyAndMouseCommandMap mKeyGlobalActions;
        KeyAndMouseCommandMap mKeyMovementControlState;
        KeyAndMouseCommandMap mKeyFreeflightControlState;
        KeyAndMouseCommandMap mKeyDialogControlState;
        KeyAndMouseCommandMap mKeyCombatControlState;
        KeyAndMouseCommandMap mKeyCutsceneControlState;

        int getKeyCode(const Ogre::String& keyDescription);
        int getMouseButtonCode(int buttonNum);
        int getMouseButtonCode(const Ogre::String& buttonDescription);
        MovementState getMovement(const Ogre::String& movementDescription);
        void buildCommandMapping();
        void buildCommandMap(KeyAndMouseCommandMap& cmdMap, const Ogre::NameValuePairList& values);
        const KeyAndMouseCommandMap& getControlStateMapping(ControlStateType) const;
    };

}

#endif
