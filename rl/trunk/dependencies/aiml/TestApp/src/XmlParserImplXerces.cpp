#include "XmlParserImplXerces.h"

#include <iostream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include "XmlMapper/XmlDocument.h"
#include "SimpleXmlNodeImplXerces.h"

XmlParserImplXerces::~XmlParserImplXerces()
{
}

XmlDocument<XercesString>* XmlParserImplXerces::parse(const XercesString& pFileName)
{
	XercesDOMParser* parser = new XercesDOMParser();
	// TODO: use own error handler for xsd validation
	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
    parser->setErrorHandler(errHandler);
	parser->setIncludeIgnorableWhitespace(false);
//	parser->setDoNamespaces(true);
	//parser->parse(pFileName);
	try 
	{
        parser->parse(pFileName.c_str());
    }
    catch (const XMLException& toCatch) 
	{
        char* message = XMLString::transcode(toCatch.getMessage());
		std::cout << "Exception message is: \n"
             << message << "\n";
        XMLString::release(&message);
        return NULL;
    }
    catch (const DOMException& toCatch) 
	{
        char* message = XMLString::transcode(toCatch.msg);
		std::cout << "Exception message is: \n"
             << message << "\n";
        XMLString::release(&message);
        return NULL;
    }
	catch (...) 
	{
		std::cout << "Unexpected Exception \n" ;
        return NULL;
    }
	DOMDocument* doc = parser->getDocument();
	DOMElement* test = doc->getDocumentElement();
	XmlNode<XercesString>* rootNode = new SimpleXmlNodeImplXerces(doc->getDocumentElement());
	delete errHandler;
	delete parser;
	return (new XmlDocument<XercesString>(rootNode));
}