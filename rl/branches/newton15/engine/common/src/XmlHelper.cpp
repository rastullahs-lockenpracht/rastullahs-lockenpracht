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

#include "XmlHelper.h"
#include "Exception.h"

using namespace XERCES_CPP_NAMESPACE;


namespace rl {

XMLTranscoder* XmlHelper::sTranscoder;
XMLTransService::Codes XmlHelper::sFailCode;


void XmlHelper::initializeTranscoder()
{
	XmlHelper::sFailCode = XMLTransService::Ok;
	XmlHelper::sTranscoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor(XMLRecognizer::UTF_8, XmlHelper::sFailCode, 16*1024);
}

DOMElement* XmlHelper::getChildNamed(DOMElement* parent, const char* const name)
{
	if( parent == NULL )
		Throw( NullPointerException, "parent darf nicht NULL sein" );

	DOMNodeList* nodes = parent->getChildNodes();
	XMLCh* nameXml = XMLString::transcode(name);

	DOMElement* rval = 0;
	
	for (unsigned int idx = 0; idx < nodes->getLength(); idx++)
	{
		DOMNode* item = nodes->item(idx);
		if (item->getNodeType() == DOMNode::ELEMENT_NODE &&
			XMLString::compareString(item->getNodeName(), nameXml)==0 )
		{
			rval = reinterpret_cast<DOMElement*>(item);
			break;
		}
	}

	XMLString::release(&nameXml);

	return rval;
}

CeGuiString XmlHelper::getValueAsString(DOMElement* element)
{
	return transcodeToString( element->getFirstChild()->getNodeValue() );
}

utf8* XmlHelper::getValueAsUtf8(DOMElement* element)
{
	return XmlHelper::transcodeToUtf8(element->getFirstChild()->getNodeValue());
}

int XmlHelper::getAttributeValueAsInteger(DOMElement* element,const char* const name)
{
	XMLCh* attrName = XMLString::transcode(name);
	int rVal = XMLString::parseInt(element->getAttribute(attrName));
	XMLString::release(&attrName);
	return rVal;
}

Ogre::Real XmlHelper::getAttributeValueAsReal(DOMElement* element,const char* const name)
{
	XMLCh* attrName = XMLString::transcode(name);
	Ogre::Real rVal = Ogre::StringConverter::parseReal(
		transcodeToString(element->getAttribute(attrName)).c_str() );
	XMLString::release(&attrName);
	return rVal;
}

CeGuiString XmlHelper::getAttributeValueAsString(DOMElement* element, const char* const name)
{
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
	XMLCh* attrName = XMLString::transcode(name);
	std::string rVal(transcodeToStdString(element->getAttribute(attrName)));
	XMLString::release(&attrName);
	return rVal;
}

bool XmlHelper::getAttributeValueAsBool(DOMElement* element,const char* const name)
{
	if( XMLString::compareIString(getAttributeValueAsString(element, name).c_str(),"true") == 0  )
		return true;
	else
		return false;
}

int XmlHelper::getValueAsInteger(DOMElement* element)
{
	return XMLString::parseInt(element->getFirstChild()->getNodeValue());
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
	char* tmpVal = XMLString::transcode(string16);
	std::string rVal(tmpVal);
	XMLString::release(&tmpVal);
	return rVal;
}

} // end namespace rl
