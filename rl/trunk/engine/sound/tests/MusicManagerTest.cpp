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
#include "SoundResource.h"
#include "math.h"
#include "OgreResourceManager.h"
#include "cppunit/extensions/HelperMacros.h"
#include "Sleep.h"

using namespace rl;
using namespace Ogre;

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
        MusicManager::getSingletonPtr()->setAuto(true);
        MusicManager::getSingletonPtr()->setLooping(false);
        MusicManager::getSingletonPtr()->playSong(); 
        
        msleep(10 * 1000);
        while (MusicManager::getSingletonPtr()->isPlaying())
        {
            msleep(10 * 1000);
        }
        MusicManager::getSingletonPtr()->stopSong();
        CPPUNIT_ASSERT(true);
    }
    

    CPPUNIT_TEST_SUITE(MusicManagerTest);
    CPPUNIT_TEST(testMusicManager_addSoundDirectory);
    CPPUNIT_TEST(testMusicManager_playForward);
    CPPUNIT_TEST_SUITE_END(); 
};
CPPUNIT_TEST_SUITE_REGISTRATION(MusicManagerTest);
