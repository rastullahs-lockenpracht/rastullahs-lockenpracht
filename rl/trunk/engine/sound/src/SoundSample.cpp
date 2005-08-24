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
#include "SoundSample.h"
#include "SoundManager.h"
#include "SoundResource.h"
#include "fmod_errors.h"


using namespace Ogre;
using namespace boost;

namespace rl {
 
String SoundSample::msMovableType = "SoundSample";

/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
SoundSample::SoundSample(const String &name):
    Sound(name),
    mSample(0)
{
}
 
/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
SoundSample::SoundSample(const SoundResourcePtr &soundres):
    Sound(soundres),
    mSample(0)
{
}

/**
 * @author JoSch
 * @date 07-04-2005
 */
SoundSample::~SoundSample()
{
    unload();
}


/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& SoundSample::getMovableType() const
{
    return msMovableType;
}

/**
 * @author JoSch
 * @date 07-12-2005
 */
void SoundSample::load() throw (RuntimeException)
{
    getSoundResource()->load();
    DataStreamPtr stream = getSoundResource()->getDataStream();
    stream->seek(0);
    int len = stream->size();
    char *data = new char[len];
    stream->read(data, len);
    unsigned int mode = FSOUND_LOADMEMORY;
    if (is3d())
    {
        mode |= FSOUND_HW3D | FSOUND_FORCEMONO;
    } else {
        mode |= FSOUND_HW2D;
    }
    mSample = FSOUND_Sample_Load(FSOUND_FREE, data, mode,
        0, len);
    if (mSample == 0)
    {
        mode |= FSOUND_FORCEMONO;
        mSample = FSOUND_Sample_Load(FSOUND_FREE, data, mode,
            0, len);
    }
}

/**
 * @author JoSch
 * @date 07-22-2005
 */
void SoundSample::unload() throw (RuntimeException)
{
    FSOUND_Sample_Free(getSample());
}

/**
 * @return TRUE wenn der Sound gueltig ist.
 * @author JoSch
 * @date 07-12-2005
 */
bool SoundSample::isValid() const throw (RuntimeException)
{
    return (getSample() != 0);
}

/**
 * @return Das Sample zurueckgeben.
 * @author JoSch
 * @date 07-12-2005
 */
FSOUND_SAMPLE *SoundSample::getSample() const
{
    return mSample;
}

/**
 * @param sample Das Sample setzen.
 * @author JoSch
 * @date 07-12-2005
 */
void SoundSample::setSample(FSOUND_SAMPLE *sample)
{
    mSample = sample;
}

/**
 * @return Der erzeugte Channel
 * @author JoSch
 * @date 08-08-2005
 */
int SoundSample::createChannel() throw (RuntimeException)
{
    int channel = FSOUND_PlaySoundEx(FSOUND_FREE, getSample(), 0, true);
    return channel;
}

void SoundSamplePtr::destroy()
{
    SharedPtr<SoundSample>::destroy();
}

} // Namespace
