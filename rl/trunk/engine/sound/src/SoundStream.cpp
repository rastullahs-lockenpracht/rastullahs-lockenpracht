/* SoundStream.cpp - Diese Klassse kapselt einen Soundstream.
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
#include "SoundStream.h"
#include "SoundManager.h"
#include "SDL.h"


using namespace Ogre;
using namespace boost;

namespace rl {
 

/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
SoundStream::SoundStream(const String &name):
    Sound(name)
{
    load();
}
 
/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
SoundStream::SoundStream(const SoundResourcePtr &soundres):
    Sound(soundres)
{
    load();
    FSOUND_Stream_SetSyncCallback(getStream(), 
        SoundStream::streamCallback, this);
    FSOUND_Stream_SetEndCallback(getStream(),
        SoundStream::streamCallback, this);
}

/**
 * @author JoSch
 * @date 07-04-2005
 */
SoundStream::~SoundStream()
{
    unload();
}


/**
 * @author JoSch
 * @date 07-12-2005
 */
void SoundStream::load() throw (RuntimeException)
{
    unsigned int mode = 0;
    if (is3d())
    {
        mode |= FSOUND_HW3D | FSOUND_FORCEMONO;
    } else {
        mode |= FSOUND_HW2D;
    } 
    SDL_WM_GrabInput(SDL_GRAB_OFF);
    mStream = FSOUND_Stream_Open(getName().c_str(), mode, 0, 0);
    if (mStream == 0)
    {
        // Stereo auf 3D?
        mode |= FSOUND_FORCEMONO;
        mStream = FSOUND_Stream_Open(getName().c_str(), mode, 0, 0);
    }
}

/**
 * @author JoSch
 * @date 07-22-2005
 */
void SoundStream::unload() throw (RuntimeException)
{
    FSOUND_Stream_Close(getStream());
    getSoundResource()->unload();
}


/**
 * @return TRUE wenn der Sound unterbrochen wurde.
 * @author JoSch
 * @date 07-12-2005
 */
bool SoundStream::isValid() const throw (RuntimeException)
{
    return (getStream() != 0);
}

/**
 * @return Den Stream zurueckgeben.
 * @author JoSch
 * @date 07-12-2005
 */
FSOUND_STREAM *SoundStream::getStream() const
{
    return mStream;
}

/**
 * @param stream Den Stream setzen.
 * @author JoSch
 * @date 07-12-2005
 */
void SoundStream::setStream(FSOUND_STREAM *stream)
{
    mStream = stream;
}

/**
 * @return Der erzeugte Channel
 * @author JoSch
 * @date 08-08-2005
 */
int SoundStream::createChannel() throw (RuntimeException)
{
    return FSOUND_Stream_PlayEx(FSOUND_FREE, getStream(), 0, true);
}

/**
 * @return 
 * @author JoSch
 * @date 24-08-2005
 */
signed char SoundStream::streamCallback(FSOUND_STREAM *stream,
    void *buf, int len, void *userdata)
{
    SoundStream *that = static_cast<SoundStream*>(userdata);
    if (that != 0)
    {
        RlAssert(that->getStream() == stream, "Stream-Daten stimmen nicht überein");
        if (len != 0) // FSOUND_Stream_Create
        {
            return false;
        } else { 
            if (buf == 0) // FSOUND_Stream_SetEndCallback
            {
            } else { // FSOUND_Stream_SetSyncCallback
            }
            return 0;
        }
    }
}

void SoundStreamPtr::destroy()
{
    SharedPtr<SoundStream>::destroy();
}

} // Namespace
