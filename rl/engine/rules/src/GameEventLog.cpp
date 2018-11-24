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

#include "GameEventLog.h"
#include "MessagePump.h"
#include "RulesMessages.h"

using namespace Ogre;

template <> rl::GameEventLog* Singleton<rl::GameEventLog>::msSingleton = 0;

namespace rl
{

    GameEventLog::GameEventLog()
        : mLog()
        , mLogSize(100)
    {
    }

    void GameEventLog::setEventQueueLength(size_t length)
    {
        mLogSize = length;
        while (mLog.size() > mLogSize)
            mLog.pop_front();
    }

    size_t GameEventLog::getEventQueueLength() const
    {
        return mLogSize;
    }

    void GameEventLog::logEvent(const CeGuiString& eventString, GameEventType type)
    {
        LOG_MESSAGE("GameEventLog", getGameEventTypeNamePrefix(type) + eventString);
        mLog.push_back(std::make_pair(type, eventString));
        MessagePump::getSingleton().sendMessage<MessageType_GameEventLog_EventAdded>(type, eventString);
        while (mLog.size() > mLogSize)
            mLog.pop_front();
    }

    CeGuiString GameEventLog::getGameEventTypeNamePrefix(GameEventType type)
    {
        if (type == GET_DIALOG)
        {
            return "[dialog] - ";
        }
        else if (type == GET_COMBAT)
        {
            return "[combat] - ";
        }
        else if (type == GET_QUEST)
        {
            return "[quest] - ";
        }
        else
        {
            return "[default] - ";
        }
    }
}
