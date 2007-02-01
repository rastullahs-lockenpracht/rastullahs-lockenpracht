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

#include "QuestBook.h"
#include "Quest.h"
#include "Exception.h"
#include "ScriptWrapper.h"

namespace rl {

QuestBook::QuestBook()
	: mRootQuest(new Quest("<root>", "<root>", "<root>")) ,
    mJournalEntries(),
	mQuestEventCaster(),
	mJournalEventCaster()
{
	mRootQuest->setQuestBook(this);
	mRootQuest->setState(Quest::OPEN);
}

QuestBook::~QuestBook()
{
	delete mRootQuest;
    for( vector<JournalEntry*>::iterator it = mJournalEntries.begin(); 
        it != mJournalEntries.end(); it++ )
        delete (*it);
    mJournalEntries.clear();

}

Quest* QuestBook::getQuest(const CeGuiString id)
{
	return getQuest(mRootQuest, id);
}

Quest* QuestBook::getQuest(Quest* parent, const CeGuiString id)
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

void QuestBook::_fireQuestBookChanged(Quest *quest, int reason )
{
	QuestEvent* evt = new QuestEvent( this, reason );
	evt->setQuest(quest);
	mQuestEventCaster.dispatchEvent(evt);
	delete evt;
}

void QuestBook::fireJournalChanged(JournalEntry* entry, int reason)
{
	JournalEvent evt = JournalEvent(this, reason, entry);
	mJournalEventCaster.dispatchEvent(&evt);
}

void QuestBook::addQuestListener(QuestListener* listener)
{
	if (mQuestEventCaster.containsListener(listener) !=
        mJournalEventCaster.containsListener(listener))
    {
        Throw(AssertionFailedError, "listener registration inconsistent");
    }
	else if (!mJournalEventCaster.containsListener(listener))
    {    
		mJournalEventCaster.addEventListener(listener);
		mQuestEventCaster.addEventListener(listener);
        ScriptWrapper::getSingleton().owned( listener );
    }
}

void QuestBook::removeQuestListener(QuestListener* listener)
{
	if (mQuestEventCaster.containsListener(listener) !=
        mJournalEventCaster.containsListener(listener))
    {
        Throw(AssertionFailedError, "listener registration inconsistent");
    }
	else if (mJournalEventCaster.containsListener( listener )) 
    {
	    mJournalEventCaster.removeEventListener(listener);
	    mQuestEventCaster.removeEventListener(listener);
        ScriptWrapper::getSingleton().disowned( listener );
    }
}

QuestVector QuestBook::getTopLevelQuests()
{
	return mRootQuest->getSubquests();
}

void QuestBook::addJournalEntry(JournalEntry* entry)
{
    mJournalEntries.push_back(entry);

    LOG_MESSAGE(
        Logger::RULES,
        Ogre::String("Journal entry added: ") + entry->getCaption());

    fireJournalChanged(entry, JournalEvent::JOURNAL_ENTRY_ADDED);
}

void QuestBook::addJournalEntry(CeGuiString caption, CeGuiString text)
{
    addJournalEntry(new JournalEntry(caption, text));
}

unsigned int QuestBook::getNumJournalEntries() const
{
    return mJournalEntries.size();
}

JournalEntry* QuestBook::getJournalEntry(unsigned int index) const
{
    if (mJournalEntries.size() <= index)
    {
        Throw(IllegalArgumentException, "No such JournalEntry.");
    }
    return mJournalEntries[index];
}

}
