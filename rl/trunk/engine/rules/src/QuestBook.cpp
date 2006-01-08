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

#include "QuestBook.h"
#include "Quest.h"

namespace rl {

QuestBook::QuestBook()
	: mRootQuest(new Quest("<root>", "<root>", "<root>")) ,
	mEventCaster()
{
	mRootQuest->setQuestBook(this);
	mRootQuest->setState(Quest::OPEN);
}

QuestBook::~QuestBook()
{
	delete mRootQuest;
}

Quest* QuestBook::getQuest(const CeGuiString& id)
{
	return getQuest(mRootQuest, id);
}

Quest* QuestBook::getQuest(Quest* parent, const CeGuiString& id)
{
	if (parent->getId().compare(id) == 0)
		return parent;

	QuestVector children = parent->getSubquests();
	for(QuestVector::iterator it = children.begin(); it != children.end(); it++)
	{
		Quest* subquest = getQuest(*it, id);
		if (subquest != NULL)
			return subquest;
	}

	return NULL;
}

void QuestBook::addQuest(Quest* quest)
{
	mRootQuest->addSubquest(quest);
}

void QuestBook::fireQuestChanged( Quest *quest, int reason )
{
	QuestChangeEvent* evt = new QuestChangeEvent( this, reason );
	evt->setQuest(quest);
	mEventCaster.dispatchEvent(evt);
	delete evt;
}

void QuestBook::addQuestChangeListener(QuestChangeListener* listener)
{
	mEventCaster.addEventListener(listener);
}

void QuestBook::removeQuestChangeListener(QuestChangeListener* listener)
{
	mEventCaster.removeEventListener(listener);
}

QuestVector QuestBook::getTopLevelQuests()
{
	return mRootQuest->getSubquests();
}

}
