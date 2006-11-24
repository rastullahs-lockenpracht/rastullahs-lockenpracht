/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
        DOMNodeList* godefChildren = static_cast<DOMElement*>(godefsXml->item(idx))->getChildNodes();
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

PropertyEntry XmlPropertyReader::processProperty(XERCES_CPP_NAMESPACE::DOMElement* domElem) const
{
    if (!XmlHelper::hasAttribute(domElem, "name")
        || !XmlHelper::hasAttribute(domElem, "type")
        || !XmlHelper::hasAttribute(domElem, "data"))
    {
        return std::make_pair("", Property(0));
    }

    Ogre::String key = XmlHelper::getAttributeValueAsStdString(domElem, "name");
    Ogre::String type = XmlHelper::getAttributeValueAsStdString(domElem, "type");
    CeGuiString value = XmlHelper::getAttributeValueAsString(domElem, "data");
    
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
    else if (type == "REAL")
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
        prop = Property(quat);
    }
    else if (type == "BOOL")
    {
        const bool boolVal = CEGUI::PropertyHelper::stringToBool(value);
        prop = Property(boolVal);
    }

    return std::make_pair(key, prop);
}

}
