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
#include "stdinc.h" //precompiled header

#include "GameObjectManager.h"

#include <CEGUIPropertyHelper.h>

#include "Armor.h"
#include "CoreSubsystem.h"
#include "Container.h"
#include "Creature.h"
#include "Exception.h"
#include "GameObject.h"
#include "Item.h"
#include "Person.h"
#include "Properties.h"
#include "XmlPropertyReader.h"
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

        mScriptPatterns.push_back("*.gof");
        Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);
        SaveGameManager::getSingleton().registerSaveGameData(this);
    }

    GameObjectManager::~GameObjectManager()
    {
        SaveGameManager::getSingleton().unregisterSaveGameData(this);
        //unregisterAllGameObjectStateListener();
        //deleteAllGameObjects();
        ///@todo: Delete all game objects, delete all class properties
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
            Ogre::String classId = curPs->getProperty(GameObject::PROPERTY_CLASS_ID).toString().c_str();
            mClassProperties[classId] = curPs;
        }
        delete propReader;

    }

    GameObject* GameObjectManager::getGameObject(unsigned int id) const
    {
        std::map<unsigned int, GameObject*>::const_iterator it
                = mGameObjects.find(id);

        if (it != mGameObjects.end())
        {
            return (*it).second;
        }

        return NULL;
    }

    std::list<const GameObject*> GameObjectManager::getAllGameObjects() const
    {
        std::list<const GameObject*> gos;
        std::map<unsigned int, GameObject*>::const_iterator it;

        //
        //    Run through all GOs and put them into the list
        //
        for( it=mGameObjects.begin(); it!=mGameObjects.end(); ++it )
        {
            gos.push_back(it->second);
        }

        return gos;
    }

    unsigned int GameObjectManager::generateId()
    {
        return mGeneratedId++;
    }

    GameObject* GameObjectManager::createGameObject(
        const Ogre::String& classId, unsigned int id)
    {
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

        PropertySet* ps = getClassProperties(classId);
        Ogre::String classname =  ps->getProperty(GameObject::PROPERTY_BASE_CLASS).toString().c_str();

        GameObject* go = mGameObjectFactory
            ->createGameObject(
                classname,
                goId);

        go->setClassId(classId);
        applyProperties(go, ps);
        mGameObjects[goId] = go;
        return go;
    }

    void GameObjectManager::deleteGameObject(unsigned int id)
    {
        if(mGameObjects.find(id) != mGameObjects.end())
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
        for(std::map<unsigned int, GameObject*>::iterator itr = mGameObjects.begin(); itr != mGameObjects.end();)
        {
            GameObject* go = itr->second;
            itr = mGameObjects.erase(itr);
            go->setActor(NULL);
            delete go;
        }
    }

    void GameObjectManager::applyProperties(GameObject* go, PropertySet* ps) const
    {
        if (ps->hasProperty(GameObject::PROPERTY_INHERITS))
        {
            PropertySet* superClassProps = 
                getClassProperties(ps->getProperty(GameObject::PROPERTY_INHERITS).toString().c_str());
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
			unsigned int goid = CEGUI::PropertyHelper::stringToUint(serializedString.substr(posDivider));
			return createGameObject(classId, goid);
		}

		return NULL;
	}

	Property GameObjectManager::toProperty(const GameObject* const go) const
	{
		return Property(go->getClassId() + "|" + CEGUI::PropertyHelper::uintToString(go->getId()));
	}

    PropertySet* GameObjectManager::getClassProperties(const Ogre::String& classId) const
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

    void GameObjectManager::gameObjectStateChanged(GameObject* go, GameObjectState oldState,
        GameObjectState newState)
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
        for(std::set<GameObjectStateListener*>::iterator itr = mGameObjectStateListeners.begin();
            itr != mGameObjectStateListeners.end();)
        {
            itr = mGameObjectStateListeners.erase(itr);
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

    CeGuiString GameObjectManager::getXmlNodeIdentifier() const
    {
        return "gameobjects";
    }

    using namespace XERCES_CPP_NAMESPACE;
    
    void GameObjectManager::writeData(SaveGameFileWriter *writer)
    {
        DOMElement* gameobjects = writer->appendChildElement(writer->getDocument(), writer->getDocument()->getDocumentElement(), getXmlNodeIdentifier().c_str());

        std::list<const GameObject*> gos = getAllGameObjects();

        for(std::list<const GameObject*>::const_iterator it_gameobjects = gos.begin(); it_gameobjects != gos.end(); it_gameobjects++)
        {
            DOMElement* gameobject = writer->appendChildElement(writer->getDocument(), gameobjects, "gameobject");
            writer->setAttributeValueAsInteger(gameobject, "ID", (*it_gameobjects)->getId());
            writer->setAttributeValueAsString(gameobject, "ClassID", (*it_gameobjects)->getClassId());
            writer->setAttributeValueAsInteger(gameobject, "State", (int)(*it_gameobjects)->getState());

            PropertyMap map = (*it_gameobjects)->getAllProperties()->toPropertyMap();
            writer->writeEachProperty(gameobject, map);
        } 
    }

    void GameObjectManager::readData(SaveGameFileReader *reader)
    {
        deleteAllGameObjects();

        reader->initializeXml();

        DOMNodeList* rootNodeList = reader->getDocument()->getDocumentElement()->getElementsByTagName(AutoXMLCh(getXmlNodeIdentifier().c_str()).data());

        if(rootNodeList->getLength())
        {
            DOMNodeList* xmlGameObjects = static_cast<DOMElement*>(rootNodeList->item(0))->getElementsByTagName(AutoXMLCh("gameobject").data()); //there should be only one "gameobjects" node
            if(xmlGameObjects->getLength())
            {
                for(XMLSize_t childIdx1 = 0; childIdx1 < xmlGameObjects->getLength(); childIdx1++)
                {
                    DOMNode* xmlGameObject = xmlGameObjects->item(childIdx1);
                    if(xmlGameObject->getNodeType() == DOMNode::ELEMENT_NODE)
                    {
                        int ID = reader->getAttributeValueAsInteger(static_cast<DOMElement*>(xmlGameObject), "ID");
                        Ogre::String classID = reader->getAttributeValueAsStdString(static_cast<DOMElement*>(xmlGameObject), "ClassID");
                        GameObjectState state = (GameObjectState)reader->getAttributeValueAsInteger(static_cast<DOMElement*>(xmlGameObject), "State");
                        PropertySet properties = reader->getPropertiesAsSet(static_cast<DOMElement*>(xmlGameObject));
                        GameObject* object = createGameObject(classID, ID);
                        applyProperties(object, &properties);
                        object->setState(state);
                    }
                }
            }
        } 

        MessagePump::getSingleton().sendMessage<MessageType_GameObjectsLoaded>();

        reader->shutdownXml();
    }

    int GameObjectManager::getPriority() const
    {
        return 100;
    }
}
