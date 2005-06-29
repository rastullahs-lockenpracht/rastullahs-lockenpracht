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


using namespace rl;
using namespace boost;

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
            SoundMovablePtr sound(new SoundMovable(soundres));
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
                
                sound->stop();
            }            
        }
        CPPUNIT_ASSERT(true);
    }
    
    void testSoundManager_loadPlayWithFade()
    {
        xtime xt;
        Ogre::ResourceManager::ResourceMapIterator it =
            SoundManager::getSingleton().getResourceIterator();
        while (it.hasMoreElements())
        {
            SoundResourcePtr soundres = it.getNext();
            SoundMovablePtr sound(new SoundMovable(soundres));
            if (!sound.isNull())
            {
                sound->play(2 * 1000);
                
                xtime_get(&xt, boost::TIME_UTC);
                xt.sec += 10;
                thread::sleep(xt);
                
                sound->stop(2 * 1000);
                
                xtime_get(&xt, boost::TIME_UTC);
                xt.sec += 5;
                thread::sleep(xt);
            }            
        }
        
        CPPUNIT_ASSERT(true);
    }

	CPPUNIT_TEST_SUITE(SoundManagerTest);
	CPPUNIT_TEST(testSoundManager_addSoundDirectory);
    CPPUNIT_TEST(testSoundManager_loadPlayUnload);
//    CPPUNIT_TEST(testSoundManager_loadPlayWithFade);
    CPPUNIT_TEST_SUITE_END();
};
//CPPUNIT_TEST_SUITE_REGISTRATION(SoundManagerTest);
