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
#include "Fmod3SoundSample.h"
#include "Fmod3SoundChannel.h"
#include "SoundManager.h"
#include "SoundResource.h"

using namespace Ogre;
using namespace boost;

namespace rl {
 
/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
Fmod3SoundSample::Fmod3SoundSample(const SoundResourcePtr &soundres):
    Fmod3Sound(soundres),
    mSample(0)
{
}

/**
 * @author JoSch
 * @date 07-04-2005
 */
Fmod3SoundSample::~Fmod3SoundSample()
{
    unload();
}


/**
 * @author JoSch
 * @date 07-12-2005
 */
void Fmod3SoundSample::load() throw (RuntimeException)
{
    getSoundResource()->load();
    int len = getSoundResource()->getSize();
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
    
    mSample = FSOUND_Sample_Load(
        FSOUND_FREE, 
        getSoundResource()->getName().c_str(), 
        mode, 
        0, 
        len);
    if (mSample == 0 && !is3d())
    {
        mode |= FSOUND_FORCEMONO;
        mSample = FSOUND_Sample_Load(
            FSOUND_FREE, 
            getSoundResource()->getName().c_str(), 
            mode,
            0, 
            len);
    }

    if( mSample == NULL )
    {
        int err = FSOUND_GetError();
        Throw( RuntimeException, "Fmod Error:" + Ogre::StringConverter::toString(err) + " while loading " + getName() );
    }   
}

/**
 * @author JoSch
 * @date 07-22-2005
 */
void Fmod3SoundSample::unload() throw (RuntimeException)
{
    FSOUND_Sample_Free(getSample());
}

/**
 * @return TRUE wenn der Sound gueltig ist.
 * @author JoSch
 * @date 07-12-2005
 */
bool Fmod3SoundSample::isValid() const throw (RuntimeException)
{
    return (getSample() != 0);
}

/**
 * @return Das Sample zurueckgeben.
 * @author JoSch
 * @date 07-12-2005
 */
FSOUND_SAMPLE *Fmod3SoundSample::getSample() const
{
    return mSample;
}

/**
 * @param sample Das Sample setzen.
 * @author JoSch
 * @date 07-12-2005
 */
void Fmod3SoundSample::setSample(FSOUND_SAMPLE *sample)
{
    mSample = sample;
}

float Fmod3SoundSample::getLength() const
{
	if (mSample == NULL)
	{
		return 0;
	}
	float samples = FSOUND_Sample_GetLength(mSample);
	float frequency = mChannel == NO_CHANNEL ? 44100 : FSOUND_GetFrequency(mChannel);

	return samples / frequency;
}

/**
 * @return Der erzeugte Channel
 * @author JoSch
 * @date 08-08-2005
 */
int Fmod3SoundSample::createChannel() throw (RuntimeException)
{
    mChannel = FSOUND_PlaySoundEx(FSOUND_FREE, getSample(), 0, true);

    if( mChannel == -1 )
    {
        int err = FSOUND_GetError();
        Throw( RuntimeException, "Fmod Error:" + Ogre::StringConverter::toString(err) + " while playing " + getName() );
    }   
	return mChannel; 
}

void Fmod3SoundSamplePtr::destroy()
{
    SharedPtr<Fmod3SoundSample>::destroy();
}

} // Namespace
