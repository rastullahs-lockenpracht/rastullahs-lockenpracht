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
#include "Fmod3Sound.h"

extern "C" {
    #include <fmod.h>
}

#include "Fmod3Driver.h"
#include "SoundEvents.h"

Ogre::String rl::Fmod3Sound::msMovableType = "Fmod3Sound";

using namespace Ogre; 

namespace rl
{

Fmod3Sound::Fmod3Sound(Fmod3Driver* driver, const SoundResourcePtr& res)
 : Sound(res),
   mDriver(driver),
   mChannel(NO_CHANNEL)
{
	mRolloffStartDistance = 1.0f;
	mRolloffEndDistance = 1000000000.0f;
}

Fmod3Sound::~Fmod3Sound()
{
    FSOUND_StopSound(mChannel);
}

/**
 * @author JoSch
 * @date 07-23-2005
 */
void Fmod3Sound::play()
{
    if (!isValid())
    {
        load();
    }

    mChannel = createChannel();

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
    FSOUND_3D_SetMinMaxDistance(mChannel, mRolloffStartDistance, mRolloffEndDistance);
    pause(false);
    SoundPlayEvent event = SoundPlayEvent(this, SoundPlayEvent::STARTEVENT);
    dispatchEvent(&event);
}

/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& Fmod3Sound::getMovableType() const
{
    return msMovableType;
}


/**
 * @return Die aktuelle Richtung der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Quaternion Fmod3Sound::getDirection() const
{
    return mDirection;
}

/**
 * @param direction Die neue Richtung der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void Fmod3Sound::setDirection (const Quaternion& direction)
{
    mDirection = direction;
}

/**
 * @return Ist der Kanal gueltig?
 * @author JoSch
 * @date 08-05-2005
 */
bool Fmod3Sound::isValid() const throw (RuntimeException)
{
    return (mChannel != NO_CHANNEL) 
        && (mChannel != 0)
        && (mChannel != -1);
}

/**
 * @return Spielt die Soundquelle noch?
 * @author JoSch
 * @date 07-04-2005
 */
const bool Fmod3Sound::isPlaying() const
{
    if (isValid())
    {
        return FSOUND_IsPlaying(mChannel);
    }
    return false;
}



/**
 * @return Die aktuelle Position der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 Fmod3Sound::getPosition() const
{
    if (isValid())
    {
        float pos[3];
        FSOUND_3D_GetAttributes(mChannel, pos, 0);
        Vector3 result(pos);
        return result;
    }
    return mPosition;
}

/**
 * @param position Die neue Position der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void Fmod3Sound::setPosition(const Vector3& position)
{
    if (isValid())
    {
		// Bei Fmod ist +z "nach vorn", im Gegensatz zu -z bei Ogre
        float pos[] = {position.x, position.y, -position.z}; 

        LOG_DEBUG(Logger::MULTIMEDIA, "Pos: "
            + StringConverter::toString(position.x)
            + StringConverter::toString(position.y)
            + StringConverter::toString(position.z));
        FSOUND_3D_SetAttributes(mChannel, pos, 0);
    }
    mPosition = position;
}

/**
 * @return Die aktuelle Geschwindigkeit der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 Fmod3Sound::getVelocity() const
{
    if (isValid())
    {
        float vel[3];
        FSOUND_3D_GetAttributes(mChannel, 0, vel);
        Vector3 result(vel);
        return result;
    }
    return mVelocity;
}

/**
 * @param velocity Die neue Geschwindigkeit der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void Fmod3Sound::setVelocity(const Vector3& velocity)
{
    if (isValid())
    {
        float vel[] = {velocity.x, velocity.y, -velocity.z};
        FSOUND_3D_SetAttributes(mChannel, 0, vel);
    }
    mVelocity = velocity;
}

/**
 * @return Die aktuelle Lautstaerke der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Ogre::Real Fmod3Sound::getVolume() const
{
    if (isValid())
    {
        return (float)FSOUND_GetVolume(mChannel) / 255.0;
    }
    return mVolume;
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void Fmod3Sound::setVolume(const Ogre::Real gain)
{
	float volume = gain;
	if (volume > 1.0)
		volume = 1.0;
	if (volume < 0.0)
		volume = 0.0;

    mVolume = volume;
    if (isValid())
    {
        FSOUND_SetVolume(mChannel, int(volume * 255.0));
    }
}

/**
 * @param pausing TRUE laesst den Sound unterbrechen.
 * @author JoSch
 * @date 07-04-2005
 */
void Fmod3Sound::pause(bool pausing)
{
    if (isValid())
    {
        FSOUND_SetPaused(mChannel, pausing);
        if (pausing)
        {
            SoundPlayEvent event = SoundPlayEvent(this, SoundPlayEvent::PAUSEEVENT);
            dispatchEvent(&event);
        }
    }
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void Fmod3Sound::stop()
{
    if (isValid())
    {
        FSOUND_StopSound(mChannel);
        mDriver->checkErrors();
        mChannel = NO_CHANNEL;
    }
    SoundPlayEvent event = SoundPlayEvent(this, SoundPlayEvent::STOPEVENT);
    dispatchEvent(&event);
}

/**
 * @return TRUE wenn der Sound unterbrochen wurde.
 * @author JoSch
 * @date 07-04-2005
 */
bool Fmod3Sound::isPaused()
{
    if (isValid())
    {
        return FSOUND_GetPaused(mChannel);
    }
    return true;
}

void Fmod3Sound::setRolloffStartDistance(const Ogre::Real& distance)
{
	Sound::setRolloffStartDistance(distance);

	if (isValid())
	{
		float min, max;
		FSOUND_3D_GetMinMaxDistance(mChannel, &min, &max);
		FSOUND_3D_SetMinMaxDistance(mChannel, distance, max);
	}
}

const Ogre::Real Fmod3Sound::getRolloffStartDistance()
{
	if (isValid())
	{
		float min, max;
		FSOUND_3D_GetMinMaxDistance(mChannel, &min, &max);
		return min;
	}
	return Sound::getRolloffStartDistance();
}

void Fmod3Sound::setRolloffEndDistance(const Ogre::Real& distance)
{
	Sound::setRolloffEndDistance(distance);

	if (isValid())
	{
		float min, max;
		FSOUND_3D_GetMinMaxDistance(mChannel, &min, &max);
		FSOUND_3D_SetMinMaxDistance(mChannel, min, distance);
	}
}

const Ogre::Real Fmod3Sound::getRolloffEndDistance()
{
	if (isValid())
	{
		float min, max;
		FSOUND_3D_GetMinMaxDistance(mChannel, &min, &max);
		return max;
	}
	return Sound::getRolloffEndDistance();
}

};
