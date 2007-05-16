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
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "XmlHelper.h"
#include "OgreXercesInput.h"

#include "Properties.h"
#include "PropertyReader.h"

#include <OgreResourceManager.h>
#include <CEGUIPropertyHelper.h>


using namespace XERCES_CPP_NAMESPACE;
using namespace Ogre;

namespace rl {

XmlPropertyReader::XmlPropertyReader()
{
}

void XmlPropertyReader::parseGameObjectFile(Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
{
    XMLPlatformUtils::Initialize();

	XmlHelper::initializeTranscoder();
	XercesDOMParser* parser = new XercesDOMParser();

    parser->setValidationScheme(XercesDOMParser::Val_Auto);    // optional.
    parser->setDoNamespaces(true);    // optional
	
    OgreInputSource source(stream);
    parser->parse(source);

    XERCES_CPP_NAMESPACE::DOMDocument* doc = parser->getDocument();

    DOMNodeList* godefsXml = doc->getDocumentElement()->getElementsByTagName(AutoXMLCh("gameobjectclass").data());
    for (unsigned int idx = 0; idx < godefsXml->getLength(); idx++)
    {
		PropertySet* ps = new PropertySet();
		DOMElement* curNode = static_cast<DOMElement*>(godefsXml->item(idx));
		
		const DOMNamedNodeMap* godefAttrs = curNode->getAttributes();
		for (XMLSize_t attrIdx = 0; attrIdx < godefAttrs->getLength(); attrIdx++)
		{
			PropertyEntry entry = processProperty(static_cast<DOMAttr*>(godefAttrs->item(attrIdx)));
            if (entry.first != "")
            {
                ps->setProperty(entry.first, entry.second);
            }
		}

        DOMNodeList* godefChildren = curNode->getChildNodes();
        for (XMLSize_t childIdx = 0; childIdx < godefChildren->getLength(); childIdx++)
        {
            DOMNode* curChild = godefChildren->item(childIdx);
            if (curChild->getNodeType() == DOMNode::ELEMENT_NODE)
            {
                PropertyEntry entry = processProperty(static_cast<DOMElement*>(curChild));
                if (entry.first != "")
                {
                    ps->setProperty(entry.first, entry.second);
                }
            }
        }
        mPropertySets.push_back(ps);
    }
	
	doc->release();

	XMLPlatformUtils::Terminate();
}

std::vector<PropertySet*> XmlPropertyReader::getPropertySets()
{
    return mPropertySets;
}

PropertyEntry XmlPropertyReader::processProperty(XERCES_CPP_NAMESPACE::DOMAttr* domAttr) const
{
	return std::make_pair(
		XmlHelper::transcodeToStdString(domAttr->getName()), 
		Property(XmlHelper::transcodeToString(domAttr->getValue())));
}

PropertyEntry XmlPropertyReader::processProperty(XERCES_CPP_NAMESPACE::DOMElement* domElem) const
{
    if (!XmlHelper::hasAttribute(domElem, "type"))
    {
        return std::make_pair("", Property(0));
    }

    Ogre::String key = "";
	if (XmlHelper::hasAttribute(domElem, "name"))
	{
		key = XmlHelper::getAttributeValueAsStdString(domElem, "name");
	}
	Ogre::String type = XmlHelper::getAttributeValueAsStdString(domElem, "type");
    CeGuiString value = "";
    if (XmlHelper::hasAttribute(domElem, "data"))
    {
        value = XmlHelper::getAttributeValueAsString(domElem, "data");
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
        if (comma1 != CeGuiString::npos 
            && comma2 != CeGuiString::npos 
            && comma3 != CeGuiString::npos)
        {
            quat.w = CEGUI::PropertyHelper::stringToFloat(value.substr(0, comma1));
            quat.x = CEGUI::PropertyHelper::stringToFloat(value.substr(comma1 + 1, comma2 - comma1 - 1));
            quat.y = CEGUI::PropertyHelper::stringToFloat(value.substr(comma2 + 1, comma3 - comma2 - 1));
            quat.z = CEGUI::PropertyHelper::stringToFloat(value.substr(comma3 + 1));
        }
        else if (comma1 != CeGuiString::npos 
            && comma2 != CeGuiString::npos 
            && comma3 == CeGuiString::npos)
        {
            Quaternion rotX, rotY, rotZ;

            rotX.FromAngleAxis(
			    Ogre::Degree(CEGUI::PropertyHelper::stringToFloat(value.substr(0, comma1))), 
			    Ogre::Vector3::UNIT_X);
            rotY.FromAngleAxis(
			    Ogre::Degree(CEGUI::PropertyHelper::stringToFloat(value.substr(comma1 + 1, comma2 - comma1 - 1))), 
			    Ogre::Vector3::UNIT_Y);
            rotZ.FromAngleAxis(
			    Ogre::Degree(CEGUI::PropertyHelper::stringToFloat(value.substr(comma2 + 1))), 
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
		for (DOMNode* curChild  = domElem->getFirstChild(); curChild != NULL;
			curChild = curChild->getNextSibling())
		{
			if (curChild->getNodeType() == DOMNode::ELEMENT_NODE)
			{
				PropertyEntry entry = processProperty(static_cast<DOMElement*>(curChild));
				vecVal.push_back(entry.second);
			}
		}
		prop = Property(vecVal);
	}
	else if (type == "INTPAIR")
    {
        CeGuiString::size_type comma1 = value.find(",");

		std::pair<int,int> intpairVal = std::make_pair(0, 0);
        if (comma1 != CeGuiString::npos)
        {
			intpairVal = std::make_pair(
				CEGUI::PropertyHelper::stringToInt(value.substr(0, comma1)),
				CEGUI::PropertyHelper::stringToInt(value.substr(comma1 + 1)));
        }
        prop = Property(intpairVal);
    }
	else if (type == "MAP")
	{
		PropertyMap mapVal;
		for (DOMNode* curChild  = domElem->getFirstChild(); curChild != NULL;
			curChild = curChild->getNextSibling())
		{
			if (curChild->getNodeType() == DOMNode::ELEMENT_NODE)
			{
				CeGuiString key = XmlHelper::getAttributeValueAsString(curChild, "name");
				PropertyEntry entry = processProperty(static_cast<DOMElement*>(curChild));
				mapVal[key] = entry.second;
			}
		}
		prop = Property(mapVal);
	}

    return std::make_pair(key, prop);
}

}
