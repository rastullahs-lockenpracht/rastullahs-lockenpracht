/** 
 * Generated CppUnit test driver template.
 * To build it, add the following line at the end of
 * your existing Makefile:
 *    include rl/engine/sound/src/OalppSoundInterfaceTest.mk
 * Build the rl/engine/sound/src/OalppSoundInterfaceTest target from the Make Target view
 */

#include "SoundPrerequisites.h"
#include "SoundManager.h"
#include "SoundResource.h"
#include "math.h"
#include "OgreIteratorWrappers.h"
#include "cppunit/extensions/HelperMacros.h"
#include <boost/thread/xtime.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>


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
        
        ResourceManager::ResourceMapIterator it =
            SoundManager::getSingleton().getResourceIterator();
        while (it.hasMoreElements())
        {
            Resource *res = it.getNext();
            SndResource* sound = dynamic_cast<SndResource*>(res);
            if (sound)
            {
                sound->load();
                sound->play();
                
                xtime_get(&xt, TIME_UTC);
                xt.sec += 5;
                thread::sleep(xt);
                
                sound->stop();
                sound->unload();
            }            
        }
        
        CPPUNIT_ASSERT(true);
    }
    
    void testSoundManager_loadPlayWithFade()
    {
        xtime xt;
        
        ResourceManager::ResourceMapIterator it =
            SoundManager::getSingleton().getResourceIterator();
        while (it.hasMoreElements())
        {
            Resource *res = it.getNext();
            SndResource* sound = dynamic_cast<SndResource*>(res);
            std::cerr << sound->getName()<<std::endl;
            if (sound)
            {
                sound->load();
                std::cerr << "Fade In Start" << std::endl;
                sound->play(5000);
                std::cerr << "Fade In Ende" << std::endl;
                
                xtime_get(&xt, TIME_UTC);
                xt.sec += 10;
                thread::sleep(xt);
                
                std::cerr << "Fade Out Start" << std::endl;
                sound->stop(5000);
                std::cerr << "Fade Out Ende" << std::endl;
                
                xtime_get(&xt, TIME_UTC);
                xt.sec += 10;
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
