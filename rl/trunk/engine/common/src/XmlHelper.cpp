/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#include <xercesc/dom/DOM.hpp>

#include "XmlHelper.h"

using namespace XERCES_CPP_NAMESPACE;


namespace rl {

XMLTranscoder* XmlHelper::sTranscoder;
XMLTransService::Codes XmlHelper::sFailCode;

void XmlHelper::initializeTranscoder()
{
	XmlHelper::sFailCode = XMLTransService::Ok;
	XmlHelper::sTranscoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor(XMLRecognizer::UTF_8, XmlHelper::sFailCode, 16*1024);
}

DOMElement* XmlHelper::getChildNamed(DOMElement* parent, const char* name)
{
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

char* XmlHelper::getValueAsString(DOMElement* element)
{
	return XMLString::transcode(element->getFirstChild()->getNodeValue());
}

utf8* XmlHelper::getValueAsUtf8(DOMElement* element)
{
	return XmlHelper::transcodeToUtf8(element->getFirstChild()->getNodeValue());
}

int XmlHelper::getAttributeValueAsInteger(DOMElement* element,XMLCh* name)
{
	return XMLString::parseInt(element->getAttribute(name));
}

char* XmlHelper::getAttributeValueAsString(DOMElement* element,XMLCh* name)
{
	return XMLString::transcode(element->getAttribute(name));
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

}
