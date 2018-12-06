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

#ifndef __SaveAbleFactory_H__
#define __SaveAbleFactory_H__

#include "CorePrerequisites.h"
#include "SaveAble.h"

namespace rl
{
    class SaveAbleFactory : public Ogre::Singleton<SaveAbleFactory>
    {
    public:
        virtual SaveAblePtr createSaveAble(PropertyRecordPtr params) = 0;
    };

    class SaveAbleFactoryPool : public Ogre::Singleton<SaveAbleFactoryPool>
    {
    public:
        void registerFactory(const Ogre::String& key, SaveAbleFactory* fac);
        void unregisterFactory(const Ogre::String& key);
        void unregisterFactory(SaveAbleFactory* fac);
        SaveAbleFactory* getFactory(const Ogre::String& key);

    protected:
        std::map<Ogre::String, SaveAbleFactory*> mFactories;
    };
}

#endif
