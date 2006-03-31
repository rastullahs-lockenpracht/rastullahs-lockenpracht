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

#include "Effect.h"
#include "CoreSubsystem.h"

namespace rl
{
	Effect::Effect()
	{
		mStartTime = CoreSubsystem::getSingleton().getClock();
		mQuantifier = 0;
	}

	Effect::~Effect()
	{
		disable();
	}

	const CeGuiString& Effect::getName() const
	{
		return mName;
	}

	void Effect::setName(CeGuiString& name)
	{
		mName = name;
	}

	const CeGuiString& Effect::getDescription() const
	{
		return mDescription;
	}

	void Effect::setDescription(CeGuiString& description)
	{
		mDescription = description;
	}

	int Effect::getDuration()
	{
		return mDuration;
	}

	void Effect::setDuration(int newDuration)
	{
		mDuration = newDuration;
	}

	int Effect::getQuantifier()
	{
		return mQuantifier;
	}

	void Effect::setQuantifier(int quantifier)
	{
		mQuantifier = quantifier;
	}

	void Effect::addTag(int tagId)
	{
		// no doubles
		Tags::const_iterator it = mTags.find(tagId);
        if (it == mTags.end())
        {
			mTags.insert(tagId);
        }
	}

	void Effect::removeTag(int tagId)
	{
		Tags::const_iterator it = mTags.find(tagId);
        if (it != mTags.end())
        {
			mTags.erase(tagId);
        }
	}

	bool Effect::queryTag(int tagId)
	{
		Tags::const_iterator it = mTags.find(tagId);
        if (it != mTags.end())
        {
			return true;
        }
		return false;
	}

	void Effect::enable()
	{
		if (!mEnabled)
		{
			mEnabled = true;
			apply();
		}
	}

	void Effect::disable()
	{
		if (mEnabled)
		{
			mEnabled = false;
			remove();
		}
	}

	RL_LONGLONG Effect::getTimePassed()
	{
		return 0;
	}
}
