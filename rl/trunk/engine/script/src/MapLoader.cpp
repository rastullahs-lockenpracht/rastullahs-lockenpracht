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
#include "LightNodeProcessor.h"
#include "ParticleSystemNodeProcessor.h"
#include "ProgressWindow.h"
#include "XmlPropertyReader.h"
#include "SoundNodeProcessor.h"
#include "WaypointProcessor.h"
#include "World.h"
#include "XmlProcessor.h"
#include "ZoneProcessor.h"

using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;
using std::list;

namespace rl {

    using XERCES_CPP_NAMESPACE::DOMDocument; //XXX: for VS 2003/2005

    MapLoader::MapLoader(const Ogre::String& resourceGroup)
        : mRootSceneNode(NULL),
          mResourceGroup(resourceGroup),
          mPercentageWindow(NULL)
    {
        mNodeProcessors.push_back(new EntityNodeProcessor(resourceGroup));
        mNodeProcessors.push_back(new GameObjectNodeProcessor());
        mNodeProcessors.push_back(new SoundNodeProcessor());
        mNodeProcessors.push_back(new LightNodeProcessor());
		mNodeProcessors.push_back(new ParticleSystemNodeProcessor());
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

    void MapLoader::loadMap(const Ogre::String& mapresource, bool loadGameObjects)
    {
        LOG_MESSAGE(Logger::RULES, "Loading map " + mapresource);

  		initializeXml();

        DOMDocument* doc = loadDocument(mapresource, mResourceGroup);

        if (doc)
        {
            setRootSceneNode(CoreSubsystem::getSingleton().getWorld()
                    ->getSceneManager()->getRootSceneNode()->createChildSceneNode(mapresource));

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

            doc->release();

            LOG_MESSAGE(Logger::RULES, "Map loaded");

			CoreSubsystem::getSingleton().getWorld()->initializeDefaultCamera();
			///@todo: Window fade jobs don't work if Core is paused, think about solution for: CoreSubsystem::getSingleton().setPaused(false);
        }
        else
        {
            LOG_ERROR(Logger::RULES, "Map resource '" + mapresource + "' not found");
        }


        shutdownXml();
    }

    const CeGuiString MapLoader::getClassName() const
    {
        return "MapLoader";
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

        CoreSubsystem::getSingleton().renderOneFrame();

        if (percentage == 1)
        {
            mPercentageWindow->setVisible(false, true);
            mPercentageWindow = NULL;
        }
    }

} // namespace rl
