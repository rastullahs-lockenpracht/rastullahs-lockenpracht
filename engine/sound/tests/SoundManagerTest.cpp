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
#include <iostream>
#include "math.h"
#include "OgreIteratorWrappers.h"
#include "cppunit/extensions/HelperMacros.h"


using namespace std;
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

        ResourceManager::ResourceMapIterator cit = 
            SoundManager::getSingleton().getResourceIterator();
        while(cit.hasMoreElements())
        {
            cout << SoundManager::getCommonPathByName((cit.getNext())->getName()) << endl;
        } 
        cout << endl;
	    CPPUNIT_ASSERT(true);
	}
 
    void testSoundManager_loadPlayUnload()
    {
        ResourceManager::ResourceMapIterator it =
            SoundManager::getSingleton().getResourceIterator();
        while (it.hasMoreElements())
        {
            SoundResource* sound = dynamic_cast<SoundResource*>(it.getNext());
            if (sound)
            {
                cerr << "Playing sound " << sound->getName() << endl;
                sound->load();
                sound->play();
                ::sleep( 5 );
                sound->stop();
                sound->unload();
            }            
        }
        
        CPPUNIT_ASSERT(true);
    }
    
    void testSoundManager_testOAL()
    {
        cerr << "Playing direct" << endl;
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
        ::sleep(5);
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
