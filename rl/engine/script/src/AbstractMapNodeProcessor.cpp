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

#include "AbstractMapNodeProcessor.h"

using namespace Ogre;

namespace rl
{
    AbstractMapNodeProcessor::AbstractMapNodeProcessor()
        : mRootSceneNode(NULL)
        , XmlPropertyReader()
    {
    }

    AbstractMapNodeProcessor::~AbstractMapNodeProcessor()
    {
    }

    Vector3 AbstractMapNodeProcessor::processVector3(const tinyxml2::XMLElement* vec3Elem) const
    {
        Vector3 rval(Vector3::ZERO);
        if (vec3Elem != NULL)
        {
            if (hasAttribute(vec3Elem, "x"))
            {
                rval.x = getAttributeValueAsReal(vec3Elem, "x");
            }
            if (hasAttribute(vec3Elem, "y"))
            {
                rval.y = getAttributeValueAsReal(vec3Elem, "y");
            }
            if (hasAttribute(vec3Elem, "z"))
            {
                rval.z = getAttributeValueAsReal(vec3Elem, "z");
            }
        }

        return rval;
    }

    Vector2 AbstractMapNodeProcessor::processVector2(const tinyxml2::XMLElement* vec2Elem) const
    {
        Vector2 rval(Vector2::ZERO);
        if (vec2Elem != NULL)
        {
            if (hasAttribute(vec2Elem, "x"))
            {
                rval.x = getAttributeValueAsReal(vec2Elem, "x");
            }
            if (hasAttribute(vec2Elem, "y"))
            {
                rval.y = getAttributeValueAsReal(vec2Elem, "y");
            }
        }

        return rval;
    }

    Quaternion AbstractMapNodeProcessor::processQuaternion(const tinyxml2::XMLElement* quatElem) const
    {
        Quaternion rval(Quaternion::IDENTITY);
        if (quatElem != NULL)
        {
            if (hasAttribute(quatElem, "qw"))
            {
                rval.w = getAttributeValueAsReal(quatElem, "qw");
            }
            if (hasAttribute(quatElem, "qx"))
            {
                rval.x = getAttributeValueAsReal(quatElem, "qx");
            }
            if (hasAttribute(quatElem, "qy"))
            {
                rval.y = getAttributeValueAsReal(quatElem, "qy");
            }
            if (hasAttribute(quatElem, "qz"))
            {
                rval.z = getAttributeValueAsReal(quatElem, "qz");
            }
        }

        return rval;
    }

    ColourValue AbstractMapNodeProcessor::processColour(const tinyxml2::XMLElement* colElem) const
    {
        ColourValue rval(1, 1, 1, 1);
        if (colElem != NULL)
        {
            if (hasAttribute(colElem, "r"))
            {
                rval.r = getAttributeValueAsReal(colElem, "r");
            }
            if (hasAttribute(colElem, "g"))
            {
                rval.g = getAttributeValueAsReal(colElem, "g");
            }
            if (hasAttribute(colElem, "b"))
            {
                rval.b = getAttributeValueAsReal(colElem, "b");
            }
            if (hasAttribute(colElem, "a"))
            {
                rval.a = getAttributeValueAsReal(colElem, "a");
            }
        }
        return rval;
    }

    Ogre::String AbstractMapNodeProcessor::getRandomName(const Ogre::String& baseName) const
    {
        static int ival = 0;
        std::stringstream rval;
        rval << baseName << "_" << ++ival;
        return rval.str();
    }

    void AbstractMapNodeProcessor::setRootSceneNode(Ogre::SceneNode* node)
    {
        mRootSceneNode = node;
    }

    Ogre::SceneNode* AbstractMapNodeProcessor::getRootSceneNode() const
    {
        return mRootSceneNode;
    }
}
