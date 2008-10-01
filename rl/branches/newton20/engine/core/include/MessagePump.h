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

#ifndef __Rl_MessagePump_H__
#define __Rl_MessagePump_H__

#include "CorePrerequisites.h"

#include "MessageType.h"
#include "GameTask.h"


#include <map>
#include <queue>

#include <boost/noncopyable.hpp>

namespace rl
{

    class MessageHandlerWrapperBase
    {
    public:
        virtual ~MessageHandlerWrapperBase() {}
        virtual bool Invoke(MessageObjectBase* obj) = 0;
    };

    template<class _MessageType>
    class MessageHandlerWrapper : public MessageHandlerWrapperBase
    {
    public:
        typedef typename _MessageType::MessageObjectType MessageObjectType;
        typedef typename _MessageType::HandlerType HandlerType;

        MessageHandlerWrapper(const HandlerType& handler)
            : mHandler(handler)
        {
        }

        virtual bool Invoke(MessageObjectBase* obj)
        {
            MessageObjectType* obj2 = static_cast<MessageObjectType*>(obj);
            return obj2->Invoke(mHandler);
        }

    private:
        HandlerType mHandler;
    };

    class _RlCoreExport MessagePump : public GameTask, public Ogre::Singleton<MessagePump>
    {
    public:
        class Connection
        {
            friend class MessagePump;
        public:
            Connection()
                : connectionId(-1), pump(NULL)
            {}
            void disconnect()
            {
                if(pump && connectionId != -1)
                    pump->disconnectHandler(connectionId);
            }
        private:
            MessagePump* pump;
            int connectionId;
        };

        class ScopedConnection : boost::noncopyable
        {
        public:
            ScopedConnection()
            {}
            ScopedConnection(const Connection& conn)
                : mConnection(conn)
            {}
            ScopedConnection& operator =(const Connection& conn)
            {
                mConnection.disconnect();
                mConnection = conn;
                return *this;
            }
            ~ScopedConnection()
            {
                mConnection.disconnect();
            }
        private:
            Connection mConnection;
        };

    private:
        struct MessageHandlerMapEntry
        {
            int connectionId;
            MessageHandlerWrapperBase* handlerWrapper;
        };

        typedef std::vector<MessageHandlerMapEntry> MessageHandlerMapEntries;
        typedef std::map<int, MessageHandlerMapEntries*> MessageHandlerMap;
        typedef std::queue<MessageObjectBase*> MessageQueue;

    public:
        MessagePump() : mNextConnectionId(0) {}
        ~MessagePump();

        template<typename _MessageType>
        Connection addMessageHandler( const typename _MessageType::HandlerType& handler )
        {
            MessageHandlerWrapper<_MessageType>* wrapper =
                new MessageHandlerWrapper<_MessageType>(handler);

            MessageHandlerMapEntries* entries = getOrCreateMapEntries(_MessageType::MessageTypeId);
            MessageHandlerMapEntry entry = {mNextConnectionId++, wrapper};
            entries->push_back(entry);

            LOG_MESSAGE("MessagePump", 
                "Added message handler for message type " 
                + Ogre::StringConverter::toString(_MessageType::MessageTypeId));

            Connection con;
            con.pump = this;
            con.connectionId = entry.connectionId;
            return con;
        }

        /*
        * sendMessage and postMessage is implemented for all possible parameter counts
        * Thanks to the idea of SFINAE, the compiler will choose 
        * the correct implementation for all MessageTypes at compile time
        *
        * sendMessage processes the message immediatly, while postMessage delays the 
        * processing to a later frame
        */
        template<typename _MessageType>
        bool sendMessage()
        {
            MessageObjectBase* obj = _MessageType::MessageObjectType::Build();
            return doSend(obj);
        }

        template<typename _MessageType>
        bool sendMessage(const typename _MessageType::MessageObjectType::Param1& p1)
        {
            MessageObjectBase* obj = _MessageType::MessageObjectType::Build(p1);
            return doSend(obj);
        }

        template<typename _MessageType>
        bool sendMessage(const typename _MessageType::MessageObjectType::Param1& p1,
            const typename _MessageType::MessageObjectType::Param2& p2)
        {
            MessageObjectBase* obj = _MessageType::MessageObjectType::Build(p1, p2);
            return doSend(obj);
        }

        template<typename _MessageType>
        bool sendMessage(const typename _MessageType::MessageObjectType::Param1& p1,
            const typename _MessageType::MessageObjectType::Param2& p2,
            const typename _MessageType::MessageObjectType::Param2& p3)
        {
            MessageObjectBase* obj = _MessageType::MessageObjectType::Build(p1, p2, p3);
            return doSend(obj);
        }

        ////////////////////////////////

        template<typename _MessageType>
        void postMessage()
        {
            MessageObjectBase* obj = _MessageType::MessageObjectType::Build();
            doPost(obj);
        }

        template<typename _MessageType>
        void postMessage(const typename _MessageType::MessageObjectType::Param1& p1)
        {
            MessageObjectBase* obj = _MessageType::MessageObjectType::Build(p1);
            doPost(obj);
        }

        template<typename _MessageType>
        void postMessage(const typename _MessageType::MessageObjectType::Param1& p1,
            const typename _MessageType::MessageObjectType::Param2& p2)
        {
            MessageObjectBase* obj = _MessageType::MessageObjectType::Build(p1,p2);
            doPost(obj);
        }

        template<typename _MessageType>
        void postMessage(const typename _MessageType::MessageObjectType::Param1& p1,
            const typename _MessageType::MessageObjectType::Param2& p2,
            const typename _MessageType::MessageObjectType::Param2& p3)
        {
            MessageObjectBase* obj = _MessageType::MessageObjectType::Build(p1, p2, p3);
            doPost(obj);
        }

        void sendPending()
        {
            while(!mMessageQueue.empty())
            {
                doSend(mMessageQueue.front());
                mMessageQueue.pop();
            }
        }

        // overloads from GameTask

        virtual void run(Ogre::Real elapsedTime);

        virtual const Ogre::String& getName() const;

    private:
        MessageHandlerMapEntries* getOrCreateMapEntries(int id) 
        {
            LOG_MESSAGE("MessagePump", "Create or get id " + Ogre::StringConverter::toString(id));
            MessageHandlerMap::iterator it = mMessageHandlerMap.find(id);
            if(it == mMessageHandlerMap.end())
            {
                MessageHandlerMapEntries* entries = new MessageHandlerMapEntries();
                mMessageHandlerMap[id] = entries;
                return entries;
            }
            return it->second;
        }

        bool doSend(MessageObjectBase* msg)
        {
            bool msgHandled = false;
            MessageHandlerMapEntries* entries = getOrCreateMapEntries(msg->getMessageTypeId());
            for(MessageHandlerMapEntries::iterator it = entries->begin(); it != entries->end(); ++it)
            {
                if((*it).handlerWrapper->Invoke(msg))
                {
                    msgHandled = true;
                }
            }
            delete msg;
            return msgHandled;;
        }

        void doPost(MessageObjectBase* msg)
        {
            mMessageQueue.push(msg);
        }

        friend class Connection;
        void disconnectHandler(int connectionId)
        {
            //not very performant...definitely needs improvement
            for (MessageHandlerMap::iterator it = mMessageHandlerMap.begin();
                it != mMessageHandlerMap.end(); ++it)
            {
                MessageHandlerMapEntries* en = it->second;
                for (MessageHandlerMapEntries::iterator jt = en->begin(); jt != en->end(); ++jt)
                {
                    if (jt->connectionId == connectionId)
                    {
                        delete jt->handlerWrapper;
                        en->erase(jt);
                        if (en->empty())
                        {
                            delete en;
                            mMessageHandlerMap.erase(it);
                        }
                        return;
                    }
                }
            }
        }

    private:
        int mNextConnectionId;
        MessageHandlerMap mMessageHandlerMap;
        MessageQueue mMessageQueue;
    };
}
#endif
