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
#include "TalentProbePredicates.h"
#include "Creature.h"

using namespace rl;

TalentProbePredicates::TalentProbePredicates(Creature* pCharacter)
	: CreaturePredicates(pCharacter)
{
}

TalentProbePredicates::~TalentProbePredicates(void)
{
}

CeGuiString TalentProbePredicates::getPredicate(const CeGuiString& pName) const
{
	try
	{
		return Ogre::StringConverter::toString(mCharacter->doTalentprobe(pName, mModifier));
	}
	catch(...)
	{
	//  TODO: exception handling
	}
	return "";
}

CeGuiString TalentProbePredicates::getType() const
{
	return "talentprobe";
}
