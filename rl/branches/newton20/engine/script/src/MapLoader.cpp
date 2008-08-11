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

#include <xercesc/dom/DOM.hpp>

#include "AbstractMapNodeProcessor.h"
#include "CoreSubsystem.h"
#include "EntityNodeProcessor.h"
#include "EnvironmentProcessor.h"
#include "GameObjectNodeProcessor.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "LightNodeProcessor.h"
#include "ParticleSystemNodeProcessor.h"
#include "ProgressWindow.h"
#include "XmlPropertyReader.h"
#include "SoundNodeProcessor.h"
#include "WaypointProcessor.h"
#include "World.h"
#include "XmlProcessor.h"
#include "ZoneProcessor.h"
#include "ContentModule.h"
#include "RubyInterpreter.h"
#include "PlaneNodeProcessor.h"

using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;
using std::list;

namespace rl {

    using XERCES_CPP_NAMESPACE::DOMDocument; //XXX: for VS 2003/2005

    const CeGuiString MapLoader::PROPERTY_ACTIVEMAPS = "activemaps";

    MapLoader::MapLoader()
        : mRootSceneNode(NULL),
          mResourceGroup(CoreSubsystem::getSingleton().getActiveAdventureModule()->getId()),
          mPercentageWindow(NULL),
          ContentLoader(mResourceGroup)
    {
        initialize(mResourceGroup);
    }

    MapLoader::MapLoader(const Ogre::String& resourceGroup)
        : mRootSceneNode(NULL),
          mResourceGroup(resourceGroup),
          mPercentageWindow(NULL),
          ContentLoader(resourceGroup)
    {
        initialize(resourceGroup);
    }

    MapLoader::~MapLoader()
    {
        for (list<AbstractMapNodeProcessor*>::const_iterator it = mNodeProcessors.begin();
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

        RequestedSceneChangeConnection = MessagePump::getSingleton().addMessageHandler<MessageType_SceneChangeRequested>(
            boost::bind(&MapLoader::changeScene, this, _1));
    }

    void MapLoader::loadContent()
    {
        if(mLoadedMaps.empty()) // No maps defined to load -> no savegame
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
        for(Ogre::StringVector::const_iterator it = mapresources.begin(); it != mapresources.end(); ++it)
        {
            loadMap(*it);
        }
    }

    void MapLoader::loadMap(const Ogre::String& mapresource, bool loadGameObjects)
    {
        bool mapLoaded = false;
        for(Ogre::StringVector::const_iterator it = mLoadedMaps.begin(); it != mLoadedMaps.end(); ++it)
        {
            if(*it == mapresource)
                mapLoaded = true;
        }
        if(!mapLoaded)
        {
            LOG_MESSAGE(Logger::RULES, "Loading map " + mapresource);

  		    initializeXml();

            DOMDocument* doc = loadDocument(mapresource, mResourceGroup);

            if (doc)
            {
                setRootSceneNode(CoreSubsystem::getSingleton().getWorld()
                        ->getSceneManager()->getRootSceneNode()->createChildSceneNode(mapresource));

                if(getAttributeValueAsString(doc->getDocumentElement(), "formatVersion") != "0.4.0")
                    LOG_ERROR(Logger::SCRIPT, "Map format version doesn't match with the required version");

			    CoreSubsystem::getSingleton().getWorld()->initializeDefaultCamera();
			    ///@todo: Window fade jobs don't work if Core is paused, think about solution for: CoreSubsystem::getSingleton().setPaused(true);

                LOG_MESSAGE(Logger::RULES, "Processing nodes");

                DOMElement* dataDocumentContent = doc->getDocumentElement();
                processSceneNodes(getChildNamed(dataDocumentContent, "nodes"), loadGameObjects);

			    ZoneProcessor zp;
			    zp.processNode(getChildNamed(dataDocumentContent, "zones"), loadGameObjects);

			    EnvironmentProcessor ep;
			    ep.processNode(getChildNamed(dataDocumentContent, "environment"), loadGameObjects);

			    WaypointProcessor wp;
			    wp.processNode(getChildNamed(dataDocumentContent, "waypoints"), loadGameObjects);

                LOG_MESSAGE(Logger::SCRIPT, "Map " + mapresource + " loaded");

                if(hasAttribute(doc->getDocumentElement(), "scenescript"))
                {
                    if(getAttributeValueAsString(doc->getDocumentElement(), "scenescript").length() != 0)
                    {
                        if(!CoreSubsystem::getSingleton().getRubyInterpreter()->executeFile(getAttributeValueAsStdString(doc->getDocumentElement(), "scenescript")))
                            LOG_MESSAGE(Logger::SCRIPT, "Executed init script of map " + mapresource);
                        else
                            LOG_ERROR(Logger::SCRIPT, "Error while executing init script of map " + mapresource);
                    }
                }


			    CoreSubsystem::getSingleton().getWorld()->initializeDefaultCamera();
			    ///@todo: Window fade jobs don't work if Core is paused, think about solution for: CoreSubsystem::getSingleton().setPaused(false);
            }
            else
            {
                LOG_ERROR(Logger::RULES, "Map resource '" + mapresource + "' not found");
            }

            shutdownXml();
            
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
        if(removeGameObjects)
            GameObjectManager::getSingleton().getAllGameObjects();
        else
        {
            std::list<GameObject*> gos = GameObjectManager::getSingleton().getAllGameObjects();
            for(std::list<GameObject*>::const_iterator it = gos.begin(); it != gos.end(); ++it)
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
        if(PROPERTY_ACTIVEMAPS == key)
        {
            PropertyArray vec;
            for(Ogre::StringVector::const_iterator it = mLoadedMaps.begin(); it != mLoadedMaps.end(); ++it)
            {
                vec.push_back(Property(*it));
            }
            return Property(vec);
        }
        return ContentLoader::getProperty(key);
    }
    
    void MapLoader::setProperty(const CeGuiString& key, const Property& value)
    {
        if(PROPERTY_ACTIVEMAPS == key)
        {
            mLoadedMaps.clear();
            PropertyArray vec(value.toArray());
            for(PropertyArray::const_iterator it = vec.begin(); it != vec.end(); ++it)
            {
                mLoadedMaps.push_back(it->toString().c_str());
            }
        }
        else
            ContentLoader::setProperty(key,value);
    }

    PropertyKeys MapLoader::getAllPropertyKeys() const
    {
        PropertyKeys keys = ContentLoader::getAllPropertyKeys();
        keys.insert(PROPERTY_ACTIVEMAPS);
        return keys;
    }

    void MapLoader::processSceneNodes(DOMElement* nodesElem, bool loadGameObjects)
    {
		if (nodesElem == NULL)
		{
			return;
		}

        setLoadingPercentage(0, "Loading map nodes");
        Ogre::Real numChildren = nodesElem->getChildNodes()->getLength();
        int count = 0;

        for (DOMNode* cur = nodesElem->getFirstChild();
            cur != NULL;
            cur = cur->getNextSibling())
        {
            if (cur->getNodeType() == DOMNode::ELEMENT_NODE)
            {
                DOMElement* curElem = static_cast<DOMElement*>(cur);

                list<AbstractMapNodeProcessor*>::iterator it = mNodeProcessors.begin();
                while (it != mNodeProcessors.end() && !(*it)->processNode(curElem, loadGameObjects))
                {
                    ++it;
                }

                if (it == mNodeProcessors.end())
                {
                    LOG_WARNING(Logger::RULES,
                        "Node " + transcodeToString(curElem->getNodeName()) + " could not be processed.");
                }
            }

			count += 1;
            if (count % 250 == 0)
            {
                setLoadingPercentage(count/numChildren,
                    Ogre::StringConverter::toString(count/numChildren*100.0f, 0) + "%");
            }
        }

		setLoadingPercentage(1);
    }

    void MapLoader::setRootSceneNode(SceneNode* node)
    {
        mRootSceneNode = node;
        for (list<AbstractMapNodeProcessor*>::const_iterator it = mNodeProcessors.begin();
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
