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
        xt.sec = 1;
        xt.nsec = 0;
        
        rl::ResourceManager::ResourceMapIterator it =
            SoundManager::getSingleton().getResourceIterator();
        while (it.hasMoreElements())
        {
            Ogre::Resource *res = it.getNext();
            SoundResource* sound = dynamic_cast<SoundResource*>(res);
            if (sound)
            {
                sound->load();
                sound->play();
                
                thread::sleep(xt);
                while (sound->isPlaying())
                    thread::sleep(xt);
                
                sound->stop();
                sound->unload();
            }            
        }
        cerr<<"ende"<<endl;
        CPPUNIT_ASSERT(true);
    }
    
    void testSoundManager_loadPlayWithFade()
    {
        xtime xt;
        xt.sec = 10;
        xt.nsec = 0;
        Ogre::ResourceManager::ResourceMapIterator it =
            SoundManager::getSingleton().getResourceIterator();
        while (it.hasMoreElements())
        {
            Ogre::Resource *res = it.getNext();
            SoundResource* sound = dynamic_cast<SoundResource*>(res);
            std::cerr << sound->getName() <<std::endl;
            if (sound)
            {
                sound->load();
                sound->play(2 * 1000);
                
                thread::sleep(xt);
                
                sound->stop(2 * 1000);
                
                thread::sleep(xt);
                
                sound->unload();
            }            
        }
        
        CPPUNIT_ASSERT(true);
    }

	CPPUNIT_TEST_SUITE(SoundManagerTest);
	CPPUNIT_TEST(testSoundManager_addSoundDirectory);
//    CPPUNIT_TEST(testSoundManager_loadPlayUnload);
    CPPUNIT_TEST(testSoundManager_loadPlayWithFade);
    CPPUNIT_TEST_SUITE_END();
};
//CPPUNIT_TEST_SUITE_REGISTRATION(SoundManagerTest);
