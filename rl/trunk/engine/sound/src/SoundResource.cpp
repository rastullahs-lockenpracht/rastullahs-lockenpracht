#include "SoundResource.h"
#include <string>
#include <iostream>
#include "SoundManager.h"

extern "C" {
#include "AL/al.h"
#include "AL/alut.h"
}

using namespace std;
using namespace Ogre;

namespace rl {

/**
 * @author JoSch
 * @date 07-23-2004
 */
SoundResource::SoundResource(const String &name):
    SndResource(name)
{
    mBufferCount = 1;
    mBuffers = new ALuint[mBufferCount];
    alGenBuffers(mBufferCount, mBuffers);
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
SoundResource::~SoundResource()
{
}

/** Laedt die Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::load()
{
    if (!mIsLoaded)
    {
        DataChunk dc;
        ALenum format;
        ALsizei size, freq;
        ALvoid *data;
        ALboolean loop = false;
            
        // Lade das WAV aus dem Speicher
        SoundManager::_findCommonResourceData(mName, dc);
        alutLoadWAVMemory((ALbyte*)dc.getPtr(), &format, &data, &size, &freq, &loop);
        if(alGetError() != AL_NO_ERROR)
        {
            Throw(RuntimeException, "Error reading sound data");
        }
        // Jetzt einen OAL-Buffer anlegen
        alBufferData(mBuffers[0], format, data, size, freq);
        if(alGetError() != AL_NO_ERROR)
        {
            Throw(RuntimeException, "Error buffering sound");
        }
        // Die Daten brauchen wir jetzt nicht mehr.
        alutUnloadWAV(format, data, size, freq);
        check(); 
        
        alSourceQueueBuffers(mSource, mBufferCount, mBuffers);
        check();
        mIsLoaded = true;
    }
}

}
