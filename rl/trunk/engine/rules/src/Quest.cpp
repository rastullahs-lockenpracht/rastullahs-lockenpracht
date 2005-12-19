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

#include "Quest.h"

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
		if ((*it)->getState() == Quest::DONE)
			done++;
	}
	return done;
}

void Quest::setPartsDone(int partsDone)
{
	mPartsDone = partsDone;
}

Quest::State Quest::getState()
{
	return mState;
}

void Quest::setState(Quest::State state)
{
	mState = state;
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
		mState == Quest::DONE;
	}

	if (mParent != NULL)
		mParent->checkDone();
}

}
