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
        SoundSample *sound1 = new SoundSample("ruchin001.ogg");
        SoundChannel *channel1 = new SoundChannel(sound1, "musik");
        SoundSample *sound2 = new SoundSample("lachen.ogg");
        SoundChannel *channel2 = new SoundChannel(sound2, "bewegung");
        channel1->set3d(false);
        channel1->play();
        channel2->setLooping(true);
        channel2->play();
        Vector3 pos(-70.0f, 0.0f, 0.0f);
        channel2->setPosition(pos);
        FSOUND_Update();
        xtime xt;
        xtime_get(&xt, TIME_UTC);
        xt.sec += 15;
        thread::sleep(xt);
 
        channel1->stop();
        channel2->stop();
        delete channel1;
        delete channel2;

        CPPUNIT_ASSERT(true);
    }
 

    CPPUNIT_TEST_SUITE(SoundTest2);
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();
};
CPPUNIT_TEST_SUITE_REGISTRATION(SoundTest2);
