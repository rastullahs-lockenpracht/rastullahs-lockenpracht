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

#include "NullSoundChannel.h"
#include <OgreMovableObject.h>
#include <OgreAxisAlignedBox.h>
#include <OgreVector3.h>
#include "Sound.h"
#include "MultimediaSubsystem.h"

Ogre::String rl::NullSoundChannel::msMovableType = "NullSoundChannel";

using namespace Ogre; 

namespace rl
{

NullSoundChannel::NullSoundChannel(Sound *sound, const Ogre::String &name)
 : SoundChannel(sound, name),
   mChannel(NO_CHANNEL)
{   
}

NullSoundChannel::~NullSoundChannel()
{
}

/**
 * @author JoSch
 * @date 07-23-2005
 */
void NullSoundChannel::play() throw (RuntimeException)
{
    if (!getSound()->isValid())
    {
        getSound()->load();
    }
    // TODO setChannel(getSound()->createChannel());
    setGain(255);
    setPosition(Vector3(0.0, 0.0, 0.0));
    setDirection(Vector3(0.0, 0.0, 0.0));
    setVelocity(Vector3(0.0, 0.0, 0.0)); 
    pause(false);
}

/**
 * @author JoSch
 * @date 07-04-2005
 * @return Der Soundkanal
 */
const signed int NullSoundChannel::getChannel() const
{
    return mChannel;
}

/**
 * @author JoSch
 * @date 07-21-2005
 * @param channel Der Soundkanal
 */
void NullSoundChannel::setChannel(signed int channel)
{
    mChannel = channel;
}

/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& NullSoundChannel::getMovableType() const
{
    return msMovableType;
}


/**
 * @return Die aktuelle Richtung der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Vector3 NullSoundChannel::getDirection() const throw (RuntimeException)
{
    return mDirection;
}

/**
 * @param direction Die neue Richtung der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void NullSoundChannel::setDirection (const Vector3& direction) throw (RuntimeException)
{
    mDirection = direction;
}

/**
 * @return Ist der Kanal gueltig?
 * @author JoSch
 * @date 08-05-2005
 */
bool NullSoundChannel::isValid() const throw (RuntimeException)
{
    return true;
}

/**
 * @return Spielt die Soundquelle noch?
 * @author JoSch
 * @date 07-04-2005
 */
const bool NullSoundChannel::isPlaying() const
{
    return mIsPlaying;
}


/**
 * @return Die aktuelle Position der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 NullSoundChannel::getPosition() const throw (RuntimeException)
{
    return mPosition;
}

/**
 * @param position Die neue Position der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void NullSoundChannel::setPosition(const Vector3& position) throw (RuntimeException)
{
    mPosition = position;
}

/**
 * @return Die aktuelle Geschwindigkeit der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 NullSoundChannel::getVelocity() const throw (RuntimeException)
{
    return mVelocity;
}

/**
 * @param velocity Die neue Geschwindigkeit der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void NullSoundChannel::setVelocity(const Vector3& velocity) throw (RuntimeException)
{
    mVelocity = velocity;
}

/**
 * @return Die aktuelle Lautstaerke der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const int NullSoundChannel::getGain() const throw (RuntimeException)
{
    return mGain;
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void NullSoundChannel::setGain(const int gain) throw (RuntimeException)
{
    mGain = gain;
}

/**
 * @param pausing TRUE l�sst den Sound unterbrechen.
 * @author JoSch
 * @date 07-04-2005
 */
void NullSoundChannel::pause(bool pausing) throw (RuntimeException)
{
    mIsPlaying = !pausing;
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void NullSoundChannel::stop() throw (RuntimeException)
{
    mIsPlaying = false;
}

/**
 * @return TRUE wenn der Sound unterbrochen wurde.
 * @author JoSch
 * @date 07-04-2005
 */
bool NullSoundChannel::isPaused() throw (RuntimeException)
{
    return !mIsPlaying;
}


};
