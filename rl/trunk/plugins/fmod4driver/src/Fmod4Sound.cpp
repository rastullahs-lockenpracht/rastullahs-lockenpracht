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
#include "Fmod4Sound.h"

#include "Fmod4Driver.h"
#include "SoundManager.h"
#include "SoundResource.h"

using namespace Ogre;
using namespace boost;

namespace rl {

String Fmod4Sound::msMovableType = "Fmod4Sound";
 
Fmod4Sound::Fmod4Sound(Fmod4Driver* driver, const rl::SoundResourcePtr &soundres, SoundType type)
  : Sound(soundres),
    mType(type),
    mSound(NULL),
    mDriver(driver),
    mChannel(NULL)
{
}

float Fmod4Sound::getLength() const
{
    if (!isValid())
    {
        return 0.0;
    }

    unsigned int length;
    mSound->getLength(&length, FMOD_TIMEUNIT_MS);
    return (float)length/1000.0;
}

void Fmod4Sound::load() throw (RuntimeException)
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

    if (mType == Fmod4Sound::SAMPLE)
    {
        FMOD_RESULT res = mDriver->_getFmodSystem()->createSound(
            getSoundResource()->getName().c_str(), 
            mode, 
            NULL, 
            &mSound);

        CHECK_FMOD4_ERRORS(res);
    }
    else if (mType == Fmod4Sound::STREAM)
    {
        FMOD_RESULT res = mDriver->_getFmodSystem()->createStream(
            getSoundResource()->getName().c_str(), 
            mode, 
            NULL, 
            &mSound);

        CHECK_FMOD4_ERRORS(res);
    }
}

void Fmod4Sound::unload() throw (RuntimeException)
{
    mSound->release();
}

bool Fmod4Sound::isValid() const throw (RuntimeException)
{
    return mSound != NULL && mChannel != NULL;
}

void Fmod4SoundPtr::destroy()
{
    SharedPtr<Fmod4Sound>::destroy();
}

const String& Fmod4Sound::getMovableType() const
{
    return msMovableType;
}

void Fmod4Sound::play()
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

    setPosition(mPosition);
    setDirection(mDirection);
	setVelocity(mVelocity); 
    //getSound()->_getFmodSound()->FSOUND_3D_SetMinMaxDistance(mChannel, mRolloffStartDistance, mRolloffEndDistance);
    pause(false);
    SoundPlayEvent event = SoundPlayEvent(this, SoundPlayEvent::STARTEVENT);
    dispatchEvent(&event);
}

void Fmod4Sound::stop()
{
    mChannel->stop();
}

void Fmod4Sound::setPosition(const Ogre::Vector3& position)
{
    FMOD_VECTOR pos;
    pos.x = position.x;
    pos.y = position.y;
    pos.z = -position.z;
    mChannel->set3DAttributes(&pos, NULL);
}

const Ogre::Vector3 Fmod4Sound::getPosition() const
{
    FMOD_VECTOR pos;
    mChannel->get3DAttributes(&pos, NULL);

    Vector3 rval;
    rval.x = pos.x;
    rval.y = pos.y;
    rval.z = -pos.z;

    return rval;
}

const Ogre::Vector3 Fmod4Sound::getVelocity() const
{
    FMOD_VECTOR vel;
    mChannel->get3DAttributes(NULL, &vel);

    Vector3 rval;
    rval.x = vel.x;
    rval.y = vel.y;
    rval.z = -vel.z;

    return rval;
}

void Fmod4Sound::setVelocity(const Ogre::Vector3& velocity)
{
    FMOD_VECTOR vel;
    vel.x = velocity.x;
    vel.y = velocity.y;
    vel.z = -velocity.z;
    mChannel->set3DAttributes(NULL, &vel);
}

void Fmod4Sound::setDirection(const Ogre::Quaternion &orientation)
{
    FMOD_VECTOR to;

    Vector3 toVec = orientation * Vector3::NEGATIVE_UNIT_Z;
    to.x = toVec.x;
    to.y = toVec.y;
    to.z = -toVec.z;

    mChannel->set3DConeOrientation(&to);
}

const Ogre::Quaternion Fmod4Sound::getDirection() const
{
    FMOD_VECTOR to;
    mChannel->get3DConeOrientation(&to);

    Vector3 toVec;
    toVec.x = to.x;
    toVec.y = to.y;
    toVec.z = -to.z;

    return Vector3::NEGATIVE_UNIT_Z.getRotationTo(toVec);
}

bool Fmod4Sound::isPaused()
{
    bool paused;
    FMOD_RESULT error = mChannel->getPaused(&paused);
    return !paused;
}

const bool Fmod4Sound::isPlaying() const
{
    bool playing;
    FMOD_RESULT error = mChannel->isPlaying(&playing);
    return playing;
}

void Fmod4Sound::pause(bool pause)
{
    mChannel->setPaused(pause);
}

void Fmod4Sound::setVolume(float vol)
{
    mChannel->setVolume(vol);
}

const float Fmod4Sound::getVolume() const
{
    float vol;
    mChannel->getVolume(&vol);
    return vol;
}

} // Namespace
