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
#ifndef __QuestBookChangeEventListener_H__
#define __QuestBookChangeEventListener_H__

#include "RulesPrerequisites.h"
#include "EventListener.h"
#include "QuestBookChangeEvent.h"
#include "JournalEntry.h"

namespace rl {


class _RlRulesExport QuestBookChangeListener
    : public EventListener<QuestBookChangeEvent>,
      public EventListener<JournalEvent>
{
public:
    /// Destruktor
	virtual ~QuestBookChangeListener();

    /// Kümmert sich um das Aufrufen benannter Methoden aus Reasons
	virtual bool eventRaised(QuestBookChangeEvent *anEvent);

	virtual bool eventRaised(JournalEvent *anEvent);

	/** Diese Methode wird aufgerufen, wenn sich der Status des Quests geändert hat
      * @param anEvent Der auslösende Event
      */  
	virtual void questStateChanged(QuestBookChangeEvent* anEvent) = 0;
	virtual void questPartsDoneChanged(QuestBookChangeEvent* anEvent) = 0;
	virtual void questKnownChanged(QuestBookChangeEvent* anEvent) = 0;
	virtual void questSubquestAdded(QuestBookChangeEvent* anEvent) = 0;

	virtual void journalEntryAdded(JournalEvent* anEvent) = 0;
};

}

#endif // __QuestBookChangeEventListener_H__
