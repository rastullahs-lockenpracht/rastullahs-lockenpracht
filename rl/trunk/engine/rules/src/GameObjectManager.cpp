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
#include "GameObjectManager.h"

#include "Armor.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "Exception.h"
#include "GameObject.h"
#include "GameObjectProxy.h"
#include "Item.h"
#include "Person.h"
#include "Properties.h"
#include "PropertyReader.h"
#include "Property.h"
#include "RubyInterpreter.h"
#include "Weapon.h"

template<> rl::GameObjectManager* 
    Ogre::Singleton<rl::GameObjectManager>::ms_Singleton = NULL;

namespace rl
{
    GameObjectManager::GameObjectManager()
        : mGameObjectFactory(NULL)
    {
        mGeneratedId = 1<<16;
        mGameObjects.clear();
        mGameObjectProxys.clear();

        mScriptPatterns.push_back("*.gof");
        Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);
    }

    GameObjectManager::~GameObjectManager()
    {
        ///@TODO: Delete all game objects, delete all class properties
    }

    const Ogre::StringVector& GameObjectManager::getScriptPatterns() const
    {
        return mScriptPatterns;
    }

    Ogre::Real GameObjectManager::getLoadingOrder() const
    {
        return 1500.0;
    }

    void GameObjectManager::parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
    {
        XmlPropertyReader* propReader = new XmlPropertyReader();
        propReader->parseGameObjectFile(stream, groupName);
        std::vector<PropertySet*> psset = propReader->getPropertySets();
        for(std::vector<PropertySet*>::iterator it = psset.begin(); it != psset.end(); it++)
        {
            PropertySet* curPs = *it;
            Ogre::String classId = curPs->getProperty(GameObjectProxy::CLASS_ID).toString().c_str();
            mClassProperties[classId] = curPs;
        }
        delete propReader;

    }

    GameObjectProxy& GameObjectManager::createGameObjectProxy(
        const Ogre::String& classId, 
        unsigned int id)
    {

        unsigned int goId = 0;

        if (id != 0)
        {
            GameObjectProxy* gopInMap = getGameObjectProxy(id);

            if (gopInMap != NULL)
            {
                return *gopInMap;
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

        GameObjectProxy* gop = new GameObjectProxy(classId, goId);

        mGameObjectProxys[goId] = gop;
        return *gop;
    }

    GameObjectProxy* GameObjectManager::getGameObjectProxy(unsigned int id) const
    {
        std::map<unsigned int, GameObjectProxy*>::const_iterator it 
                = mGameObjectProxys.find(id);

        if (it != mGameObjectProxys.end())
        {
            return (*it).second;
        }

        return NULL;
    }

    unsigned int GameObjectManager::generateId()
    {
        return mGeneratedId++;
    }

    GameObject* GameObjectManager::createGameObject(const rl::GameObjectProxy *proxy)
    {
        PropertySet* ps = getClassProperties(proxy->getClassId());
        Ogre::String classname =  ps->getProperty(GameObjectProxy::BASE_CLASS).toString().c_str();

        GameObject* go = mGameObjectFactory
            ->createGameObject(
                classname, 
                proxy->getId());
        
        go->setProperties(ps);
        mGameObjects[proxy] = go;
        return go;
    }

    PropertySet* GameObjectManager::getClassProperties(const Ogre::String& classId)
    {
        ClassPropertyMap::const_iterator it = mClassProperties.find(classId);
        if (it == mClassProperties.end())
        {
            Throw(rl::Exception, "No properties found for classId "+classId);
        }
        return (*it).second; 
    }

    void GameObjectManager::setGameObjectFactory(GameObjectFactory* gof)
    {
        mGameObjectFactory = gof;
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
        else if (classname == Person::CLASS_NAME)
        {
            return new Person(id);
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
}
