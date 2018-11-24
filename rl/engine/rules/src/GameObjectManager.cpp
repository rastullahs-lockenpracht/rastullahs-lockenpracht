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

#include "GameObjectManager.h"

#include <CEGUI/PropertyHelper.h>

#include "Armor.h"
#include "Container.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "Exception.h"
#include "GameObject.h"
#include "Item.h"
#include "Properties.h"
#include "Property.h"
#include "RubyInterpreter.h"
#include "RulesMessages.h"
#include "SaveGameManager.h"
#include "Weapon.h"
#include "XmlPropertyReader.h"

template <> rl::GameObjectManager* Ogre::Singleton<rl::GameObjectManager>::msSingleton = NULL;

namespace rl
{
    GameObjectManager::GameObjectManager()
        : mGameObjectFactory(NULL)
    {
        mGeneratedId = 1 << 16;
        mGameObjects.clear();

        mScriptPatterns.push_back("*.gof");
        Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);
    }

    GameObjectManager::~GameObjectManager()
    {
        unregisterAllGameObjectStateListener();
        deleteAllGameObjects();
    }

    const Ogre::StringVector& GameObjectManager::getScriptPatterns() const
    {
        return mScriptPatterns;
    }

    Ogre::Real GameObjectManager::getLoadingOrder() const
    {
        return 1500.0;
    }

    void GameObjectManager::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
    {
        XmlPropertyReader* propReader = new XmlPropertyReader();
        propReader->parseGameObjectFile(stream, groupName);
        std::vector<PropertyRecordPtr> psset = propReader->getPropertyRecords();
        for (std::vector<PropertyRecordPtr>::iterator it = psset.begin(); it != psset.end(); it++)
        {
            PropertyRecordPtr curPs = *it;
            Ogre::String classId = curPs->getProperty(GameObject::PROPERTY_CLASS_ID).toString().c_str();
            mClassProperties[classId] = curPs;
        }
        delete propReader;
    }

    GameObject* GameObjectManager::getGameObject(unsigned int id) const
    {
        std::map<unsigned int, GameObject*>::const_iterator it = mGameObjects.find(id);

        if (it != mGameObjects.end())
        {
            return (*it).second;
        }

        return NULL;
    }

    std::list<GameObject*> GameObjectManager::getAllGameObjects() const
    {
        std::list<GameObject*> gos;
        std::map<unsigned int, GameObject*>::const_iterator it;

        //
        //    Run through all GOs and put them into the list
        //
        for (it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
        {
            gos.push_back(it->second);
        }

        return gos;
    }

    unsigned int GameObjectManager::generateId()
    {
        return mGeneratedId++;
    }

    GameObject* GameObjectManager::createGameObject(const CeGuiString& classId, unsigned int id)
    {
        LOG_MESSAGE("GameObjectManager",
            "Create/Get GameObject of type " + classId + " #" + Ogre::StringConverter::toString((int)id));
        unsigned int goId;

        if (id != GameObject::NO_OBJECT_ID)
        {
            GameObject* goInMap = getGameObject(id);

            if (goInMap != NULL)
            {
                return goInMap;
            }
            else
            {
                goId = id;
            }
        }
        else
        {
            goId = generateId();
        }

        PropertyRecordPtr ps = getClassProperties(classId);
        Ogre::String classname = ps->getProperty(GameObject::PROPERTY_BASE_CLASS).toString().c_str();

        GameObject* go = mGameObjectFactory->createGameObject(classname, goId);
        if (go)
        {
            go->setClassId(classId);
            applyProperties(go, ps);
            mGameObjects[goId] = go;
        }
        return go;
    }

    void GameObjectManager::deleteGameObject(unsigned int id)
    {
        if (mGameObjects.find(id) != mGameObjects.end())
        {
            GameObject* go = mGameObjects[id];
            mGameObjects.erase(id);
            delete go;
        }
    }

    void GameObjectManager::deleteGameObject(GameObject* obj)
    {
        deleteGameObject(obj->getId());
    }

    void GameObjectManager::deleteAllGameObjects()
    {
        while (!mGameObjects.empty())
        {
            GameObject* go = mGameObjects.begin()->second;
            delete go;
            mGameObjects.erase(mGameObjects.begin());
        }
    }

    void GameObjectManager::applyProperties(GameObject* go, PropertyRecordPtr ps) const
    {
        if (ps->hasProperty(GameObject::PROPERTY_INHERITS))
        {
            PropertyRecordPtr superClassProps
                = getClassProperties(ps->getProperty(GameObject::PROPERTY_INHERITS).toString().c_str());
            applyProperties(go, superClassProps);
        }
        go->setProperties(ps);
    }

    GameObject* GameObjectManager::createGameObjectFromProperty(const Property& goProp)
    {
        CeGuiString serializedString = goProp.toString();

        CeGuiString::size_type posDivider = serializedString.find("|");

        if (posDivider != CeGuiString::npos)
        {
            Ogre::String classId(serializedString.substr(0, posDivider).c_str());
            unsigned int goid
                = CEGUI::PropertyHelper<unsigned int>::fromString(serializedString.substr(posDivider + 1));
            return createGameObject(classId, goid);
        }

        return NULL;
    }

    Property GameObjectManager::toProperty(const GameObject* const go) const
    {
        return Property(go->getClassId() + "|" + CEGUI::PropertyHelper<unsigned int>::toString(go->getId()));
    }

    const PropertyRecordPtr GameObjectManager::getClassProperties(const CeGuiString& classId) const
    {
        ClassPropertyMap::const_iterator it = mClassProperties.find(classId);
        if (it == mClassProperties.end())
        {
            Throw(rl::Exception, "No properties found for classId " + classId);
        }
        return (*it).second;
    }

    void GameObjectManager::setGameObjectFactory(GameObjectFactory* gof)
    {
        mGameObjectFactory = gof;
    }

    void GameObjectManager::gameObjectStateChanged(GameObject* go, GameObjectState oldState, GameObjectState newState)
    {
        for (GameObjectStateListenerSet::iterator it = mGameObjectStateListeners.begin();
             it != mGameObjectStateListeners.end(); ++it)
        {
            (*it)->gameObjectStateChanged(go, oldState, newState);
        }
    }

    void GameObjectManager::registerGameObjectStateListener(GameObjectStateListener* listener)
    {
        mGameObjectStateListeners.insert(listener);
    }

    void GameObjectManager::unregisterGameObjectStateListener(GameObjectStateListener* listener)
    {
        mGameObjectStateListeners.erase(listener);
    }

    void GameObjectManager::unregisterAllGameObjectStateListener()
    {
        for (std::set<GameObjectStateListener*>::iterator itr = mGameObjectStateListeners.begin();
             itr != mGameObjectStateListeners.end();)
        {
            mGameObjectStateListeners.erase(itr++);
        }
    }

    GameObjectFactory::GameObjectFactory()
    {
    }

    GameObject* GameObjectFactory::createGameObject(const Ogre::String& classname, unsigned int id)
    {
        if (classname == GameObject::CLASS_NAME)
        {
            return new GameObject(id);
        }
        else if (classname == Creature::CLASS_NAME)
        {
            return new Creature(id);
        }
        else if (classname == Weapon::CLASS_NAME)
        {
            return new Weapon(id);
        }
        else if (classname == Armor::CLASS_NAME)
        {
            return new Armor(id);
        }
        else if (classname == Container::CLASS_NAME)
        {
            return new Container(id);
        }
        else if (classname == Item::CLASS_NAME)
        {
            return new Item(id);
        }

        return createRubyGameObject(classname, id);
    }

    PropertyMap GameObjectManager::getPropertyMapDifference(PropertyMap map1, PropertyMap map2)
    {
        PropertyRecord pr;

        if (!map1.empty() && !map2.empty())
        {
            for (PropertyMap::const_iterator iter = map1.begin(); iter != map1.end(); ++iter)
            {
                if (map2.find(iter->first) == map2.end())
                    pr.setProperty(iter->first.c_str(), iter->second);
                else if (map2.find(iter->first)->second != iter->second)
                    pr.setProperty(iter->first.c_str(), iter->second);
            }
        }

        return pr.toPropertyMap();
    }
}
