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
#include "QuestChangeEvent.h"
#include "QuestChangeListener.h"

#include "Exception.h"

namespace rl {

CeGuiString	Quest::STATE_NAMES[5] = 
		{	"OPEN",		"ASSIGNED",	"FAILED", "SUCCEEDED", "COMPLETED"};
CeGuiString	Quest::KNOWN_NAMES[2] = 
		{	"UNKNOWN",	"KNOWN"};

Quest::Quest(const CeGuiString id, const CeGuiString name, const CeGuiString description)
:	mId(id),
	mName(name),
	mDescription(description),
	mPartsToDo(1),
	mPartsDone(0),
	mState(Quest::OPEN),
	mKnown(false),
	mParent(NULL),
	mQuestBook(NULL)	
{
}

Quest::~Quest()
{
}

const CeGuiString Quest::getId()
{
	return mId;
}

const CeGuiString Quest::getName()
{
	return mName;
}

const CeGuiString Quest::getDescription()
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
	if (!hasSubquests())
		return mPartsDone;

	int done = 0;
	for(QuestVector::iterator it = mSubquests.begin(); 
		it != mSubquests.end(); it++)
	{
		Quest* cur = (*it);
		if (cur->getState() == Quest::COMPLETED 
			|| cur->getState() == Quest::SUCCEEDED)
			done++;
	}
	return done;
}

void Quest::setPartsDone(int partsDone)
{
	if (hasSubquests())
		return;

	if( partsDone != mPartsDone )
	{
		mPartsDone = partsDone;
		notify(QuestChangeEvent::QUEST_PARTSDONE);
		checkDone();
	}
}

void Quest::increasePartsDone(int parts)
{
	if (hasSubquests())
		return;

	if (parts != 0)
	{
		mPartsDone += parts;
		notify(QuestChangeEvent::QUEST_PARTSDONE);
		checkDone();
	}
}

void Quest::decreasePartsDone(int parts)
{
	increasePartsDone(-parts);
}


Quest::State Quest::getState()
{
	return mState;
}

const CeGuiString Quest::getStateName()
{
	return Quest::STATE_NAMES[mState];
}

const CeGuiString Quest::getKnownName()
{
	if (mKnown)
	{
		return Quest::KNOWN_NAMES[Quest::KNOWN];
	}
	else
	{
		return Quest::KNOWN_NAMES[Quest::UNKNOWN];
	}
}

void Quest::setState(Quest::State state)
{
	if( mState != state )
	{
		mState = state;

		notify(QuestChangeEvent::QUEST_STATE);

		if (state == Quest::OPEN && mParent != NULL
			&& mParent->getState() == Quest::UNKNOWN)
		{
			mParent->setState(Quest::OPEN);
		}

		if( mState == Quest::COMPLETED || mState == Quest::SUCCEEDED)
			mParent->checkDone();
		else
			checkDone();
	}
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
	notify(QuestChangeEvent::QUEST_SUBQUEST);
}

bool Quest::hasSubquests()
{
	return mSubquests.size() > 0;
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
	if (getPartsDone() >= getPartsToDo()
		&& mState != Quest::COMPLETED
		&& mState != Quest::SUCCEEDED)
	{
		mState = Quest::SUCCEEDED;
		notify(QuestChangeEvent::QUEST_STATE);
	}

	if (mParent != NULL)
		mParent->checkDone();
}

void Quest::notify( int reason )
{
	if (mQuestBook != NULL)
		mQuestBook->fireQuestChanged(this, reason);
}

void Quest::setQuestBook(QuestBook* questBook)
{
	mQuestBook = questBook;
}

Quest::State Quest::getStateFromName(const CeGuiString stateName)
{
	if (stateName == Quest::STATE_NAMES[Quest::OPEN]) 
		return Quest::OPEN;
	else if (stateName == Quest::STATE_NAMES[Quest::FAILED]) 
		return Quest::FAILED;
	else if (stateName == Quest::STATE_NAMES[Quest::COMPLETED]) 
		return Quest::COMPLETED;
	else if (stateName == Quest::STATE_NAMES[Quest::SUCCEEDED]) 
		return Quest::SUCCEEDED;
	else if (stateName == Quest::STATE_NAMES[Quest::ASSIGNED]) 
		return Quest::ASSIGNED;

	const char* msg = (stateName + " is no valid quest state.").c_str();
	Throw(IllegalArgumentException, msg);
}

bool Quest::getKnownFromName(const CeGuiString knownName)
{
	if (knownName == Quest::KNOWN_NAMES[Quest::UNKNOWN])
		return false;
	else if (knownName == Quest::KNOWN_NAMES[Quest::KNOWN])
		return true;

	Throw(
		IllegalArgumentException, 
		(knownName + " is no valid quest known state.").c_str());
}

bool Quest::isKnown()
{
	return mKnown;
}

void Quest::setKnown(bool known)
{
	if( known != mKnown )
	{
		mKnown = known;

		if (known == true && mParent != NULL && !mParent->isKnown())
			mParent->setKnown(true);

		notify(QuestChangeEvent::QUEST_KNOWN);
	}
}

}
