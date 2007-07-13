/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#include "stdinc.h" //precompiled header

#include "QuestPartsPredicates.h"

#include <CEGUIPropertyHelper.h>

#include "Exception.h"
#include "QuestBook.h"
#include "Quest.h"
#include "RulesSubsystem.h"

using CEGUI::PropertyHelper;

namespace rl {

QuestPartsPredicates::QuestPartsPredicates()
{
}

QuestPartsPredicates::~QuestPartsPredicates(void)
{
}

CeGuiString QuestPartsPredicates::getPredicate(const CeGuiString& name) const
{
	Quest* quest = RulesSubsystem::getSingletonPtr()->getQuestBook()
		->getQuest(name);

	if (quest == NULL)
	{
		Throw(
			IllegalArgumentException,
			("Quest '"
			+ name
			+ "' not found").c_str());
	}

	return PropertyHelper::intToString(quest->getPartsDone());
}

void QuestPartsPredicates::setPredicate(const CeGuiString& name, const CeGuiString& value)
{
	Quest* quest = RulesSubsystem::getSingletonPtr()->getQuestBook()
		->getQuest(name);

	if (quest == NULL)
	{
		Throw(
			IllegalArgumentException,
			("Quest '"
			+ name
			+ "' not found").c_str());
	}

	static CeGuiString INCREASE = "increase";
	static CeGuiString DECREASE = "decrease";

	if (value == INCREASE)
	{
		quest->increasePartsDone();
	}
	else if (value.substr(0, INCREASE.length()) == INCREASE)
	{
		int parts = PropertyHelper::stringToInt(value.substr(INCREASE.length()));
		quest->increasePartsDone(parts);
	}
	else if (value == DECREASE)
	{
		quest->decreasePartsDone();
	}
	else if (value.substr(0, DECREASE.length()) == DECREASE)
	{
		int parts = PropertyHelper::stringToInt(value.substr(DECREASE.length()));
		quest->decreasePartsDone(parts);
	}
	else
	{
		int parts = PropertyHelper::stringToInt(value);
		quest->setPartsDone(parts);
	}
}

CeGuiString QuestPartsPredicates::getType() const
{
	return "questparts";
}

}

