/** 
 * Generated CppUnit test driver template.
 * To build it, add the following line at the end of
 * your existing Makefile:
 *    include rl/engine/sound/src/OalppSoundInterfaceTest.mk
 * Build the rl/engine/sound/src/OalppSoundInterfaceTest target from the Make Target view
 */

#include "OgreNoMemoryMacros.h"
#include "SoundPrerequisites.h"
#include "MusicManager.h"
#include "MusicResource.h"
#include "math.h"
#include "OgreResourceManager.h"
#include "cppunit/extensions/HelperMacros.h"
#include <boost/thread/xtime.hpp>
#include <boost/thread/thread.hpp>

using namespace rl;
using namespace Ogre;
using namespace boost;

class MusicManagerTest : public CppUnit::TestFixture {
private:

public:
    MusicManagerTest()
    {
    }
    
	void setUp()
	{
	}

	void tearDown()
	{
	}

	void testMusicManager_addSoundDirectory()
	{
        MusicManager::getSingletonPtr()->addSounds();
        
	    CPPUNIT_ASSERT(true);
	}
 

    void testMusicManager_playForward()
    {
        xtime xt;
        
        MusicManager::getSingletonPtr()->setAuto(true);
        MusicManager::getSingletonPtr()->setLooping(false);
        MusicManager::getSingletonPtr()->playSong(); 
        
        xtime_get(&xt, TIME_UTC);
        xt.sec += 2;
        thread::sleep(xt);
        while (MusicManager::getSingletonPtr()->isPlaying())
        {
            xtime_get(&xt, TIME_UTC);
            xt.sec += 10;
            thread::sleep(xt);
        }
        MusicManager::getSingletonPtr()->stopSong();
        CPPUNIT_ASSERT(true);
    }
    

    CPPUNIT_TEST_SUITE(MusicManagerTest);
    CPPUNIT_TEST(testMusicManager_addSoundDirectory);
    CPPUNIT_TEST(testMusicManager_playForward);
    CPPUNIT_TEST_SUITE_END(); 
};
//CPPUNIT_TEST_SUITE_REGISTRATION(MusicManagerTest);
