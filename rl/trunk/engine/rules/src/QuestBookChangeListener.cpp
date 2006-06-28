/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include "QuestBookChangeListener.h"

#include "Exception.h"

namespace rl {

    QuestBookChangeListener::~QuestBookChangeListener()
    {
    }

    bool QuestBookChangeListener::eventRaised(QuestBookChangeEvent* evt)
    {
        try
        {
            if( evt->getReason() == QuestBookChangeEvent::QUEST_STATE )
                questStateChanged(evt);
            else if( evt->getReason() == QuestBookChangeEvent::QUEST_PARTSDONE )
                questPartsDoneChanged(evt);
            else if( evt->getReason() == QuestBookChangeEvent::QUEST_KNOWN )
                questKnownChanged(evt);
            else if( evt->getReason() == QuestBookChangeEvent::QUEST_SUBQUEST )
                questSubquestAdded(evt);
        }
        catch( ScriptInvocationFailedException& sife )
        {
            Logger::getSingleton().log(Logger::RULES, Logger::LL_ERROR, sife.toString() );
        }

        // consumed or not
        return false;
    }

    bool QuestBookChangeListener::eventRaised(JournalEvent* evt)
    {
        try
        {
            if (evt->getReason() == JournalEvent::JOURNAL_ENTRY_ADDED)
            {
                journalEntryAdded(evt);
            }
        }
        catch (ScriptInvocationFailedException& sife)
        {
            Logger::getSingleton().log(Logger::RULES, Logger::LL_ERROR, sife.toString());
        }

        // consumed or not
        return false;
    }
}
