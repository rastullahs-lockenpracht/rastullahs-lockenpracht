#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <iostream>

#include "DateTest.h"
#include "TalentTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( DateTest );
CPPUNIT_TEST_SUITE_REGISTRATION( TalentTest );

int main(int argc, char** argv)
{
   TextUi::TestRunner runner;
   TestFactoryRegistry& registry = TestFactoryRegistry::getRegistry();
   runner.addTest(registry.makeTest());
   runner.run();
   return 0;
}
