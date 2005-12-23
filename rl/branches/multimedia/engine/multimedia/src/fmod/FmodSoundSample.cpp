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
#ifdef WITH_FMOD
#include "FmodSoundSample.h"
#include "FmodSoundChannel.h"
#include "SoundManager.h"
#include "SoundResource.h"

using namespace Ogre;
using namespace boost;

namespace rl {
 
String FmodSoundSample::msMovableType = "FmodSoundSample";

/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
FmodSoundSample::FmodSoundSample(const String &name):
    Sound(name),
    mSample(0)
{
}
 
/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
FmodSoundSample::FmodSoundSample(const SoundResourcePtr &soundres):
    Sound(soundres),
    mSample(0)
{
}

/**
 * @author JoSch
 * @date 07-04-2005
 */
FmodSoundSample::~FmodSoundSample()
{
    unload();
}


/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& FmodSoundSample::getMovableType() const
{
    return msMovableType;
}

/**
 * @author JoSch
 * @date 07-12-2005
 */
void FmodSoundSample::load() throw (RuntimeException)
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
    if (isLooping())
    {
        mode |= FSOUND_LOOP_NORMAL;
    } else {
        mode |= FSOUND_LOOP_OFF;
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
void FmodSoundSample::unload() throw (RuntimeException)
{
    FSOUND_Sample_Free(getSample());
}

/**
 * @return TRUE wenn der Sound gueltig ist.
 * @author JoSch
 * @date 07-12-2005
 */
bool FmodSoundSample::isValid() const throw (RuntimeException)
{
    return (getSample() != 0);
}

/**
 * @return Das Sample zurueckgeben.
 * @author JoSch
 * @date 07-12-2005
 */
FSOUND_SAMPLE *FmodSoundSample::getSample() const
{
    return mSample;
}

/**
 * @param sample Das Sample setzen.
 * @author JoSch
 * @date 07-12-2005
 */
void FmodSoundSample::setSample(FSOUND_SAMPLE *sample)
{
    mSample = sample;
}

/**
 * @return Der erzeugte Channel
 * @author JoSch
 * @date 08-08-2005
 */
SoundChannel *FmodSoundSample::createChannel() throw (RuntimeException)
{
    int channel = FSOUND_PlaySoundEx(FSOUND_FREE, getSample(), 0, true);
    FmodSoundChannel *fmodchannel = new FmodSoundChannel(this, getName());
    fmodchannel->setChannel(channel);
    return fmodchannel; 
}

void FmodSoundSamplePtr::destroy()
{
    SharedPtr<FmodSoundSample>::destroy();
}

} // Namespace
#endif // WITH_FMOD
