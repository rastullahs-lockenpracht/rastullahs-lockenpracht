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
#include <iostream>
#include "Sleep.h"
#include "SoundManager.h"
#include "SoundResource.h"

using namespace rl;

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
        rl::ResourceManager::ResourceMapIterator it =
            SoundManager::getSingleton().getResourceIterator();
        while (it.hasMoreElements())
        {
            Resource *res = it.getNext();
            SoundResource* sound = dynamic_cast<SoundResource*>(res);
            if (sound)
            {
                sound->load();
                sound->play();
                
                msleep(5 * 1000);
                
                sound->stop();
                sound->unload();
            }            
        }
        
        CPPUNIT_ASSERT(true);
    }
    
    void testSoundManager_loadPlayWithFade()
    {
        rl::ResourceManager::ResourceMapIterator it =
            SoundManager::getSingleton().getResourceIterator();
        while (it.hasMoreElements())
        {
            Resource *res = it.getNext();
            SoundResource* sound = dynamic_cast<SoundResource*>(res);
            std::cerr << sound->getName()<<std::endl;
            if (sound)
            {
                sound->load();
                sound->play(5000);
                
                msleep(10 * 1000);
                
                sound->stop(5000);
                
                msleep(10 * 1000);
                
                sound->unload();
            }            
        }
        
        CPPUNIT_ASSERT(true);
    }

	CPPUNIT_TEST_SUITE(SoundManagerTest);
	CPPUNIT_TEST(testSoundManager_addSoundDirectory);
//    CPPUNIT_TEST(testSoundManager_loadPlayUnload)
    CPPUNIT_TEST(testSoundManager_loadPlayWithFade);
    CPPUNIT_TEST_SUITE_END();
};
CPPUNIT_TEST_SUITE_REGISTRATION(SoundManagerTest);
