/**
 * Generated CppUnit test driver template.
 * To build it, add the following line at the end of
 * your existing Makefile:
 *    include rl/engine/sound/src/OalppSoundInterfaceTest.mk
 * Build the rl/engine/sound/src/OalppSoundInterfaceTest target from the Make Target view
 */

#include "MultimediaPrerequisites.h"
#include "OgreNoMemoryMacros.h"
#include <cppunit/extensions/HelperMacros.h>
#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include "ListenerMovable.h"
#include "Logger.h"
#include "Sound.h"
#include "SoundChannel.h"
#include "SoundDriver.h"
#include "SoundManager.h"
#include "SoundResource.h"
#include <OgreIteratorWrappers.h>
#include <boost/thread.hpp>
#include <math.h>

using namespace rl;
using namespace boost;
using namespace Ogre;

class SoundTest7 : public CppUnit::TestFixture
{
private:
public:
    SoundTest7()
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
        MultimediaSubsystem* mm = MultimediaSubsystem::getSingletonPtr();
        SoundDriver* driver = mm->getActiveDriver();

        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Starte Test #7");
        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Using Driver " + driver->getName());

        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Beende Test #7");
        CPPUNIT_ASSERT(true);
    }

    CPPUNIT_TEST_SUITE(SoundTest7);
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();
};
CPPUNIT_TEST_SUITE_REGISTRATION(SoundTest7);
