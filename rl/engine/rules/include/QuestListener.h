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
#ifndef __QuestListener_H__
#define __QuestListener_H__

#include "EventListener.h"
#include "JournalEntry.h"
#include "QuestEvent.h"
#include "RulesPrerequisites.h"

namespace rl
{

    class _RlRulesExport QuestListener : public EventListener<QuestEvent>, public EventListener<JournalEvent>
    {
    public:
        /// Destruktor
        virtual ~QuestListener();

        /// Kuemmert sich um das Aufrufen benannter Methoden aus Reasons
        virtual bool eventRaised(QuestEvent* anEvent);

        virtual bool eventRaised(JournalEvent* anEvent);

        /** Diese Methode wird aufgerufen, wenn sich der Status des Quests geaendert hat
         * @param anEvent Der ausloesende Event
         */
        virtual void questStateChanged(QuestEvent* anEvent) = 0;
        virtual void questPartsDoneChanged(QuestEvent* anEvent) = 0;
        virtual void questKnownChanged(QuestEvent* anEvent) = 0;
        virtual void questSubquestAdded(QuestEvent* anEvent) = 0;

        virtual void journalEntryAdded(JournalEvent* anEvent) = 0;
        virtual void journalEntryDeleted(JournalEvent* anEvent) = 0;
    };
}

#endif // __QuestEventListener_H__
