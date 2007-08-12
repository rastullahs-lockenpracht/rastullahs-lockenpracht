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

CeGuiString XmlHelper::getValueAsString(DOMElement* element)
{
    RlAssert(element != NULL, "XmlHelper::getValueAsString: Element must not be NULL");
	return transcodeToString( element->getFirstChild()->getNodeValue() );
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

int XmlHelper::getAttributeValueAsInteger(DOMElement* element,const char* const name)
{
    RlAssert(element != NULL, "XmlHelper::getAttributeValueAsInteger: Element must not be NULL");
	XMLCh* attrName = XMLString::transcode(name);
	const XMLCh* attribute = element->getAttribute(attrName);
	int rVal = XMLString::parseInt(attribute);
	XMLString::release(&attrName);
	return rVal;
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

std::string XmlHelper::getAttributeValueAsStdString(DOMElement* element, const char* const name)
{
    RlAssert(element != NULL, "XmlHelper::getAttributeValueAsStdString: Element must not be NULL");
	XMLCh* attrName = XMLString::transcode(name);
	std::string rVal(transcodeToStdString(element->getAttribute(attrName)));
	XMLString::release(&attrName);
	return rVal;
}

bool XmlHelper::getAttributeValueAsBool(DOMElement* element,const char* const name)
{
    RlAssert(element != NULL, "XmlHelper::getAttributeValueAsBool: Element must not be NULL");
	if( XMLString::compareIString(getAttributeValueAsString(element, name).c_str(),"true") == 0  )
		return true;
	else
		return false;
}

int XmlHelper::getValueAsInteger(DOMElement* element)
{
    RlAssert(element != NULL, "XmlHelper::getValueAsInteger: Element must not be NULL");
	return XMLString::parseInt(element->getFirstChild()->getNodeValue());
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
