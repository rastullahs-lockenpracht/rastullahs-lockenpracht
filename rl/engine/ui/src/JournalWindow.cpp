/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#include "JournalWindow.h"

#include <CEGUI/PropertyHelper.h>
#include <CEGUI/widgets/ListboxTextItem.h>
#include <boost/bind.hpp>

#include "Quest.h"
#include "QuestBook.h"
#include "RulesSubsystem.h"

using namespace CEGUI;

namespace rl
{

    JournalWindow::JournalWindow()
        : AbstractWindow("journalwindow.xml", WIT_MOUSE_INPUT)
        , mQuests(0)
        , mQuestTitle(0)
        , mQuestState(0)
        , mQuestDescription(0)
        , mJournalEntries(0)
        , mJournalEntryTitle(0)
        , mJournalEntryText(0)
        , mSelectionBrush()
        , mSelectionImageset()
        , mSelectionColour()
    {
        bindHideWindowToXButton();

        mQuests = getListbox("JournalWindow/Quests/QuestList");
        mQuests->setMultiselectEnabled(false);
        mQuests->subscribeEvent(
            CEGUI::Listbox::EventSelectionChanged, boost::bind(&JournalWindow::updateQuestSelection, this));

        mQuestTitle = getWindow("JournalWindow/Quests/QuestTitle");
        mQuestState = getWindow("JournalWindow/Quests/QuestState");
        mQuestDescription = getMultiLineEditbox("JournalWindow/Quests/QuestDescription");
        mQuestDescription->setReadOnly(true);

        mJournalEntries = getListbox("JournalWindow/Journal/JournalEntryList");
        mJournalEntries->setMultiselectEnabled(false);
        mJournalEntries->subscribeEvent(
            CEGUI::Listbox::EventSelectionChanged, boost::bind(&JournalWindow::updateJournalEntrySelection, this));

        mJournalEntryTitle = getWindow("JournalWindow/Journal/JournalEntryTitle");
        mJournalEntryText = getMultiLineEditbox("JournalWindow/Journal/JournalEntryText");
        mJournalEntryText->setReadOnly(true);

        mSelectionColour = CEGUI::ListboxItem::DefaultSelectionColour;
        mSelectionImageset = "RastullahLook-Images";
        mSelectionBrush = "ListboxSelectionBrush";

        updateQuests();
        updateJournal();
    }

    JournalWindow::~JournalWindow()
    {
    }

    void JournalWindow::questStateChanged(QuestEvent* anEvent)
    {
        updateQuests();
    }

    void JournalWindow::questPartsDoneChanged(QuestEvent* anEvent)
    {
        updateQuests();
    }

    void JournalWindow::questKnownChanged(QuestEvent* anEvent)
    {
        updateQuests();
    }

    void JournalWindow::questSubquestAdded(QuestEvent* anEvent)
    {
        updateQuests();
    }

    void JournalWindow::journalEntryAdded(JournalEvent* anEvent)
    {
        updateJournal();
    }

    void JournalWindow::journalEntryDeleted(JournalEvent* anEvent)
    {
        updateJournal();
    }

    void JournalWindow::updateJournal()
    {
        mJournalEntries->clearAllSelections();
        selectJournalEntry(0);

        while (mJournalEntries->getItemCount() > 0)
            mJournalEntries->removeItem(mJournalEntries->getListboxItemFromIndex(0));

        QuestBook* questBook = RulesSubsystem::getSingleton().getQuestBook();
        unsigned int count = questBook->getNumJournalEntries();
        for (unsigned int i = 0; i < count; ++i)
        {
            addJournalEntry(questBook->getJournalEntry(i));
        }

        if (mJournalEntries->getItemCount() > 0)
        {
            mJournalEntries->setItemSelectState(static_cast<size_t>(0), true);
        }
    }

    void JournalWindow::updateQuests()
    {
        mQuests->clearAllSelections();
        selectQuest(NULL);

        while (mQuests->getItemCount() > 0)
            mQuests->removeItem(mQuests->getListboxItemFromIndex(0));

        QuestBook* questBook = RulesSubsystem::getSingleton().getQuestBook();
        QuestVector quests = questBook->getTopLevelQuests();
        for (QuestVector::iterator it = quests.begin(); it != quests.end(); it++)
        {
            addQuest(*it);
        }

        if (mQuests->getItemCount() > 0)
            mQuests->setItemSelectState((size_t)0, true);
    }

    bool JournalWindow::updateQuestSelection()
    {
        if (mQuests->getItemCount() == 0)
            selectQuest(NULL);
        else
            selectQuest(mQuests->getFirstSelectedItem());

        return true;
    }

    void JournalWindow::addQuest(Quest* quest, int level)
    {
        static CeGuiString INDENT = "   ";

        if (!quest->isKnown())
            return;

        CeGuiString questName = "";
        for (int idx = 0; idx < level; idx++)
            questName.append(INDENT);
        questName.append(quest->getName());

        ListboxTextItem* item = new ListboxTextItem(questName, 0, quest);
        // TODO: item->setSelectionBrushImage(CEGUI::ImageManager mSelectionImageset, mSelectionBrush);
        mQuests->addItem(item);
        QuestVector quests = quest->getSubquests();
        for (QuestVector::iterator it = quests.begin(); it != quests.end(); it++)
        {
            addQuest(*it, level + 1);
        }
    }

    void JournalWindow::selectQuest(CEGUI::ListboxItem* item)
    {
        if (item == NULL)
        {
            mQuestDescription->setText("");
            mQuestState->setText("");
            mQuestTitle->setText("");
        }
        else
        {
            Quest* quest = static_cast<Quest*>(item->getUserData());

            mQuestState->setText(quest->getStateName());
            mQuestTitle->setText(quest->getName());
            mQuestDescription->setText(quest->getDescription());
        }
    }

    void JournalWindow::selectJournalEntry(CEGUI::ListboxItem* item)
    {
        if (item == NULL)
        {
            mJournalEntryText->setText("");
            mJournalEntryTitle->setText("");
        }
        else
        {
            JournalEntry* entry = static_cast<JournalEntry*>(item->getUserData());

            mJournalEntryTitle->setText(entry->getCaption());
            mJournalEntryText->setText(entry->getText());
        }
    }

    bool JournalWindow::updateJournalEntrySelection()
    {
        if (mJournalEntries->getItemCount() == 0)
        {
            selectJournalEntry(NULL);
        }
        else
        {
            selectJournalEntry(mJournalEntries->getFirstSelectedItem());
        }
        return true;
    }

    void JournalWindow::addJournalEntry(JournalEntry* entry)
    {
        ListboxTextItem* item = new ListboxTextItem(entry->getCaption(), 0, entry);
        // TODO: item->setSelectionBrushImage(mSelectionImageset, mSelectionBrush);
        mJournalEntries->addItem(item);
    }
}
