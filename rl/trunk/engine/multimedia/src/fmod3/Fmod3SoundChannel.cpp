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
void Fmod3SoundChannel::play() throw (RuntimeException)
{
    if (!getSound()->isValid())
    {
        getSound()->load();
    }
    setChannel(dynamic_cast<Fmod3Sound*>(getSound())->createChannel());
    setGain(255);
    setPosition(Vector3(0.0, 0.0, 0.0));
    setDirection(Vector3(0.0, 0.0, 0.0));
    setVelocity(Vector3(0.0, 0.0, 0.0)); 
    FSOUND_3D_SetMinMaxDistance(mChannel, 4.0, 9999999.0);
    pause(false);
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
const Vector3 Fmod3SoundChannel::getDirection() const throw (RuntimeException)
{
    Vector3 result;
    return result;
}

/**
 * @param direction Die neue Richtung der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void Fmod3SoundChannel::setDirection (const Vector3& direction) throw (RuntimeException)
{
    // TODO
}

/**
 * @return Ist der Kanal gueltig?
 * @author JoSch
 * @date 08-05-2005
 */
bool Fmod3SoundChannel::isValid() const throw (RuntimeException)
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
const Vector3 Fmod3SoundChannel::getPosition() const throw (RuntimeException)
{
    float pos[3];
    if (isValid())
    {
        FSOUND_3D_GetAttributes(getChannel(), pos, 0);
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
void Fmod3SoundChannel::setPosition(const Vector3& position) throw (RuntimeException)
{
    if (isValid())
    {
        float pos[] = {position[0], position[1], position[2]};
        MultimediaSubsystem::log(LML_TRIVIAL, "Pos: "
            + StringConverter::toString(position.x)
            + StringConverter::toString(position.y)
            + StringConverter::toString(position.z));
        FSOUND_3D_SetAttributes(getChannel(), pos, 0);
        FSOUND_Update();
    }
}

/**
 * @return Die aktuelle Geschwindigkeit der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 Fmod3SoundChannel::getVelocity() const throw (RuntimeException)
{
    float vel[3];
    if (isValid())
    {
        FSOUND_3D_GetAttributes(getChannel(), 0, vel);
    }
    Vector3 result(vel);
    return result;
}

/**
 * @param velocity Die neue Geschwindigkeit der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void Fmod3SoundChannel::setVelocity(const Vector3& velocity) throw (RuntimeException)
{
    if (isValid())
    {
        float vel[] = {velocity[0], velocity[1], velocity[2]};
        FSOUND_3D_SetAttributes(getChannel(), 0, vel);
    }
}

/**
 * @return Die aktuelle Lautstaerke der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const int Fmod3SoundChannel::getGain() const throw (RuntimeException)
{
    if (isValid())
    {
        return FSOUND_GetVolume(getChannel());
    }
    return 0;
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void Fmod3SoundChannel::setGain(const int gain) throw (RuntimeException)
{
    if (isValid())
    {
        FSOUND_SetVolume(getChannel(), gain);
    }
}

/**
 * @param pausing TRUE l�sst den Sound unterbrechen.
 * @author JoSch
 * @date 07-04-2005
 */
void Fmod3SoundChannel::pause(bool pausing) throw (RuntimeException)
{
    if (isValid())
    {
        FSOUND_SetPaused(getChannel(), pausing);
    }
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void Fmod3SoundChannel::stop() throw (RuntimeException)
{
    if (isValid())
    {
        FSOUND_StopSound(getChannel());
    }
}

/**
 * @return TRUE wenn der Sound unterbrochen wurde.
 * @author JoSch
 * @date 07-04-2005
 */
bool Fmod3SoundChannel::isPaused() throw (RuntimeException)
{
    if (isValid())
    {
        return FSOUND_GetPaused(getChannel());
    }
    return true;
}


};
#endif // WITH_FMOD3
