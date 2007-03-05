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
#include "EigenschaftsProbePredicates.h"
#include "Creature.h"

namespace rl {

EigenschaftsProbePredicates::EigenschaftsProbePredicates(Creature* pCharacter)
	: CreaturePredicates(pCharacter)
{
}

EigenschaftsProbePredicates::~EigenschaftsProbePredicates(void)
{
}

CeGuiString EigenschaftsProbePredicates::getPredicate(const CeGuiString& pName) const
{
	try
	{
		if(pName == "MR")
		{
			// doMagieresistenzProbe?
		}
		else
		{
			return Ogre::StringConverter::toString(mCharacter->doEigenschaftsprobe(pName, mModifier));
		}
	}
	catch(...)
	{
	//  TODO: exception handling
	}
	return "";
}

CeGuiString EigenschaftsProbePredicates::getType() const
{
	return "eigenschaftsprobe";
}

}
