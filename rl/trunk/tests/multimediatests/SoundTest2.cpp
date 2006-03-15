 /** 
 * Generated CppUnit test driver template.
 * To build it, add the following line at the end of
 * your existing Makefile:
 *    include rl/engine/sound/src/OalppSoundInterfaceTest.mk
 * Build the rl/engine/sound/src/OalppSoundInterfaceTest target from the Make Target view
 */

#include "MultimediaPrerequisites.h"
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
#include "SoundDriver.h"
#include "MultimediaSubsystem.h"
#include "ListenerMovable.h"
#include "Logger.h"


using namespace rl;
using namespace boost;
using namespace Ogre;

class SoundTest2 : public CppUnit::TestFixture {
private:

public:
    SoundTest2()
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
		MultimediaSubsystem *mm = MultimediaSubsystem::getSingletonPtr();
		SoundDriver *driver = mm->getActiveDriver();
		
        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Starte Test #2");
        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Using Driver " + driver->getName());

        Sound *sound1 = driver->createSample("ruchin001.ogg");
        SoundChannel *channel1 = driver->createChannel(sound1, "musik");
        Sound *sound2 = driver->createSample("lachen.ogg");
        SoundChannel *channel2 = driver->createChannel(sound2, "bewegung");
        sound1->set3d(false);
        channel1->play();
        channel2->setLooping(true);
        channel2->play();
        Vector3 pos(-50.0f, 0.0f, 0.0f);
        channel2->setPosition(pos);
        driver->update();
        xtime xt;
        xtime_get(&xt, TIME_UTC);
        xt.sec += 15;
        thread::sleep(xt);
 
        channel1->stop();
        channel2->stop();
        delete channel1;
        delete channel2;

        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Beende Test #2");
        CPPUNIT_ASSERT(true);
    }
 

    CPPUNIT_TEST_SUITE(SoundTest2);
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();
};
//CPPUNIT_TEST_SUITE_REGISTRATION(SoundTest2);
