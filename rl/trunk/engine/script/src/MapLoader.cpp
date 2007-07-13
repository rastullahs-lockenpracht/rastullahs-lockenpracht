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

#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "MapLoader.h"

#include <OgreResourceManager.h>

#include "AbstractMapNodeProcessor.h"
#include "CoreSubsystem.h"
#include "EntityNodeProcessor.h"
#include "EnvironmentProcessor.h"
#include "GameObjectNodeProcessor.h"
#include "LightNodeProcessor.h"
#include "ParticleSystemNodeProcessor.h"
#include "ProgressWindow.h"
#include "PropertyReader.h"
#include "SoundNodeProcessor.h"
#include "WaypointProcessor.h"
#include "World.h"
#include "XmlHelper.h"
#include "XmlResource.h"
#include "XmlResourceManager.h"
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
        mXmlPropertyProcessor = new XmlPropertyReader();

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
        delete mXmlPropertyProcessor;
        delete mPercentageWindow;
    }

    void MapLoader::loadMap(const Ogre::String& mapresource, bool loadGameObjects)
    {
        LOG_MESSAGE(Logger::RULES, "Loading map " + mapresource);
  		XMLPlatformUtils::Initialize();

		XmlHelper::initializeTranscoder();
		XercesDOMParser* parser = new XercesDOMParser();

        parser->setValidationScheme(XercesDOMParser::Val_Auto);    // optional.
        parser->setDoNamespaces(true);    // optional

        XmlPtr res = XmlResourceManager::getSingleton().getByName(mapresource);
        if (res.isNull())
        {
            Ogre::String group = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
            if (ResourceGroupManager::getSingleton().resourceExists(
                mResourceGroup, mapresource))
            {
                group = mResourceGroup;
            }
            res = XmlResourceManager::getSingleton().create(mapresource, group);
        }

        if (!res.isNull())
        {
            res->parseBy(parser);

            setRootSceneNode(CoreSubsystem::getSingleton().getWorld()
                    ->getSceneManager()->getRootSceneNode()->createChildSceneNode(mapresource));

            DOMDocument* doc = parser->getDocument();
            DOMElement* dataDocumentContent = doc->getDocumentElement();

			CoreSubsystem::getSingleton().getWorld()->initializeDefaultCamera();
			///@todo: Window fade jobs don't work if Core is paused, think about solution for: CoreSubsystem::getSingleton().setPaused(true);

            LOG_MESSAGE(Logger::RULES, "Processing nodes");

            processSceneNodes(XmlHelper::getChildNamed(dataDocumentContent, "nodes"), loadGameObjects);

			ZoneProcessor zp;
			zp.processNode(XmlHelper::getChildNamed(dataDocumentContent, "zones"), loadGameObjects);

			EnvironmentProcessor ep;
			ep.processNode(XmlHelper::getChildNamed(dataDocumentContent, "environment"), loadGameObjects);

			WaypointProcessor wp;
			wp.processNode(XmlHelper::getChildNamed(dataDocumentContent, "waypoints"), loadGameObjects);

            LOG_MESSAGE(Logger::RULES, "Map loaded");

            doc->release();

			CoreSubsystem::getSingleton().getWorld()->initializeDefaultCamera();
			///@todo: Window fade jobs don't work if Core is paused, think about solution for: CoreSubsystem::getSingleton().setPaused(false);
        }
        else
        {
            LOG_ERROR(Logger::RULES, "Map resource '" + mapresource + "' not found");
        }

		XMLPlatformUtils::Terminate();
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
                        "Node " + XmlHelper::transcodeToString(curElem->getNodeName()) + " could not be processed.");
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
