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
#ifndef __UnifiedFactory_H__
#define __UnifiedFactory_H__

#include "ScriptPrerequisites.h"
#include "TriggerFactory.h"
#include "EffectFactory.h"
#include "GameObjectManager.h"
#include "AgentManager.h"

namespace rl
{
    class _RlScriptExport UnifiedFactory :
        public TriggerFactory, 
        public EffectFactory, 
        public GameObjectFactory,
        public BehaviourFactory
    {
    public:
        virtual ~UnifiedFactory() {}

        virtual Trigger* createTrigger(const Ogre::String& classname, const Ogre::String& name) = 0;
        virtual Effect* createEffect(const Ogre::String& name, int stufe) = 0;
        virtual GameObject* createRubyGameObject(const Ogre::String& classname, unsigned int id) = 0;
        virtual SteeringBehaviour* createBehaviour(const Ogre::String& classname) = 0;
    };
}

#endif // __UnifiedFactory_H__
