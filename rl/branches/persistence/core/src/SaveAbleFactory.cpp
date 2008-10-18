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

#include "stdinc.h"

#include "SaveAbleFactory.h"

template<> rl::SaveAbleFactory* Ogre::Singleton<rl::SaveAbleFactory>::ms_Singleton = 0;

template<> rl::SaveAbleFactoryPool* Ogre::Singleton<rl::SaveAbleFactoryPool>::ms_Singleton = 0;

namespace rl
{
	void SaveAbleFactoryPool::registerFactory(const Ogre::String &key, rl::SaveAbleFactory *fac)
	{
		if(mFactories.find(key) != mFactories.end())
			LOG_ERROR("CORE", "SaveAbleFactory already registered!");
		mFactories[key] = fac;
	}

	void SaveAbleFactoryPool::unregisterFactory(const Ogre::String &key)
	{
		std::map<Ogre::String,SaveAbleFactory*>::iterator it = mFactories.find(key);
		mFactories.erase(it);
	}

	void SaveAbleFactoryPool::unregisterFactory(rl::SaveAbleFactory *fac)
	{
	}

	SaveAbleFactory* SaveAbleFactoryPool::getFactory(const Ogre::String &key)
	{
		std::map<Ogre::String,SaveAbleFactory*>::iterator it = mFactories.find(key);
		if(it == mFactories.end())
			return NULL;
		return it->second;
	}
}