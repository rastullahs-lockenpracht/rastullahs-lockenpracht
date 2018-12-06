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
#include "SimplePlaylist.h"
#include "Sound.h"
#include "SoundChannel.h"
#include "SoundDriver.h"
#include "SoundManager.h"
#include "SoundObject.h"
#include "SoundResource.h"
#include <OgreIteratorWrappers.h>
#include <boost/thread.hpp>
#include <math.h>

using namespace rl;
using namespace boost;
using namespace Ogre;

class SoundTest6 : public CppUnit::TestFixture
{
private:
public:
    SoundTest6()
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

        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Starte Test #6");
        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Using Driver " + driver->getName());

        SimplePlaylist playlist;

        Sound* sound1 = driver->createSample("lachen.ogg");
        SoundObject* so1 = new SoundObject(sound1, "lachen");
        Sound* sound2 = driver->createSample("schlucken.ogg");
        SoundObject* so2 = new SoundObject(sound2, "schlucken");
        Sound* sound3 = driver->createSample("doorcreak.ogg");
        SoundObject* so3 = new SoundObject(sound2, "tuer");

        playlist.add(so1);
        playlist.add(so2);
        playlist.add(so3);
        playlist.start();

        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Beende Test #6");
        CPPUNIT_ASSERT(true);
    }

    CPPUNIT_TEST_SUITE(SoundTest6);
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();
};
CPPUNIT_TEST_SUITE_REGISTRATION(SoundTest6);
