/** 
 * Generated CppUnit test driver template.
 * To build it, add the following line at the end of
 * your existing Makefile:
 *    include rl/engine/sound/src/OalppSoundInterfaceTest.mk
 * Build the rl/engine/sound/src/OalppSoundInterfaceTest target from the Make Target view
 */

#include "SoundPrerequisites.h"
#include "MusicManager.h"
#include "MusicResource.h"
#include "SoundManager.h"
#include "SoundResource.h"
#include <iostream>
#include "math.h"
#include "OgreResourceManager.h"
#include "cppunit/extensions/HelperMacros.h"

using namespace std;
using namespace rl;
using namespace Ogre;


class SoundMusicManagerTest : public CppUnit::TestFixture {
private:

public:
    SoundMusicManagerTest()
    {
    }
    
	void setUp()
	{
	}

	void tearDown()
	{
	}



    void testSoundMusicManager_playForward()
    {
        MusicManager::getSingletonPtr()->addSounds();
        SoundManager::getSingletonPtr()->addSounds();
        cout << "Now playing: " << *MusicManager::getSingletonPtr()->getSounds().begin() << endl;
        MusicManager::getSingletonPtr()->setAuto(true);
        MusicManager::getSingletonPtr()->setLooping(false);
        MusicManager::getSingletonPtr()->playSong(); 
        sleep(1);
        cout << "Now playing sound " << endl;

        SoundResource* sound = dynamic_cast<SoundResource*>(
            SoundManager::getSingletonPtr()->getByName("testsound.wav")
        );
        if (sound != 0)
        {
            sound->load();
            for(int i = 1; i < 5; i++)
            {
                sound->play();
                sleep(2);
                sound->rewind();
            }
            sound->stop();
            sound->unload();
        }        
        CPPUNIT_ASSERT(true);
    }
    
 
    CPPUNIT_TEST_SUITE(SoundMusicManagerTest);
    CPPUNIT_TEST(testSoundMusicManager_playForward);
    CPPUNIT_TEST_SUITE_END(); 
};
//CPPUNIT_TEST_SUITE_REGISTRATION(SoundMusicManagerTest);
