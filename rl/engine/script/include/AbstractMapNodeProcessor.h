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
#ifndef __AbstractMapNodeProcessor_H__
#define __AbstractMapNodeProcessor_H__

#include "ScriptPrerequisites.h"

#include "XmlPropertyReader.h"

namespace rl
{
    class XmlPropertyReader;

    class AbstractMapNodeProcessor : protected XmlPropertyReader
    {
    public:
        virtual ~AbstractMapNodeProcessor();

        virtual bool processNode(
            const tinyxml2::XMLElement* nodeElem, const Ogre::String& resourceGroup, bool loadGameObjects)
            = 0;

        void setRootSceneNode(Ogre::SceneNode* node);

    protected:
        AbstractMapNodeProcessor();

        Ogre::String getRandomName(const Ogre::String& baseName) const;
        Ogre::Vector3 processVector3(const tinyxml2::XMLElement* nodeElem) const;
        Ogre::Vector2 processVector2(const tinyxml2::XMLElement* nodeElem) const;
        Ogre::Quaternion processQuaternion(const tinyxml2::XMLElement* nodeElem) const;
        Ogre::ColourValue processColour(const tinyxml2::XMLElement* colElem) const;

        Ogre::SceneNode* getRootSceneNode() const;

    private:
        Ogre::SceneNode* mRootSceneNode;
    };
}

#endif // __MapNodeProcessor_H__
