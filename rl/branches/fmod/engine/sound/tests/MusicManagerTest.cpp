
#include "OgreNoMemoryMacros.h"
#include "SoundPrerequisites.h"
#include "MusicManager.h"
#include "SoundResource.h"
#include "math.h"
#include "OgreResourceManager.h"
#include "cppunit/extensions/HelperMacros.h"
#include "SoundManager.h"
#include <boost/thread.hpp>

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
        MusicManager::getSingleton().addPlayList(SoundManager::getSingleton().getSounds());
	    CPPUNIT_ASSERT(true);
	}
 
    void testMusicManager_playSingle()
    {
        SoundResourcePtr ptr = SoundResourcePtr(
            dynamic_cast<SoundResource*>(SoundManager::getSingleton().getByName("doorcreak.ogg").getPointer()));
        SoundStreamMovable *ssm = new SoundStreamMovable(ptr);
        ssm->play();
        while(ssm->isPlaying())
        {
            boost::xtime xt;
            boost::xtime_get(&xt, boost::TIME_UTC);
            xt.sec += 5;
            boost::thread::sleep(xt);
        }
        ssm->stop();
        ssm->unload();
        delete ssm;        
        ptr = SoundResourcePtr(
            dynamic_cast<SoundResource*>(SoundManager::getSingleton().getByName("doorcreak.ogg").getPointer()));
        ssm = new SoundStreamMovable(ptr);
        ssm->play();
        while(ssm->isPlaying())
        {
            boost::xtime xt;
            boost::xtime_get(&xt, boost::TIME_UTC);
            xt.sec += 5;
            boost::thread::sleep(xt);
        }
        ssm->stop();
        ssm->unload();
        delete ssm;        
        CPPUNIT_ASSERT(true);
    }

    void testMusicManager_playForward()
    {
        boost::xtime xt;
        MusicManager::getSingletonPtr()->setAuto(true);
        MusicManager::getSingletonPtr()->setLooping(false);
        MusicManager::getSingletonPtr()->playSong(); 
        
        boost::xtime_get(&xt, boost::TIME_UTC);
        xt.sec += 10;
        boost::thread::sleep(xt);
        while (MusicManager::getSingletonPtr()->isPlaying())
        {
            boost::xtime_get(&xt, boost::TIME_UTC);
            xt.sec += 5;
            boost::thread::sleep(xt);
        }
        cerr<<endl<<"Ende"<<endl;
        MusicManager::getSingletonPtr()->stopSong();
        CPPUNIT_ASSERT(true);
    }
    
    void testMusicManager_playStop()
    {
        boost::xtime xt;
        MusicManager::getSingletonPtr()->setAuto(true);
        MusicManager::getSingletonPtr()->setLooping(false);
        MusicManager::getSingletonPtr()->playSong(); 
        
        boost::xtime_get(&xt, boost::TIME_UTC);
        xt.sec += 15;
        boost::thread::sleep(xt);
        MusicManager::getSingletonPtr()->stopSong();
        boost::xtime_get(&xt, boost::TIME_UTC);
        xt.sec += 15;
        boost::thread::sleep(xt);

        CPPUNIT_ASSERT(true);
    }

    CPPUNIT_TEST_SUITE(MusicManagerTest);
    CPPUNIT_TEST(testMusicManager_addSoundDirectory);
    CPPUNIT_TEST(testMusicManager_playSingle);
//    CPPUNIT_TEST(testMusicManager_playForward);
//    CPPUNIT_TEST(testMusicManager_playStop);
    CPPUNIT_TEST_SUITE_END(); 
};
CPPUNIT_TEST_SUITE_REGISTRATION(MusicManagerTest);
