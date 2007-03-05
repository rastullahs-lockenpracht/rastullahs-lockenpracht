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
#include "QuestStatePredicates.h"

#include "Exception.h"
#include "QuestBook.h"
#include "Quest.h"
#include "RulesSubsystem.h"

namespace rl {

QuestStatePredicates::QuestStatePredicates()
{
}

QuestStatePredicates::~QuestStatePredicates(void)
{
}

CeGuiString QuestStatePredicates::getPredicate(const CeGuiString& name) const
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

	return quest->getStateName();
}

void QuestStatePredicates::setPredicate(const CeGuiString& name, const CeGuiString& value)
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

	quest->setState(quest->getStateFromName(value));
}

CeGuiString QuestStatePredicates::getType() const
{
	return "queststate";
}

}
