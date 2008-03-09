/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "Fmod4SoundStitching.h"

#include "Fmod4Driver.h"
#include "SoundManager.h"
#include "SoundResource.h"

using namespace Ogre;
using namespace boost;

namespace rl {

String Fmod4SoundStitching::msMovableType = "Fmod4SoundStitching";
 
Fmod4SoundStitching::Fmod4SoundStitching(unsigned int numSlots, const CeGuiString& name, SoundDriver* creator)
  : SoundStitching(numSlots, name, creator),
    mSound(NULL),
    mChannel(NULL)	
{
    setPriority(128);
}

void Fmod4SoundStitching::load() throw (RuntimeException)
{
    FMOD_CREATESOUNDEXINFO exinfo;
    FMOD_MODE mode = FMOD_OPENUSER;

    if (is3d())
    {
        mode |= FMOD_3D;
    }
    else
    {
        mode |= FMOD_2D;
    }

    if (isLooping())
    {
        mode |= FMOD_LOOP_NORMAL;
    }
    else
    {
        mode |= FMOD_LOOP_OFF;
    }

    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    
    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.defaultfrequency = 44100;
    exinfo.numsubsounds = getNumSlots();
    exinfo.numchannels = is3d() ? 1 : 2;
    exinfo.format = FMOD_SOUND_FORMAT_PCM16;

    FMOD_RESULT res = mDriver->_getFmodSystem()->createStream(
        getSoundResource()->getName().c_str(), 
        mode, 
        &exinfo, 
        &mSound);

    CHECK_FMOD4_ERRORS(res);

    int *sentence = new int[getNumSlots()];
    for(unsigned int i = 0; i < getNumSlots(); i++)
    {
        sentence[i] = i;
    }
    res = mSound->setSubSoundSentence(sentence, getNumSlots());
    CHECK_FMOD4_ERRORS(res);
}

void Fmod4SoundStitching::unload() throw (RuntimeException)
{
    mSound->release();
}

bool Fmod4SoundStitching::isValid() const throw (RuntimeException)
{
    return mSound != NULL && mChannel != NULL;
}

void Fmod4SoundStitchingPtr::destroy()
{
    SharedPtr<Fmod4SoundStitching>::destroy();
}

const String& Fmod4SoundStitching::getMovableType() const
{
    return msMovableType;
}

void Fmod4SoundStitching::play(bool destroyWhenDone)
{
    if (!isValid())
    {
        load();
    }

    FMOD_RESULT res = mDriver->_getFmodSystem()->playSound(
        FMOD_CHANNEL_FREE, 
        mSound, 
        true,
        &mChannel);

    CHECK_FMOD4_ERRORS(res);

    RlAssert1(mChannel != NULL);
    mAutoDestroy = destroyWhenDone;
//    mDriver->_registerChannel(mChannel, this);

    float vol;
	if (is3d())
	{
		vol = mDriver->getDefaultSoundVolume();
	}
	else
	{
		vol = mDriver->getDefaultMusicVolume();
	}
	setVolume(vol);

    setPriority(mPriority);
    setPosition(mPosition);
    setDirection(mDirection);
	setVelocity(mVelocity); 
    pause(false);
    SoundPlayEvent event = SoundPlayEvent(this, SoundPlayEvent::STARTEVENT);
    dispatchEvent(&event);
}

void Fmod4SoundStitching::stop()
{
    mChannel->stop();
}

void Fmod4SoundStitching::setPosition(const Ogre::Vector3& position)
{
	mPosition = position;

	if (isValid())
	{
		FMOD_VECTOR pos;
		pos.x = position.x;
		pos.y = position.y;
		pos.z = -position.z;
		mChannel->set3DAttributes(&pos, NULL);
	}
}

const Ogre::Vector3 Fmod4SoundStitching::getPosition() const
{
	if (isValid())
	{
		FMOD_VECTOR pos;
		mChannel->get3DAttributes(&pos, NULL);

		Vector3 rval;
		rval.x = pos.x;
		rval.y = pos.y;
		rval.z = -pos.z;

		return rval;
	}

	return mPosition;
}

const Ogre::Vector3 Fmod4SoundStitching::getVelocity() const
{
	if (isValid())
	{
		FMOD_VECTOR vel;
		mChannel->get3DAttributes(NULL, &vel);

		Vector3 rval;
		rval.x = vel.x;
		rval.y = vel.y;
		rval.z = -vel.z;

		return rval;
	}

	return mVelocity;
}

void Fmod4SoundStitching::setVelocity(const Ogre::Vector3& velocity)
{
	mVelocity = velocity;

	if (isValid())
	{
		FMOD_VECTOR vel;
		vel.x = velocity.x;
		vel.y = velocity.y;
		vel.z = -velocity.z;
		mChannel->set3DAttributes(NULL, &vel);
	}
}

void Fmod4SoundStitching::setDirection(const Ogre::Quaternion &orientation)
{
	mDirection = orientation;

	if (isValid())
	{
		FMOD_VECTOR to;

		Vector3 toVec = orientation * Vector3::NEGATIVE_UNIT_Z;
		to.x = toVec.x;
		to.y = toVec.y;
		to.z = -toVec.z;

		mChannel->set3DConeOrientation(&to);
	}
}

const Ogre::Quaternion Fmod4SoundStitching::getDirection() const
{
	if (isValid())
	{
		FMOD_VECTOR to;
		mChannel->get3DConeOrientation(&to);

		Vector3 toVec;
		toVec.x = to.x;
		toVec.y = to.y;
		toVec.z = -to.z;

		return Vector3::NEGATIVE_UNIT_Z.getRotationTo(toVec);
	}

	return mDirection;
}

bool Fmod4SoundStitching::isPaused()
{
    bool paused;
    FMOD_RESULT error = mChannel->getPaused(&paused);
    return !paused;
}

const bool Fmod4SoundStitching::isPlaying() const
{
    bool playing;
    FMOD_RESULT error = mChannel->isPlaying(&playing);
    return playing;
}

void Fmod4SoundStitching::pause(bool pause)
{
    mChannel->setPaused(pause);
}

void Fmod4SoundStitching::setVolume(float vol)
{
	mVolume = vol;
	if (isValid())
	{
	    mChannel->setVolume(vol);
	}
}

const float Fmod4SoundStitching::getVolume() const
{
	if (isValid())
	{
		float vol;
		mChannel->getVolume(&vol);
		return vol;
	}
	return mVolume;
}

/**
 * @author JoSch
 * @date 14-03-2007
 * @version 1.0
 * @param priority The new priority of this sound
 */ 
void Fmod4SoundStitching::setPriority(const int priority)
{
    mPriority = priority;
    if (mPriority < 0)
    {
        mPriority = 0;
    }
    if (mPriority > 256)
    {
        mPriority = 256;
    }
    if (isValid())
    {
        mChannel->setPriority(mPriority);
    }
}

/**
 * @author JoSch
 * @date 14-03-2007
 * @version 1.0
 * @return The new priority of this sound
 */ 
const int Fmod4SoundStitching::getPriority() const
{
    int priority = mPriority;
    if (isValid())
    {
        mChannel->getPriority(&priority);
    }
    return priority;
}


/**
 * @author JoSch
 * @date 15-03-2007
 * @version 1.0
 * @return The Fmod4 Channel object
 */ 
FMOD::Channel *Fmod4SoundStitching::getFmodChannel() const
{
    return mChannel;
}

/**
 * @author JoSch
 * @date 15-03-2007
 * @version 1.0
 * @return The Fmod4 Sound object
 */ 
FMOD::Sound *Fmod4SoundStitching::getFmodSound() const
{
    return mSound;
}

/**
 * @author JoSch
 * @date 15-03-2007
 * @version 1.0
 * @return TRUE, if the sound should autodestroy when playing ended.
 */ 
bool Fmod4SoundStitching::isAutoDestroying() const
{
    return mAutoDestroy;
}

/**
 * @author JoSch
 * @date 08-03-2008
 * @version 1.0
 */ 
void Fmod4SoundStitching::putSoundIntoSlot(unsigned int slot, CeGuiString label)
{
    if (isValid())
    {
        FMOD_MODE mode = FMOD_DEFAULT;

        if (is3d())
        {
            mode |= FMOD_3D;
        }
        else
        {
            mode |= FMOD_2D;
        }

        if (isLooping())
        {
            mode |= FMOD_LOOP_NORMAL;
        }
        else
        {
            mode |= FMOD_LOOP_OFF;
        }
        FMOD::Sound *sound;
        FMOD_RESULT result = mDriver->_getFmodSystem()->createStream(
            mSoundCache[label]->getName().c_str(), 
            mode, 
            NULL, 
            &sound);
        CHECK_FMOD4_ERRORS(result);
        if (mSoundSlots[slot] != NULL)
        {
            mSoundSlots[slot]->release();
        }
        mSoundSlots[slot] = sound;

        result = sound->setSubSound(slot, sound);
        CHECK_FMOD4_ERRORS(result);
    }
}

/**
 * @author JoSch
 * @date 08-03-2008
 * @version 1.0
 * @return TRUE, if the sound should autodestroy when playing ended.
 */ 
unsigned int Fmod4SoundStitching::getPlayingSlot()
{
    if (isValid())
    {
        unsigned int currentslot = -1;
        FMOD_RESULT result = mChannel->getPosition(&currentslot, (FMOD_TIMEUNIT)(FMOD_TIMEUNIT_SENTENCE_SUBSOUND | FMOD_TIMEUNIT_BUFFERED));
        CHECK_FMOD4_ERRORS(result);
        return currentslot;
    }
    return -1;
}

} // Namespace
