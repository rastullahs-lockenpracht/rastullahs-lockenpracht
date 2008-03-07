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

#include "EffectManagementTask.h"

#include "Date.h"
#include "GameObject.h"
#include "GameObjectManager.h"


namespace rl
{

EffectManagementTask::EffectManagementTask()
: GameTask(true),
  mElapsedTime(0.0F)
{
}

EffectManagementTask::~EffectManagementTask()
{
}

void EffectManagementTask::run(Ogre::Real elapsedTime)
{
    mElapsedTime += elapsedTime * mTimeFactor;
    while (mElapsedTime > Date::ONE_AKTION)
    {
        std::list<GameObject*> golist = GameObjectManager::getSingleton().getAllGameObjects();
        for (std::list<GameObject*>::iterator it = golist.begin(); it != golist.end(); ++it)
        {
            (*it)->_checkEffects();
        }

        mElapsedTime -= Date::ONE_AKTION;
    }
}

void EffectManagementTask::setTimeFactor(Ogre::Real timeFactor)
{
    mTimeFactor = timeFactor;
}

const Ogre::String& EffectManagementTask::getName() const
{
    static Ogre::String NAME("EffectManagementTask");
    return NAME;
}

}
