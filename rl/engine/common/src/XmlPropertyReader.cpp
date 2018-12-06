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

#include "Properties.h"

#include "XmlProcessor.h"
#include "XmlPropertyReader.h"

#include <CEGUIPropertyHelper.h>

using namespace Ogre;

namespace rl
{

    XmlPropertyReader::XmlPropertyReader()
    {
    }

    XmlPropertyReader::~XmlPropertyReader()
    {
    }

    void XmlPropertyReader::parseGameObjectFile(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
    {
        TiXmlDocument* doc = loadDocument(stream);
        doc->Accept(this);
    }

    bool XmlPropertyReader::VisitEnter(const TiXmlElement& element, const TiXmlAttribute* firstAttribute)
    {
        if (element.ValueTStr() == "gameobjectclass")
        {
            processGameObjectClassNode(element);
            return false;
        }
        return true;
    }

    void XmlPropertyReader::processGameObjectClassNode(const TiXmlElement& element)
    {
        PropertyRecordPtr ps(new PropertyRecord());

        for (const TiXmlAttribute* curAttr = element.FirstAttribute(); curAttr; curAttr = curAttr->Next())
        {
            PropertyEntry entry = processProperty(curAttr);
            if (entry.first != "")
            {
                ps->setProperty(entry.first, entry.second);
            }
        }

        for (const TiXmlNode* curChild = element.FirstChild(); curChild; curChild = curChild->NextSibling())
        {
            if (curChild->Type() == TiXmlNode::ELEMENT)
            {
                PropertyEntry entry = processProperty(curChild->ToElement());
                if (entry.first != "")
                {
                    ps->setProperty(entry.first, entry.second);
                }
            }
        }
        mPropertyRecords.push_back(ps);
    }

    PropertyRecordVector XmlPropertyReader::getPropertyRecords()
    {
        return mPropertyRecords;
    }

    PropertyEntry XmlPropertyReader::processProperty(const TiXmlAttribute* attribute) const
    {
        return std::make_pair(attribute->Name(), Property(CeGuiString(attribute->Value())));
    }

    PropertyEntry XmlPropertyReader::processProperty(const TiXmlElement* domElem) const
    {
        if (!hasAttribute(domElem, "type"))
        {
            return std::make_pair("", Property(0));
        }

        Ogre::String key = "";
        if (hasAttribute(domElem, "name"))
        {
            key = getAttributeValueAsStdString(domElem, "name");
        }
        Ogre::String type = getAttributeValueAsStdString(domElem, "type");
        CeGuiString value = "";
        if (hasAttribute(domElem, "data"))
        {
            value = getAttributeValueAsString(domElem, "data");
        }

        Property prop;
        if (type == "STRING")
        {
            prop = Property(value);
        }
        else if (type == "INT")
        {
            const int intVal = CEGUI::PropertyHelper::stringToInt(value);
            prop = Property(intVal);
        }
        else if (type == "REAL" || type == "FLOAT")
        {
            const Ogre::Real realVal = CEGUI::PropertyHelper::stringToFloat(value);
            prop = Property(realVal);
        }
        else if (type == "VECTOR3")
        {
            CeGuiString::size_type comma1 = value.find(",");
            CeGuiString::size_type comma2 = value.find(",", comma1 + 1);

            Ogre::Vector3 vec(Ogre::Vector3::ZERO);
            if (comma1 != CeGuiString::npos && comma2 != CeGuiString::npos)
            {
                vec.x = CEGUI::PropertyHelper::stringToFloat(value.substr(0, comma1));
                vec.y = CEGUI::PropertyHelper::stringToFloat(value.substr(comma1 + 1, comma2 - comma1 - 1));
                vec.z = CEGUI::PropertyHelper::stringToFloat(value.substr(comma2 + 1));
            }
            prop = Property(vec);
        }
        else if (type == "QUATERNION")
        {
            CeGuiString::size_type comma1 = value.find(",");
            CeGuiString::size_type comma2 = value.find(",", comma1 + 1);
            CeGuiString::size_type comma3 = value.find(",", comma2 + 1);

            Ogre::Quaternion quat(Ogre::Quaternion::IDENTITY);
            if (comma1 != CeGuiString::npos && comma2 != CeGuiString::npos && comma3 != CeGuiString::npos)
            {
                quat.w = CEGUI::PropertyHelper::stringToFloat(value.substr(0, comma1));
                quat.x = CEGUI::PropertyHelper::stringToFloat(value.substr(comma1 + 1, comma2 - comma1 - 1));
                quat.y = CEGUI::PropertyHelper::stringToFloat(value.substr(comma2 + 1, comma3 - comma2 - 1));
                quat.z = CEGUI::PropertyHelper::stringToFloat(value.substr(comma3 + 1));
            }
            else if (comma1 != CeGuiString::npos && comma2 != CeGuiString::npos && comma3 == CeGuiString::npos)
            {
                Quaternion rotX, rotY, rotZ;

                rotX.FromAngleAxis(
                    Ogre::Degree(CEGUI::PropertyHelper::stringToFloat(value.substr(0, comma1))), Ogre::Vector3::UNIT_X);
                rotY.FromAngleAxis(
                    Ogre::Degree(CEGUI::PropertyHelper::stringToFloat(value.substr(comma1 + 1, comma2 - comma1 - 1))),
                    Ogre::Vector3::UNIT_Y);
                rotZ.FromAngleAxis(Ogre::Degree(CEGUI::PropertyHelper::stringToFloat(value.substr(comma2 + 1))),
                    Ogre::Vector3::UNIT_Z);

                quat = rotX * rotY * rotZ;
            }
            prop = Property(quat);
        }
        else if (type == "BOOL")
        {
            const bool boolVal = CEGUI::PropertyHelper::stringToBool(value);
            prop = Property(boolVal);
        }
        else if (type == "ARRAY")
        {
            std::vector<Property> vecVal;
            for (const TiXmlNode* curChild = domElem->FirstChild(); curChild; curChild = curChild->NextSibling())
            {
                if (curChild->Type() == TiXmlNode::ELEMENT)
                {
                    PropertyEntry entry = processProperty(curChild->ToElement());
                    vecVal.push_back(entry.second);
                }
            }
            prop = Property(vecVal);
        }
        else if (type == "INTPAIR")
        {
            CeGuiString::size_type comma1 = value.find(",");

            std::pair<int, int> intpairVal = std::make_pair(0, 0);
            if (comma1 != CeGuiString::npos)
            {
                intpairVal = std::make_pair(CEGUI::PropertyHelper::stringToInt(value.substr(0, comma1)),
                    CEGUI::PropertyHelper::stringToInt(value.substr(comma1 + 1)));
            }
            prop = Property(intpairVal);
        }
        else if (type == "INTTRIPLE")
        {
            CeGuiString::size_type comma1 = value.find(",");
            CeGuiString::size_type comma2 = value.find(",", comma1 + 1);

            Tripel<int> intTripel(0, 0, 0);
            if (comma1 != CeGuiString::npos && comma2 != CeGuiString::npos)
            {
                intTripel.first = CEGUI::PropertyHelper::stringToFloat(value.substr(0, comma1));
                intTripel.second = CEGUI::PropertyHelper::stringToFloat(value.substr(comma1 + 1, comma2 - comma1 - 1));
                intTripel.third = CEGUI::PropertyHelper::stringToFloat(value.substr(comma2 + 1));
            }
            prop = Property(intTripel);
        }
        else if (type == "MAP")
        {
            PropertyMap mapVal;
            for (const TiXmlNode* curChild = domElem->FirstChild(); curChild; curChild = curChild->NextSibling())
            {
                if (curChild->Type() == TiXmlNode::ELEMENT)
                {
                    const TiXmlElement* curElem = curChild->ToElement();
                    CeGuiString key = getAttributeValueAsString(curElem, "name");
                    PropertyEntry entry = processProperty(curElem);
                    mapVal[key] = entry.second;
                }
            }
            prop = Property(mapVal);
        }

        return std::make_pair(key, prop);
    }

    PropertyRecordPtr XmlPropertyReader::getPropertiesAsRecord(const TiXmlElement* parent)
    {
        PropertyRecordPtr ps(new PropertyRecord());

        for (const TiXmlNode* curChild = parent->FirstChild(); curChild; curChild = curChild->NextSibling())
        {
            if (curChild->Type() == TiXmlNode::ELEMENT)
            {
                PropertyEntry entry = processProperty(curChild->ToElement());
                if (entry.first != "")
                {
                    ps->setProperty(entry.first, entry.second);
                }
            }
        }
        return ps;
    }
}
