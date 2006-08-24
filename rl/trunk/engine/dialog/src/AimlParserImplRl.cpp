#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include "AimlParserImplRl.h"
#include "XmlResourceManager.h"
#include "XmlMapper/XmlDocument.h"
#include "AimlNodeImplRl.h"

#include "CoreSubsystem.h"
#include "ContentModule.h"

using namespace rl;
using namespace Ogre;

AimlParserImplRl::~AimlParserImplRl(void)
{
}

XmlDocument<CeGuiString>* AimlParserImplRl::parse(const CeGuiString& fileName)
{
	XercesDOMParser* parser = new XercesDOMParser();

	parser->setIncludeIgnorableWhitespace(false);
	parser->setDoNamespaces(true);
	//parser->parse(pFileName);

	try
	{	
		XmlPtr res = getXmlResource(fileName);
		res->parseBy(parser);
	}
	catch(const SAXParseException& exc)
	{
		// get & log xerces error message
		char* excmsg = XMLString::transcode(exc.getMessage());
		CeGuiString excs="Exception while parsing AimlDocument: ";
		excs+=excmsg;
		LOG_MESSAGE(Logger::DIALOG, excs);
		// cleanup
		if (parser)delete parser;
		throw(exc);
	}
	catch (const DOMException& exc) 
	{
        char* excmsg = XMLString::transcode(exc.getMessage());
		CeGuiString excs="Exception while parsing AimlDocument: ";
		excs+=excmsg;
		LOG_MESSAGE(Logger::DIALOG, excs);
		// cleanup
		if (parser)delete parser;
		throw(exc);
    }
    catch (const XMLException& exc) 
	{
		char* excmsg = XMLString::transcode(exc.getMessage());
		CeGuiString excs="Exception while parsing AimlDocument: ";
		excs+=excmsg;
		LOG_MESSAGE(Logger::DIALOG, excs);
		// cleanup
		if(parser)delete parser;
		return NULL;
    }
	catch (...) 
	{
		LOG_MESSAGE(Logger::DIALOG, 
			"Unknown Exception while parsing AimlDocument");
        if(parser)delete parser;
		return NULL;
    }
	DOMDocument* doc = parser->getDocument();
	DOMElement* test = doc->getDocumentElement();
	XmlNode<CeGuiString>* rootNode = new AimlNodeImplRl(doc->getDocumentElement());
	delete parser;
	return (new XmlDocument<CeGuiString>(rootNode));
}

ResourcePtr AimlParserImplRl::getXmlResource(const CeGuiString& fileName)
{
	ResourcePtr res = XmlResourceManager::getSingleton().getByName(fileName.c_str());

    if (res.isNull())
    {
        Ogre::String group = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
        if (ResourceGroupManager::getSingleton().resourceExists(
			CoreSubsystem::getSingleton().getActiveAdventureModule()->getId(), fileName.c_str()))
        {
            group = CoreSubsystem::getSingleton().getActiveAdventureModule()->getId();
        }
		res = XmlResourceManager::getSingleton().create(fileName.c_str(), group);

    }
    return res;
}
