#include "MusicResource.h"
#include <string>
#include <iostream>
#include "SoundManager.h"
extern "C"
{
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
}
#include <boost/thread/xtime.hpp>

using namespace std;
using namespace Ogre;
using namespace boost;

namespace rl
{

/// Die Callback-Struktur, die Vorbisfile braucht
ov_callbacks rl::MusicResource::mVorbisCallbacks = {
    &rl::MusicResource::VorbisRead,
    &rl::MusicResource::VorbisSeek,
    &rl::MusicResource::VorbisClose,
    &rl::MusicResource::VorbisTell
};

size_t MusicResource::VorbisRead (void *ptr, size_t byteSize, size_t sizeToRead     /* How much we can read */
                                  , void *datasource)
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
int MusicResource::VorbisSeek (void *datasource
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
int MusicResource::VorbisClose (void *datasource
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
long MusicResource::VorbisTell (void *datasource
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

/**
 * @param name. Der Resourcenname.
 * @author JoSch
 * @date 07-12-2004
 */
MusicResource::MusicResource (const String & name)
    : SndResource (name),
      mThread(0),
      mMusicThread(this),
      mPlaying(false)
{
    mBufferCount = 4;
    mBuffers = new ALuint[mBufferCount];
    alGenBuffers(mBufferCount, mBuffers);
}

/**
 * @author
 * @date 07-12-2004
 */
MusicResource::~MusicResource ()
{
    delete mThread;
}

/** Laedt die Ogg-Datei.
 * @author JoSch
 * @date 07-12-2004
 */
void MusicResource::load ()
{
    DataChunk dc;
    SoundManager::_findCommonResourceData (getName (), dc);
    mData = dc.getPtr ();
    mDataSize = dc.getSize ();
    mIsLoaded = true;
    mPlaying = false;
}


MusicResource::MusicThread::MusicThread(MusicResource *that)
    : that(that)
{
}

/**
 * Spielt die Musik im Thread ab. Das ist noetig, weil Vorbis-Dateien
 * nicht komplett, ausgepackt werden koennen.
 * @author JoSch
 * @date 07-15-2004
 */
void MusicResource::MusicThread::operator()()
{
    xtime xt;

    try {
        that->open(that->mData, that->mDataSize);
        
        if(!that->playback())
            Throw(RuntimeException, "Ogg refused to play.");
                        
        while(that->update() && that->mPlaying)
        {
            if(!that->playing())
            {
                if(!that->playback())
                    Throw(RuntimeException, "Ogg abruptly stopped.");
                else
                    cerr << "Ogg stream was interrupted.\n";
            }
    	    // Let's wait.
    	    // boost::Thread::sleep(50000);
            xtime_get(&xt, TIME_UTC);
            xt.nsec += 1 * 1000;
            that->mThread->sleep(xt); 
        }
        that->stop();
        that->release();  
    } catch (...) {
    }
    delete that->mThread;
    that->mThread = 0; 
    that->mPlaying = false;
}

/**
 * Ist das Musikstueck noch am Spielen?
 * @return TRUE, wenn das Stueck noch spielt
 * @author JoSch
 * @date 07-16-2004
 */
const bool MusicResource::isPlaying () const
{
    return mPlaying;
}

/**
 * Abspielen starten.
 * @author JoSch
 * @date 07-16-2004
 */
void MusicResource::play() throw (RuntimeException)
{
    if (!mIsLoaded) {
        load();
    }
    mPlaying = mIsLoaded;
    if (mPlaying)
    {
        mThread = new thread(mMusicThread);
    }
}



void MusicResource::open (unsigned char *data, unsigned int size)
{
    mOggMemoryFile.mDataPtr = (char*)data; //new char[size];
    memcpy (mOggMemoryFile.mDataPtr, data, size);
    mOggMemoryFile.mDataSize = size;
    mOggMemoryFile.mDataRead = 0;


    // Open the file from memory.  We need to pass it a pointer to our data (in this case our SOggFile structure),
    // a pointer to our ogg stream (which the vorbis libs will fill up for us), and our callbacks
    if (ov_open_callbacks
        (&mOggMemoryFile, &mOggStream, NULL, 0, MusicResource::mVorbisCallbacks) != 0)
        throw string ("Could not read Ogg file from memory");



/************************************************************************************************************************
      From now on, the code is exactly the same as in Jesse Maurais's lesson 8
   ************************************************************************************************************************/

    mVorbisInfo = ov_info (&mOggStream, -1);
    mVorbisComment = ov_comment (&mOggStream, -1);

    if (mVorbisInfo->channels == 1)
        mFormat = AL_FORMAT_MONO16;
    else
        mFormat = AL_FORMAT_STEREO16;

    alSourcef (mSource, AL_ROLLOFF_FACTOR, 0.0);
    alSourcei (mSource, AL_SOURCE_RELATIVE, AL_TRUE);
}

/**
 * Abspielen stoppen.
 * @author JoSch
 * @date 07-27-2004
 */
void MusicResource::stop () throw (RuntimeException)
{
    if (alIsSource(mSource))
    {
        alSourceStop (mSource);
        check();
        empty ();
    }
}


void MusicResource::release ()
{
    check ();
    if (alIsSource (mSource)) {
        alSourceStop (mSource);
        empty ();
    }

    ov_clear (&mOggStream);

    delete[]mOggMemoryFile.mDataPtr;
    mOggMemoryFile.mDataPtr = NULL;
}




void MusicResource::display ()
{
    cout << "version         " << mVorbisInfo->version << "\n"
        << "channels        " << mVorbisInfo->channels << "\n"
        << "rate (hz)       " << mVorbisInfo->rate << "\n"
        << "bitrate upper   " << mVorbisInfo->bitrate_upper << "\n"
        << "bitrate nominal " << mVorbisInfo->bitrate_nominal << "\n"
        << "bitrate lower   " << mVorbisInfo->bitrate_lower << "\n"
        << "bitrate window  " << mVorbisInfo->bitrate_window << "\n"
        << "\n" << "vendor " << mVorbisComment->vendor << "\n";

    for (int i = 0; i < mVorbisComment->comments; i++)
        cout << "   " << mVorbisComment->user_comments[i] << "\n";

    cout << endl;
}




bool MusicResource::playback ()
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


bool MusicResource::playing ()
{
    ALenum state;

    alGetSourcei (mSource, AL_SOURCE_STATE, &state);

    return (state == AL_PLAYING);
}

bool MusicResource::update ()
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




bool MusicResource::stream (ALuint buffer)
{
    char pcm[BUFFER_SIZE];
    int size = 0;
    int section;
    int result;
    

    while (size < BUFFER_SIZE) {
        result =
            ov_read (&mOggStream, pcm + size, BUFFER_SIZE - size, 0, 2, 1,
                     &section);

        if (result > 0) {
            size += result;
        }
        else if (result < 0) {
            throw errorString (result);
        }
        else {
            break;
        }
       
    }

    if (size < BUFFER_SIZE) {
        return false;
    }

    alBufferData (buffer, mFormat, pcm, size, mVorbisInfo->rate);
    check ();

    return true;
}




void MusicResource::empty ()
{
    int queued;

    alGetSourcei (mSource, AL_BUFFERS_QUEUED, &queued);

    while (queued--) {
        ALuint buffer;

        alSourceUnqueueBuffers (mSource, 1, &buffer);
        check ();
    }
}




void MusicResource::check ()
{
    int error = alGetError ();

    if (error != AL_NO_ERROR) {
        cerr << alGetString (error) << endl;
        Throw (RuntimeException, (const char *) alGetString (error));
    }
}



String MusicResource::errorString (int code)
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



}
