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

#ifndef __Rl_UiMessages_H__
#define __Rl_UiMessages_H__

#include "UiPrerequisites.h"
#include "MessageType.h"

namespace rl
{
	// 0x04XXXXXX is mask for UI messages
    enum UiMessageTypeIds
    {
        // Combat interface
		// Mask
        RLMSG_COMBAT_IO_EVENTS = 0x04000100,
        // Events
        RLMSG_COMBAT_IO_ENEMY_BUTTON_CLICKED = 0x04000101,
    };

    /// Message sent after enemy action button has been pressed.
	/// UI internal event
	/// First int arg - handle of the button set
	/// Second int arg - button index
    typedef MessageType<RLMSG_COMBAT_IO_ENEMY_BUTTON_CLICKED, int, int> MessageType_CombatIoEnemyButtonClicked;
}
#endif
