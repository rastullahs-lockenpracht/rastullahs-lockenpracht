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

utf8* XmlHelper::getValueAsUtf(DOMElement* element)
{
	const XMLCh* val = element->getFirstChild()->getNodeValue();
	int length = XMLString::stringLen(val);
	utf8* rval = new utf8[length+1];
	unsigned int eaten;
	sTranscoder->transcodeTo(val, length, rval, length, eaten, XMLTranscoder::UnRep_RepChar);
	return rval;
}

int XmlHelper::getValueAsInteger(DOMElement* element)
{
	return XMLString::parseInt(element->getFirstChild()->getNodeValue());
}

}
