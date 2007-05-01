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
#ifndef __MapLoader_H__
#define __MapLoader_H__

#include <xercesc/dom/DOMElement.hpp>

#include "ScriptPrerequisites.h"

#include <OgreSceneNode.h>
#include <list>

namespace rl {

    class AbstractMapNodeProcessor;
    class ProgressWindow;
    class XmlPropertyReader;

    class _RlScriptExport MapLoader 
    {
    public:
        MapLoader(const Ogre::String& resourceGroup);
        ~MapLoader();

        /** Loads a Rastullahs Lockenpracht map
        *
        * @param mapresource the name of the map file resource
        * @param loadGameObjects set <code>false</code> if the MapLoader should not load the GameObjects of the map file (e.g. when loading a saved game)
        */
        void loadMap(const Ogre::String& mapresource, bool loadGameObjects = true);

    private:
        XmlPropertyReader* mXmlPropertyProcessor;
        std::list<AbstractMapNodeProcessor*> mNodeProcessors;

        Ogre::SceneNode* mRootSceneNode;
        /// Alle statischen GeometrieNodes
        Ogre::String mResourceGroup;
        ProgressWindow* mPercentageWindow;

        void setRootSceneNode(Ogre::SceneNode* node);
        void processSceneNodes(XERCES_CPP_NAMESPACE::DOMElement* nodesElem, bool loadGameObjects);
        void processZones(XERCES_CPP_NAMESPACE::DOMElement* zonesElem);
		void processSkySettings(XERCES_CPP_NAMESPACE::DOMElement* skyElem);
        void setLoadingPercentage(Ogre::Real percentage, const Ogre::String& text = "");
    };

} // namespace rl

#endif //__MapLoader_H__
