/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Perl Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Perl Artistic License for more details.
*
*  You should have received a copy of the Perl Artistic License
*  along with this program; if not you can get it here
*  http://www.perldoc.com/perl5.6/Artistic.html.
*/

#include "SoundChannel.h"
#include <OgreMovableObject.h>
#include <OgreAxisAlignedBox.h>
#include <OgreVector3.h>
#include "Sound.h"

Ogre::String rl::SoundChannel::msMovableType = "SoundChannel";
Ogre::AxisAlignedBox rl::SoundChannel::msAABox = Ogre::AxisAlignedBox(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5);

using namespace Ogre; 

namespace rl
{

SoundChannel::SoundChannel(Sound *sound, const Ogre::String &name)
 : MovableObject(),
   mChannel(NO_CHANNEL),
   mSound(sound),
   mName(name)
{
    
}

SoundChannel::~SoundChannel()
{
}

/**
 * @author JoSch
 * @date 07-23-2005
 */
void SoundChannel::play() throw (RuntimeException)
{
    if (!mSound->isValid())
    {
        mSound->load();
    }
    setChannel(mSound->createChannel());
    setGain(255);
    setPosition(Vector3(0.0, 0.0, 0.0));
    setDirection(Vector3(1.0, 0.0, 0.0));
    setVelocity(Vector3(0.0, 0.0, 0.0));
    pause(false);
}

/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Namen
 */
const String& SoundChannel::getName() const
{
    return mName;
}

/**
 * @author JoSch
 * @date 07-04-2005
 * @return Der Soundkanal
 */
const signed int SoundChannel::getChannel() const
{
    return mChannel;
}

/**
 * @author JoSch
 * @date 07-21-2005
 * @param channel Der Soundkanal
 */
void SoundChannel::setChannel(signed int channel)
{
    mChannel = channel;
}

/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& SoundChannel::getMovableType() const
{
    return msMovableType;
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
void SoundChannel::_notifyCurrentCamera(Camera *cam)
{
    // Brauchen wir nicht zu wissen.
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
const AxisAlignedBox& SoundChannel::getBoundingBox() const 
{
    return msAABox;
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
Real SoundChannel::getBoundingRadius (void) const
{
    return 0.0;
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
void SoundChannel::_updateRenderQueue(RenderQueue *queue)
{
    // Brauchen wir nicht
}


/**
 * @return Die aktuelle Richtung der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Vector3 SoundChannel::getDirection() const throw (RuntimeException)
{
    Vector3 result;
    return result;
}

/**
 * @param direction Die neue Richtung der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundChannel::setDirection (const Vector3& direction) throw (RuntimeException)
{
    // TODO
}

/**
 * @return Ist der Kanal gueltig?
 * @author JoSch
 * @date 08-05-2005
 */
bool SoundChannel::isValid() const throw (RuntimeException)
{
    return mSound->isValid() && (mChannel > 0);
}

/**
 * @return Spielt die Soundquelle noch?
 * @author JoSch
 * @date 07-04-2005
 */
const bool SoundChannel::isPlaying() const
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
const Vector3 SoundChannel::getPosition() const throw (RuntimeException)
{
    float pos[3];
    if (isValid())
    {
        FSOUND_3D_GetAttributes(getChannel(), pos, 0);
    }
    Vector3 result(pos);
    return result;
}

class SoundSampleMovable;
/**
 * @param position Die neue Position der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void SoundChannel::setPosition(const Vector3& position) throw (RuntimeException)
{
    if (isValid())
    {
        float pos[] = {position[0], position[1], position[2]};
        FSOUND_3D_SetAttributes(getChannel(), pos, 0);
    }
}

/**
 * @return Die aktuelle Geschwindigkeit der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 SoundChannel::getVelocity() const throw (RuntimeException)
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
void SoundChannel::setVelocity(const Vector3& velocity) throw (RuntimeException)
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
const int SoundChannel::getGain() const throw (RuntimeException)
{
    if (isValid())
    {
        return FSOUND_GetVolume(getChannel());
    }
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void SoundChannel::setGain(const int gain) throw (RuntimeException)
{
    if (isValid())
    {
        FSOUND_SetVolume(getChannel(), gain);
    }
}

/**
 * @param pausing TRUE lässt den Sound unterbrechen.
 * @author JoSch
 * @date 07-04-2005
 */
void SoundChannel::pause(bool pausing) throw (RuntimeException)
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
void SoundChannel::stop() throw (RuntimeException)
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
bool SoundChannel::isPaused() throw (RuntimeException)
{
    if (isValid())
    {
        return FSOUND_GetPaused(getChannel());
    }
}


};
