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
#include "SoundMovable.h"
#include "SoundSampleMovable.h"
#include "SoundStreamMovable.h"
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
            SoundSampleMovablePtr sound(new SoundSampleMovable(soundres));
            if (!sound.isNull())
            {
                sound->play();
                
                xtime_get(&xt, TIME_UTC);
                xt.sec++;
                thread::sleep(xt);
                while (sound->isPlaying()) {
                    xtime_get(&xt, TIME_UTC);
                    xt.sec++;
                    thread::sleep(xt);
                }
                
                sound.setNull();
            }            
        }
        CPPUNIT_ASSERT(true);
    }
    
    void testSoundManager_playWith3D()
    {
        xtime xt;
        
        Ogre::ResourceManager::ResourceMapIterator it =
            SoundManager::getSingleton().getResourceIterator();
//        ListenerMovable listener("main");
//        listener.setActive();
        FSOUND_3D_SetRolloffFactor(2.5);
        
        while (it.hasMoreElements())
        {
            SoundResourcePtr soundres = it.getNext();
            SoundSampleMovablePtr sound(new SoundSampleMovable(soundres));
            if (!sound.isNull())
            {
                sound->load();
                FSOUND_3D_SetMinMaxDistance(sound->getChannel(), 4.0, 100000.0f);
            FSOUND_3D_Listener_SetAttributes(0, 0,
                0, 0, 1,
                0, 1, 0);
                sound->play();
                float angle = 0.0f;
                
                xtime_get(&xt, TIME_UTC);
                xt.sec++;
                thread::sleep(xt);
                while (sound->isPlaying()) {
                    xtime_get(&xt, TIME_UTC);
                    xt.nsec+=100000;
                    thread::sleep(xt);
                    Vector3 pos(100.0f*sinf(angle), 100.0f*cosf(angle), 0.0f);
                    sound->setPosition(pos);
                    pos = sound->getPosition();
                    cerr <<pos[0]<<":"<<pos[1]<<":"<<pos[2]<<endl;
                    angle += 0.01;
                    if (angle > 2 * M_PI)
                    {
                        angle = 0.0f;
                    }
                    FSOUND_Update();
                }
                
                sound.setNull();
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
            SoundMovablePtr sound(new SoundStreamMovable(soundres));
            if (!sound.isNull())
            {
                sound->play();
                
                xtime_get(&xt, boost::TIME_UTC);
                xt.sec += 10;
                thread::sleep(xt);
                
                sound->stop();
                
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
