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

#include "XmlProcessor.h"

#include "Properties.h"
#include "XmlPropertyWriter.h"

#include <CEGUI/PropertyHelper.h>

using namespace Ogre;

namespace rl
{

    XmlPropertyWriter::XmlPropertyWriter()
    {
    }

    XmlPropertyWriter::~XmlPropertyWriter()
    {
    }

    tinyxml2::XMLDocument* XmlPropertyWriter::getDocument()
    {
        return mDocument;
    }

    void XmlPropertyWriter::setPropertyRecords(PropertyRecordVector sets)
    {
        mPropertyRecords = sets;
    }

    void XmlPropertyWriter::addPropertyRecord(PropertyRecordPtr set)
    {
        mPropertyRecords.push_back(set);
    }

    tinyxml2::XMLElement* XmlPropertyWriter::processProperty(tinyxml2::XMLElement* parent, const PropertyEntry& entry)
    {
        tinyxml2::XMLElement* element = NULL;
        if (!entry.second.isEmpty())
        {
            if (entry.second.isArray())
            {
                element = this->processPropertyArray(parent, entry.first.c_str(), entry.second.toArray());
            }
            else if (entry.second.isMap())
            {
                element = this->processPropertyMap(parent, entry.first.c_str(), entry.second.toMap());
            }
            else
            {
                element = appendChildElement(parent, "property");
                if (!entry.first.empty())
                    setAttribute(element, "name", entry.first.c_str());

                // Ogre::String typeName = entry.second.getTypeName();
                Ogre::String name = entry.second.getName();
                Ogre::StringUtil::toUpperCase(name);
                setAttributeValueAsString(element, "type", name);

                if (entry.second.isBool())
                    setAttributeValueAsBool(element, "data", entry.second.toBool());
                else if (entry.second.isInt())
                    setAttributeValueAsInteger(element, "data", entry.second.toInt());
                else if (entry.second.isIntPair())
                    setAttributeValueAsIntegerPair(element, "data", entry.second.toIntPair());
                else if (entry.second.isIntTriple())
                    setAttributeValueAsIntegerTriple(element, "data", entry.second.toIntTriple());
                else if (entry.second.isQuaternion())
                    setAttributeValueAsQuaternion(element, "data", entry.second.toQuaternion());
                else if (entry.second.isReal())
                    setAttributeValueAsReal(element, "data", entry.second.toReal());
                else if (entry.second.isString())
                    setAttributeValueAsString(element, "data", entry.second.toString());
                else if (entry.second.isVector3())
                    setAttributeValueAsVector3(element, "data", entry.second.toVector3());
            }
        }
        return element;
    }

    tinyxml2::XMLElement* XmlPropertyWriter::processPropertyRecord(
        tinyxml2::XMLElement* parent, const char* const name, const PropertyRecord& set)
    {
        PropertyMap map = set.toPropertyMap();

        return processPropertyMap(parent, name, map);
    }

    tinyxml2::XMLElement* XmlPropertyWriter::processPropertyArray(
        tinyxml2::XMLElement* parent, const char* const name, const PropertyArray& vector)
    {
        tinyxml2::XMLElement* element = appendChildElement(parent, "property");
        if (name[0] != '\0')
            setAttribute(element, "name", name);
        setAttribute(element, "type", "ARRAY");

        PropertyArray::const_iterator iter;
        for (iter = vector.begin(); iter != vector.end(); iter++)
        {
            processProperty(element, PropertyEntry(Ogre::String(), *iter));
        }
        return element;
    }

    tinyxml2::XMLElement* XmlPropertyWriter::processPropertyMap(
        tinyxml2::XMLElement* parent, const char* const name, const PropertyMap& map)
    {
        tinyxml2::XMLElement* element = appendChildElement(parent, "property");
        if (name[0] != '\0')
            setAttribute(element, "name", name);
        setAttribute(element, "type", "MAP");

        PropertyMap::const_iterator iter;
        for (iter = map.begin(); iter != map.end(); iter++)
        {
            processProperty(element, PropertyEntry(iter->first.c_str(), iter->second));
        }
        return element;
    }

    void XmlPropertyWriter::writeEachPropertyToElem(tinyxml2::XMLElement* parent, const rl::PropertyMap& map)
    {
        PropertyMap::const_iterator it_properties;
        for (it_properties = map.begin(); it_properties != map.end(); it_properties++)
        {
            this->processProperty(parent, PropertyEntry(it_properties->first.c_str(), it_properties->second));
        }
    }
}
