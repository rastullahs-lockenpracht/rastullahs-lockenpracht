#include <OgreNoMemoryMacros.h>

#include <cppunit/Exception.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <iostream>

#include "Logger.h"
#include "ScriptObjectRepository.h"

#include "CombatTest.h"
#include "DateTest.h"
#include "TalentTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TalentTest );
CPPUNIT_TEST_SUITE_REGISTRATION( DateTest );
CPPUNIT_TEST_SUITE_REGISTRATION( CombatTest );

int main(int argc, char** argv)
{
	new rl::Logger(
		"./logs/rastullah.log",
		"./logs/ogre.log");
    rl::Logger::getSingleton().setLogDetail(Ogre::LL_BOREME);

	TextUi::TestRunner runner;
	TestFactoryRegistry& registry = TestFactoryRegistry::getRegistry();
	runner.addTest(registry.makeTest());
	runner.run();
	return 0;
}
