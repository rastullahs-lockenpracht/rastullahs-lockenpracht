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

#ifndef __QUESTBOOK_H__
#define __QUESTBOOK_H__

#include "RulesPrerequisites.h"

#include "EventSource.h"
#include "EventCaster.h"

#include "Quest.h"
#include "QuestChangeEvent.h"
#include "QuestChangeListener.h"

namespace rl {


class _RlRulesExport QuestBook : public EventSource
{
public:
	QuestBook();
	~QuestBook();


	/**
	 * Sucht einen Quest anhand einer ID
	 * @return der Quest
	 */
	Quest* getQuest(const CeGuiString& id);

	QuestVector getTopLevelQuests();

	/**
	 * Fuegt einen Quest hinzu
	 * @param quest der Qubquest
	 */
	void addQuest(Quest* quest);

	/**
	 * Sendet das Ereignis, dass sich ein Quest geändert hat
	 * @param quest der Quest, der verändert wurde
	 */
	void fireQuestChanged( Quest* quest, int reason );

	void addQuestChangeListener(QuestChangeListener* listener);
	void removeQuestChangeListener(QuestChangeListener* listener);

private:
	Quest* getQuest(Quest* parent, const CeGuiString& id);
	Quest* mRootQuest;
	EventCaster<QuestChangeEvent> mEventCaster;
};

}
#endif
