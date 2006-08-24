#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include "AimlParserImplRl.h"
#include "XmlResourceManager.h"
#include "XmlMapper/XmlDocument.h"
#include "AimlNodeImplRl.h"
#include "Logger.h"

// needed for resources
#include "CoreSubsystem.h"
#include "ContentModule.h"

using namespace rl;
using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;

AimlParserImplRl::~AimlParserImplRl(void)
{
}

XmlDocument<CeGuiString>* AimlParserImplRl::parse(const CeGuiString& fileName)
{
	XercesDOMParser parser;// = new XercesDOMParser();

	parser.setIncludeIgnorableWhitespace(false);
	parser.setDoNamespaces(true);

	XmlPtr res = getXmlResource(fileName);

    bool result = res->parseBy(&parser);
    
    if(result)
    {
        XERCES_CPP_NAMESPACE::DOMDocument* doc = parser.getDocument();
	    XERCES_CPP_NAMESPACE::DOMElement* test = doc->getDocumentElement();
	    XmlNode<CeGuiString>* rootNode = new AimlNodeImplRl(doc->getDocumentElement());
	    return (new XmlDocument<CeGuiString>(rootNode));
    }
    std::string message = "File '"+ std::string(fileName.c_str()) +"' could not be parsed.";
    message += "Possible reason: Wrong file format.";
    LOG_ERROR(Logger::DIALOG, message );
    return NULL;
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
