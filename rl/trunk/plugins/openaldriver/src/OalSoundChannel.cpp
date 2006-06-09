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
#include "OalSoundChannel.h"
#include <OgreMovableObject.h>
#include <OgreVector3.h>
#include "Sound.h"

Ogre::String rl::OalSoundChannel::msMovableType = "OalSoundChannel";

using namespace Ogre; 

namespace rl
{

OalSoundChannel::OalSoundChannel(SoundDriver* driver, Sound *sound, const Ogre::String &name)
 : SoundChannel(driver, sound, name),
   mChannel(NO_CHANNEL)
{   
}

OalSoundChannel::~OalSoundChannel()
{
}

/**
 * @author JoSch
 * @date 07-23-2005
 */
void OalSoundChannel::play()
{
    if (!getSound()->isValid())
    {
        getSound()->load();
    }
    // TODO setChannel(getSound()->createChannel());
    setVolume(100);
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
const signed int OalSoundChannel::getChannel() const
{
    return mChannel;
}

/**
 * @author JoSch
 * @date 07-21-2005
 * @param channel Der Soundkanal
 */
void OalSoundChannel::setChannel(signed int channel)
{
    mChannel = channel;
}

/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& OalSoundChannel::getMovableType() const
{
    return msMovableType;
}


/**
 * @return Die aktuelle Richtung der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Quaternion OalSoundChannel::getDirection() const
{
    return mDirection;
}

/**
 * @param direction Die neue Richtung der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void OalSoundChannel::setDirection (const Quaternion& direction)
{
    mDirection = direction;
}

/**
 * @return Ist der Kanal gueltig?
 * @author JoSch
 * @date 08-05-2005
 */
bool OalSoundChannel::isValid() const
{
    return true;
}

/**
 * @return Spielt die Soundquelle noch?
 * @author JoSch
 * @date 07-04-2005
 */
const bool OalSoundChannel::isPlaying() const
{
    return mIsPlaying;
}


/**
 * @return Die aktuelle Position der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 OalSoundChannel::getPosition() const
{
    return mPosition;
}

/**
 * @param position Die neue Position der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void OalSoundChannel::setPosition(const Vector3& position)
{
    mPosition = position;
}

/**
 * @return Die aktuelle Geschwindigkeit der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 OalSoundChannel::getVelocity() const
{
    return mVelocity;
}

/**
 * @param velocity Die neue Geschwindigkeit der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void OalSoundChannel::setVelocity(const Vector3& velocity)
{
    mVelocity = velocity;
}

/**
 * @return Die aktuelle Lautstaerke der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const unsigned int OalSoundChannel::getVolume() const
{
    return mVolume;
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void OalSoundChannel::setVolume(const unsigned int gain)
{
    mVolume = gain;
}

/**
 * @param pausing TRUE l�sst den Sound unterbrechen.
 * @author JoSch
 * @date 07-04-2005
 */
void OalSoundChannel::pause(bool pausing)
{
    mIsPlaying = !pausing;
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void OalSoundChannel::stop()
{
    mIsPlaying = false;
}

/**
 * @return TRUE wenn der Sound unterbrochen wurde.
 * @author JoSch
 * @date 07-04-2005
 */
bool OalSoundChannel::isPaused()
{
    return !mIsPlaying;
}

};
