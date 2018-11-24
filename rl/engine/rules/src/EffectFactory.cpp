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

#include "EffectFactory.h"

template <> rl::EffectFactoryManager* Ogre::Singleton<rl::EffectFactoryManager>::msSingleton = 0;

namespace rl
{
    EffectFactoryManager::EffectFactoryManager()
        : mFactory(NULL)
    {
    }

    Effect* EffectFactoryManager::createEffect(const Ogre::String& name, int stufe)
    {
        if (mFactory == NULL)
        {
            LOG_ERROR(Logger::RULES, "Could not create effect " + name + ". No factory registered");
            return NULL;
        }

        Effect* rval = mFactory->createEffect(name, stufe);

        if (rval == NULL)
        {
            LOG_ERROR(Logger::RULES, "Effect " + name + " was not created.");
        }

        return rval;
    }

    void EffectFactoryManager::setEffectFactory(rl::EffectFactory* factory)
    {
        mFactory = factory;
    }
}
