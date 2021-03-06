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

#ifndef __GAMEOBJECTMANAGER_H__
#define __GAMEOBJECTMANAGER_H__

#include "GameObjectStateListener.h"
#include "RulesPrerequisites.h"

#include "Properties.h"
#include "SaveGameData.h"

#include <map>

namespace rl
{
    class GameObject;

    class _RlRulesExport GameObjectFactory
    {
    public:
        GameObjectFactory();

        GameObject* createGameObject(const Ogre::String& classname, unsigned int id);
        virtual GameObject* createRubyGameObject(const Ogre::String& classname, unsigned int id) = 0;
    };

    class _RlRulesExport GameObjectManager : public Ogre::Singleton<GameObjectManager>,
                                             public GameObjectStateListener,
                                             public Ogre::ScriptLoader
    {
    public:
        GameObjectManager();
        ~GameObjectManager();

        GameObject* getGameObject(unsigned int id) const;
        GameObject* createGameObject(const CeGuiString& classId, unsigned int id = 0);

        void deleteGameObject(unsigned int id);
        void deleteGameObject(GameObject* obj);
        void deleteAllGameObjects();

        GameObject* createGameObjectFromProperty(const Property& goProp);
        Property toProperty(const GameObject* const co) const;
        std::list<GameObject*> getAllGameObjects() const;
        void setGameObjectFactory(GameObjectFactory* gof);
        void loadProperties(const Ogre::String& module);

        virtual const Ogre::StringVector& getScriptPatterns() const;
        virtual Ogre::Real getLoadingOrder() const;
        virtual void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);

        /// Override from GameObjectStateListener.
        /// Used to propagate to global GameObjectStateListeners.
        virtual void gameObjectStateChanged(GameObject* go, GameObjectState oldState, GameObjectState newState);

        void registerGameObjectStateListener(GameObjectStateListener* listener);
        void unregisterGameObjectStateListener(GameObjectStateListener* listener);
        void unregisterAllGameObjectStateListener();

    private:
        typedef std::map<const CeGuiString, PropertyRecordPtr> ClassPropertyMap;
        typedef std::set<GameObjectStateListener*> GameObjectStateListenerSet;

        Ogre::StringVector mScriptPatterns;

        std::map<unsigned int, GameObject*> mGameObjects;
        ClassPropertyMap mClassProperties;
        unsigned int mGeneratedId;
        GameObjectFactory* mGameObjectFactory;
        GameObjectStateListenerSet mGameObjectStateListeners;

        unsigned int generateId();
        const PropertyRecordPtr getClassProperties(const CeGuiString& classId) const;
        void applyProperties(GameObject* go, PropertyRecordPtr ps) const;

        PropertyMap getPropertyMapDifference(PropertyMap map1, PropertyMap map2);
    };
}

#endif //__GAMEOBJECTMANAGER_H__
