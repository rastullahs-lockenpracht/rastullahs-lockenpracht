 /** 
 * Generated CppUnit test driver template.
 * To build it, add the following line at the end of
 * your existing Makefile:
 *    include rl/engine/sound/src/OalppSoundInterfaceTest.mk
 * Build the rl/engine/sound/src/OalppSoundInterfaceTest target from the Make Target view
 */

#include "MultimediaPrerequisites.h"
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

class SoundSampleTest : public CppUnit::TestFixture {
private:

public:
    SoundSampleTest()
    {
    }
    
	void setUp()
	{
	}

	void tearDown()
	{
	}

	void test_addSoundDirectory()
	{
        SoundManager::getSingleton().addSounds();

	    CPPUNIT_ASSERT(true);
	}
 
    void test_PlayLoop()
    {
        xtime xt;
        
        Ogre::ResourceManager::ResourceMapIterator it =
            SoundManager::getSingleton().getResourceIterator();
        while (it.hasMoreElements())
        {
            SoundResourcePtr soundres = it.getNext();
            SoundSample *sound = new SoundSample(soundres);
            sound->setLooping(true);
            SoundChannel *channel = new SoundChannel(sound, soundres->getName());
            if (channel)
            {
                channel->play();
                
                xtime_get(&xt, TIME_UTC);
                xt.sec+=10;
                thread::sleep(xt);
                
                delete sound;
            }            
        }
        CPPUNIT_ASSERT(true);
    }
    
    void test_playWith3D()
    {
        xtime xt;
        
        Ogre::ResourceManager::ResourceMapIterator it =
            SoundManager::getSingleton().getResourceIterator();

        while (it.hasMoreElements())
        {
            SoundResourcePtr soundres = it.getNext();
            SoundSample *sound = new SoundSample(soundres);
            SoundChannel *channel = new SoundChannel(sound, soundres->getName());
            if (channel)
            {
                sound->load();
                channel->play();
                float angle = 0.0f;
                
                xtime_get(&xt, TIME_UTC);
                xt.sec++;
                thread::sleep(xt);
                while (channel->isPlaying()) {
                    Vector3 pos(1.0f*sinf(angle), 20.0f*cosf(angle), 0.0f);
                    channel->setPosition(pos);
                    angle += 0.005;
                    if (angle > 2 * M_PI)
                    {
                        angle = 0.0f;
                    }
                    FSOUND_Update();
                    xtime_get(&xt, TIME_UTC);
                    xt.nsec+=10000;
                    thread::sleep(xt);
                }
                
                delete channel;
            }            
        }        
    }


    void testSoundManager_loadPlayWithFade()
    {
        xtime xt;
        Ogre::ResourceManager::ResourceMapIterator it =
            SoundManager::getSingleton().getResourceIterator();
        while (it.hasMoreElements())
        {
            SoundResourcePtr soundres = it.getNext();
            SoundSample *sound = new SoundSample(soundres);
            SoundChannel *channel = new SoundChannel(sound, soundres->getName());
            if (channel)
            {
                channel->play();
                
                xtime_get(&xt, boost::TIME_UTC);
                xt.sec += 10;
                thread::sleep(xt);
                
                channel->stop();
                
                xtime_get(&xt, boost::TIME_UTC);
                xt.sec += 5;
                thread::sleep(xt);
            }            
        }
        
        CPPUNIT_ASSERT(true);
    }

	CPPUNIT_TEST_SUITE(SoundSampleTest);
	CPPUNIT_TEST(test_addSoundDirectory);
    CPPUNIT_TEST(test_playWith3D);
    CPPUNIT_TEST(test_PlayLoop);
//    CPPUNIT_TEST(test_loadPlayWithFade);
    CPPUNIT_TEST_SUITE_END();
};
//CPPUNIT_TEST_SUITE_REGISTRATION(SoundSampleTest);
