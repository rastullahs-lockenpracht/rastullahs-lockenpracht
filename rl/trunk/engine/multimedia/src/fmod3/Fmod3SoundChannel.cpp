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

#ifdef WITH_FMOD3
#include "Fmod3SoundChannel.h"
#include <OgreMovableObject.h>
#include <OgreAxisAlignedBox.h>
#include <OgreVector3.h>
#include "Fmod3Sound.h"
#include "MultimediaSubsystem.h"
#include "SoundDriver.h"
#include "SoundEvents.h"

extern "C" {
    #include <fmod.h>
}

Ogre::String rl::Fmod3SoundChannel::msMovableType = "Fmod3SoundChannel";

using namespace Ogre; 

namespace rl
{

Fmod3SoundChannel::Fmod3SoundChannel(Sound *sound, const Ogre::String &name)
 : SoundChannel(sound, name),
   mChannel(NO_CHANNEL)
{
    
}

Fmod3SoundChannel::~Fmod3SoundChannel()
{
    FSOUND_StopSound(getChannel());
}

/**
 * @author JoSch
 * @date 07-23-2005
 */
void Fmod3SoundChannel::play()
{
    if (!getSound()->isValid())
    {
        getSound()->load();
    }
    setChannel(dynamic_cast<Fmod3Sound*>(getSound())->createChannel());

	int vol;
	if (is3d())
	{
		vol = MultimediaSubsystem::getSingleton().getActiveDriver()->getDefaultSoundVolume();
	}
	else
	{
		vol = MultimediaSubsystem::getSingleton().getActiveDriver()->getDefaultMusicVolume();
	}
	setVolume(vol);

    setPosition(Vector3(0.0, 0.0, 0.0));
    setDirection(Quaternion(0.0, 0.0, 0.0));
    setVelocity(Vector3(0.0, 0.0, 0.0)); 
    FSOUND_3D_SetMinMaxDistance(mChannel, 1.0, 9999999.0);
    pause(false);
    SoundPlayEvent *event = new SoundPlayEvent(this, SoundPlayEvent::STARTEVENT);
    dispatchEvent(event);
}

/**
 * @author JoSch
 * @date 07-04-2005
 * @return Der Soundkanal
 */
const signed int Fmod3SoundChannel::getChannel() const
{
    return mChannel;
}

/**
 * @author JoSch
 * @date 07-21-2005
 * @param channel Der Soundkanal
 */
void Fmod3SoundChannel::setChannel(signed int channel)
{
    mChannel = channel;
}

/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& Fmod3SoundChannel::getMovableType() const
{
    return msMovableType;
}


/**
 * @return Die aktuelle Richtung der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Quaternion Fmod3SoundChannel::getDirection() const
{
    return mDirection;
}

/**
 * @param direction Die neue Richtung der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void Fmod3SoundChannel::setDirection (const Quaternion& direction)
{
    mDirection = direction;
}

/**
 * @return Ist der Kanal gueltig?
 * @author JoSch
 * @date 08-05-2005
 */
bool Fmod3SoundChannel::isValid() const
{
    return SoundChannel::isValid() && (mChannel > 0);
}

/**
 * @return Spielt die Soundquelle noch?
 * @author JoSch
 * @date 07-04-2005
 */
const bool Fmod3SoundChannel::isPlaying() const
{
    if (isValid())
    {
        return FSOUND_IsPlaying(getChannel());
    }
    return false;
}



/**
 * @return Die aktuelle Position der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 Fmod3SoundChannel::getPosition() const
{
    if (isValid())
    {
        float pos[3];
        FSOUND_3D_GetAttributes(getChannel(), pos, 0);
        Vector3 result(pos);
        return result;
    }
    return mPosition;
}

class FmodSoundSample;
/**
 * @param position Die neue Position der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void Fmod3SoundChannel::setPosition(const Vector3& position)
{
    if (isValid())
    {
        float pos[] = {position[0], position[1], position[2]};
        MultimediaSubsystem::log(LML_TRIVIAL, "Pos: "
            + StringConverter::toString(position.x)
            + StringConverter::toString(position.y)
            + StringConverter::toString(position.z));
        FSOUND_3D_SetAttributes(getChannel(), pos, 0);
    }
    mPosition = position;
}

/**
 * @return Die aktuelle Geschwindigkeit der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 Fmod3SoundChannel::getVelocity() const
{
    if (isValid())
    {
        float vel[3];
        FSOUND_3D_GetAttributes(getChannel(), 0, vel);
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
void Fmod3SoundChannel::setVelocity(const Vector3& velocity)
{
    if (isValid())
    {
        float vel[] = {velocity[0], velocity[1], velocity[2]};
        FSOUND_3D_SetAttributes(getChannel(), 0, vel);
    }
    mVelocity = velocity;
}

/**
 * @return Die aktuelle Lautstaerke der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const unsigned int Fmod3SoundChannel::getVolume() const
{
    if (isValid())
    {
        return int(FSOUND_GetVolume(getChannel()) / 2.55);
    }
    return mVolume;
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void Fmod3SoundChannel::setVolume(const unsigned int gain)
{
    mVolume = gain;
    if (isValid())
    {
        FSOUND_SetVolume(getChannel(), int(gain * 2.55));
    }
}

/**
 * @param pausing TRUE laesst den Sound unterbrechen.
 * @author JoSch
 * @date 07-04-2005
 */
void Fmod3SoundChannel::pause(bool pausing)
{
    if (isValid())
    {
        FSOUND_SetPaused(getChannel(), pausing);
        if (pausing)
        {
            SoundPlayEvent *event = new SoundPlayEvent(this, SoundPlayEvent::PAUSEEVENT);
            dispatchEvent(event);
        }
    }
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void Fmod3SoundChannel::stop()
{
    if (isValid())
    {
        FSOUND_StopSound(getChannel());
    }
    SoundPlayEvent *event = new SoundPlayEvent(this, SoundPlayEvent::STOPEVENT);
    dispatchEvent(event);
}

/**
 * @return TRUE wenn der Sound unterbrochen wurde.
 * @author JoSch
 * @date 07-04-2005
 */
bool Fmod3SoundChannel::isPaused()
{
    if (isValid())
    {
        return FSOUND_GetPaused(getChannel());
    }
    return true;
}


};
#endif // WITH_FMOD3
