/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Clarified Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Clarified Artistic License for more details.
*
*  You should have received a copy of the Clarified Artistic License
*  along with this program; if not you can get it here
*  http://www.jpaulmorrison.com/fbp/artistic2.htm.
*/
#include "SoundStream.h"
#include "SoundManager.h"

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
}
 
/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
SoundStream::SoundStream(const SoundResourcePtr &soundres):
    Sound(soundres)
{
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
    if (isLooping())
    {
        mode |= FSOUND_LOOP_NORMAL;
    } else {
        mode |= FSOUND_LOOP_OFF;
    }
    mStream = FSOUND_Stream_Open(getName().c_str(), mode, 0, 0);
    if (mStream == 0)
    {
        // Stereo auf 3D?
        mode |= FSOUND_FORCEMONO;
        mStream = FSOUND_Stream_Open(getName().c_str(), mode, 0, 0);
    }
    if (mStream != 0)
    {
        FSOUND_Stream_SetSyncCallback(getStream(), 
            (FSOUND_STREAMCALLBACK)SoundStream::streamSyncCallback, this);
        FSOUND_Stream_SetEndCallback(getStream(),
            (FSOUND_STREAMCALLBACK)SoundStream::streamEndCallback, this);
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
 * @return Wird ignoriert
 * @author JoSch
 * @date 24-08-2005
 */
signed char SoundStream::streamEndCallback(FSOUND_STREAM *stream,
    void *buf, int len, void *userdata)
{
    SoundStream *that = static_cast<SoundStream*>(userdata);
    if (that != 0)
    {
        RlAssert(that->getStream() == stream, "Stream-Daten stimmen nicht überein");
    }
    return 0;
}

/**
 * @return Wird ignoriert
 * @author JoSch
 * @date 24-08-2005
 */
signed char SoundStream::streamSyncCallback(FSOUND_STREAM *stream,
    void *buf, int len, void *userdata)
{
    SoundStream *that = static_cast<SoundStream*>(userdata);
    if (that != 0)
    {
        RlAssert(that->getStream() == stream, "Stream-Daten stimmen nicht überein");
    }
    return 0;
}

void SoundStreamPtr::destroy()
{
    SharedPtr<SoundStream>::destroy();
}

} // Namespace
