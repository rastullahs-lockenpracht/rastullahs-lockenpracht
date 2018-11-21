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

#include "MapLoader.h"

#include "AbstractMapNodeProcessor.h"
#include "ContentModule.h"
#include "CoreSubsystem.h"
#include "EntityNodeProcessor.h"
#include "EnvironmentProcessor.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "GameObjectNodeProcessor.h"
#include "LightNodeProcessor.h"
#include "ParticleSystemNodeProcessor.h"
#include "PlaneNodeProcessor.h"
#include "ProgressWindow.h"
#include "RubyInterpreter.h"
#include "SoundNodeProcessor.h"
#include "WaypointProcessor.h"
#include "World.h"
#include "XmlProcessor.h"
#include "XmlPropertyReader.h"
#include "ZoneProcessor.h"

using namespace Ogre;
using std::list;

namespace rl
{

    const CeGuiString MapLoader::PROPERTY_ACTIVEMAPS = "activemaps";

    MapLoader::MapLoader()
        : mRootSceneNode(NULL)
        , mResourceGroup(CoreSubsystem::getSingleton().getActiveAdventureModule()->getId())
        , mPercentageWindow(NULL)
        , ContentLoader(mResourceGroup)
    {
        initialize(mResourceGroup);
    }

    MapLoader::MapLoader(const Ogre::String& resourceGroup)
        : mRootSceneNode(NULL)
        , mResourceGroup(resourceGroup)
        , mPercentageWindow(NULL)
        , ContentLoader(resourceGroup)
    {
        initialize(resourceGroup);
    }

    MapLoader::~MapLoader()
    {
        for (std::list<AbstractMapNodeProcessor*>::const_iterator it = mNodeProcessors.begin();
             it != mNodeProcessors.end(); ++it)
        {
            delete *it;
        }
        delete mPercentageWindow;
    }

    void MapLoader::initialize(const Ogre::String& resourceGroup)
    {
        mNodeProcessors.push_back(new EntityNodeProcessor(resourceGroup));
        mNodeProcessors.push_back(new GameObjectNodeProcessor());
        mNodeProcessors.push_back(new SoundNodeProcessor());
        mNodeProcessors.push_back(new LightNodeProcessor());
        mNodeProcessors.push_back(new ParticleSystemNodeProcessor());
        mNodeProcessors.push_back(new PlaneNodeProcessor());

        RequestedSceneChangeConnection
            = MessagePump::getSingleton().addMessageHandler<MessageType_SceneChangeRequested>(
                boost::bind(&MapLoader::changeScene, this, _1));
    }

    void MapLoader::loadContent()
    {
        if (mLoadedMaps.empty()) // No maps defined to load -> no savegame
        {
            loadScene(mDefaultMaps);
        }
        else
        {
            loadScene(mLoadedMaps);
        }
    }

    void MapLoader::unloadContent()
    {
        unloadAllMaps(false);
    }

    void MapLoader::setDefaultMaps(Ogre::StringVector maps)
    {
        mDefaultMaps = maps;
    }

    void MapLoader::loadScene(Ogre::StringVector mapresources, bool loadGameObjects)
    {
        for (Ogre::StringVector::const_iterator it = mapresources.begin(); it != mapresources.end(); ++it)
        {
            loadMap(*it);
        }
    }

    void MapLoader::loadMap(const Ogre::String& mapresource, bool loadGameObjects)
    {
        bool mapLoaded = false;
        for (Ogre::StringVector::const_iterator it = mLoadedMaps.begin(); it != mLoadedMaps.end(); ++it)
        {
            if (*it == mapresource)
                mapLoaded = true;
        }
        if (!mapLoaded)
        {
            LOG_MESSAGE(Logger::RULES, "Loading map " + mapresource);

            tinyxml2::XMLDocument* doc = loadDocument(mapresource, mResourceGroup);

            if (doc)
            {
                setRootSceneNode(CoreSubsystem::getSingleton()
                                     .getWorld()
                                     ->getSceneManager()
                                     ->getRootSceneNode()
                                     ->createChildSceneNode(mapresource));

                tinyxml2::XMLElement* dataDocumentContent = doc->RootElement();

                if (getAttributeValueAsString(dataDocumentContent, "formatVersion") != "0.4.0")
                    LOG_ERROR(Logger::SCRIPT, "Map format version doesn't match with the required version");

                CoreSubsystem::getSingleton().getWorld()->initializeDefaultCamera();
                ///@todo: Window fade jobs don't work if Core is paused, think about solution for:
                ///CoreSubsystem::getSingleton().setPaused(true);

                LOG_MESSAGE(Logger::RULES, "Processing nodes");

                processSceneNodes(getChildNamed(dataDocumentContent, "nodes"), loadGameObjects);

                ZoneProcessor zp;
                zp.processNode(getChildNamed(dataDocumentContent, "zones"), mResourceGroup, loadGameObjects);

                EnvironmentProcessor ep;
                ep.processNode(getChildNamed(dataDocumentContent, "environment"), mResourceGroup, loadGameObjects);

                WaypointProcessor wp;
                wp.processNode(getChildNamed(dataDocumentContent, "waypoints"), mResourceGroup, loadGameObjects);

                LOG_MESSAGE(Logger::SCRIPT, "Map " + mapresource + " loaded");

                if (hasAttribute(dataDocumentContent, "scenescript"))
                {
                    if (getAttributeValueAsString(dataDocumentContent, "scenescript").length() != 0)
                    {
                        if (!CoreSubsystem::getSingleton().getRubyInterpreter()->executeFile(
                                getAttributeValueAsStdString(dataDocumentContent, "scenescript")))
                            LOG_MESSAGE(Logger::SCRIPT, "Executed init script of map " + mapresource);
                        else
                            LOG_ERROR(Logger::SCRIPT, "Error while executing init script of map " + mapresource);
                    }
                }

                CoreSubsystem::getSingleton().getWorld()->initializeDefaultCamera();
                ///@todo: Window fade jobs don't work if Core is paused, think about solution for:
                ///CoreSubsystem::getSingleton().setPaused(false);
            }
            else
            {
                LOG_ERROR(Logger::RULES, "Map resource '" + mapresource + "' not found");
            }

            mLoadedMaps.push_back(mapresource);
        }
        else
        {
            LOG_ERROR(Logger::SCRIPT, "Map is already loaded!");
        }
    }

    void MapLoader::requestSceneChange(StringVector mapresources)
    {
        LOG_MESSAGE(Logger::SCRIPT, "requested changing scene " + Ogre::StringConverter::toString(mapresources));
        MessagePump::getSingleton().postMessage<MessageType_SceneChangeRequested>(mapresources);
    }

    void MapLoader::unloadAllMaps(bool removeGameObjects)
    {
        mLoadedMaps.clear();
        if (removeGameObjects)
            GameObjectManager::getSingleton().getAllGameObjects();
        else
        {
            std::list<GameObject*> gos = GameObjectManager::getSingleton().getAllGameObjects();
            for (std::list<GameObject*>::const_iterator it = gos.begin(); it != gos.end(); ++it)
            {
                (*it)->removeFromScene();
            }
        }
        CoreSubsystem::getSingleton().getWorld()->clearScene();
    }

    const CeGuiString MapLoader::getClassName() const
    {
        return "MapLoader";
    }

    const Property MapLoader::getProperty(const CeGuiString& key) const
    {
        if (PROPERTY_ACTIVEMAPS == key)
        {
            PropertyArray vec;
            for (Ogre::StringVector::const_iterator it = mLoadedMaps.begin(); it != mLoadedMaps.end(); ++it)
            {
                vec.push_back(Property(*it));
            }
            return Property(vec);
        }
        return ContentLoader::getProperty(key);
    }

    void MapLoader::setProperty(const CeGuiString& key, const Property& value)
    {
        if (PROPERTY_ACTIVEMAPS == key)
        {
            mLoadedMaps.clear();
            PropertyArray vec(value.toArray());
            for (PropertyArray::const_iterator it = vec.begin(); it != vec.end(); ++it)
            {
                mLoadedMaps.push_back(it->toString().c_str());
            }
        }
        else
            ContentLoader::setProperty(key, value);
    }

    PropertyKeys MapLoader::getAllPropertyKeys() const
    {
        PropertyKeys keys = ContentLoader::getAllPropertyKeys();
        keys.insert(PROPERTY_ACTIVEMAPS);
        return keys;
    }

    void MapLoader::processSceneNodes(const tinyxml2::XMLElement* nodesElem, bool loadGameObjects)
    {
        if (nodesElem == NULL)
        {
            return;
        }

        setLoadingPercentage(0, "Loading map nodes");
        Ogre::Real numChildren = 0;
        for (const tinyxml2::XMLNode* cur = nodesElem->FirstChild(); cur != NULL; cur = cur->NextSibling())
        {
            numChildren++;
        }

        int count = 0;

        for (const tinyxml2::XMLNode* cur = nodesElem->FirstChild(); cur != NULL; cur = cur->NextSibling())
        {
            const tinyxml2::XMLElement* curElem = cur->ToElement();

            if (curElem)
            {
                std::list<AbstractMapNodeProcessor*>::iterator it = mNodeProcessors.begin();
                while (it != mNodeProcessors.end() && !(*it)->processNode(curElem, mResourceGroup, loadGameObjects))
                {
                    ++it;
                }

                if (it == mNodeProcessors.end())
                {
                    LOG_WARNING(Logger::RULES, "Node " + Ogre::String(curElem->Value()) + " could not be processed.");
                }
            }

            count += 1;
            if (count % 250 == 0)
            {
                setLoadingPercentage(
                    count / numChildren, Ogre::StringConverter::toString(count / numChildren * 100.0f, 0) + "%");
            }
        }

        setLoadingPercentage(1);
    }

    void MapLoader::setRootSceneNode(SceneNode* node)
    {
        mRootSceneNode = node;
        for (std::list<AbstractMapNodeProcessor*>::const_iterator it = mNodeProcessors.begin();
             it != mNodeProcessors.end(); ++it)
        {
            (*it)->setRootSceneNode(node);
        }
    }

    void MapLoader::setLoadingPercentage(Ogre::Real percentage, const Ogre::String& text)
    {
        if (mPercentageWindow == NULL)
        {
            mPercentageWindow = new ProgressWindow();
            mPercentageWindow->setVisible(true);
        }

        mPercentageWindow->setProgress(percentage);
        if (text != "")
        {
            mPercentageWindow->setText(text);
        }

        CoreSubsystem::getSingleton().renderOneFrame(false);

        if (percentage == 1)
        {
            mPercentageWindow->setVisible(false, true);
            mPercentageWindow = NULL;
        }
    }

    bool MapLoader::changeScene(Ogre::StringVector mapresources)
    {
        LOG_MESSAGE(Logger::SCRIPT, "Changing scene " + Ogre::StringConverter::toString(mapresources));
        unloadAllMaps(false);
        loadScene(mapresources);
        return true;
    }

} // namespace rl
