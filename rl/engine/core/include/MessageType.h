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

#ifndef __Rl_MessageType_H__
#define __Rl_MessageType_H__

#include "CorePrerequisites.h"
#include "MessageObject.h"

namespace rl
{
    template<int _MessageTypeId, typename Param1, typename Param2, typename Param3>
    class MessageType
    {
    public:
        static const int MessageTypeId = _MessageTypeId;
        typedef MessageObject<_MessageTypeId, Param1, Param2, Param3> MessageObjectType;
        typedef typename MessageObjectType::HandlerType HandlerType;
    };
}
#endif
