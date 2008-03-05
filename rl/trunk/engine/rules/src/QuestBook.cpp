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

#include "QuestBook.h"
#include "Quest.h"
#include "Exception.h"
#include "ScriptWrapper.h"

using namespace std;
using namespace XERCES_CPP_NAMESPACE;

namespace rl {
    const Ogre::String QuestBook::PROPERTY_QUESTS = "quests";
    const Ogre::String QuestBook::PROPERTY_JOURNAL = "journal";

    QuestBook::QuestBook()
	    : mJournalEntries(),
	    mQuestEventCaster(),
	    mJournalEventCaster(),
        ScriptLoader()
    {
        mScriptPatterns.push_back("*.quests");
        createRoot();
        SaveGameManager::getSingleton().registerSaveGameData(this);
    }

    QuestBook::~QuestBook()
    {
        SaveGameManager::getSingleton().unregisterSaveGameData(this);
	    delete mRootQuest;
        for( vector<JournalEntry*>::iterator it = mJournalEntries.begin();
            it != mJournalEntries.end(); it++ )
        {
            delete *it;
        }
        mJournalEntries.clear();
    }

    void QuestBook::createRoot()
    {
        mRootQuest = new Quest("<root>", "<root>", "<root>"); 
	    mRootQuest->setQuestBook(this);
	    mRootQuest->setState(Quest::OPEN);
    }

    Quest* QuestBook::getQuest(const CeGuiString id) const
    {
	    return getQuest(mRootQuest, id);
    }

    Quest* QuestBook::getQuest(Quest* parent, const CeGuiString id) const
    {
	    if (parent->getId().compare(id) == 0)
		    return parent;

	    QuestVector children = parent->getSubquests();
	    for(QuestVector::iterator it = children.begin(); it != children.end(); it++)
	    {
		    Quest* subquest = getQuest(*it, id);
		    if (subquest)
		    {
			    return subquest;
		    }
	    }

	    return NULL;
    }

    void QuestBook::addQuest(Quest* quest)
    {
	    mRootQuest->addSubquest(quest);
    }

    void QuestBook::_fireQuestBookChanged(Quest *quest, int reason)
    {
	    QuestEvent* evt = new QuestEvent(this, reason);
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

    QuestVector QuestBook::getTopLevelQuests() const
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

    const Property QuestBook::getProperty(const Ogre::String& key) const
    {
        if (key == PROPERTY_QUESTS)
        {
            PropertyArray quests;
            QuestVector allQuests = getAllQuests();
            for (QuestVector::const_iterator it = allQuests.begin();
                it != allQuests.end(); ++it)
            {
                PropertyRecord* questProps = (*it)->getAllProperties();
                quests.push_back(Property(questProps->toPropertyMap()));
                delete questProps;
            }

            return Property(quests);
        }
        else if (key == PROPERTY_JOURNAL)
        {
            PropertyArray journals;
            for(std::vector<JournalEntry*>::const_iterator iter = mJournalEntries.begin(); iter != mJournalEntries.end(); iter++)
            {
                PropertyRecord journal;
                journal.setProperty(JournalEntry::PROPERTY_CAPTION, Property((*iter)->getCaption()));
                journal.setProperty(JournalEntry::PROPERTY_TEXT, Property((*iter)->getText()));
                journals.push_back(journal.toPropertyMap());
            }
            return Property(journals);
        }
        else
        {
            Throw(IllegalArgumentException, key + " is not a property of QuestBook");
        }
    }

    void QuestBook::setProperty(const Ogre::String& key, const Property& value)
    {
        if (key == PROPERTY_QUESTS)
        {
            PropertyArray quests = value.toArray();
            for (PropertyArray::const_iterator it = quests.begin(); 
                it != quests.end(); ++it)
            {
                PropertyMap curVal = it->toMap();
                CeGuiString id = curVal[Quest::PROPERTY_ID];
                Quest* quest = getQuest(id);
                if (!quest)
                {
                    quest = new Quest(id);
                    addQuest(quest);
                }
               
                quest->setProperties(curVal);
            }
            ///@todo implement
        }
        else if (key == PROPERTY_JOURNAL)
        {
            PropertyArray journals = value.toArray();
            for(PropertyArray::const_iterator it = journals.begin(); it != journals.end(); it++)
            {
                PropertyMap curVal = it->toMap();
                Property caption = curVal[JournalEntry::PROPERTY_CAPTION];
                Property text = curVal[JournalEntry::PROPERTY_TEXT];

                addJournalEntry(caption.toString(), text.toString());
            }
        }
        else
        {
            Throw(IllegalArgumentException, key + " is not a property of QuestBook");
        }
    }

    PropertyRecord* QuestBook::getAllProperties() const
    {
        PropertyRecord* props = new PropertyRecord();
        props->setProperty(PROPERTY_QUESTS, getProperty(PROPERTY_QUESTS));
        props->setProperty(PROPERTY_JOURNAL, getProperty(PROPERTY_JOURNAL));
        return props;
    }

    QuestVector QuestBook::getAllQuests() const
    {
        QuestVector quests = getTopLevelQuests();
        unsigned int lastPos = 0;
        while (lastPos < quests.size())
        {
            QuestVector sub = quests[lastPos]->getSubquests();

            for (QuestVector::const_iterator it = sub.begin(); it != sub.end(); ++it)
            {
                quests.push_back(*it);
            }

            ++lastPos;
        }
        return quests;
    }

    CeGuiString QuestBook::getXmlNodeIdentifier() const
    {
        return "questbook";
    }

    void QuestBook::writeData(SaveGameFileWriter *writer)
    {
        LOG_MESSAGE(Logger::RULES, "Saving questbook");

        PropertyRecord* set = getAllProperties();
        writer->writeEachProperty(this, set->toPropertyMap());
    }

    void QuestBook::readData(SaveGameFileReader* reader)
    {
        LOG_MESSAGE(Logger::RULES, "Loading questbook");

        clear();
        PropertyRecord properties = reader->getAllPropertiesAsRecord(this);
        setProperties(&properties);
    }

    void QuestBook::clear()
    {
        delete mRootQuest;
        for( vector<JournalEntry*>::iterator it = mJournalEntries.begin();
            it != mJournalEntries.end(); )
        {
            JournalEntry* entry = *it;
            it = mJournalEntries.erase(it);
            fireJournalChanged(entry, JournalEvent::JOURNAL_ENTRY_DELETED);
            delete entry;
        }

        createRoot();
    }

    int QuestBook::getPriority() const
    {
        return 101;
    }

    const Ogre::StringVector &QuestBook::getScriptPatterns(void) const
    {
        return mScriptPatterns;
    }

    void QuestBook::parseScript(Ogre::DataStreamPtr& stream,const Ogre::String& groupname)
    {
        initializeXml();

        DOMDocument* doc = loadDocument(stream);
        if (doc)
        {
            for (DOMNode* cur = doc->getDocumentElement()->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
            {
                if(hasNodeName(cur, "quest"))
                {
                    processQuest(static_cast<DOMElement*>(cur), mRootQuest);
                }
            }
        }
        else
            LOG_ERROR(Logger::RULES,"Quests XML is not valid!");

        shutdownXml();
    }

    Ogre::Real QuestBook::getLoadingOrder(void) const
    {
        return 1000;
    }

    Quest* QuestBook::processQuest(XERCES_CPP_NAMESPACE::DOMElement* questXml, Quest* parent)
    {
        Quest* quest = new Quest(getAttributeValueAsString(questXml, "id"));
        parent->addSubquest(quest);
        quest->setKnown(false);
        quest->setState(Quest::OPEN);
        for (DOMNode* cur = questXml->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
        {
            if(hasNodeName(cur, "name"))
                quest->setProperty(Quest::PROPERTY_NAME,  Property(getValueAsString(static_cast<DOMElement*>(cur))));
            
            if(hasNodeName(cur, "description"))
                quest->setProperty(Quest::PROPERTY_DESCRIPTION, Property(getValueAsString(static_cast<DOMElement*>(cur))));
            
            if(hasNodeName(cur, "known"))
                quest->setKnown(getValueAsBool(static_cast<DOMElement*>(cur)));                
            
            if(hasNodeName(cur, "state"))
                quest->setState(Quest::getStateFromName(getValueAsString(static_cast<DOMElement*>(cur))));

            if(hasNodeName(cur, "quest"))
                processQuest(static_cast<DOMElement*>(cur), quest);
        }

        return quest;
    }
}
