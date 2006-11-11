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

#ifndef __GAMEOBJECTMANAGER_H__
#define __GAMEOBJECTMANAGER_H__

#include "RulesPrerequisites.h"
#include <OgreSingleton.h>
#include <map>

namespace rl
{
    class GameObject;
    class GameObjectProxy;
    class PropertySet;

    class _RlRulesExport GameObjectFactory
    {
    public:
        GameObjectFactory();

        GameObject* createGameObject(const Ogre::String& classname, unsigned int id);
        virtual GameObject* createRubyGameObject(const Ogre::String& classname, unsigned int id) = 0;
    };

    class _RlRulesExport GameObjectManager : 
        public Ogre::Singleton<GameObjectManager>
    {
    public:
        GameObjectManager();
        ~GameObjectManager();

        GameObjectProxy& createGameObjectProxy(const Ogre::String& classId, unsigned int id = 0);
        GameObject* createGameObject(const GameObjectProxy* proxy);
        void setGameObjectFactory(GameObjectFactory* gof);
        void loadProperties(const Ogre::String& module);

    private:
        typedef std::map<const Ogre::String, PropertySet*> ClassPropertyMap;

        std::map<unsigned int, GameObjectProxy*> mGameObjectProxys;
        std::map<const GameObjectProxy*, GameObject*> mGameObjects;
        ClassPropertyMap mClassProperties;
        unsigned int mGeneratedId;
        GameObjectFactory* mGameObjectFactory;

        unsigned int generateId();
        PropertySet* getClassProperties(const Ogre::String& classId);
    };
}

#endif //__GAMEOBJECTMANAGER_H__
