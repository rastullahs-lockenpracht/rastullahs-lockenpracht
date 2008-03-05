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

#ifndef __Rl_UiMessages_H__
#define __Rl_UiMessages_H__

#include "UiPrerequisites.h"
#include "MessageType.h"

namespace rl
{
	class Combatant;

	// 0x04XXXXXX is mask for UI messages
    enum UiMessageTypeIds
    {
        // Combat interface
		// Mask
        RLMSG_COMBAT_IO_EVENTS = 0x04000100,
        // Events
        RLMSG_COMBAT_IO_ENEMY_BUTTON_CLICKED = 0x04000101,
        RLMSG_COMBAT_IO_END_TURN_BUTTON_CLICKED = 0x04000102,

        RLMSG_COMBAT_IO_ATTACK_OPPONENT = 0x04000103,
        RLMSG_COMBAT_IO_PARRY_OPPONENT = 0x04000104,
        RLMSG_COMBAT_IO_END_TURN_REQUESTED = 0x04000105
    };

    /// Message sent after enemy action button has been pressed.
	/// UI internal event
	/// First int arg - handle of the button set
	/// Second int arg - button index
    typedef MessageType<RLMSG_COMBAT_IO_ENEMY_BUTTON_CLICKED, int, int> MessageType_CombatIoEnemyButtonClicked;
    typedef MessageType<RLMSG_COMBAT_IO_END_TURN_BUTTON_CLICKED> MessageType_CombatIoEndTurnButtonClicked;

    typedef MessageType<RLMSG_COMBAT_IO_ATTACK_OPPONENT, Combatant*> MessageType_CombatIoAttackOpponent;
    typedef MessageType<RLMSG_COMBAT_IO_PARRY_OPPONENT, Combatant*> MessageType_CombatIoParryOpponent;
    typedef MessageType<RLMSG_COMBAT_IO_END_TURN_REQUESTED> MessageType_CombatIoEndTurnRequested;
}
#endif
