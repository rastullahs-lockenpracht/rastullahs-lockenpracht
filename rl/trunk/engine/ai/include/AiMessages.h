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

#ifndef __Rl_AiMessages_H__
#define __Rl_AiMessages_H__

#include "AiPrerequisites.h"
#include "MessageType.h"

namespace rl
{
    class DialogCharacter;

    enum AiMessageTypeIds
    {
        // Controllers //
        // Mask
        RLMSG_CONTROLLERS_EVENTS            = 0x02000100,
        // Events
        RLMSG_CONTROLLERS_DIALOG_STARTED    = 0x02000101,
        RLMSG_CONTROLLERS_DIALOG_ENDED      = 0x02000102,
    };

    /// Message sent when a dialog with the player will be started
    typedef MessageType<RLMSG_CONTROLLERS_DIALOG_STARTED/*, DialogCharacter* */> 
        MessageType_DialogStarted;
}

#endif
