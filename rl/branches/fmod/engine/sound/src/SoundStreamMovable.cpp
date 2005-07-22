/* SoundStreamMovable.cpp - Diese Klassse kapselt einen Soundstream.
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
#include "SoundStreamMovable.h"
#include "SoundManager.h"


using namespace Ogre;
using namespace boost;

namespace rl {
 
String SoundStreamMovable::msMovableType = "SoundStreamMovable";

/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
SoundStreamMovable::SoundStreamMovable(const String &name):
    SoundMovable(name)
{
    /// Ein paar Standardwerte setzen
    setGain(255);
    setPosition(Vector3(0.0, 0.0, 0.0));
    setVelocity(Vector3(0.0, 0.0, 0.0));
    setDirection(Vector3(0.0, 0.0, 0.0));
}
 
/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
SoundStreamMovable::SoundStreamMovable(const SoundResourcePtr &soundres):
    SoundMovable(soundres)
{
    /// Ein paar Standardwerte setzen
    setGain(255);
    setPosition(Vector3(0.0, 0.0, 0.0));
    setVelocity(Vector3(0.0, 0.0, 0.0));
    setDirection(Vector3(0.0, 0.0, 0.0));
}

/**
 * @author JoSch
 * @date 07-04-2005
 */
SoundStreamMovable::~SoundStreamMovable()
{
    if (isValid())
    {
        stop();
    }
    unload();
}


/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& SoundStreamMovable::getMovableType() const
{
    return msMovableType;
}


/**
 * @author JoSch
 * @date 07-12-2005
 */
void SoundStreamMovable::load() throw (RuntimeException)
{
    getSoundResource().getPointer()->load();
    DataStreamPtr stream = getSoundResource()->getDataStream();
    int len = stream->size();
    char *data = new char[len];
    stream->read(data, len);
    
    unsigned int mode = FSOUND_LOADMEMORY;
    if (is3d())
    {
        mode |= FSOUND_HW3D;
    } else {
        mode |= FSOUND_HW2D;
    } 
    mStream = FSOUND_Stream_Open(data, mode, 0, len);
    if (mStream == 0)
    {
        // Stereo auf 3D?
        set3d(false);
        mode = FSOUND_LOADMEMORY | FSOUND_HW2D;
        mStream = FSOUND_Stream_Open(data, mode, 0, len);
    }
}

/**
 * @author JoSch
 * @date 07-22-2005
 */
void SoundStreamMovable::unload() throw (RuntimeException)
{
    if (isPlaying())
    {
        stop();
    }
    FSOUND_Stream_Close(getStream());
    getSoundResource()->unload();
}

/**
 * @author JoSch
 * @date 09-15-2004
 */
void SoundStreamMovable::play() throw (RuntimeException)
{
    if (getStream() == 0)
    {
        load();
    }
    if (getStream() != 0)
    {
        setChannel(FSOUND_Stream_Play(FSOUND_FREE, getStream()));
        if (getChannel() < 0)
        {
            int mode = FSOUND_Stream_GetMode(getStream());
            unload();
            set3d(false); // Wahrscheinlich ein Stereosound
            load();
            setChannel(FSOUND_Stream_Play(FSOUND_FREE, getStream()));
            if (getChannel() < 0) // Jetzt weiss cih auch nicht weiter.
            {
                Throw(RuntimeException, "Sound konnte nicht gespielt werden");        
            }
        }
    }
}

/**
 * @return TRUE wenn der Sound unterbrochen wurde.
 * @author JoSch
 * @date 07-12-2005
 */
bool SoundStreamMovable::isValid() const throw (RuntimeException)
{
    return (getChannel() >= 0) && (getStream() != 0);
}

/**
 * @return Den Stream zurueckgeben.
 * @author JoSch
 * @date 07-12-2005
 */
FSOUND_STREAM *SoundStreamMovable::getStream() const
{
    return mStream;
}

/**
 * @param stream Den Stream setzen.
 * @author JoSch
 * @date 07-12-2005
 */
void SoundStreamMovable::setStream(FSOUND_STREAM *stream)
{
    mStream = stream;
}

void SoundStreamMovablePtr::destroy()
{
    SharedPtr<SoundStreamMovable>::destroy();
}

} // Namespace
