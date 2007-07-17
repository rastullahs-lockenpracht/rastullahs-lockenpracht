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

#ifndef __Rl_CoreMessages_H__
#define __Rl_CoreMessages_H__

#include "CorePrerequisites.h"
#include "MessageType.h"

namespace rl
{
    enum CoreMessageTypeIds
    {
        RLMSG_SCENE_LOADED = 0x01000001,
        RLMSG_SCENE_CLEARING = 0x01000002
    };

    /// Message sent right after a scene has been loaded.
    typedef MessageType<RLMSG_SCENE_LOADED> MessageType_SceneLoaded;

    /// Message sent just before a scene will be cleared.
    typedef MessageType<RLMSG_SCENE_CLEARING> MessageType_SceneClearing;
}
#endif
