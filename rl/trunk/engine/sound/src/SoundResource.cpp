/* SoundResource.cpp - Diese Klassse kapselt einen Sound.
 * (C) 2004. Team Pantheon. www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#include <string>
#include <iostream>
#include <OgreString.h>
#include "SoundManager.h"
#include "SoundResource.h"
#include "Sleep.h"

using namespace std;
using namespace Ogre;

/// Die Callback-Struktur, die Vorbisfile braucht
ov_callbacks rl::SoundResource::mVorbisCallbacks = {
    &rl::SoundResource::VorbisRead,
    &rl::SoundResource::VorbisSeek,
    &rl::SoundResource::VorbisClose,
    &rl::SoundResource::VorbisTell
};

namespace rl {
/**
 * @author JoSch
 * @date 07-23-2004
 */
SoundResource::SoundResource(const String &name):
    EventListener<SoundEvent>(),
    Resource(),
    mFadeInThread(this, true),
    mFadeOutThread(this, false),
    mStreamThread(this),
    mData(0)
{
    mName = name;
    alGenSources(1, &mSource);
    mBuffers = new ALuint[mBufferCount];
    alGenBuffers(mBufferCount, mBuffers);
    check();
    setGain(1.0f);
    setPosition(Vector3(0.0, 0.0, 0.0));
    setVelocity(Vector3(0.0, 0.0, 0.0));
    setDirection(Vector3(0.0, 0.0, 0.0));
}

/**
 * @author JoSch
 * @date 07-24-2004
 */
SoundResource::~SoundResource()
{
    alDeleteBuffers(mBufferCount, mBuffers);
    delete[] mBuffers;
    alDeleteSources(1, &mSource);
}

/**
 * @return Wenn false, wird das Ereignis nicht mehr weitergereicht.
 * @author JoSch
 * @date 10-14-2004
 */
bool SoundResource::eventRaised(SoundEvent &anEvent)
{
    return true;
}

/**
 * @return Die aktuelle Richtung der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Vector3 SoundResource::getDirection() const throw (RuntimeException)
{
    Vector3 result;
    alGetSource3f(mSource, AL_DIRECTION,
        &result.x, &result.y, &result.z);
    check();
    return result;
}

/**
 * @param direction Die neue Richtung der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::setDirection (const Vector3& direction) throw (RuntimeException)
{
    alSource3f(mSource, AL_DIRECTION,
        direction[0], direction[1], direction[2]);
    check();
}

/**
 * @return Die aktuelle Position der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Vector3 SoundResource::getPosition() const throw (RuntimeException)
{
    Vector3 result;
    alGetSource3f(mSource, AL_POSITION,
        &result.x, &result.y, &result.z);
    check();
    return result;
}

/**
 * @param position Die neue Position der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::setPosition(const Vector3& position) throw (RuntimeException)
{
    alSource3f(mSource, AL_POSITION,
        position[0], position[1], position[2]);
    check();
}

/**
 * @return Die aktuelle Geschwindigkeit der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Vector3 SoundResource::getVelocity() const throw (RuntimeException)
{
    Vector3 result;
    alGetSource3f(mSource, AL_VELOCITY,
        &result.x, &result.y, &result.z);
    check();
    return result;
}

/**
 * @param velocity Die neue Geschwindigkeit der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::setVelocity(const Vector3& velocity) throw (RuntimeException)
{
    alSource3f(mSource, AL_VELOCITY,
        velocity[0], velocity[1], velocity[2]);
    check();
}

/**
 * @return Die aktuelle Lautstaerke der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const ALfloat SoundResource::getGain() const throw (RuntimeException)
{
    ALfloat result;
    mGainMutex.lock();
    alGetSourcef(mSource, AL_GAIN, &result);
    mGainMutex.unlock();
    check();
    return result;
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::setGain(const ALfloat gain) throw (RuntimeException)
{
    mGainMutex.lock();
    alSourcef(mSource, AL_GAIN, gain);
    mGainMutex.unlock();
    check();
}

/** Laedt die Soundquelle und setzt die Werte.
 * @author JoSch
 * @date 10-11-2004
 */
void SoundResource::load()
{
    if (!mIsLoaded)
    {
        mData = new DataChunk();
        // Holen wir erstmal die Daten.
        rl::ResourceManager::_findCommonResourceData(mName, *mData);
        if (StringUtil::endsWith(mName, ".ogg"))
        {
            mSoundDataType = OggVorbis;
        } else if (StringUtil::endsWith(mName, ".wav"))
        {
            mSoundDataType = Wave;
        } else {
            delete mData;
            mData = 0;
            mIsLoaded = false;
            return;
        }
        // Jetzt mal die Soundparameter ermitteln.
        switch(mSoundDataType)
        {
            vorbis_info *vorbisInfo;
            case OggVorbis:
                mOggMemoryFile.mDataPtr = mData->getPtr();
                mOggMemoryFile.mDataSize = mData->getSize();
                mOggMemoryFile.mDataRead = 0;
                if (ov_open_callbacks(&mOggMemoryFile, &mOggStream, NULL, 0,
                         SoundResource::mVorbisCallbacks) != 0)
                    throw string ("Could not read Ogg file from memory");

                vorbisInfo = ov_info(&mOggStream, -1);
                if (vorbisInfo->channels == 1)
                    mFormat = AL_FORMAT_MONO16;
                else
                    mFormat = AL_FORMAT_STEREO16;
                mFrequency = vorbisInfo->rate;  
                mSize = mData->getSize();
                mBits = 16;
                mLoop = false;
                mTime = ov_time_total(&mOggStream, -1);
                ov_clear(&mOggStream);
                delete vorbisInfo;
                
                break;
            case Wave:
                alutLoadWAVMemory((ALbyte*)mData->getPtr(), &mFormat, &mWAVData,
                     &mSize, &mFrequency, &mLoop);
                check();
                
                break;
            default:
                // Nicht erlaubt;
                delete mData;
                mData = 0;
                mIsLoaded = false;
                return;
        }
    }
 
}

/** Entlaedt die Soundquelle und setzt die Werte.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::unload()
{
    if (mIsLoaded)
    {
        stop();
        empty();
        // Fehler zuruecksetzen.
        alGetError();
    }
    // Aufraeumen abh. von den Sounddaten
    switch(mSoundDataType)
    {
        case OggVorbis:
            ov_clear(&mOggStream);
            
            break;
        case Wave:
            alutUnloadWAV(mFormat, mWAVData, mSize, mFrequency);
            check();
            
            break;
    }
    
    
    if (mData != 0)
    {
        delete mData;
    }
    this->mIsLoaded = false;
}

/**
 * @author JoSch
 * @date 09-17-2004
 */
void SoundResource::fadeIn(unsigned int msec)
{
    if (msec != 0)
    {
        mFadeInThread.setDauer(msec);
        mFadeInThread.start();
    }
}
/**
 * @author JoSch
 * @date 09-15-2004
 */
void SoundResource::fadeOut(unsigned int msec)
{
    if (msec != 0 )
    {
        mFadeOutThread.setDauer(msec);
        mFadeOutThread.start();
    }
}
/**
 * @author JoSch
 * @date 09-15-2004
 */
void SoundResource::play(unsigned int msec) throw (RuntimeException)
{
    if (getState() != AL_PLAYING)
    {
        // Abspielen.
        alSourcePlay(mSource);
        check();
        fadeIn(msec);
    }
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::pause() throw (RuntimeException)
{
    alSourcePause(mSource);
    check();
}

/**
 * Abspielen stoppen.
 * @author JoSch
 * @date 07-27-2004
 */
void SoundResource::stop (unsigned int msec) throw (RuntimeException)
{
/* TODO    if (alIsSource(mSource) && mPlaying)
    {
        SndResource::stop(msec);
        mMusicThread.mFadeOut = msec;
    } */
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::rewind() throw (RuntimeException)
{
    alSourceRewind(mSource);
    check();
}

/**
 * @return Der momentane Zustand der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
const ALenum SoundResource::getState() const throw (RuntimeException)
{
    ALenum result;
    alGetSourcei(mSource, AL_SOURCE_STATE, &result);    
    check();
    return result;
}

/**
 * @return Ueberprueft, ob Fehler aufgetreten ist, ansonsten Exception.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::check() const throw (RuntimeException)
{
    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        Throw(RuntimeException, string((char*)alGetString(error)));
    }
}


/**
 * Bestimmt, ob der Sound spielen sollte, d.h.
 * ob der Streamthread laufen sollte.
 * Dies hat nicht mit getState von OpenAL zu tun.
 * @return True, wenn der Sound spielen sollte.
 * @author JoSch
 * @date 10-11-2004
 */
const bool SoundResource::isPlaying() const
{
    return true; // TODO
}



/**
 */
SoundResource::FadeThread::FadeThread(SoundResource *that, bool fadeIn) :
    Thread(),
    EventSource(),
    EventCaster<SoundEvent>(),
    that(that),
    mFadeIn(fadeIn)
{
}

/**
 * Fuehrt das Fade aus
 * @author JoSch
 * @date 09-16-2004
 */
void SoundResource::FadeThread::run()
{
    try {
        if (mFadeIn)
        {
            if (mDauer != 0)
            {
                // Alte Lautstaerke speichern.
                ALfloat gain = that->getGain();
                
                that->setGain(0.0f);
                for(unsigned int time = 0; time <= mDauer; time += 10)
                {
                    
                    // Warten
                    msleep(10);
                    ALfloat newgain = calculateFadeIn(time);
                    // Lautstaerke hochsetzen.
                    that->setGain((newgain > gain)?gain:newgain);
                }
                that->setGain(gain);
                that->check();
            }
        } else {
            if (mDauer != 0)
            {
                
                // Alte Lautstaerke speichern.
                ALfloat gain = that->getGain();
                
                for (unsigned int time = 0; time <= mDauer; time += 10)
                {
                    // Warten
                    msleep(10);
                    ALfloat newgain = calculateFadeOut(time);
                    // Lautstaerke hochsetzen.
                    that->setGain((newgain > gain)?gain:newgain);
                }
                that->setGain(0.0f);
                that->check();
                
            }
            // Stoppen.
            alSourceStop(that->mSource);
            that->check();
        }
    } catch(...)
    {
    }   
}

/**
 * @param duration. Die Zeit in msek., die der Fade In insgesamt dauern soll.
 * @param time. Die Zeit in msek., wo der Fade In gerade ist
 * @param gain. Der aktuelle Lautstaerkewert, vom dem ausgegangen wird.
 * @author JoSch
 * @09-15-2004
 */
ALfloat SoundResource::FadeThread::calculateFadeIn(unsigned RL_LONGLONG time)
{
    try {
        ALfloat x = (time * 1.0f) / getDauer();
        return (1.0f - exp(-x)) * getGain();
    } catch(...)
    {
        return getGain();
    }
}

/**
 * @param duration. Die Zeit in msek., die der Fade Out insgesamt dauern soll.
 * @param time. Die Zeit in msek., wo der Fade Out gerade ist
 * @param gain. Der aktuelle Lautstaerkewert, vom dem ausgegangen wird.
 * @author JoSch
 * @09-15-2004
 */
ALfloat SoundResource::FadeThread::calculateFadeOut(unsigned RL_LONGLONG time)
{
    try {
        ALfloat x = (time * 2.0f) / getDauer();
        return exp(-x) * getGain();
    } catch(...)
    {
        return (ALfloat)0.0f;
    }
}

/**
 * @param dauer Die Dauer des Fades in Millisekunden.
 * @author JoSch
 * @10-13-2004
 */
void SoundResource::FadeThread::setDauer(const unsigned long int dauer)
{
    mDauerMutex.lock();
    mDauer = dauer;
    mDauerMutex.unlock();
}

/**
 * @return Die Dauer des Fades in Millisekunden.
 * @author JoSch
 * @10-13-2004
 */
const unsigned long int SoundResource::FadeThread::getDauer() const
{
    mDauerMutex.lock();
    unsigned long int dauer = mDauer;
    mDauerMutex.unlock();
    return dauer;
}

/**
 * @param gain Die Ausgangslautstaerke des Fades.
 * @author JoSch
 * @10-13-2004
 */
void SoundResource::FadeThread::setGain(const ALfloat gain)
{
    mGainMutex.lock();
    mGain = gain;
    mGainMutex.unlock();
}

/**
 * @return Die Dauer des Fades in Millisekunden.
 * @author JoSch
 * @10-13-2004
 */
const ALfloat SoundResource::FadeThread::getGain() const
{
    mGainMutex.lock();
    ALfloat gain = mGain;
    mGainMutex.unlock();
    return gain;
}


/**
 * Das Streamen passiert in diesem Thread.
 * @author JoSch
 * @date 10-11-2004
 */
SoundResource::StreamThread::StreamThread(SoundResource *that) :
    Thread(),
    EventSource(),
    EventCaster<SoundEvent>(),
    that(that)
{
}

/**
 * Fuehrt das Streamen aus
 * @author JoSch
 * @date 10-11-2004
 */
void SoundResource::StreamThread::run()
{
}

/**
 ******************************************************************************
 * Der folgende Code wurde dem Beispielcode von Jesse Maurais und Spree Tree
 * auf DevMaster.net entliehen und den hiesigen Bedürfnissen angepasst.
 * Der Originalcode kann von hier geladen werden:
 *  http://www.devmaster.net/articles.php?catID=6
 ******************************************************************************/


size_t SoundResource::VorbisRead(void *ptr, size_t byteSize, size_t sizeToRead,
                                 void *datasource)
{
    size_t spaceToEOF;      // How much more we can read till we hit the EOF marker
    size_t actualSizeToRead;        // How much data we are actually going to read from memory
    SOggFile *vorbisData;   // Our vorbis data, for the typecast

    // Get the data in the right format
    vorbisData = (SOggFile *) datasource;

    // Calculate how much we need to read.  This can be sizeToRead*byteSize or less depending on how near the EOF marker we are
    spaceToEOF = vorbisData->mDataSize - vorbisData->mDataRead;
    if ((sizeToRead * byteSize) < spaceToEOF)
        actualSizeToRead = (sizeToRead * byteSize);
    else
        actualSizeToRead = spaceToEOF;

    // A simple copy of the data from memory to the datastruct that the vorbis libs will use
    if (actualSizeToRead) {
        // Copy the data from the start of the file PLUS how much we have already read in
        memcpy (ptr, (char *) vorbisData->mDataPtr + vorbisData->mDataRead,
                actualSizeToRead);
        // Increase by how much we have read by
        vorbisData->mDataRead += (actualSizeToRead);
    }

    // Return how much we read (in the same way fread would)
    return actualSizeToRead;
}

//---------------------------------------------------------------------------------
// Function : VorbisSeek
// Purpose : Callback for the Vorbis seek function
// Info     : 
//---------------------------------------------------------------------------------
int SoundResource::VorbisSeek (void *datasource
                               /*this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct */
                               , ogg_int64_t offset /*offset from the point we wish to seek to */
                               , int whence /*where we want to seek to */ )
{
    size_t spaceToEOF;      // How much more we can read till we hit the EOF marker
    ogg_int64_t actualOffset;       // How much we can actually offset it by
    SOggFile *vorbisData;   // The data we passed in (for the typecast)

    // Get the data in the right format
    vorbisData = (SOggFile *) datasource;

    // Goto where we wish to seek to
    switch (whence) {
        case SEEK_SET:     // Seek to the start of the data file
            // Make sure we are not going to the end of the file
            if (vorbisData->mDataSize >= offset)
                actualOffset = offset;
            else
                actualOffset = vorbisData->mDataSize;
            // Set where we now are
            vorbisData->mDataRead = (int) actualOffset;
            break;
        case SEEK_CUR:     // Seek from where we are
            // Make sure we dont go past the end
            spaceToEOF = vorbisData->mDataSize - vorbisData->mDataRead;
            if (offset < spaceToEOF)
                actualOffset = (offset);
            else
                actualOffset = spaceToEOF;
            // Seek from our currrent location
            vorbisData->mDataRead += actualOffset;
            break;
        case SEEK_END:     // Seek from the end of the file
            vorbisData->mDataRead = vorbisData->mDataSize + 1;
            break;
        default:
            printf ("*** ERROR *** Unknown seek command in VorbisSeek\n");
            break;
    };

    return 0;
}

//---------------------------------------------------------------------------------
// Function    : VorbisClose
// Purpose    : Callback for the Vorbis close function
// Info        : 
//---------------------------------------------------------------------------------
int SoundResource::VorbisClose (void *datasource
                                /*this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct */
    )
{
    // This file is called when we call ov_close.  If we wanted, we could free our memory here, but
    // in this case, we will free the memory in the main body of the program, so dont do anything
    return 1;
}

//---------------------------------------------------------------------------------
// Function    : VorbisTell
// Purpose : Classback for the Vorbis tell function
// Info        : 
//---------------------------------------------------------------------------------
long SoundResource::VorbisTell (void *datasource
                                /*this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct */
    )
{
    SOggFile *vorbisData;

    // Get the data in the right format
    vorbisData = (SOggFile *) datasource;

    // We just want to tell the vorbis libs how much we have read so far
    return vorbisData->mDataRead;
}
/************************************************************************************************************************
   End of Vorbis callback functions
************************************************************************************************************************/


void SoundResource::open (unsigned char *data, unsigned int size)
{
/*    mOggMemoryFile.mDataPtr = (char*)data; //new char[size];
    memcpy (mOggMemoryFile.mDataPtr, data, size);
    mOggMemoryFile.mDataSize = size;
    mOggMemoryFile.mDataRead = 0;


    // Open the file from memory.  We need to pass it a pointer to our data (in this case our SOggFile structure),
    // a pointer to our ogg stream (which the vorbis libs will fill up for us), and our callbacks
    if (ov_open_callbacks
        (&mOggMemoryFile, &mOggStream, NULL, 0, SoundResource::mVorbisCallbacks) != 0)
        throw string ("Could not read Ogg file from memory");



/************************************************************************************************************************
      From now on, the code is exactly the same as in Jesse Maurais's lesson 8
   ************************************************************************************************************************/

/*    mVorbisInfo = ov_info (&mOggStream, -1);
    mVorbisComment = ov_comment (&mOggStream, -1);

    if (mVorbisInfo->channels == 1)
        mFormat = AL_FORMAT_MONO16;
    else
        mFormat = AL_FORMAT_STEREO16; */

/*    for(int i = 0; i < mBufferCount; i++)
    {
        ALint i;
        alGetBufferi(mBuffers[i], AL_FREQUENCY, &i);
        cerr << "Rate " << i << endl;
        alBufferi_LOKI(mBuffers[i], AL_FREQUENCY, mVorbisInfo->rate);
        alGetBufferi(mBuffers[i], AL_FREQUENCY, &i);
        cerr << "Rate " << i << endl;
    }  */
/*    alSourcef(mSource, AL_ROLLOFF_FACTOR, 0.0);
    alSourcei(mSource, AL_SOURCE_RELATIVE, AL_TRUE); */
}

void SoundResource::release ()
{
    check ();
    if (alIsSource (mSource)) {
        alSourceStop (mSource);
        empty ();
    }

// TODO    ov_clear (&mOggStream);

// TODO     delete[]mOggMemoryFile.mDataPtr;
// TODO    mOggMemoryFile.mDataPtr = NULL;
}

void SoundResource::display ()
{
/*    cout << "version         " << mVorbisInfo->version << "\n"
        << "channels        " << mVorbisInfo->channels << "\n"
        << "rate (hz)       " << mVorbisInfo->rate << "\n"
        << "bitrate upper   " << mVorbisInfo->bitrate_upper << "\n"
        << "bitrate nominal " << mVorbisInfo->bitrate_nominal << "\n"
        << "bitrate lower   " << mVorbisInfo->bitrate_lower << "\n"
        << "bitrate window  " << mVorbisInfo->bitrate_window << "\n"
        << "\n" << "vendor " << mVorbisComment->vendor << "\n";

    for (int i = 0; i < mVorbisComment->comments; i++)
        cout << "   " << mVorbisComment->user_comments[i] << "\n";

     cout << endl; */
}




bool SoundResource::playback ()
{

    if (playing ())
        return true;
    
    for (int i = 0; i < mBufferCount; i++)
    {
        if (!stream (mBuffers[i]))
        {
            return false;
        }
    }

    alSourceQueueBuffers (mSource, mBufferCount, mBuffers);
    alSourcePlay (mSource);

    return true;
}


bool SoundResource::playing ()
{
    ALenum state;

    alGetSourcei (mSource, AL_SOURCE_STATE, &state);

    return (state == AL_PLAYING);
}

bool SoundResource::update ()
{
    int processed;
    bool active = true;

    alGetSourcei (mSource, AL_BUFFERS_PROCESSED, &processed);

    while (processed--) {
        ALuint buffer;

        alSourceUnqueueBuffers (mSource, 1, &buffer);
        check ();

        active = stream (buffer);

        alSourceQueueBuffers (mSource, 1, &buffer);
        check ();
    }

    return active;
}




bool SoundResource::stream (ALuint buffer)
{
    char pcm[BUFFER_SIZE];
    int size = 0;
    int section;
    int result;
    

    while (size < BUFFER_SIZE) {
/*        result =
            ov_read(&mOggStream, pcm + size, BUFFER_SIZE - size, 0, 2, 1,
                     &section);

        if (result > 0) {
            size += result;
        }
        else if (result < 0) {
            throw errorString (result);
        }
        else {
            break;
        } */
       
    }

    if (size < BUFFER_SIZE) {
        return false;
    }

    alBufferData(buffer, mFormat, pcm, mBits, mFrequency);
    check ();

    return true; 
}

void SoundResource::empty ()
{
    int queued;

    alGetSourcei (mSource, AL_BUFFERS_QUEUED, &queued);

    while (queued--) {
        ALuint buffer;

        alSourceUnqueueBuffers (mSource, 1, &buffer);
        check ();
    }
}




void SoundResource::check ()
{
    int error = alGetError ();

    if (error != AL_NO_ERROR) {
        cerr << alGetString (error) << endl;
        Throw (RuntimeException, (const char *) alGetString (error));
    }
}



String SoundResource::errorString (int code)
{
    switch (code) {
        case OV_EREAD:
            return string ("Read from media.");
        case OV_ENOTVORBIS:
            return string ("Not Vorbis data.");
        case OV_EVERSION:
            return string ("Vorbis version mismatch.");
        case OV_EBADHEADER:
            return string ("Invalid Vorbis header.");
        case OV_EFAULT:
            return
                string
                ("Internal logic fault (bug or heap/stack corruption.");
        default:
            return string ("Unknown Ogg error.");
    }
}


} /* End of Namespace */

