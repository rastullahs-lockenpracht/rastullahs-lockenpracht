#include <xercesc/util/PlatformUtils.hpp>

#define _USE_BOOST
#include "XercesString.h"
#include "XmlParserImplXerces.h"
#include "AimlCore.h"


XERCES_CPP_NAMESPACE_USE 
using namespace MadaBot;

int main(int argc, char* argv[])
{
	try 
	{
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) 
	{
		return 1;
	}
	//SimpleXmlNode<XmlString>* test = new SimpleXmlNode<XmlString>();

	// this will be inherited by DialogSubsystem
	AimlCore<XercesString> core;
	core.setParser(new XmlParserImplXerces());

	// this will be inherited by DialogCharacter
	AimlBot<XercesString>* bot = core.loadBot("Alrike", "startup.xml");

	if(bot)
	{
		Response<XercesString>* response = bot->createResponse("555 Ä");
		delete bot;
	}
	XMLPlatformUtils::Terminate();
	return 0;
}