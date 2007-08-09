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
