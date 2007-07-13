/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include <vector>
#include <map>

#include "Quest.h"

#include "Property.h"
#include "QuestBook.h"
#include "QuestEvent.h"
#include "QuestListener.h"
#include "ScriptWrapper.h"

#include "Exception.h"

namespace rl {

const CeGuiString	Quest::STATE_NAMES[5] =
		{	"OPEN",
            "ASSIGNED",
            "FAILED",
            "SUCCEEDED",
            "COMPLETED"};

const CeGuiString	Quest::KNOWN_NAMES[2] =
		{	"UNKNOWN",
            "KNOWN"};

const Ogre::String Quest::PROP_NAME = "str_name";
const Ogre::String Quest::PROP_DESCRIPTION = "str_description";
const Ogre::String Quest::PROP_KNOWN = "bool_known";

Quest::Quest(const CeGuiString id, const CeGuiString name, const CeGuiString description)
:	PropertySet(),
    mId(id),
	mName(name),
	mDescription(description),
	mPartsToDo(1),
	mPartsDone(0),
	mState(Quest::OPEN),
	mKnown(false),
	mParent(NULL),
	mQuestBook(NULL),
    mSubquests()
{
}

Quest::~Quest()
{
    for( QuestVector::iterator it = mSubquests.begin();
        it != mSubquests.end(); it++ )
           ScriptWrapper::getSingleton().disowned( (*it) );
    mSubquests.clear();
}

const CeGuiString Quest::getId() const
{
	return mId;
}

const CeGuiString Quest::getName() const
{
	return mName;
}

const CeGuiString Quest::getDescription() const
{
	return mDescription;
}

int Quest::getPartsToDo() const
{
	if (mSubquests.size() == 0)
		return mPartsToDo;
	return mSubquests.size();
}

void Quest::setPartsToDo(int partsToDo)
{
	mPartsToDo = partsToDo;
}

int Quest::getPartsDone() const
{
	if (!hasSubquests())
		return mPartsDone;

	int done = 0;
	for(QuestVector::const_iterator it = mSubquests.begin();
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
		notify(QuestEvent::QUEST_PARTSDONE);
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
		notify(QuestEvent::QUEST_PARTSDONE);
		checkDone();
	}
}

void Quest::decreasePartsDone(int parts)
{
	increasePartsDone(-parts);
}


Quest::State Quest::getState() const
{
	return mState;
}

const CeGuiString Quest::getStateName() const
{
	return Quest::STATE_NAMES[mState];
}

const CeGuiString Quest::getKnownName() const
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

		notify(QuestEvent::QUEST_STATE);

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

QuestVector Quest::getSubquests() const
{
	return mSubquests;
}

void Quest::addSubquest(Quest* quest)
{
	if (mQuestBook->getQuest(quest->getId()) != NULL)
		Throw(rl::DuplicateIdException, ("Duplicate quest id " + quest->getId()).c_str());

	mSubquests.push_back(quest);
    ScriptWrapper::getSingleton().owned( quest );
	quest->setParent(this);
	quest->setQuestBook(mQuestBook);
	notify(QuestEvent::QUEST_SUBQUEST);
}

bool Quest::hasSubquests() const
{
	return mSubquests.size() > 0;
}

void Quest::setParent(Quest* quest)
{
	mParent = quest;
}

Quest* Quest::getParent() const
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
		notify(QuestEvent::QUEST_STATE);
	}

	if (mParent != NULL)
		mParent->checkDone();
}

void Quest::notify( int reason )
{
	if (mQuestBook != NULL)
		mQuestBook->_fireQuestBookChanged(this, reason);
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

bool Quest::isKnown() const
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

		notify(QuestEvent::QUEST_KNOWN);
	}
}

const Property Quest::getProperty(const Ogre::String& key) const
{
    if (key == PROP_KNOWN)
    {
        return Property(mKnown);
    }
    else if (key == PROP_NAME)
    {
        return Property(mName);
    }
    else if (key == PROP_DESCRIPTION)
    {
        return Property(mDescription);
    }
    else
    {
        return PropertySet::getProperty(key);
    }
}

void Quest::setProperty(const Ogre::String& key, const Property& value)
{
    if (key == PROP_KNOWN)
    {
        mKnown = value.toBool();
    }
    else if (key == PROP_NAME)
    {
        mName = value.toString();
    }
    else if (key == PROP_DESCRIPTION)
    {
        mDescription = value.toString();
    }
    else
    {
        PropertySet::setProperty(key, value);
    }
}

PropertySet* Quest::getAllProperties() const
{
    PropertySet* ps = PropertySet::getAllProperties();

    ps->setProperty(PROP_NAME, Property(mName));
    ps->setProperty(PROP_DESCRIPTION, Property(mDescription));
    ps->setProperty(PROP_KNOWN, Property(mKnown));

    return ps;
}

}
