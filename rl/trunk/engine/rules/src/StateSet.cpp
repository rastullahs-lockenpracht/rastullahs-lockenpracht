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

#include "StateSet.h"

namespace rl
{
	StateSet::StateSet()
		: mOriginalValue(0),
		mModifier(0),
		mMultiplier(1)
	{
	}

	StateSet::~StateSet()
	{
	}

	int StateSet::getValue(bool getUnmodifiedValue) const
	{
		int rval = mOriginalValue;
		rval = getUnmodifiedValue ?  rval : (rval + mModifier) * mMultiplier;
		return rval;
	}

	int StateSet::getOriginalValue() const
	{
		return mOriginalValue;
	}

	void StateSet::setOriginalValue(int newValue)
	{
		mOriginalValue = newValue;
	}

	int StateSet::getModifier() const
	{
		return mModifier;
	}

	void StateSet::setModifier(int newModifier)
	{
		mModifier = newModifier;
	}

	int StateSet::getProbenModifier() const
	{
		return mProbenModifier;
	}

	void StateSet::setProbenModifier(int newProbenModifier)
	{
		mProbenModifier = newProbenModifier;
	}

	int StateSet::getMultiplier() const
	{
		return mMultiplier;
	}

	void StateSet::setMultiplier(int newMultiplier)
	{
		mMultiplier = newMultiplier;
	}
}