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

#include <xercesc/dom/DOM.hpp>

#include "XmlHelper.h"
#include "XmlErrorHandler.h"
#include "Exception.h"
#include "Logger.h"

#include <CEGUIPropertyHelper.h>

using namespace XERCES_CPP_NAMESPACE;


namespace rl {

XMLTranscoder* XmlHelper::sTranscoder;
XMLTransService::Codes XmlHelper::sFailCode;
XmlErrorHandler XmlHelper::sErrorHandler;

void XmlHelper::initializeTranscoder()
{
	XmlHelper::sFailCode = XMLTransService::Ok;
	XmlHelper::sTranscoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor(XMLRecognizer::UTF_8, XmlHelper::sFailCode, 16*1024);
}

DOMElement* XmlHelper::appendChildElement(DOMDocument* doc, DOMElement* parent, const char* const name)
{
    RlAssert(parent != NULL, "XmlHelper::appendChildElement: parent must not be NULL" );

    DOMElement* child = doc->createElement(XMLString::transcode(name));
    parent->appendChild(child);
    return child;
}

DOMElement* XmlHelper::getChildNamed(DOMElement* parent, const char* const name)
{
    RlAssert(parent != NULL, "XmlHelper::getChildNamed: parent must not be NULL" );

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

DOMElement* XmlHelper::setValueAsString(DOMDocument* doc, DOMElement *element, const CeGuiString &value)
{
    RlAssert(element != NULL, "XmlHelper::setValueAsString: Element must not be NULL");
    DOMText* text = doc->createTextNode(XMLString::transcode(value.c_str()));
    element->appendChild(text);
    return element;
}

CeGuiString XmlHelper::getValueAsString(DOMElement* element)
{
    RlAssert(element != NULL, "XmlHelper::getValueAsString: Element must not be NULL");
	return transcodeToString( element->getFirstChild()->getNodeValue() );
}

std::string XmlHelper::getValueAsStdString(DOMElement* element)
{
    RlAssert(element != NULL, "XmlHelper::getValueAsStdString: Element must not be NULL");
	return transcodeToStdString( element->getFirstChild()->getNodeValue() );
}

DOMElement* XmlHelper::setValueAsUtf8(DOMDocument* doc, DOMElement* element, utf8* value)
{
    RlAssert(element != NULL, "XmlHelper::setValueAsUtf8: Element must not be NULL");
    CeGuiString temp(value);
    DOMText* text = doc->createTextNode(XMLString::transcode(temp.c_str()));
    element->appendChild(text);
    return element;
}

utf8* XmlHelper::getValueAsUtf8(DOMElement* element)
{
    RlAssert(element != NULL, "XmlHelper::getValueAsUtf8: Element must not be NULL");
	return XmlHelper::transcodeToUtf8(element->getFirstChild()->getNodeValue());
}

bool XmlHelper::hasAttribute(DOMElement* element,const char* const name)
{
    RlAssert(element != NULL, "XmlHelper::hasAttribute: Element must not be NULL");
    XMLCh* attrName = XMLString::transcode(name);
    bool rVal = element->hasAttribute(attrName);
    XMLString::release(&attrName);
    return rVal;
}

bool XmlHelper::hasNodeName(DOMNode* node, const char* const name)
{
    RlAssert(node != NULL, "XmlHelper::hasNodeName: node must not be NULL");
    XMLCh* nodeName = XMLString::transcode(name);
    int ival = XMLString::compareString(node->getNodeName(), nodeName);
    bool rVal = (0 == ival);
    XMLString::release(&nodeName);
    return rVal;
}

void XmlHelper::setAttribute(DOMElement* element, const char* const name, const char* const value)
{
    RlAssert(element != NULL, "XmlHelper::setAttribute: Element must not be NULL");
    XMLCh* attrName = XMLString::transcode(name);
    element->setAttribute(attrName,XMLString::transcode(value));
    XMLString::release(&attrName);
}

DOMElement* XmlHelper::setAttributeValueAsInteger(DOMElement *element, const char *const name, int value)
{
    RlAssert(element != NULL, "XmlHelper::setAttributeValueAsInteger: Element must not be NULL");
	XMLCh* attrName = XMLString::transcode(name);
    CeGuiString temp = Ogre::StringConverter::toString(value);
    element->setAttribute(attrName,XMLString::transcode(temp.c_str()));
    XMLString::release(&attrName);
    return element;
}

int XmlHelper::getAttributeValueAsInteger(DOMElement* element,const char* const name)
{
    RlAssert(element != NULL, "XmlHelper::getAttributeValueAsInteger: Element must not be NULL");
	XMLCh* attrName = XMLString::transcode(name);
	const XMLCh* attribute = element->getAttribute(attrName);
	int rVal = XMLString::parseInt(attribute);
	XMLString::release(&attrName);
	return rVal;
}

DOMElement* XmlHelper::setAttributeValueAsIntegerPair(DOMElement* element, const char* const name, IntPair value)
{
    RlAssert(element != NULL, "XmlHelper::setAttributeValueAsIntegerPair: Element must not be NULL");
	XMLCh* attrName = XMLString::transcode(name);
    CeGuiString temp = Ogre::StringConverter::toString(value.first) + "," + Ogre::StringConverter::toString(value.second);
    element->setAttribute(attrName,XMLString::transcode(temp.c_str()));
    XMLString::release(&attrName);
    return element;
}

IntPair XmlHelper::getAttributeValueAsIntegerPair(DOMElement* element, const char* const name)
{
    RlAssert(element != NULL, "XmlHelper::getAttributeValueAsIntegerPair: Element must not be NULL");
	XMLCh* attrName = XMLString::transcode(name);
	const XMLCh* attribute = element->getAttribute(attrName);
	
    CeGuiString value = transcodeToString(attribute);
    CeGuiString::size_type comma1 = value.find(",");

	std::pair<int,int> intPairVal = std::make_pair(0, 0);
    if (comma1 != CeGuiString::npos)
    {
		intPairVal = std::make_pair(
			CEGUI::PropertyHelper::stringToInt(value.substr(0, comma1)),
			CEGUI::PropertyHelper::stringToInt(value.substr(comma1 + 1)));
    }
    XMLString::release(&attrName);

    return intPairVal;
}

DOMElement* XmlHelper::setAttributeValueAsIntegerTriple(DOMElement *element, const char *const name, Tripel<int> value)
{
    RlAssert(element != NULL, "XmlHelper::setAttributeValueAsIntegerTriple: Element must not be NULL");
	XMLCh* attrName = XMLString::transcode(name);
    CeGuiString temp = Ogre::StringConverter::toString(value.first) + "," + Ogre::StringConverter::toString(value.second) + "" + Ogre::StringConverter::toString(value.third);
    element->setAttribute(attrName,XMLString::transcode(temp.c_str()));
    XMLString::release(&attrName);
    return element;
}

Tripel<int> XmlHelper::getAttributeValueAsIntegerTriple(DOMElement* element, const char* const name)
{
    RlAssert(element != NULL, "XmlHelper::getAttributeValueAsIntegerTriple: Element must not be NULL");
	XMLCh* attrName = XMLString::transcode(name);
	const XMLCh* attribute = element->getAttribute(attrName);

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
    XMLString::release(&attrName);

    return intTripel;
}

DOMElement* XmlHelper::setAttributeValueAsReal(DOMElement *element, const char *const name, Ogre::Real value)
{
    RlAssert(element != NULL, "XmlHelper::setAttributeValueAsReal: Element must not be NULL");
	XMLCh* attrName = XMLString::transcode(name);
    CeGuiString temp = Ogre::StringConverter::toString(value);
    element->setAttribute(attrName,XMLString::transcode(temp.c_str()));
    XMLString::release(&attrName);
    return element;
}

Ogre::Real XmlHelper::getAttributeValueAsReal(DOMElement* element,const char* const name)
{
    RlAssert(element != NULL, "XmlHelper::getAttributeValueAsReal: Element must not be NULL");
	XMLCh* attrName = XMLString::transcode(name);
	Ogre::Real rVal = Ogre::StringConverter::parseReal(
		transcodeToString(element->getAttribute(attrName)).c_str() );
	XMLString::release(&attrName);
	return rVal;
}

DOMElement* XmlHelper::setAttributeValueAsString(DOMElement *element, const char *const name, const CeGuiString &value)
{
    RlAssert(element != NULL, "XmlHelper::setAttributeValueAsString: Element must not be NULL");
	XMLCh* attrName = XMLString::transcode(name);
    element->setAttribute(attrName,XMLString::transcode(value.c_str()));
    XMLString::release(&attrName);
    return element;
}

CeGuiString XmlHelper::getAttributeValueAsString(DOMElement* element, const char* const name)
{
    RlAssert(element != NULL, "XmlHelper::getAttributeValueAsString: Element must not be NULL");
	XMLCh* attrName = XMLString::transcode(name);
	CeGuiString rVal(transcodeToString(element->getAttribute(attrName)));
	XMLString::release(&attrName);
	return rVal;
}

CeGuiString XmlHelper::getAttributeValueAsString(const XERCES_CPP_NAMESPACE::Attributes& attributes, const char* const name)
{
	XMLCh* attrName = XMLString::transcode(name);
	const XMLCh* valStr = attributes.getValue(attrName);
	XMLString::release(&attrName);
	if(valStr != NULL)
	{
		return transcodeToString(valStr);
	}
	return CeGuiString();
}

DOMElement* XmlHelper::setAttributeValueAsStdString(DOMElement *element, const char *const name, const std::string &value)
{
    RlAssert(element != NULL, "XmlHelper::setAttributeValueAsStdString: Element must not be NULL");
	XMLCh* attrName = XMLString::transcode(name);
    element->setAttribute(attrName,XMLString::transcode(value.c_str()));
    XMLString::release(&attrName);
    return element;
}

std::string XmlHelper::getAttributeValueAsStdString(DOMElement* element, const char* const name)
{
    RlAssert(element != NULL, "XmlHelper::getAttributeValueAsStdString: Element must not be NULL");
	XMLCh* attrName = XMLString::transcode(name);
	std::string rVal(transcodeToStdString(element->getAttribute(attrName)));
	XMLString::release(&attrName);
	return rVal;
}

DOMElement* XmlHelper::setAttributeValueAsBool(DOMElement *element, const char *const name, bool value)
{
    RlAssert(element != NULL, "XmlHelper::setAttributeValueAsBool: Element must not be NULL");
	XMLCh* attrName = XMLString::transcode(name);
    CeGuiString temp = Ogre::StringConverter::toString(value);
    element->setAttribute(attrName,XMLString::transcode(temp.c_str()));
    XMLString::release(&attrName);
    return element;
}

bool XmlHelper::getAttributeValueAsBool(DOMElement* element,const char* const name)
{
    RlAssert(element != NULL, "XmlHelper::getAttributeValueAsBool: Element must not be NULL");
	if( XMLString::compareIString(getAttributeValueAsString(element, name).c_str(),"true") == 0  )
		return true;
	else
		return false;
}

DOMElement* XmlHelper::setValueAsInteger(DOMDocument* doc, DOMElement *element, int value)
{
    RlAssert(element != NULL, "XmlHelper::setValueAsInteger: Element must not be NULL");
    CeGuiString temp = Ogre::StringConverter::toString(value);
    DOMText* text = doc->createTextNode(XMLString::transcode(temp.c_str()));
    element->appendChild(text);
    return element;
}

int XmlHelper::getValueAsInteger(DOMElement* element)
{
    RlAssert(element != NULL, "XmlHelper::getValueAsInteger: Element must not be NULL");
	return XMLString::parseInt(element->getFirstChild()->getNodeValue());
}

Ogre::Real XmlHelper::getValueAsReal(XERCES_CPP_NAMESPACE::DOMElement* element)
{
    RlAssert(element != NULL, "XmlHelper::getValueAsInteger: Element must not be NULL");
    return Ogre::StringConverter::parseReal(
        transcodeToString(element->getFirstChild()->getNodeValue()).c_str());
}

DOMElement* XmlHelper::setValueAsIntegerPair(DOMDocument *doc, DOMElement *element, IntPair value)
{
    RlAssert(element != NULL, "XmlHelper::setValueAsIntegerPair: Element must not be NULL");
    String temp = CEGUI::PropertyHelper::intToString(value.first) + ',' + CEGUI::PropertyHelper::intToString(value.second);
    DOMText* text = doc->createTextNode(XMLString::transcode(temp.c_str()));
    element->appendChild(text);
    return element;
}

IntPair XmlHelper::getValueAsIntegerPair(DOMElement* element)
{
    RlAssert(element != NULL, "XmlHelper::getValueAsIntegerPair: Element must not be NULL");
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

DOMElement* XmlHelper::setValueAsIntegerTriple(DOMDocument *doc, DOMElement *element, Tripel<int> value)
{
    RlAssert(element != NULL, "XmlHelper::setValueAsIntegerTriple: Element must not be NULL");
    RlAssert(element != NULL, "XmlHelper::setValueAsIntegerPair: Element must not be NULL");
    String temp = CEGUI::PropertyHelper::intToString(value.first) + ',' + CEGUI::PropertyHelper::intToString(value.second) + ',' + CEGUI::PropertyHelper::intToString(value.third);
    DOMText* text = doc->createTextNode(XMLString::transcode(temp.c_str()));
    element->appendChild(text);
    return element;
}

Tripel<int> XmlHelper::getValueAsIntegerTriple(DOMElement *element)
{
    RlAssert(element != NULL, "XmlHelper::getValueAsIntegerTriple: Element must not be NULL");
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

DOMElement* XmlHelper::setValueAsVector3( DOMElement *element, Ogre::Vector3 value)
{
    RlAssert(element != NULL, "XmlHelper::setValueAsVector3: Element must not be NULL");
    setAttribute(element, "x", Ogre::StringConverter::toString(value.x).c_str());
    setAttribute(element, "y", Ogre::StringConverter::toString(value.y).c_str());
    setAttribute(element, "z", Ogre::StringConverter::toString(value.z).c_str());
    return element;
}

Ogre::Vector3 XmlHelper::getValueAsVector3(DOMElement* element)
{
    RlAssert(element != NULL, "XmlHelper::getValueAsVector3: Element must not be NULL");
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

DOMElement* XmlHelper::setValueAsQuaternion(DOMElement *element, Ogre::Quaternion value)
{
    RlAssert(element != NULL, "XmlHelper::setValueAsQuaternion: Element must not be NULL");
    setAttribute(element, "x", Ogre::StringConverter::toString(value.x).c_str());
    setAttribute(element, "y", Ogre::StringConverter::toString(value.y).c_str());
    setAttribute(element, "z", Ogre::StringConverter::toString(value.z).c_str());
    setAttribute(element, "w", Ogre::StringConverter::toString(value.w).c_str());
    return element;
}

Ogre::Quaternion XmlHelper::getValueAsQuaternion(DOMElement* element)
{
    RlAssert(element != NULL, "XmlHelper::getValueAsQuaternion: Element must not be NULL");
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

utf8* XmlHelper::transcodeToUtf8(const XMLCh* const string16)
{
	unsigned int str16len = XMLString::stringLen(string16);
	if (str16len == 0)
		return (utf8*)"";

	utf8* rval;
	unsigned int eaten = 0;
	unsigned int size = str16len;

	do
	{
		rval = new utf8[size+1];
	
		sTranscoder->transcodeTo(string16, str16len, rval, size, eaten, XMLTranscoder::UnRep_RepChar);
		rval[size] = 0;

		if (eaten < str16len)
		{
			size += str16len - eaten;
			delete[] rval;
		}
	}
	while (eaten < str16len);

	return rval;	
}

CeGuiString XmlHelper::transcodeToString(const XMLCh* const string16)
{
	unsigned int str16len = XMLString::stringLen(string16);
	if (str16len == 0)
	{
		return CeGuiString();
	}

	initializeTranscoder();

	utf8* tmpVal = transcodeToUtf8(string16);
	CeGuiString rVal(tmpVal);
	delete[] tmpVal;
	return rVal;
}

std::string XmlHelper::transcodeToStdString(const XMLCh* const string16)
{
    if( string16 == NULL )
        return "";

	char* tmpVal = XMLString::transcode(string16);
	std::string rVal(tmpVal);
	XMLString::release(&tmpVal);
	return rVal;
}

XmlErrorHandler* XmlHelper::getErrorHandler()
{
    return &XmlHelper::sErrorHandler;
}

} // end namespace rl
