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
#include <iostream>
#include "math.h"
#include "OgreResourceManager.h"
#include "cppunit/extensions/HelperMacros.h"

using namespace std;
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
        StringList list = MusicManager::getSingletonPtr()->getSounds();
        StringList::const_iterator cit = list.begin();
        while(cit != list.end())
        {
            cout << *cit << endl;
            cit++;
        }
        
	    CPPUNIT_ASSERT(true);
	}
 

    void testMusicManager_playForward()
    {
        cout << "Now playing: " << *MusicManager::getSingletonPtr()->getSounds().begin() << endl;
        MusicManager::getSingletonPtr()->setAuto(true);
        MusicManager::getSingletonPtr()->setLooping(false);
        MusicManager::getSingletonPtr()->playSong(); 
        sleep(2);
        while (MusicManager::getSingletonPtr()->isPlaying())
        {
            usleep( 10 * 1000 * 1000 );
        }
        MusicManager::getSingletonPtr()->stopSong();
        CPPUNIT_ASSERT(true);
    }
    
/*    void testMusicManager_playOgg()
    {
        FILE*   tempOggFile;
        int     sizeOfFile; 
        char    tempChar;
        int     tempArray;

        if(!(tempOggFile = fopen("sound/bomb.ogg", "rb")))
            throw string("Could not open Ogg file.");

        // Find out how big the file is
        sizeOfFile = 0;
        while (!feof(tempOggFile))
        {
            tempChar = getc(tempOggFile);
            sizeOfFile++;
        }

        // Save the data into memory
        char *dataPtr = new char[sizeOfFile];
        rewind(tempOggFile);
        tempArray = 0;
        while (!feof(tempOggFile))
        {
            dataPtr[tempArray] = getc(tempOggFile);
            tempArray++;
        }

        // Close the ogg file
        fclose(tempOggFile);


        ogg_stream stream;
        stream.open((unsigned char*)dataPtr, sizeOfFile);
        
        if(!stream.playback())
            Throw(RuntimeException, "Ogg refused to play.");
                        
        while(stream.update())
        {
            if(!stream.playing())
            {
                if(!stream.playback())
                    Throw(RuntimeException, "Ogg abruptly stopped.");
                else
                    cerr << "Ogg stream was interrupted.\n";
            }
           // Let's wait.
         ::usleep(50 * 1000);
        }
        stream.stop();
        stream.release();  

    } */

    CPPUNIT_TEST_SUITE(MusicManagerTest);
    CPPUNIT_TEST(testMusicManager_addSoundDirectory);
//    CPPUNIT_TEST(testMusicManager_playOgg);
    CPPUNIT_TEST(testMusicManager_playForward);
    CPPUNIT_TEST_SUITE_END(); 
};
CPPUNIT_TEST_SUITE_REGISTRATION(MusicManagerTest);
