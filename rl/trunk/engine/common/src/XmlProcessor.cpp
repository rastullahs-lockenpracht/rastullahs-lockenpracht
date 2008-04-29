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

#include <CEGUIPropertyHelper.h>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/SAXParseException.hpp>

#include "XmlProcessor.h"

#include "Exception.h"
#include "Logger.h"
#include "OgreXercesInput.h"
#include "XmlResource.h"
#include "XmlResourceManager.h"

using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;
using CEGUI::utf8;

namespace rl 
{

XERCES_CPP_NAMESPACE::XMLTranscoder* XmlProcessor::sTranscoder = NULL;
XERCES_CPP_NAMESPACE::XMLTransService::Codes XmlProcessor::sFailCode = XERCES_CPP_NAMESPACE::XMLTransService::Ok;

DOMElement* XmlProcessor::appendChildElement(DOMDocument* doc, DOMElement* parent, const char* const name) const
{
    RlAssert(parent != NULL, "XmlProcessor::appendChildElement: parent must not be NULL" );

    DOMElement* child = doc->createElement(XMLString::transcode(name));
    parent->appendChild(child);
    return child;
}

DOMElement* XmlProcessor::getChildNamed(DOMElement* parent, const char* const name) const
{
    RlAssert(parent != NULL, "XmlProcessor::getChildNamed: parent must not be NULL" );

	AutoXMLCh nameXml = name;

	DOMElement* rval = 0;
	
	for (DOMNode* cur = parent->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
	{
		if (cur->getNodeType() == DOMNode::ELEMENT_NODE &&
			XMLString::compareString(cur->getNodeName(), nameXml.data()) == 0)
		{
			rval = static_cast<DOMElement*>(cur);
			break;
		}
	}

	return rval;
}

void XmlProcessor::setValueAsString(DOMDocument* doc, DOMElement *element, const CeGuiString &value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsString: Element must not be NULL");
    DOMText* text = doc->createTextNode(XMLString::transcode(value.c_str()));
    element->appendChild(text);
}

CeGuiString XmlProcessor::getValueAsString(DOMElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsString: Element must not be NULL");
	return transcodeToString( element->getFirstChild()->getNodeValue() );
}

std::string XmlProcessor::getValueAsStdString(DOMElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsStdString: Element must not be NULL");
	return transcodeToStdString( element->getFirstChild()->getNodeValue() );
}

void XmlProcessor::setValueAsUtf8(DOMDocument* doc, DOMElement* element, utf8* value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsUtf8: Element must not be NULL");
    CeGuiString temp(value);
    DOMText* text = doc->createTextNode(XMLString::transcode(temp.c_str()));
    element->appendChild(text);
}

utf8* XmlProcessor::getValueAsUtf8(DOMElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsUtf8: Element must not be NULL");
	return XmlProcessor::transcodeToUtf8(element->getFirstChild()->getNodeValue());
}

bool XmlProcessor::hasAttribute(DOMElement* element,const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::hasAttribute: Element must not be NULL");
    AutoXMLCh attrName(name);
    bool rVal = element->hasAttribute(attrName.data());
    return rVal;
}

bool XmlProcessor::hasNodeName(DOMNode* node, const char* const name) const
{
    RlAssert(node != NULL, "XmlProcessor::hasNodeName: node must not be NULL");
    XMLCh* nodeName = XMLString::transcode(name);
    int ival = XMLString::compareString(node->getNodeName(), nodeName);
    bool rVal = (0 == ival);
    XMLString::release(&nodeName);
    return rVal;
}

void XmlProcessor::setAttribute(DOMElement* element, const char* const name, const char* const value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttribute: Element must not be NULL");
    AutoXMLCh attrName(name);
    element->setAttribute(attrName.data(), XMLString::transcode(value));
}

void XmlProcessor::setAttributeValueAsInteger(DOMElement *element, const char *const name, int value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsInteger: Element must not be NULL");
	AutoXMLCh attrName(name);
    CeGuiString temp = Ogre::StringConverter::toString(value);
    element->setAttribute(attrName.data(), XMLString::transcode(temp.c_str()));
}

int XmlProcessor::getAttributeValueAsInteger(DOMElement* element,const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsInteger: Element must not be NULL");
	AutoXMLCh attrName(name);
    return XMLString::parseInt(element->getAttribute(attrName.data()));
}

void XmlProcessor::setAttributeValueAsInt64(DOMElement *element, const char *const name, RL_LONGLONG value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsInt64: Element must not be NULL");
	AutoXMLCh attrName(name);
    
    bool negative = false;
    if (value < 0)
    {
        value *= -1;
        negative = true;
    }

    CeGuiString temp("");
    RL_LONGLONG divider = 1000000000;
    while (value > divider)
    {
        RL_LONGLONG remainder = value % divider;
        temp = Ogre::StringConverter::toString(static_cast<int>(remainder), 8, '0') + temp;
        value /= divider;
    }

    if (negative)
    {
        temp = "-" + temp;
    }
    
    element->setAttribute(attrName.data(), XMLString::transcode(temp.c_str()));
}

RL_LONGLONG XmlProcessor::getAttributeValueAsInt64(DOMElement* element,const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsInteger: Element must not be NULL");
	AutoXMLCh attrName(name);    
	const XMLCh* attribute = element->getAttribute(attrName.data());
    Ogre::String value = transcodeToStdString(attribute);

    RL_LONGLONG sign = 1;
    if (value[0] == '-')
    {
        sign = -1;
        value = value.substr(1);
    }

    RL_LONGLONG rVal = 0;
    while (value.length() > 8)
    {
        RL_LONGLONG curPart = Ogre::StringConverter::parseInt(value.substr(0, 8));
        for (size_t i = 0; i < value.length() - 8; ++i)
        {
            curPart *= 10;
        }
        rVal += curPart;
        value = value.substr(8);
    }

    rVal += CEGUI::PropertyHelper::stringToInt(value);
	return rVal;
}

void XmlProcessor::setAttributeValueAsIntegerPair(DOMElement* element, const char* const name, IntPair value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsIntegerPair: Element must not be NULL");
	AutoXMLCh attrName(name);
    CeGuiString temp = Ogre::StringConverter::toString(value.first) + "," + Ogre::StringConverter::toString(value.second);
    element->setAttribute(attrName.data(), XMLString::transcode(temp.c_str()));
}

IntPair XmlProcessor::getAttributeValueAsIntegerPair(DOMElement* element, const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsIntegerPair: Element must not be NULL");
	AutoXMLCh attrName(name);
	const XMLCh* attribute = element->getAttribute(attrName.data());
	
    CeGuiString value = transcodeToString(attribute);
    CeGuiString::size_type comma1 = value.find(",");

	std::pair<int,int> intPairVal = std::make_pair(0, 0);
    if (comma1 != CeGuiString::npos)
    {
		intPairVal = std::make_pair(
			CEGUI::PropertyHelper::stringToInt(value.substr(0, comma1)),
			CEGUI::PropertyHelper::stringToInt(value.substr(comma1 + 1)));
    }

    return intPairVal;
}

void XmlProcessor::setAttributeValueAsIntegerTriple(DOMElement *element, const char *const name, Tripel<int> value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsIntegerTriple: Element must not be NULL");
	AutoXMLCh attrName(name);
    CeGuiString temp = Ogre::StringConverter::toString(value.first) + "," + Ogre::StringConverter::toString(value.second) + "," + Ogre::StringConverter::toString(value.third);
    element->setAttribute(attrName.data(), XMLString::transcode(temp.c_str()));
}

Tripel<int> XmlProcessor::getAttributeValueAsIntegerTriple(DOMElement* element, const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsIntegerTriple: Element must not be NULL");
	AutoXMLCh attrName(name);
	const XMLCh* attribute = element->getAttribute(attrName.data());

    CeGuiString value = transcodeToString(attribute);

    CeGuiString::size_type comma1 = value.find(",");
    CeGuiString::size_type comma2 = value.find(",", comma1 + 1);

    Tripel<int> intTripel(0,0,0);
    if (comma1 != CeGuiString::npos && comma2 != CeGuiString::npos)
    {
        intTripel.first = CEGUI::PropertyHelper::stringToFloat(value.substr(0, comma1));
        intTripel.second = CEGUI::PropertyHelper::stringToFloat(value.substr(comma1 + 1, comma2 - comma1 - 1));
        intTripel.third = CEGUI::PropertyHelper::stringToFloat(value.substr(comma2 + 1));
    }

    return intTripel;
}

void XmlProcessor::setAttributeValueAsReal(DOMElement *element, const char *const name, Ogre::Real value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsReal: Element must not be NULL");
	AutoXMLCh attrName(name);
    CeGuiString temp = Ogre::StringConverter::toString(value);
    element->setAttribute(attrName.data(), XMLString::transcode(temp.c_str()));
}

Ogre::Real XmlProcessor::getAttributeValueAsReal(DOMElement* element,const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsReal: Element must not be NULL");
	AutoXMLCh attrName(name);
	Ogre::Real rVal = Ogre::StringConverter::parseReal(
		transcodeToString(element->getAttribute(attrName.data())).c_str() );
	return rVal;
}

void XmlProcessor::setAttributeValueAsString(DOMElement *element, const char *const name, const CeGuiString &value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsString: Element must not be NULL");
	AutoXMLCh attrName(name);
    element->setAttribute(attrName.data(), XMLString::transcode(value.c_str()));
}

CeGuiString XmlProcessor::getAttributeValueAsString(DOMElement* element, const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsString: Element must not be NULL");
	AutoXMLCh attrName(name);
	CeGuiString rVal(transcodeToString(element->getAttribute(attrName.data())));
	return rVal;
}

CeGuiString XmlProcessor::getAttributeValueAsString(const XERCES_CPP_NAMESPACE::Attributes& attributes, const char* const name) const
{
	AutoXMLCh attrName(name);
	const XMLCh* valStr = attributes.getValue(attrName.data());
	if (valStr != NULL)
	{
		return transcodeToString(valStr);
	}
	return CeGuiString();
}

void XmlProcessor::setAttributeValueAsStdString(DOMElement *element, const char *const name, const std::string &value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsStdString: Element must not be NULL");
	AutoXMLCh attrName(name);
    element->setAttribute(attrName.data(), XMLString::transcode(value.c_str()));
}

std::string XmlProcessor::getAttributeValueAsStdString(DOMElement* element, const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsStdString: Element must not be NULL");
	AutoXMLCh attrName(name);
	std::string rVal(transcodeToStdString(element->getAttribute(attrName.data())));
	return rVal;
}

void XmlProcessor::setAttributeValueAsBool(DOMElement *element, const char *const name, bool value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsBool: Element must not be NULL");
	AutoXMLCh attrName(name);
    CeGuiString temp = Ogre::StringConverter::toString(value);
    element->setAttribute(attrName.data(), XMLString::transcode(temp.c_str()));
}

bool XmlProcessor::getAttributeValueAsBool(DOMElement* element,const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsBool: Element must not be NULL");
	if ( XMLString::compareIString(getAttributeValueAsString(element, name).c_str(),"true") == 0  )
		return true;
	else
		return false;
}

void XmlProcessor::setValueAsBool(DOMDocument* doc, DOMElement *element, bool value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsBool: Element must not be NULL");
    CeGuiString temp = Ogre::StringConverter::toString(value);
    DOMText* text = doc->createTextNode(XMLString::transcode(temp.c_str()));
    element->appendChild(text);
}

bool XmlProcessor::getValueAsBool(DOMElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsBool: Element must not be NULL");
	
    if ( XMLString::compareIString(getValueAsString(element).c_str(),"true") == 0  )
		return true;
	else
		return false;
}

void XmlProcessor::setValueAsInteger(DOMDocument* doc, DOMElement *element, int value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsInteger: Element must not be NULL");
    CeGuiString temp = Ogre::StringConverter::toString(value);
    DOMText* text = doc->createTextNode(XMLString::transcode(temp.c_str()));
    element->appendChild(text);
}

int XmlProcessor::getValueAsInteger(DOMElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsInteger: Element must not be NULL");
	return XMLString::parseInt(element->getFirstChild()->getNodeValue());
}

Ogre::Real XmlProcessor::getValueAsReal(XERCES_CPP_NAMESPACE::DOMElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsInteger: Element must not be NULL");
    return Ogre::StringConverter::parseReal(
        transcodeToString(element->getFirstChild()->getNodeValue()).c_str());
}

void XmlProcessor::setValueAsIntegerPair(DOMDocument *doc, DOMElement *element, IntPair value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsIntegerPair: Element must not be NULL");
    CeGuiString temp = CEGUI::PropertyHelper::intToString(value.first) + ',' + CEGUI::PropertyHelper::intToString(value.second);
    DOMText* text = doc->createTextNode(XMLString::transcode(temp.c_str()));
    element->appendChild(text);
}

IntPair XmlProcessor::getValueAsIntegerPair(DOMElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsIntegerPair: Element must not be NULL");
    CeGuiString value = getValueAsString(element);
    CeGuiString::size_type comma1 = value.find(",");

	std::pair<int,int> intPairVal = std::make_pair(0, 0);
    if (comma1 != CeGuiString::npos)
    {
		intPairVal = std::make_pair(
			CEGUI::PropertyHelper::stringToInt(value.substr(0, comma1)),
			CEGUI::PropertyHelper::stringToInt(value.substr(comma1 + 1)));
    }
    return intPairVal;
}

void XmlProcessor::setValueAsIntegerTriple(DOMDocument *doc, DOMElement *element, Tripel<int> value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsIntegerTriple: Element must not be NULL");
    RlAssert(element != NULL, "XmlProcessor::setValueAsIntegerPair: Element must not be NULL");
    CeGuiString temp = CEGUI::PropertyHelper::intToString(value.first) + ',' + CEGUI::PropertyHelper::intToString(value.second) + ',' + CEGUI::PropertyHelper::intToString(value.third);
    DOMText* text = doc->createTextNode(XMLString::transcode(temp.c_str()));
    element->appendChild(text);
}

Tripel<int> XmlProcessor::getValueAsIntegerTriple(DOMElement *element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsIntegerTriple: Element must not be NULL");
    CeGuiString value = getValueAsString(element);

    CeGuiString::size_type comma1 = value.find(",");
    CeGuiString::size_type comma2 = value.find(",", comma1 + 1);

    Tripel<int> intTripel(0,0,0);
    if (comma1 != CeGuiString::npos && comma2 != CeGuiString::npos)
    {
        intTripel.first = CEGUI::PropertyHelper::stringToFloat(value.substr(0, comma1));
        intTripel.second = CEGUI::PropertyHelper::stringToFloat(value.substr(comma1 + 1, comma2 - comma1 - 1));
        intTripel.third = CEGUI::PropertyHelper::stringToFloat(value.substr(comma2 + 1));
    }
    return intTripel;
}

void XmlProcessor::setAttributeValueAsVector3( DOMElement *element, const char* const name, Ogre::Vector3 value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsVector3: Element must not be NULL");
	AutoXMLCh attrName(name);
    CeGuiString temp = Ogre::StringConverter::toString(value.x) + "," + Ogre::StringConverter::toString(value.y) + "," + Ogre::StringConverter::toString(value.z);
    element->setAttribute(attrName.data(), XMLString::transcode(temp.c_str()));
}

Ogre::Vector3 XmlProcessor::getAttributeValueAsVector3(DOMElement* element, const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsVector3: Element must not be NULL");
	AutoXMLCh attrName(name);
	const XMLCh* attribute = element->getAttribute(attrName.data());

    CeGuiString value = transcodeToString(attribute);

    CeGuiString::size_type comma1 = value.find(",");
    CeGuiString::size_type comma2 = value.find(",", comma1 + 1);

    Ogre::Vector3 vec(0,0,0);
    if (comma1 != CeGuiString::npos && comma2 != CeGuiString::npos)
    {
        vec.x = CEGUI::PropertyHelper::stringToFloat(value.substr(0, comma1));
        vec.y = CEGUI::PropertyHelper::stringToFloat(value.substr(comma1 + 1, comma2 - comma1 - 1));
        vec.z = CEGUI::PropertyHelper::stringToFloat(value.substr(comma2 + 1));
    }

    return vec;
}

void XmlProcessor::setValueAsVector3( DOMElement *element, Ogre::Vector3 value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsVector3: Element must not be NULL");
    setAttribute(element, "x", Ogre::StringConverter::toString(value.x).c_str());
    setAttribute(element, "y", Ogre::StringConverter::toString(value.y).c_str());
    setAttribute(element, "z", Ogre::StringConverter::toString(value.z).c_str());
}

Ogre::Vector3 XmlProcessor::getValueAsVector3(DOMElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsVector3: Element must not be NULL");
	RlAssert(
		hasAttribute(element, "x") 
		&& hasAttribute(element, "y") 
		&& hasAttribute(element, "z"),
		"Element must have attributes x, y and z");
	return Ogre::Vector3(
		getAttributeValueAsReal(element, "x"),
		getAttributeValueAsReal(element, "y"),
		getAttributeValueAsReal(element, "z"));
}

void XmlProcessor::setValueAsQuaternion(DOMElement *element, Ogre::Quaternion value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsQuaternion: Element must not be NULL");
    setAttribute(element, "x", Ogre::StringConverter::toString(value.x).c_str());
    setAttribute(element, "y", Ogre::StringConverter::toString(value.y).c_str());
    setAttribute(element, "z", Ogre::StringConverter::toString(value.z).c_str());
    setAttribute(element, "w", Ogre::StringConverter::toString(value.w).c_str());
}

Ogre::Quaternion XmlProcessor::getValueAsQuaternion(DOMElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsQuaternion: Element must not be NULL");
	RlAssert(
		hasAttribute(element, "x") 
		&& hasAttribute(element, "y") 
		&& hasAttribute(element, "z")
        && hasAttribute(element, "w"),
		"Element must have attributes x, y z and w");
    return Ogre::Quaternion(
		getAttributeValueAsReal(element, "x"),
		getAttributeValueAsReal(element, "y"),
		getAttributeValueAsReal(element, "z"),
        getAttributeValueAsReal(element, "w"));
}

void XmlProcessor::setAttributeValueAsQuaternion(DOMElement *element, const char* const name, Ogre::Quaternion value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsQuaternion: Element must not be NULL");
	AutoXMLCh attrName(name);
    CeGuiString temp = Ogre::StringConverter::toString(value.w) + "," + Ogre::StringConverter::toString(value.x) + "," + Ogre::StringConverter::toString(value.y) + "," + Ogre::StringConverter::toString(value.z);
    element->setAttribute(attrName.data(), XMLString::transcode(temp.c_str()));
}

Ogre::Quaternion XmlProcessor::getAttributeValueAsQuaternion(DOMElement* element, const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsQuaternion: Element must not be NULL");
	AutoXMLCh attrName(name);
	const XMLCh* attribute = element->getAttribute(attrName.data());

    CeGuiString value = transcodeToString(attribute);

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
    else 
    {
        LOG_ERROR(Logger::COMMON, "'" + value + "' is no valid quaternion. Format is 'w, x, y, z' or 'rotX, rotY, rotZ'.");
    }

    return quat;
}

utf8* XmlProcessor::transcodeToUtf8(const XMLCh* const string16) const
{
	unsigned int str16len = XMLString::stringLen(string16);

    /// Check if the XMLCh contains nothing but whitespaces. If so, remove them 
    /// @note this does also mean, that this method will never return something like " " 
    XMLCh* tmpVal = XMLString::replicate(string16);
    if(XMLChar1_0::isAllSpaces(tmpVal, str16len) && str16len != 1)
    {
        XMLString::removeWS(tmpVal);
        str16len = XMLString::stringLen(tmpVal);
    }

	utf8* rval;
	unsigned int eaten = 0;
	unsigned int size = str16len;

	do
	{
		rval = new utf8[size+1];
	
		sTranscoder->transcodeTo(tmpVal, str16len, rval, size, eaten, XMLTranscoder::UnRep_RepChar);
		rval[size] = 0;

		if (eaten < str16len)
		{
			size += str16len - eaten;
			delete[] rval;
		}
	}
	while (eaten < str16len);
    XMLString::release(&tmpVal);
	return rval;	
}

CeGuiString XmlProcessor::transcodeToString(const XMLCh* const string16) const
{
	unsigned int str16len = XMLString::stringLen(string16);
	if (str16len == 0)
	{
		return CeGuiString();
	}

	utf8* tmpVal = transcodeToUtf8(string16);
	CeGuiString rVal(tmpVal);
	delete[] tmpVal;
	return rVal;
}

std::string XmlProcessor::transcodeToStdString(const XMLCh* const string16) const
{
    if ( string16 == NULL )
        return "";

	char* tmpVal = XMLString::transcode(string16);
	std::string rVal(tmpVal);
	XMLString::release(&tmpVal);
	return rVal;
}

DOMDocument* XmlProcessor::loadDocument(
    const Ogre::String& resourceName, const Ogre::String& resourceGroup)
{
    if( mOpenParser != NULL )
    {
        LOG_WARNING(Logger::COMMON, 
                "XmlProcessor: there is already an opened DOMParser, but perhaps its still used, so we don't delete it");
    }
    mOpenParser = new XercesDOMParser();

    mOpenParser->setValidationScheme(XercesDOMParser::Val_Auto);    // optional.
    //mOpenParser->setIncludeIgnorableWhitespace(false); // optional, if you want to ignore whitespaces
    mOpenParser->setDoNamespaces(true);    // optional

    XmlPtr res = XmlResourceManager::getSingleton().getByName(resourceName);
    if (res.isNull())
    {
        Ogre::String group = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
        if (resourceGroup != "" 
            && ResourceGroupManager::getSingleton().resourceExists(
                    resourceGroup, resourceName))
        {
            group = resourceGroup;
        }
        res = XmlResourceManager::getSingleton().create(resourceName, group);
    }

    mOpenXmlFileName = resourceName;

    if (!res.isNull() && res->parseBy(mOpenParser, this))
    {
        mOpenXmlFileName = "";
        return mOpenParser->getDocument();
    }


    return NULL;
}

DOMDocument* XmlProcessor::loadDocument(const Ogre::DataStreamPtr& stream)
{
    if( mOpenParser != NULL )
    {
        LOG_WARNING(Logger::COMMON, 
                "XmlProcessor: there is already an opened DOMParser, but perhaps its still used, so we don't delete it");
    }
    mOpenParser = new XercesDOMParser();

    mOpenParser->setValidationScheme(XercesDOMParser::Val_Auto);    // optional.
    mOpenParser->setDoNamespaces(true);    // optional

    OgreInputSource source(stream);
    mOpenParser->setErrorHandler(this);

    mOpenXmlFileName = stream->getName();
    mOpenParser->parse(source);
    mOpenXmlFileName = "";

    if (mOpenParser->getErrorCount() == 0)
    {
        return mOpenParser->getDocument();
    }

    return NULL;
}

void XmlProcessor::initializeXml()
{
  	XMLPlatformUtils::Initialize();

	sTranscoder 
        = XMLPlatformUtils::fgTransService->makeNewTranscoderFor(
            XMLRecognizer::UTF_8, sFailCode, 16*1024);
}

void XmlProcessor::shutdownXml()
{
    if( mOpenParser )
    {
        delete mOpenParser;
        mOpenParser = NULL;
    }
    delete sTranscoder;
    sTranscoder = NULL;
    
    XMLPlatformUtils::Terminate();
}

std::string XmlProcessor::toString( const std::string& type,
        const XERCES_CPP_NAMESPACE::SAXParseException& exc ) const
{
    std::stringstream strs;
    strs << "A" << type << " occured while parsing " << mOpenXmlFileName
         << " at line " << exc.getLineNumber() << " column " <<  exc.getColumnNumber();

    if( exc.getSystemId() != NULL )
        strs << " with system " << transcodeToStdString( exc.getSystemId() );
    if( exc.getPublicId() != NULL )
        strs << " with public " << transcodeToStdString( exc.getPublicId() );

    std::string msg = transcodeToStdString(exc.getMessage());
    strs << ": " << msg;

    return strs.str();
}

void XmlProcessor::warning(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
{
    LOG_MESSAGE(Logger::CORE, toString( " warning ", exc ) );
}

void XmlProcessor::error(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
{
    LOG_ERROR(Logger::CORE, toString( "n error", exc ) );
}

void XmlProcessor::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
{
    LOG_CRITICAL(Logger::CORE, toString( " fatal error", exc ) );
}

void XmlProcessor::resetErrors()
{
}


} // end namespace rl
