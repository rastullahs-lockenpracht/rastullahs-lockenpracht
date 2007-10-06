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

#include "XmlProcessor.h"

#include "Properties.h"
#include "XmlPropertyWriter.h"

#include <OgreResourceManager.h>
#include <CEGUIPropertyHelper.h>


using namespace XERCES_CPP_NAMESPACE;
using namespace Ogre;

namespace rl {

    XmlPropertyWriter::XmlPropertyWriter()
    {
    }

    XmlPropertyWriter::~XmlPropertyWriter()
    {
    }

    DOMDocument* XmlPropertyWriter::getDocument()
    {
        return mDocument;
    }

    void XmlPropertyWriter::setPropertySets(std::vector<PropertySet*> sets)
    {
        mPropertySets = sets;
    }

    void XmlPropertyWriter::addPropertySet(PropertySet* set)
    {
        mPropertySets.push_back(set);
    }

    DOMElement* XmlPropertyWriter::processProperty(DOMElement* parent, PropertyEntry entry)
    {
        DOMElement* element = NULL;
        if(!entry.second.isEmpty())
        {
            if(entry.second.isArray())
            {
                element = this->processPropertyArray(parent, entry.first.c_str(), entry.second.toArray());
            }
            else if(entry.second.isMap())
            {
                element = this->processPropertyMap(parent, entry.first.c_str(), entry.second.toMap());
            }
            else
            {
                element = appendChildElement(mDocument, parent, "property");
                if(entry.first != "\0")
                    setAttribute(element, "name", entry.first.c_str());

                //Ogre::String typeName = entry.second.getTypeName();
                Ogre::String name = entry.second.getName();
                Ogre::StringUtil::toUpperCase(name);
                setAttributeValueAsString(element, "type", name);

                if(entry.second.isBool())
                    setAttributeValueAsBool(element, "data", entry.second.toBool());
                else if(entry.second.isInt())
                    setAttributeValueAsInteger(element, "data", entry.second.toInt());
                else if(entry.second.isIntPair())
                    setAttributeValueAsIntegerPair(element, "data", entry.second.toIntPair());
                else if(entry.second.isIntPair())
                    setAttributeValueAsIntegerTriple(element, "data", entry.second.toIntTriple());
                else if(entry.second.isQuaternion())
                    setValueAsQuaternion(element, entry.second.toQuaternion());
                else if(entry.second.isReal())
                    setAttributeValueAsReal(element, "data", entry.second.toReal());
                else if(entry.second.isString())
                    setAttributeValueAsString(element, "data", entry.second.toString());
                else if(entry.second.isVector3())
                    setValueAsVector3(element, entry.second.toVector3());
            }
        }
        return element;
    }

    DOMElement* XmlPropertyWriter::processPropertySet(DOMElement* parent, const char* const name, PropertySet set)
    {
        PropertyMap map = set.toPropertyMap();

        return processPropertyMap(parent, name, map);
    }

    DOMElement* XmlPropertyWriter::processPropertyArray(DOMElement *parent, const char *const name, PropertyVector vector)
    {
        DOMElement* element = appendChildElement(mDocument, parent, "property");
        setAttribute(element, "name", name);
        setAttribute(element, "type", "ARRAY");

        PropertyVector::iterator iter;
        for(iter = vector.begin(); iter != vector.end(); iter++)
        {
            processProperty(element, PropertyEntry("\0",*iter));
        }
        return element;
    }

    DOMElement* XmlPropertyWriter::processPropertyMap(DOMElement *parent, const char *const name, PropertyMap map)
    {
        DOMElement* element = appendChildElement(mDocument, parent, "property");
        setAttribute(element, "name", name);
        setAttribute(element, "type", "MAP");

        PropertyMap::iterator iter;
        for(iter = map.begin(); iter != map.end(); iter++)
        {
            processProperty(element, PropertyEntry(iter->first.c_str(), iter->second));
        }
        return element;
    }

    void XmlPropertyWriter::writeEachProperty(DOMElement* parent, const rl::PropertyMap &map)
    {
        PropertyMap::const_iterator it_properties;
        for(it_properties = map.begin(); it_properties != map.end(); it_properties++)
        {
            this->processProperty(parent, PropertyEntry(it_properties->first.c_str(), it_properties->second));
        }
    }
}
