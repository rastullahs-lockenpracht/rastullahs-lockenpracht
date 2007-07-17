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

#ifndef __Rl_MessageObject_H__
#define __Rl_MessageObject_H__

#include "CorePrerequisites.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>

namespace rl
{

    //This class is a dummy that indicated that a parameter is not used
    class MessageObject_EmptyParam 
    {};

    template<int MessageTypeId,
        typename Param1 = MessageObject_EmptyParam,
        typename Param2 = MessageObject_EmptyParam>
    class MessageType;

    class MessageObjectBase
    {
    public:
        virtual ~MessageObjectBase() {}
        virtual int getMessageTypeId() = 0;
    };

    //MessageObject with 0 parameters
    class MessageObject_0 : public MessageObjectBase
    {
    public:
        typedef boost::function< bool () > HandlerType;

        template<typename Handler>
        bool Invoke(const Handler& handler)
        {
            return handler();
        }
    };

    //MessageObject with 1 parameters
    template<typename _Param1>
    class MessageObject_1 : public MessageObjectBase
    {
    public:
        typedef _Param1 Param1;
        Param1 param1;

        typedef boost::function< bool (const Param1& p1) > HandlerType;

        template<typename Handler>
        bool Invoke(const Handler& handler)
        {
            return handler(param1);
        }
    };

    //MessageObject with 2 parameters
    template<typename _Param1, typename _Param2>
    class MessageObject_2 : public MessageObjectBase
    {
    public:
        typedef _Param1 Param1;
        typedef _Param2 Param2;

        Param1 param1;
        Param2 param2;

        typedef boost::function< bool (const Param1& p1, const Param2& p2) > HandlerType;

        template<typename Handler>
        bool Invoke(const Handler& handler)
        {
            return handler(param1, param2);
        }
    };

    //forward declaration of MessageObject_x
    //It is implemented in several specializations, 
    //each derives from another MessageObject base 
    //to select the correct parameter count
    template<int _MessageTypeId, typename Param1, typename Param2>
    class MessageObject_x;

    template<int _MessageTypeId>
    class MessageObject_x<_MessageTypeId, MessageObject_EmptyParam, MessageObject_EmptyParam>
        : public MessageObject_0
    {
    public:
        static MessageObject_x* Build()
        {
            MessageObject_x* msgObj = new MessageObject<_MessageTypeId>();
            return msgObj;
        }
    };

    template<int _MessageTypeId, typename Param1>
    class MessageObject_x<_MessageTypeId, Param1, MessageObject_EmptyParam>
        : public MessageObject_1<Param1>
    {
    public:
        static MessageObject_x* Build(const Param1& p1)
        {
            MessageObject_x* msgObj = new MessageObject<_MessageTypeId, Param1>();
            msgObj->param1 = p1;
            return msgObj;
        }
    };

    template<int _MessageTypeId, typename Param1, typename Param2>
    class MessageObject_x : public MessageObject_2<Param1, Param2>
    {
    public:
        static MessageObject_x* Build(const Param1& p1, const Param2& p2)
        {
            MessageObject_x* msgObj = new MessageObject<_MessageTypeId, Param1, Param2>();
            msgObj->param1 = p1;
            msgObj->param2 = p2;
            return msgObj;
        }
    };

    /////////////////////////////////////////////////////

    //The final MessageObject. The real implementation is chosen through the given template parameters
    template<int _MessageTypeId,
        typename Param1 = MessageObject_EmptyParam,
        typename Param2 = MessageObject_EmptyParam>
    class MessageObject : public MessageObject_x<_MessageTypeId, Param1, Param2>
    {
    public:
        virtual int getMessageTypeId()
        {
            return _MessageTypeId;
        }
    };

}

#endif
