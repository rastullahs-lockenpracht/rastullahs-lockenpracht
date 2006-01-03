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

#include <boost/bind.hpp>

#include "JournalWindow.h"
#include "RulesSubsystem.h"
#include "Quest.h"
#include "QuestBook.h"

namespace rl {

	JournalWindow::JournalWindow()
		: CeGuiWindow("journalwindow.xml", WND_MOUSE_INPUT)
	{
		mQuests = getListbox("JournalWindow/Quests/QuestList");
		mQuests->setMultiselectEnabled(false);
		mQuests->subscribeEvent(
			CEGUI::Listbox::EventSelectionChanged,
			boost::bind(&JournalWindow::updateSelection, this));

		mQuestTitle = getStaticText("JournalWindow/Quests/QuestTitle");
		
		mQuestState = getStaticText("JournalWindow/Quests/QuestState");

		mQuestDescription = getMultiLineEditbox("JournalWindow/Quests/QuestDescription");
		mQuestDescription->setReadOnly(true);


		addToRoot(mWindow);

		updateQuests();
	}
	
	JournalWindow::~JournalWindow()
	{
	}

	bool JournalWindow::eventRaised(QuestStateChangeEvent *anEvent)
	{
		updateQuests();
		return false;
	}

	void JournalWindow::updateQuests()
	{
		while (mQuests->getItemCount() > 0)
			mQuests->removeItem(mQuests->getListboxItemFromIndex(0));

		QuestBook* questBook = RulesSubsystem::getSingleton().getQuestBook();
		QuestVector quests = questBook->getTopLevelQuests();
		for (QuestVector::iterator it = quests.begin(); it != quests.end(); it++)
		{
			addQuest(*it);
		}

		if (mQuests->getItemCount() == 0)
			selectQuest(NULL);
		else
			selectQuest(mQuests->getListboxItemFromIndex(0));
	}

	bool JournalWindow::updateSelection()
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

		if (quest->getState() == Quest::UNKNOWN)
			return;

		CeGuiString questName = "";
		for (int idx = 0; idx < level; idx++)
			questName.append(INDENT);
		questName.append(quest->getName());

		mQuests->addItem(new CEGUI::ListboxTextItem(questName, 0, quest));
		QuestVector quests = quest->getSubquests();
		for (QuestVector::iterator it = quests.begin(); it != quests.end(); it++)
		{
			addQuest(*it, level+1);
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
}
