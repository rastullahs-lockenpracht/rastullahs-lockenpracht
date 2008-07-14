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

#include "SceneLoader.h"

#include <xercesc/dom/DOM.hpp>

#include "Scene.h"

using namespace XERCES_CPP_NAMESPACE;

namespace rl
{

Scene* SceneLoader::loadScene(Ogre::DataStreamPtr &data)
{
    initializeXml();

    DOMDocument* doc = loadDocument(data);
    DOMElement* root = doc->getDocumentElement();
    Scene* scene = new Scene(getAttributeValueAsString(root, "name"));
    
    DOMNodeList* children = root->getChildNodes();
    for (XMLSize_t i = 0; i < children->getLength(); ++i)
    {
        DOMNode* cur = children->item(i);
        if (cur->getNodeType() == DOMNode::ELEMENT_NODE)
        {
            DOMElement* elem = static_cast<DOMElement*>(cur);
            if (hasNodeName(elem, "map"))
            {
                scene->addMap(getAttributeValueAsStdString(elem, "file"));
            }
        }
    }

    shutdownXml();

    return scene;
}

}
