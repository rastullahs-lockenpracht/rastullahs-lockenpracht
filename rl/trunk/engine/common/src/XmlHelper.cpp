#include <xercesc/dom/DOM.hpp>

#include "XmlHelper.h"

using namespace XERCES_CPP_NAMESPACE;


namespace rl {



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

}
