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

#include "Effect.h"
#include "DsaManager.h"
#include "Creature.h"

namespace rl
{
	Effect::Effect(int stufe)
	{
		mStufe = stufe;
		mStartTime = DsaManager::getSingleton().getTimestamp();
		mQuantifier = QUANTIFIER_MULTIPLE;
	}

	Effect::~Effect()
	{
		disable();
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

	Creature* Effect::getOwner() const
	{
		return mOwner;
	}

	void Effect::setOwner(Creature* owner)
	{
		mOwner = owner;
	}

	int Effect::getDuration()
	{
		return mDuration;
	}

	void Effect::setDuration(int newDuration)
	{
		mDuration = newDuration;
	}

    Effect::Quantifier Effect::getQuantifier()
	{
		return mQuantifier;
	}

	void Effect::setQuantifier(Quantifier quantifier)
	{
		mQuantifier = quantifier;
	}

	void Effect::addTag(EffectTag tagId)
	{
		// no doubles
		Tags::const_iterator it = mTags.find(tagId);
        if (it == mTags.end())
        {
			mTags.insert(tagId);
        }
	}

	void Effect::removeTag(EffectTag tagId)
	{
		Tags::const_iterator it = mTags.find(tagId);
        if (it != mTags.end())
        {
			mTags.erase(tagId);
        }
	}

	bool Effect::queryTag(EffectTag tagId)
	{
		Tags::const_iterator it = mTags.find(tagId);
        if (it != mTags.end())
        {
			return true;
        }
		return false;
	}

	const int Effect::getStufe()
	{
		return mStufe;
	}

	void Effect::increaseStufe()
	{
		apply();
	}

	void Effect::decreaseStufe()
	{
		remove();
	}

	void Effect::enable()
	{
		if (!mEnabled)
		{
			mEnabled = true;
			for (int i = 0; i < mStufe; i++)
				apply();
		}
	}

	void Effect::disable()
	{
		if (mEnabled)
		{
			mEnabled = false;
			for (int i = 0; i < mStufe; i++)
				remove();
		}
	}

    RL_LONGLONG Effect::check()
    {
      return PERMANENT;
    }

	RL_LONGLONG Effect::getTimePassed()
	{		
        return (DsaManager::getSingleton().getTimestamp() - mStartTime);
	}
    
    Effect::Status Effect::getStatus()
    {
      // should be overlaoaded in the specific derivated effects.
      return STATUS_NONE;
    }
}
