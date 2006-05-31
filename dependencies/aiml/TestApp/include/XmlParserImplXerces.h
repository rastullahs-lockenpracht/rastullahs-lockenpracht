/**
 *
 */
#ifndef XML_PARSER_IMPL_XERCES_H
#define XML_PARSER_IMPL_XERCES_H

#include "XmlMapper/XmlParser.h"
#include "XercesString.h"

using namespace XmlMapper;
XERCES_CPP_NAMESPACE_USE

class XmlParserImplXerces : public XmlParser<XercesString>
{
public:
	virtual ~XmlParserImplXerces();
	
	virtual XmlDocument<XercesString>* parse(const XercesString& pFileName);

};

#endif
