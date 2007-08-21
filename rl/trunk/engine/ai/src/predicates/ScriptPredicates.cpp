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

#include "DialogCharacter.h"
#include "ScriptPredicates.h"

namespace rl {

ScriptPredicates::ScriptPredicates(DialogCharacter* pCharacter)
	: mCharacter(pCharacter)
{
}

ScriptPredicates::~ScriptPredicates(void)
{
}

CeGuiString ScriptPredicates::getPredicate(const CeGuiString& pName) const
{
	CeGuiString context = mCharacter->getPredicates("default")->getPredicate("contextType");
	context.c_str();
	int result = 0;
	if(context == "response")
	{
		result = mCharacter->calcResponseValue(pName);
	}
	else if(context == "option")
	{
		result =mCharacter->calcOptionValue(pName);
	}
	else if(context == "selection")
	{
		result = mCharacter->calcSelectionValue(pName);
	}
	return Ogre::StringConverter::toString(result);
}

void ScriptPredicates::setPredicate(const CeGuiString& pName, const CeGuiString& pValue)
{
}

CeGuiString ScriptPredicates::getType() const
{
	return "script";
}

}
