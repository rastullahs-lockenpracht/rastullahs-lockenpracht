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
#include "GrundWertPredicates.h"
#include "Creature.h"

namespace rl {

GrundWertPredicates::GrundWertPredicates(Creature* pCharacter)
	: CreaturePredicates(pCharacter) 
{
}

GrundWertPredicates::~GrundWertPredicates(void)
{
}

CeGuiString GrundWertPredicates::getPredicate(const CeGuiString& pName) const
{
	try
	{
	//	TODO: design an architecture that handles all possible names for the base values
		if(pName == "LeP" || pName == "LE")
		{
			return Ogre::StringConverter::toString(mCharacter->getLe());
		}
		else if( pName == "AuP" || pName == "AU")
		{
			return Ogre::StringConverter::toString(mCharacter->getAu());
		}
		else if( pName == "AsP" || pName == "AE")
		{
			return Ogre::StringConverter::toString(mCharacter->getAe());
		}
	}
	catch(...)
	{
	//  TODO: excpetionhandling
	}
	return "";
}

void GrundWertPredicates::setPredicate(const CeGuiString& pName, const CeGuiString& pValue)
{
	try
	{
	//	TODO: design an architecture that handles all possible names for the base values
		if(pName == "LeP" || pName == "LE")
		{
			mCharacter->modifyLe(Ogre::StringConverter::parseInt(pValue.c_str()));
		}
		else if( pName == "AuP" || pName == "AU")
		{
			mCharacter->modifyAu(Ogre::StringConverter::parseInt(pValue.c_str()));
		}
		else if( pName == "AsP" || pName == "AE")
		{
			mCharacter->modifyAe(Ogre::StringConverter::parseInt(pValue.c_str()));
		}
	}
	catch(...)
	{
	//  TODO: excpetionhandling
	}
}

CeGuiString GrundWertPredicates::getType() const
{
	return "grundwert";
}

}

