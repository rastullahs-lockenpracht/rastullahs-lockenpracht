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

CeGuiString	Quest::STATE_NAMES[5] = {"UNKNOWN", "OPEN", "FAILED", "COMPLETED", "CLOSED"};

Quest::Quest(const CeGuiString& id, const CeGuiString& name, const CeGuiString& description)
:	mId(id),
	mName(name),
	mDescription(description),
	mPartsToDo(1),
	mPartsDone(0),
	mState(Quest::UNKNOWN),
	mParent(NULL),
	mQuestBook(NULL),
	mKnown(false)
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

const CeGuiString& Quest::getStateName()
{
	return Quest::STATE_NAMES[mState];
}

void Quest::setState(Quest::State state)
{
	mState = state;

	if (state == Quest::OPEN && mParent != NULL
		&& mParent->getState() == Quest::UNKNOWN)
	{
		mParent->setState(Quest::OPEN);
	}

	notify();
	checkDone();
}

QuestVector Quest::getSubquests()
{
	return mSubquests;
}

void Quest::addSubquest(Quest* quest)
{
	if (mQuestBook->getQuest(quest->getId()) != NULL)
		Throw(rl::DuplicateIdException, ("Duplicate quest id " + quest->getId()).c_str());

	mSubquests.push_back(quest);
	quest->setParent(this);
	quest->setQuestBook(mQuestBook);
	notify();
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
		&& mState != Quest::COMPLETED
		&& mState != Quest::CLOSED)
	{
		mState = Quest::COMPLETED;
		notify();
	}

	if (mParent != NULL)
		mParent->checkDone();
}

void Quest::notify()
{
	if (mQuestBook != NULL)
		mQuestBook->fireQuestStateChanged(this);
}

void Quest::setQuestBook(QuestBook* questBook)
{
	mQuestBook = questBook;
}

Quest::State Quest::getStateFromName(const CeGuiString& stateName)
{
	if (stateName == Quest::STATE_NAMES[Quest::UNKNOWN]) 
		return Quest::UNKNOWN;
	if (stateName == Quest::STATE_NAMES[Quest::OPEN]) 
		return Quest::OPEN;
	if (stateName == Quest::STATE_NAMES[Quest::FAILED]) 
		return Quest::FAILED;
	if (stateName == Quest::STATE_NAMES[Quest::COMPLETED]) 
		return Quest::COMPLETED;
	if (stateName == Quest::STATE_NAMES[Quest::CLOSED]) 
		return Quest::CLOSED;

	const char* msg = (stateName + " is no valid quest state.").c_str();
	Throw(InvalidArgumentException, msg);
	return Quest::UNKNOWN;
}

bool Quest::isKnown()
{
	return mKnown;
}

void Quest::setKnown(bool known)
{
	mKnown = known;

	if (known == true && mParent != NULL && !mParent->isKnown())
		mParent->setKnown(true);

	notify();
}


}
