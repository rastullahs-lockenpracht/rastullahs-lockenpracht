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
#ifndef __EffectFactory_H__
#define __EffectFactory_H__

#include "RulesPrerequisites.h"


namespace rl 
{
    class Effect;

    class _RlRulesExport EffectFactory
    {
    public:
        virtual ~EffectFactory() {};
        virtual Effect* createEffect(const Ogre::String& name, int stufe) = 0;
    };

    class _RlRulesExport EffectFactoryManager : public Ogre::Singleton<EffectFactoryManager>
    {
    public:
        EffectFactoryManager();

        Effect* createEffect(const Ogre::String& name, int stufe = 0);
        void setEffectFactory(EffectFactory* factory);

    private:
        EffectFactory* mFactory;
    };


}

#endif
