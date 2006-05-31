/**
 *
 */
#ifndef SIMPLE_XML_NODE_IMPL_XERCES_H
#define SIMPLE_XML_NODE_IMPL_XERCES_H

#include <xercesc/dom/DOM.hpp>

#include "XercesString.h"
#include "XmlMapper/SimpleXmlNode.h"

//typedef SimpleXmlNode<XercesString> SimpleXmlNodeImplXerces;

//#include "AimlNode.h"

XERCES_CPP_NAMESPACE_USE
class XERCES_CPP_NAMESPACE::DOMNode;

using namespace XmlMapper;

class SimpleXmlNodeImplXerces : public SimpleXmlNode<XercesString>
{
public:
	SimpleXmlNodeImplXerces(DOMNode* node);

//	void setNodeValue(const XmlString& v){}

	bool isElement(){ return false;}

private:
//	std::string transcode(const XMLCh* const data);

//	DOMNode* mNode;
};

#endif
