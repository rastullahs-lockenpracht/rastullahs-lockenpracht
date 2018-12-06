#include "MultimediaPrerequisites.h"
#include <OgreIteratorWrappers.h>
#include <OgreNoMemoryMacros.h>
#include <cppunit/extensions/HelperMacros.h>
#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include "ListenerMovable.h"
#include "Logger.h"
#include "MultimediaSubsystem.h"
#include "Sound.h"
#include "SoundChannel.h"
#include "SoundDriver.h"
#include "SoundManager.h"
#include "SoundResource.h"
#include <boost/thread.hpp>
#include <math.h>

using namespace rl;
using namespace boost;
using namespace Ogre;

class SoundTest1 : public CppUnit::TestFixture
{
private:
public:
    SoundTest1()
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

        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Starte Test #1");
        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Using Driver " + driver->getName());
        xtime xt;
        Sound* sound1 = driver->createSample("lachen.ogg");
        SoundChannel* channel1 = driver->createChannel(sound1, "sample");
        Sound* sound2 = driver->createSample("lachen.ogg");
        SoundChannel* channel2 = driver->createChannel(sound2, "stream");

        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Starte ersten Sound");
        channel1->play();
        xtime_get(&xt, TIME_UTC);
        xt.sec += 1;
        thread::sleep(xt);
        while (channel1->isPlaying())
        {
            driver->update();
            xtime_get(&xt, TIME_UTC);
            xt.sec += 1;
            thread::sleep(xt);
        }
        channel1->stop();
        delete channel1;

        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Starte zweiten Sound");
        channel2->play();
        xtime_get(&xt, TIME_UTC);
        xt.sec += 1;
        thread::sleep(xt);
        while (channel2->isPlaying())
        {
            driver->update();
            xtime_get(&xt, TIME_UTC);
            xt.sec += 1;
            thread::sleep(xt);
        }
        channel2->stop();
        delete channel2;

        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Beende Test #1");
        CPPUNIT_ASSERT(true);
    }

    CPPUNIT_TEST_SUITE(SoundTest1);
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();
};
CPPUNIT_TEST_SUITE_REGISTRATION(SoundTest1);
