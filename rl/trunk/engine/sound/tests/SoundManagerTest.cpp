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
            SoundResource* sound = dynamic_cast<SoundResource*>(it.getNext());
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
    
    void testSoundManager_testOAL()
    {
        xtime xt;

        DataChunk dc;
        ALenum format;
        ALvoid *data;
        ALsizei size, freq;
        ALboolean loop = false;
        ALuint buffer, source;
            
        // Lade das WAV aus dem Speicher
        SoundManager::_findCommonResourceData("testsound.wav", dc);
        alutLoadWAVMemory((ALbyte*)dc.getPtr(), &format, &data, &size, &freq, &loop);
        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, data, size, freq);
        alutUnloadWAV(format, data, size, freq);
        alGenSources(1, &source);
        alSourceQueueBuffers(source,1,&buffer);
        alSourcePlay(source);
        
        xtime_get(&xt, TIME_UTC);
        xt.sec += 5;
        thread::sleep(xt);
        
        alSourceStop(source);
        alSourceUnqueueBuffers(source,1,&buffer);
        alDeleteSources(1,&source);
        alDeleteBuffers(1,&buffer);
        CPPUNIT_ASSERT(true);
    }
    
	CPPUNIT_TEST_SUITE(SoundManagerTest);
	CPPUNIT_TEST(testSoundManager_addSoundDirectory);
    CPPUNIT_TEST(testSoundManager_testOAL);
    CPPUNIT_TEST(testSoundManager_loadPlayUnload);
    CPPUNIT_TEST_SUITE_END();
};
CPPUNIT_TEST_SUITE_REGISTRATION(SoundManagerTest);
