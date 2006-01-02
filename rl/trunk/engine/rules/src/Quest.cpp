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
#include <vector>
#include <map>

#include "Quest.h"
#include "QuestBook.h"
#include "Exception.h"

namespace rl {

Quest::Quest(const CeGuiString& id, const CeGuiString& name, const CeGuiString& description)
:	mId(id),
	mName(name),
	mDescription(description),
	mPartsToDo(1),
	mPartsDone(0),
	mState(Quest::UNKNOWN),
	mParent(NULL)
{
}

Quest::~Quest()
{
}

const CeGuiString& Quest::getId()
{
	return mId;
}

const CeGuiString& Quest::getName()
{
	return mName;
}

const CeGuiString& Quest::getDescription()
{
	return mDescription;
}

int Quest::getPartsToDo()
{
	if (mSubquests.size() == 0)
		return mPartsToDo;
	return mSubquests.size();
}

void Quest::setPartsToDo(int partsToDo)
{
	mPartsToDo = partsToDo;
}

int Quest::getPartsDone()
{
	if (mSubquests.size() == 0)
		return mPartsDone;

	int done = 0;
	for(QuestVector::iterator it = mSubquests.begin(); it != mSubquests.end(); it++)
	{
		Quest* cur = (*it);
		if (cur->getState() == Quest::COMPLETED || cur->getState() == Quest::CLOSED)
			done++;
	}
	return done;
}

void Quest::setPartsDone(int partsDone)
{
	mPartsDone = partsDone;
	notify();
	checkDone();
}

Quest::State Quest::getState()
{
	return mState;
}

void Quest::setState(Quest::State state)
{
	mState = state;

	if (state == Quest::OPEN && mParent != 0 
		&& mParent->getState() == Quest::UNKNOWN)
		mParent->setState(Quest::OPEN);

	notify();
	checkDone();
}

QuestVector Quest::getSubquests()
{
	return mSubquests;
}

void Quest::addSubquest(Quest* quest)
{
	mSubquests.push_back(quest);
	quest->setParent(this);
	quest->setQuestBook(mQuestBook);
}

void Quest::setParent(Quest* quest)
{
	mParent = quest;
}

Quest* Quest::getParent()
{
	return mParent;
}

void Quest::checkDone()
{
	if (getPartsDone() == getPartsToDo()
		&& mState == Quest::OPEN)
	{
		mState = Quest::COMPLETED;
		notify();
	}

	if (mParent != NULL)
		mParent->checkDone();
}

void Quest::notify()
{
	mQuestBook->fireQuestStateChanged(this);
}

void Quest::setQuestBook(QuestBook* questBook)
{
	mQuestBook = questBook;
}

Quest::State Quest::getStateFromName(const CeGuiString& stateName)
{
	if (stateName == "UNKNOWN") 
		return Quest::UNKNOWN;
	if (stateName == "OPEN") 
		return Quest::OPEN;
	if (stateName == "FAILED") 
		return Quest::FAILED;
	if (stateName == "COMPLETED") 
		return Quest::COMPLETED;
	if (stateName == "CLOSED") 
		return Quest::CLOSED;

	Throw(InvalidArgumentException, (stateName + " is no valid quest state.").c_str());
	return Quest::UNKNOWN;
}


}
