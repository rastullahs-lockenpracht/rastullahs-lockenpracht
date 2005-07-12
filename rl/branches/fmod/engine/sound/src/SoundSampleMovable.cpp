/* SoundSampleMovable.cpp - Diese Klassse kapselt einen Soundstream.
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
#include "SoundSampleMovable.h"
#include "SoundManager.h"
#include "SoundResource.h"



using namespace Ogre;
using namespace boost;

namespace rl {
 
String SoundSampleMovable::msMovableType = "SoundSampleMovable";

/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
SoundSampleMovable::SoundSampleMovable(const String &name):
    SoundMovable(name),
    mSample(0)
{
}
 
/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
SoundSampleMovable::SoundSampleMovable(const SoundResourcePtr &soundres):
    SoundMovable(soundres),
    mSample(0)
{
}

/**
 * @author JoSch
 * @date 07-04-2005
 */
SoundSampleMovable::~SoundSampleMovable()
{
    if (isValid())
    {
        stop();
        FSOUND_Sample_Free(getSample());
    }
}


/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& SoundSampleMovable::getMovableType() const
{
    return msMovableType;
}

/**
 * @author JoSch
 * @date 09-15-2004
 */
void SoundSampleMovable::play() throw (RuntimeException)
{
    if (getSample() != 0)
    {
        FSOUND_PlaySoundEx(FSOUND_FREE, getSample(), 0, true);
    }
}

/**
 * @author JoSch
 * @date 07-12-2005
 */
void SoundSampleMovable::load() throw (RuntimeException)
{
    getSoundResource()->load();
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
    mSample = FSOUND_Sample_Load(FSOUND_FREE, data, mode,
        0, len);
}

/**
 * @return TRUE wenn der Sound gueltig ist.
 * @author JoSch
 * @date 07-12-2005
 */
bool SoundSampleMovable::isValid() const throw (RuntimeException)
{
    return (getChannel() < 0) && (getSample() != 0);
}

/**
 * @return Das Sample zurueckgeben.
 * @author JoSch
 * @date 07-12-2005
 */
FSOUND_SAMPLE *SoundSampleMovable::getSample() const
{
    return mSample;
}

/**
 * @param sample Das Sample setzen.
 * @author JoSch
 * @date 07-12-2005
 */
void SoundSampleMovable::setSample(FSOUND_SAMPLE *sample)
{
    mSample = sample;
}


void SoundSampleMovablePtr::destroy()
{
    SharedPtr<SoundSampleMovable>::destroy();
}

} // Namespace
