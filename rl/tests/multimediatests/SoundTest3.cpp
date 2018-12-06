 /** 
 * Generated CppUnit test driver template.
 * To build it, add the following line at the end of
 * your existing Makefile:
 *    include rl/engine/sound/src/OalppSoundInterfaceTest.mk
 * Build the rl/engine/sound/src/OalppSoundInterfaceTest target from the Make Target view
 */

#include <cppunit/extensions/HelperMacros.h>
#include "OgreNoMemoryMacros.h"
#include "MultimediaPrerequisites.h"
#ifdef  _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <OgreIteratorWrappers.h>
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
		MultimediaSubsystem *mm = MultimediaSubsystem::getSingletonPtr();
		SoundDriver *driver = mm->getActiveDriver();
		
        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Starte Test #3");
        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Using Driver " + driver->getName());

        // Den Listener etwas vom Mittelpunkt des Kreises weg, sonst ist die Distanz 
        // immer gleich und im Softwaremodus von fmod hört man keine Unterschiede.
        ListenerMovable *listener = MultimediaSubsystem::getSingleton().getActiveDriver()->createListener("Listener");
        MultimediaSubsystem::getSingleton().setActiveListener(listener);
        listener->setPosition(Vector3(1, 1, 0));
        
        // Der Stereosound wird auf Mono gezwungen (hoffentlich!)
        Sound *sound = driver->createSample("feuer_knisternd_01.ogg");
        SoundChannel *channel = driver->createChannel(sound, "musik");
        sound->set3d(true);
        channel->setLooping(true);
        channel->play();
        
        // Fünfmal im Kreis herum.
        for (int i = 1; i <= 5; i++)
        {
            for(float d = 0; d <= 2 * M_PI; d += 0.1)
            {
                float x = 10 * sinf(d);
                float y = 20 * cosf(d);
                channel->setPosition(Vector3(x, y, 0.0f));
                driver->update();
                
                xtime xt;
                xtime_get(&xt, TIME_UTC);
                xt.nsec += 50000000;
                thread::sleep(xt);
            }
        }


        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Beende Test #3");
        CPPUNIT_ASSERT(true);
    }
 

    CPPUNIT_TEST_SUITE(SoundTest3);
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();
};
CPPUNIT_TEST_SUITE_REGISTRATION(SoundTest3);
