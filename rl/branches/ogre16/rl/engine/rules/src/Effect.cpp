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

#include "Effect.h"
#include "DsaManager.h"

namespace rl
{

	Effect::Effect(int stufe)
	{
		mStufe = stufe;
		mQuantifier = QUANTIFIER_MULTIPLE;
	}

	const CeGuiString Effect::getName() const
	{
		return mName;
	}

	void Effect::setName(CeGuiString name)
	{
		mName = name;
	}

	const CeGuiString Effect::getDescription() const
	{
		return mDescription;
	}

	void Effect::setDescription(CeGuiString description)
	{
		mDescription = description;
	}

    Effect::Quantifier Effect::getQuantifier()
	{
		return mQuantifier;
	}

	void Effect::setQuantifier(Quantifier quantifier)
	{
		mQuantifier = quantifier;
	}


	const int Effect::getStufe()
	{
		return mStufe;
	}

	void Effect::increaseStufe()
	{
	}

	void Effect::decreaseStufe()
	{
	}

	void Effect::enable()
	{
		if (!mEnabled)
		{
			mEnabled = true;
		}
	}

	void Effect::disable()
	{
		if (mEnabled)
		{
			mEnabled = false;
		}
	}

    RL_LONGLONG Effect::timeCheck()
    {
      return PERMANENT;
    }

	Effect::LifeState Effect::getLifeState() const
    {
      // should be overlaoaded in the specific derivated effects.
      return LS_NONE;
    }

    int Effect::getMod(CeGuiString target, ModType type, ModTag tag)
    {
        return 0;
    }

    const Property Effect::getProperty(const CeGuiString& key) const
    {
        //else
        //{
            Throw(
                IllegalArgumentException, 
                key + " is not a property of this effect (" + mName + ")");
        //}
    }

    void Effect::setProperty(const CeGuiString& key, const Property& value)
    {
        // else
        //{
            LOG_WARNING(
                Logger::RULES,
                key + " is not a property of this Effect ("+mName+")");
        //}
    }

    PropertyKeys Effect::getAllPropertyKeys() const
    {
        PropertyKeys keys;
        return keys;
    }

}
