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

#include "AbstractMapNodeProcessor.h"

#include <OgreColourValue.h>

#include "PropertyReader.h"
#include "XmlHelper.h"

using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;

namespace rl
{
    AbstractMapNodeProcessor::AbstractMapNodeProcessor()
    : mRootSceneNode(NULL),
      mXmlPropertyReader()
    {
    }

    AbstractMapNodeProcessor::~AbstractMapNodeProcessor()
    {
        delete mXmlPropertyReader;
    }

    Vector3 AbstractMapNodeProcessor::processVector3(DOMElement* vec3Elem) const
    {
        Vector3 rval(Vector3::ZERO);
        if (vec3Elem != NULL)
        {
            if (XmlHelper::hasAttribute(vec3Elem, "x"))
            {
                rval.x = XmlHelper::getAttributeValueAsReal(vec3Elem, "x");
            }
            if (XmlHelper::hasAttribute(vec3Elem, "y"))
            {
                rval.y = XmlHelper::getAttributeValueAsReal(vec3Elem, "y");
            }
            if (XmlHelper::hasAttribute(vec3Elem, "z"))
            {
                rval.z = XmlHelper::getAttributeValueAsReal(vec3Elem, "z");
            }
        }

        return rval;
    }

    Quaternion AbstractMapNodeProcessor::processQuaternion(DOMElement* quatElem) const
    {
        Quaternion rval(Quaternion::IDENTITY);
        if (quatElem != NULL)
        {
            if (XmlHelper::hasAttribute(quatElem, "qw"))
            {
                rval.w = XmlHelper::getAttributeValueAsReal(quatElem, "qw");
            }
            if (XmlHelper::hasAttribute(quatElem, "qx"))
            {
                rval.x = XmlHelper::getAttributeValueAsReal(quatElem, "qx");
            }
            if (XmlHelper::hasAttribute(quatElem, "qy"))
            {
                rval.y = XmlHelper::getAttributeValueAsReal(quatElem, "qy");
            }
            if (XmlHelper::hasAttribute(quatElem, "qz"))
            {
                rval.z = XmlHelper::getAttributeValueAsReal(quatElem, "qz");
            }
        }

        return rval;
    }

    ColourValue AbstractMapNodeProcessor::processColour(DOMElement* colElem) const
    {
        ColourValue rval(1, 1, 1, 1);
        if (colElem != NULL)
        {
            if (XmlHelper::hasAttribute(colElem, "r"))
            {
                rval.r = XmlHelper::getAttributeValueAsReal(colElem, "r");
            }
            if (XmlHelper::hasAttribute(colElem, "g"))
            {
                rval.g = XmlHelper::getAttributeValueAsReal(colElem, "g");
            }
            if (XmlHelper::hasAttribute(colElem, "b"))
            {
                rval.b = XmlHelper::getAttributeValueAsReal(colElem, "b");
            }
            if (XmlHelper::hasAttribute(colElem, "a"))
            {
                rval.a = XmlHelper::getAttributeValueAsReal(colElem, "a");
            }
        }
        return rval;
    }

    Ogre::String AbstractMapNodeProcessor::getRandomName(const Ogre::String& baseName) const
    {
//        int rnd = rand();
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

    XmlPropertyReader* AbstractMapNodeProcessor::getXmlPropertyReader() const
    {
        return mXmlPropertyReader;
    }
}
