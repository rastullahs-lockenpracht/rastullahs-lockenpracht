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

#include "CreaturePredicates.h"
#include "Creature.h"

namespace rl {

CreaturePredicates::CreaturePredicates(Creature* pCharacter)
	: mCharacter(pCharacter)
{
}

CreaturePredicates::~CreaturePredicates()
{
}

void CreaturePredicates::setPredicate(const CeGuiString& pName, const CeGuiString& pValue)
{
	if(pName == "mod")
	{
		mModifier = Ogre::StringConverter::parseInt(pValue.c_str());
	}
}

}
