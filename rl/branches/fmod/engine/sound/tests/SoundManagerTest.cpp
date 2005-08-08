 /** 
 * Generated CppUnit test driver template.
 * To build it, add the following line at the end of
 * your existing Makefile:
 *    include rl/engine/sound/src/OalppSoundInterfaceTest.mk
 * Build the rl/engine/sound/src/OalppSoundInterfaceTest target from the Make Target view
 */

#include "SoundPrerequisites.h"
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

class SoundManagerTest : public CppUnit::TestFixture {
private:

public:
    SoundManagerTest()
    {
    }
    
	void setUp()
	{
	}

	void tearDown()
	{
	}

	void testSoundManager_addSoundDirectory()
	{
        SoundManager::getSingleton().addSounds();

	    CPPUNIT_ASSERT(true);
	}
 
    void testSoundManager_loadPlayUnload()
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
                
                xtime_get(&xt, TIME_UTC);
                xt.sec++;
                thread::sleep(xt);
                while (channel->isPlaying()) {
                    xtime_get(&xt, TIME_UTC);
                    xt.sec++;
                    thread::sleep(xt);
                }
                
                delete sound;
            }            
        }
        CPPUNIT_ASSERT(true);
    }
    
    void testSoundManager_playWith3D()
    {
        xtime xt;
        
        Ogre::ResourceManager::ResourceMapIterator it =
            SoundManager::getSingleton().getResourceIterator();
        FSOUND_3D_SetRolloffFactor(0.5);
        ListenerMovable listener("main");
        listener.setActive();

        while (it.hasMoreElements())
        {
            SoundResourcePtr soundres = it.getNext();
            SoundSample *sound = new SoundSample(soundres);
            SoundChannel *channel = new SoundChannel(sound, soundres->getName());
            if (channel)
            {
                sound->load();
                FSOUND_3D_SetMinMaxDistance(channel->getChannel(), 4.0, 100000.0f);
                channel->play();
                float angle = 0.0f;
                
                xtime_get(&xt, TIME_UTC);
                xt.sec++;
                thread::sleep(xt);
                while (channel->isPlaying()) {
                    xtime_get(&xt, TIME_UTC);
                    xt.nsec+=100000;
                    thread::sleep(xt);
                    Vector3 pos(1.0f*sinf(angle), 20.0f*cosf(angle), 0.0f);
                    channel->setPosition(pos);
                    //pos = channel->getPosition();
                    //cerr <<pos[0]<<":"<<pos[1]<<":"<<pos[2]<<endl;
                    angle += 0.005;
                    if (angle > 2 * M_PI)
                    {
                        angle = 0.0f;
                    }
                    FSOUND_Update();
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
            SoundStream *sound = new SoundStream(soundres);
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

	CPPUNIT_TEST_SUITE(SoundManagerTest);
	CPPUNIT_TEST(testSoundManager_addSoundDirectory);
//    CPPUNIT_TEST(testSoundManager_loadPlayUnload);
    CPPUNIT_TEST(testSoundManager_playWith3D);
//    CPPUNIT_TEST(testSoundManager_loadPlayWithFade);
    CPPUNIT_TEST_SUITE_END();
};
CPPUNIT_TEST_SUITE_REGISTRATION(SoundManagerTest);
