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
        DOMNamedNodeMap* attrMap = static_cast<DOMElement*>(godefsXml->item(idx))->getAttributes();
        for (XMLSize_t attrIdx = 0; attrIdx < attrMap->getLength(); attrIdx++)
        {
            DOMAttr* curAttr = static_cast<DOMAttr*>(attrMap->item(attrIdx));
            Ogre::String propKey = XmlHelper::transcodeToStdString(curAttr->getName());
            Property propVal = getProperty(propKey, XmlHelper::transcodeToString(curAttr->getValue()));
            ps->setProperty(
                propKey,
                propVal);
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

Property XmlPropertyReader::getProperty(const Ogre::String& key, const CeGuiString& value)
{
    if (key.substr(0, 4) == "str_")
    {
        return Property(value);
    }
    else if (key.substr(0, 4) == "int_")
    {
        const int intVal = CEGUI::PropertyHelper::stringToInt(value);
        return Property(intVal);
    }
    else if (key.substr(0, 5) == "real_")
    {
        const int realVal = CEGUI::PropertyHelper::stringToFloat(value);
        return Property(realVal);
    }


    return Property(value);
}

}
