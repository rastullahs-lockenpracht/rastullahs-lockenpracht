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
#include "SoundStream.h"
#include "SoundChannel.h"
#include "ListenerMovable.h"


using namespace rl;
using namespace boost;
using namespace Ogre;

class SoundTest1 : public CppUnit::TestFixture {
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
        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Starte Test #1");
        xtime xt;
        Sound *sound1 = new SoundSample("lachen.ogg");
        SoundChannel *channel1 = new SoundChannel(sound1, "sample");
        Sound *sound2 = new SoundStream("lachen.ogg");
        SoundChannel *channel2 = new SoundChannel(sound2, "stream");
        
        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Starte ersten Sound");
        channel1->play();
        xtime_get(&xt, TIME_UTC);
        xt.sec+=1;
        thread::sleep(xt);
        while (channel1->isPlaying())
        {
            FSOUND_Update();
            xtime_get(&xt, TIME_UTC);
            xt.sec+=1;
            thread::sleep(xt);
        }
        channel1->stop();
        delete channel1;
        
        Logger::getSingleton().log("SoundTest", Ogre::LML_NORMAL, "Starte zweiten Sound");
        channel2->play();
        xtime_get(&xt, TIME_UTC);
        xt.sec+=1;
        thread::sleep(xt);
        while (channel2->isPlaying())
        {
            FSOUND_Update();
            xtime_get(&xt, TIME_UTC);
            xt.sec+=1;
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
