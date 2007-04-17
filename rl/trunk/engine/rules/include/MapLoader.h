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

#include "RulesPrerequisites.h"

#include <OgreSceneNode.h>

namespace rl {

    class XmlPropertyReader;

    class _RlRulesExport MapLoader 
    {
    public:
        MapLoader();

        /** Loads a Rastullahs Lockenpracht map
        *
        * @param mapresource the name of the map file resource
        * @param loadGameObjects set <code>false</code> if the MapLoader should not load the GameObjects of the map file (e.g. when loading a saved game)
        */
        void loadMap(const Ogre::String& mapresource, const Ogre::String& resourceGroup, bool loadGameObjects = true);

    private:
        enum {ENTITIES, GAMEOBJECTS, SOUNDS, LIGHTS};
    
        XmlPropertyReader* mXmlPropertyProcessor;
        int mStatistics[4];

        Ogre::SceneNode* mSceneNode;
		/// Alle statischen GeometrieNodes
		std::map<int,Ogre::SceneNode*> mStaticNodes;
        Ogre::String mResourceGroup;

        Ogre::String getRandomName(const Ogre::String& baseName) const;
        void processNode(XERCES_CPP_NAMESPACE::DOMElement* nodeElem, bool loadGameObjects);
        Ogre::Vector3 processVector3(XERCES_CPP_NAMESPACE::DOMElement* nodeElem) const;
        Ogre::Quaternion processQuaternion(XERCES_CPP_NAMESPACE::DOMElement* nodeElem) const;

        void createGameObject(XERCES_CPP_NAMESPACE::DOMElement* nodeElem, XERCES_CPP_NAMESPACE::DOMElement* gameobjElem) const;
        void createSound(XERCES_CPP_NAMESPACE::DOMElement* nodeElem, XERCES_CPP_NAMESPACE::DOMElement* gameobjElem) const;
        void createLight(XERCES_CPP_NAMESPACE::DOMElement* nodeElem, XERCES_CPP_NAMESPACE::DOMElement* gameobjElem) const;
        void createEntity(XERCES_CPP_NAMESPACE::DOMElement* nodeElem, XERCES_CPP_NAMESPACE::DOMElement* gameobjElem);
    };

} // namespace rl

#endif //__MapLoader_H__
