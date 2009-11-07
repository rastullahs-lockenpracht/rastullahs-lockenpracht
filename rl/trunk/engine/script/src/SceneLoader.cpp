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

#include "Scene.h"

namespace rl
{

Scene* SceneLoader::loadScene(Ogre::DataStreamPtr &data)
{
    TiXmlDocument* doc = loadDocument(data);
    TiXmlElement* root = doc->RootElement();
    Scene* scene = new Scene(getAttributeValueAsString(root, "name"));
    
    for (TiXmlNode* cur = root->FirstChild(); cur; cur = cur->NextSibling())
    {
        if (cur->Type() == TiXmlNode::ELEMENT)
        {
            TiXmlElement* elem = cur->ToElement();
            if (hasNodeName(elem, "map"))
            {
                scene->addMap(getAttributeValueAsStdString(elem, "file"));
            }
        }
    }

    delete doc;

    return scene;
}

}
