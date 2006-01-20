 /** 
 * Generated CppUnit test driver template.
 * To build it, add the following line at the end of
 * your existing Makefile:
 *    include rl/engine/sound/src/OalppSoundInterfaceTest.mk
 * Build the rl/engine/sound/src/OalppSoundInterfaceTest target from the Make Target view
 */

#include "SoundPrerequisites.h"
#ifdef  _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <OgreIteratorWrappers.h>
#include <cppunit/extensions/HelperMacros.h>
#include <boost/thread.hpp>
#include "SoundManager.h"
#include "SoundResource.h"
#include "Sound.h"
#include "SoundSample.h"
#include "SoundChannel.h"
#include "ListenerMovable.h"


using namespace rl;
using namespace boost;
using namespace Ogre;

class SoundTest3 : public CppUnit::TestFixture {
private:

public:
    SoundTest3()
    {
    }
    
    void setUp()
    {
    }

    void tearDown()
    {
    }

    void test()
    {
        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Starte Test #3");

        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Beende Test #3");
        CPPUNIT_ASSERT(true);
    }
 

    CPPUNIT_TEST_SUITE(SoundTest3);
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();
};
CPPUNIT_TEST_SUITE_REGISTRATION(SoundTest3);
