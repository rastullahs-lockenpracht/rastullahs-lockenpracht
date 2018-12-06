/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#include "EigenschaftenStateSet.h"

namespace rl
{

	EigenschaftenStateSet::EigenschaftenStateSet() : StateSet(),
		mStartValue(0),
		mPermanentModifier(0),
		mModifierWithoutRecalculation(0)
	{
	}

	EigenschaftenStateSet::~EigenschaftenStateSet()
	{
	}

	int EigenschaftenStateSet::getStartValue()
	{
		return mStartValue;
	}

	void EigenschaftenStateSet::setStartValue( int newStartValue )
	{
		mStartValue = newStartValue;
	}

	int EigenschaftenStateSet::getPermanentModifier()
	{
		return mPermanentModifier;
	}

	void EigenschaftenStateSet::setPermanentModifier( int newPermanentModifier )
	{
		mPermanentModifier = newPermanentModifier;
	}

	int EigenschaftenStateSet::getModifierWithoutRecalculation()
	{
		return mModifierWithoutRecalculation;
	}

	void EigenschaftenStateSet::setModifierWithoutRecalculation( int newModifierWithoutRecalculation )
	{
		mModifierWithoutRecalculation = newModifierWithoutRecalculation;
	}

	int EigenschaftenStateSet::getValue( bool getUnmodifiedValue)
	{
		if ( getUnmodifiedValue ) return mOriginalValue;
		return ( mOriginalValue + mModifier + mModifierWithoutRecalculation );
	}

	int EigenschaftenStateSet::getValueForBasiswertCalculation()
	{
		return ( mOriginalValue + mModifier );
	}

}

