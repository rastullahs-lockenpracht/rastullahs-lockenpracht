#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLChar.hpp>
#include <xercesc/util/TransService.hpp>
//#include <string>
#include "SimpleXmlNodeImplXerces.h"


XERCES_CPP_NAMESPACE_USE

SimpleXmlNodeImplXerces::SimpleXmlNodeImplXerces(DOMNode* pNode)
	: SimpleXmlNode(pNode->getNodeName(), pNode->getNodeValue())
{
	// remove whitespaces from textnodes
	if(mNodeName == "#text")
	{
		XMLCh* tmp = XMLString::replicate(pNode->getNodeValue());
		XMLString::collapseWS(tmp);
		mNodeValue = tmp;
		XMLString::release(&tmp);
	}
//	mNodeName  = transcode(pNode->getNodeName());
//	mNodeValue = transcode(pNode->getNodeValue());
//	XmlString name(pNode->getNodeValue());

	DOMNamedNodeMap* attributes = pNode->getAttributes();
	if(attributes != NULL)
	{
		for(unsigned int i = 0; i < attributes->getLength(); ++i)
		{
			mAttributes.insert(Attributes::value_type(
				attributes->item(i)->getNodeName(),	
				attributes->item(i)->getNodeValue()));
			const XMLCh* test = attributes->item(i)->getBaseURI();
			test = attributes->item(i)->getLocalName();
			test = attributes->item(i)->getPrefix();
			XercesString testStr = test;
		}
	}
	DOMNode* child = pNode->getFirstChild();

	for(; child !=NULL; child = child->getNextSibling())
	{
		// don't add pure whitespaces as AimlNode
		XMLCh* cmp = XMLString::transcode("#text");
		if(XMLString::equals(child->getNodeName(), cmp))
		{
			if(XMLChar1_0::isAllSpaces(child->getNodeValue(), XMLString::stringLen(child->getNodeValue())))
			{
				continue;
			}
		}
		XMLString::release(&cmp);
		mChildren.push_back(new SimpleXmlNodeImplXerces(child));
	}
	if(!mChildren.empty())
	{
		std::vector<XmlNode<XercesString>*>::iterator itr = mChildren.begin();
		std::vector<XmlNode<XercesString>*>::iterator itrnext;
		for(; itr != mChildren.end()-1; ++itr)
		{
			itrnext= itr+1;
			(*itr)->setNextSibling((*itrnext));
		}
		(*itr)->setNextSibling(NULL);

	}
}
/*
AimlNodeImplXerces::AimlNodeImplXerces(AimlNodeImplXerces& pData)
{
	pData.getNodeName();
//	mNodeName  = pData.getNodeName();
//	mNodeValue = pData.getNodeValue();
	
}
*/
//std::string AimlNodeImplXerces::transcode(const XMLCh* const data)
//{
/*	if(data != NULL)
	{
		char* tmp  = XMLString::transcode(data);
		
		std::string rVal(tmp);
		XMLString::release(&tmp);
*/		
/*
		unsigned int length = XMLString::stringLen(data);
		unsigned int eaten = 0;
		unsigned int size = length;

		XMLTranscoder* transcoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor(XMLRecognizer::UTF_8, XMLTransService::Ok, 16*1024);

		unsigned char* tmp;
		do
		{
			tmp = new unsigned char[size+1];
		
			transcoder->transcodeTo(data, length, tmp, size, eaten, XMLTranscoder::UnRep_RepChar);
			tmp[size] = 0;

			if (eaten < length)
			{
				size += length - eaten;
				delete[] tmp;
			}
		}
		while (eaten < length);
		delete transcoder;
*/
//		return rVal;
//	}
//	return "";
//}
/*
AimlNode* AimlNodeImplXerces::clone()
{
	return new AimlNodeImplXerces(*this);
}
*/