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

#ifndef __GAMEOBJECTPROXY_H__
#define __GAMEOBJECTPROXY_H__

#include "RulesPrerequisites.h"
#include "GameObjectState.h"
#include "Properties.h"

namespace rl
{
    class GameObject;

    class _RlRulesExport GameObjectProxy
    {
    public:
        static const unsigned int NO_ID = 0;
        static const Ogre::String CLASS_ID;
        static const Ogre::String OBJECT_ID;
        static const Ogre::String BASE_CLASS;


        GameObjectProxy(const Ogre::String& classid, unsigned int id);

        GameObject* getGameObject();

        unsigned int getId() const;
        const Ogre::String& getClassId() const;

        void unload();
        void load();

        GameObjectState getState() const;

    private:
        unsigned int mId;
        Ogre::String mClassId;
        GameObject* mGameObject;
        const PropertySet* mProperties;

        void createGameObject();
        void deleteGameObject();
    };
}

#endif //__GAMEOBJECTPROXY_H__
