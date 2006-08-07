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

#include "Fmod4SoundChannel.h"

#include <OgreMovableObject.h>
#include <fmod_errors.h>

#include "Fmod4Sound.h"
#include "SoundDriver.h"
#include "SoundEvents.h"


using namespace Ogre; 

namespace rl
{

String Fmod4SoundChannel::msMovableType = "Fmod4SoundChannel";

Fmod4SoundChannel::Fmod4SoundChannel(Fmod4Driver *driver, Sound *sound, const String &name)
    : SoundChannel(driver, sound, name),
    mChannel(NULL)
{
}

Fmod4SoundChannel::~Fmod4SoundChannel()
{
}

const String& Fmod4SoundChannel::getMovableType() const
{
    return msMovableType;
}

void Fmod4SoundChannel::play()
{
    if (!getSound()->isValid())
    {
        getSound()->load();
    }

    FMOD_RESULT res = getDriver()->_getFmodSystem()->playSound(
        FMOD_CHANNEL_FREE, 
        getSound()->_getFmodSound(), 
        true,
        &mChannel);

    getDriver()->checkErrors(res);

    RlAssert1(mChannel != NULL);

    float vol;
	if (is3d())
	{
		vol = getDriver()->getDefaultSoundVolume();
	}
	else
	{
		vol = getDriver()->getDefaultMusicVolume();
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

void Fmod4SoundChannel::stop()
{
    mChannel->stop();
}

Fmod4Driver* Fmod4SoundChannel::getDriver() const
{
    return static_cast<Fmod4Driver*>(SoundChannel::getDriver());
}

Fmod4Sound* Fmod4SoundChannel::getSound() const
{
    return static_cast<Fmod4Sound*>(SoundChannel::getSound());
}

void Fmod4SoundChannel::setPosition(const Ogre::Vector3& position)
{
    FMOD_VECTOR pos;
    pos.x = position.x;
    pos.y = position.y;
    pos.z = -position.z;
    mChannel->set3DAttributes(&pos, NULL);
}

const Ogre::Vector3 Fmod4SoundChannel::getPosition() const
{
    FMOD_VECTOR pos;
    mChannel->get3DAttributes(&pos, NULL);

    Vector3 rval;
    rval.x = pos.x;
    rval.y = pos.y;
    rval.z = -pos.z;

    return rval;
}

const Ogre::Vector3 Fmod4SoundChannel::getVelocity() const
{
    FMOD_VECTOR vel;
    mChannel->get3DAttributes(NULL, &vel);

    Vector3 rval;
    rval.x = vel.x;
    rval.y = vel.y;
    rval.z = -vel.z;

    return rval;
}

void Fmod4SoundChannel::setVelocity(const Ogre::Vector3& velocity)
{
    FMOD_VECTOR vel;
    vel.x = velocity.x;
    vel.y = velocity.y;
    vel.z = -velocity.z;
    mChannel->set3DAttributes(NULL, &vel);
}

void Fmod4SoundChannel::setDirection(const Ogre::Quaternion &orientation)
{
    FMOD_VECTOR to;

    Vector3 toVec = orientation * Vector3::NEGATIVE_UNIT_Z;
    to.x = toVec.x;
    to.y = toVec.y;
    to.z = -toVec.z;

    mChannel->set3DConeOrientation(&to);
}

const Ogre::Quaternion Fmod4SoundChannel::getDirection() const
{
    FMOD_VECTOR to;
    mChannel->get3DConeOrientation(&to);

    Vector3 toVec;
    toVec.x = to.x;
    toVec.y = to.y;
    toVec.z = -to.z;

    return Vector3::NEGATIVE_UNIT_Z.getRotationTo(toVec);
}

bool Fmod4SoundChannel::isPaused()
{
    bool paused;
    FMOD_RESULT error = mChannel->getPaused(&paused);
    return !paused;
}

const bool Fmod4SoundChannel::isPlaying() const
{
    bool playing;
    FMOD_RESULT error = mChannel->isPlaying(&playing);
    return playing;
}

void Fmod4SoundChannel::pause(bool pause)
{
    mChannel->setPaused(pause);
}

void Fmod4SoundChannel::setVolume(float vol)
{
    mChannel->setVolume(vol);
}

const float Fmod4SoundChannel::getVolume() const
{
    float vol;
    mChannel->getVolume(&vol);
    return vol;
}

bool Fmod4SoundChannel::isValid() const
{
    return mChannel != NULL;
}

};
