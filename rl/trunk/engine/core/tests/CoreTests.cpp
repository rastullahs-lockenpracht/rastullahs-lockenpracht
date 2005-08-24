
#include "cppunit/extensions/HelperMacros.h"
#include "CoreSubsystem.h"
#include "GameObject.h"
#include "ActorManager.h"
#include "LightObject.h"

using namespace rl;

class CoreTest : public CppUnit::TestFixture {

    void testActors()
    {
        CPPUNIT_ASSERT(true);
    }


    CPPUNIT_TEST_SUITE(CoreTest);
    CPPUNIT_TEST(testActors);
    CPPUNIT_TEST_SUITE_END(); 
};
CPPUNIT_TEST_SUITE_REGISTRATION(CoreTest);
