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

#include "Scene.h"

#include "CoreSubsystem.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "MapLoader.h"
#include "World.h"

namespace rl 
{

const Ogre::String Scene::PROPERTY_GAMEOBJECTS = "gameobjects";

Scene::Scene(const CeGuiString& name)
: mName(name)
{
}

Scene::~Scene() 
{
}

void Scene::addMap(const Ogre::String &mapfile)
{
    mMaps.push_back(mapfile);
}

void Scene::load(bool loadGameObjects)
{
    CoreSubsystem::getSingleton().getWorld()->clearScene();
    MapLoader* loader = new MapLoader();
    loader->loadScene(mMaps, loadGameObjects);
}

const CeGuiString& Scene::getName() const
{
    return mName;
}

const Property Scene::getProperty(const CeGuiString& key) const
{
    Property prop;

    if (key == PROPERTY_GAMEOBJECTS)
    {
        PropertyArray goProps;

        std::list<GameObject*> golist = GameObjectManager::getSingleton().getAllGameObjects();
        std::list<GameObject*>::iterator end = golist.end();
        for (std::list<GameObject*>::iterator it = golist.begin(); it != end; ++it)
        {
            GameObject* cur = *it;
            if (cur->getState() == GOS_LOADED || cur->getState() == GOS_IN_SCENE)
            {
                goProps.push_back(cur->getAllProperties()->toPropertyMap());
            }
        }

        prop = goProps;
    }
    else
    {
        Throw(IllegalArgumentException, "Property '" + key + "' is no property of a scene.");
    }

    return prop;
}

void Scene::setProperty(const CeGuiString& key, const Property& value)
{
    try 
    {
        if (key == PROPERTY_GAMEOBJECTS)
        {
            PropertyArray goProps = value.toArray();

            for (PropertyArray::iterator it = goProps.begin(); it != goProps.end(); ++it)
            {
                PropertyRecord cur(it->toMap());
                GameObject* go = GameObjectManager::getSingleton().createGameObject(
                    cur.getProperty(GameObject::PROPERTY_BASE_CLASS).toString(),
                    cur.getProperty(GameObject::PROPERTY_CLASS_ID).toInt());
            }
        }
        else
        {
            LOG_WARNING("Scene", "Property '" + key + "' is no property of a scene.");
        }
    }
    catch (WrongFormatException ex)
    {
        LOG_ERROR(
            "Scene",
            "property " + key + " has the wrong format");
    }
}

PropertyKeys Scene::getAllPropertyKeys() const
{
    PropertyKeys keys;
    keys.insert(Scene::PROPERTY_GAMEOBJECTS);
    return keys;
}

}