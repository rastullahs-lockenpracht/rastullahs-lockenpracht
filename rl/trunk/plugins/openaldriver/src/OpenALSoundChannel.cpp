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
#include "OpenALSoundChannel.h"
#include "Sound.h"

Ogre::String rl::OpenALSoundChannel::msMovableType = "OpenALSoundChannel";

using namespace Ogre; 

namespace rl
{

OpenALSoundChannel::OpenALSoundChannel(SoundDriver* driver, Sound *sound, const Ogre::String &name)
 : SoundChannel(driver, sound, name),
   mChannel(NO_CHANNEL)
{   
}

OpenALSoundChannel::~OpenALSoundChannel()
{
}

/**
 * @author JoSch
 * @date 07-23-2005
 */
void OpenALSoundChannel::play()
{
    if (!getSound()->isValid())
    {
        getSound()->load();
    }
    // TODO setChannel(getSound()->createChannel());
    setVolume(1.0);
    setPosition(Vector3(0.0, 0.0, 0.0));
    setDirection(Quaternion(0.0, 0.0, 0.0));
    setVelocity(Vector3(0.0, 0.0, 0.0)); 
    pause(false);
}

/**
 * @author JoSch
 * @date 07-04-2005
 * @return Der Soundkanal
 */
const signed int OpenALSoundChannel::getChannel() const
{
    return mChannel;
}

/**
 * @author JoSch
 * @date 07-21-2005
 * @param channel Der Soundkanal
 */
void OpenALSoundChannel::setChannel(signed int channel)
{
    mChannel = channel;
}

float OpenALSoundChannel::getLength() const
{
	return 0.0;
}

/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& OpenALSoundChannel::getMovableType() const
{
    return msMovableType;
}


/**
 * @return Die aktuelle Richtung der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Quaternion OpenALSoundChannel::getDirection() const
{
    return mDirection;
}

/**
 * @param direction Die neue Richtung der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void OpenALSoundChannel::setDirection (const Quaternion& direction)
{
    mDirection = direction;
}

/**
 * @return Ist der Kanal gueltig?
 * @author JoSch
 * @date 08-05-2005
 */
bool OpenALSoundChannel::isValid() const
{
    return true;
}

/**
 * @return Spielt die Soundquelle noch?
 * @author JoSch
 * @date 07-04-2005
 */
const bool OpenALSoundChannel::isPlaying() const
{
    return mIsPlaying;
}


/**
 * @return Die aktuelle Position der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 OpenALSoundChannel::getPosition() const
{
    return mPosition;
}

/**
 * @param position Die neue Position der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void OpenALSoundChannel::setPosition(const Vector3& position)
{
    mPosition = position;
}

/**
 * @return Die aktuelle Geschwindigkeit der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 OpenALSoundChannel::getVelocity() const
{
    return mVelocity;
}

/**
 * @param velocity Die neue Geschwindigkeit der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void OpenALSoundChannel::setVelocity(const Vector3& velocity)
{
    mVelocity = velocity;
}

/**
 * @return Die aktuelle Lautstaerke der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const float OpenALSoundChannel::getVolume() const
{
    return mVolume;
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void OpenALSoundChannel::setVolume(const float gain)
{
    mVolume = gain;
}

/**
 * @param pausing TRUE l�sst den Sound unterbrechen.
 * @author JoSch
 * @date 07-04-2005
 */
void OpenALSoundChannel::pause(bool pausing)
{
    mIsPlaying = !pausing;
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void OpenALSoundChannel::stop()
{
    mIsPlaying = false;
}

/**
 * @return TRUE wenn der Sound unterbrochen wurde.
 * @author JoSch
 * @date 07-04-2005
 */
bool OpenALSoundChannel::isPaused()
{
    return !mIsPlaying;
}

};
