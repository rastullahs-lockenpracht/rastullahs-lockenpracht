#include <OgreNoMemoryMacros.h>

#include <cppunit/Exception.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <iostream>

#define NDEBUG
#undef _DEBUG

#include "Exception.h"
#include "Logger.h"
#include "ScriptObjectRepository.h"

#include "CombatTest.h"
#include "DateTest.h"
#include "TalentTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CombatTest );
CPPUNIT_TEST_SUITE_REGISTRATION( DateTest );
CPPUNIT_TEST_SUITE_REGISTRATION( TalentTest );

int main(int argc, char** argv)
{
	new rl::Logger(
		"./logs/rastullah.log",
		"./logs/ogre.log");
	rl::Logger::getSingleton().setLogDetail(Ogre::LL_LOW); // Tests sollen dokumentieren, nicht die Logs

	TextUi::TestRunner runner;
	TestFactoryRegistry& registry = TestFactoryRegistry::getRegistry();
	runner.addTest(registry.makeTest());
	try
	{
		runner.run();
	}
	catch (rl::Exception& ex)
	{
		std::string message = ex.toString();
		printf("%s", message.c_str());
	}
	return 0;
}
