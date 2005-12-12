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
#include "SoundSubsystem.h"

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
    // TODO: setChannel(getSound()->createChannel());
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
    Vector3 result;
    return result;
}

/**
 * @param direction Die neue Richtung der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void NullSoundChannel::setDirection (const Vector3& direction) throw (RuntimeException)
{
    // TODO
}

/**
 * @return Ist der Kanal gueltig?
 * @author JoSch
 * @date 08-05-2005
 */
bool NullSoundChannel::isValid() const throw (RuntimeException)
{
    return SoundChannel::isValid() && (mChannel > 0);
}

/**
 * @return Spielt die Soundquelle noch?
 * @author JoSch
 * @date 07-04-2005
 */
const bool NullSoundChannel::isPlaying() const
{
    if (isValid())
    {
    }
    return false;
}



/**
 * @return Die aktuelle Position der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 NullSoundChannel::getPosition() const throw (RuntimeException)
{
    float pos[3];
    if (isValid())
    {
    }
    Vector3 result(pos);
    return result;
}

class FmodSoundSample;
/**
 * @param position Die neue Position der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void NullSoundChannel::setPosition(const Vector3& position) throw (RuntimeException)
{
    if (isValid())
    {
        float pos[] = {position[0], position[1], position[2]};
    }
}

/**
 * @return Die aktuelle Geschwindigkeit der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 NullSoundChannel::getVelocity() const throw (RuntimeException)
{
    float vel[3];
    if (isValid())
    {
    }
    Vector3 result(vel);
    return result;
}

/**
 * @param velocity Die neue Geschwindigkeit der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void NullSoundChannel::setVelocity(const Vector3& velocity) throw (RuntimeException)
{
    if (isValid())
    {
        float vel[] = {velocity[0], velocity[1], velocity[2]};
    }
}

/**
 * @return Die aktuelle Lautstaerke der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const int NullSoundChannel::getGain() const throw (RuntimeException)
{
    if (isValid())
    {
    }
    return 0;
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void NullSoundChannel::setGain(const int gain) throw (RuntimeException)
{
    if (isValid())
    {
    }
}

/**
 * @param pausing TRUE l�sst den Sound unterbrechen.
 * @author JoSch
 * @date 07-04-2005
 */
void NullSoundChannel::pause(bool pausing) throw (RuntimeException)
{
    if (isValid())
    {
    }
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void NullSoundChannel::stop() throw (RuntimeException)
{
    if (isValid())
    {
    }
}

/**
 * @return TRUE wenn der Sound unterbrochen wurde.
 * @author JoSch
 * @date 07-04-2005
 */
bool NullSoundChannel::isPaused() throw (RuntimeException)
{
    if (isValid())
    {
    }
    return true;
}


};
