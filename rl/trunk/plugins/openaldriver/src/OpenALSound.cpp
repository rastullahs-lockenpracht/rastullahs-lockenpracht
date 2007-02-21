/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include "OpenALSound.h"

#include "OpenALDriver.h"
#include "Logger.h"

Ogre::String rl::OpenALSound::msMovableType = "OpenALSound";

using namespace Ogre; 

namespace rl
{

OpenALSound::OpenALSound(OpenALDriver* driver, const SoundResourcePtr& res)
 : Sound(res, driver),
   mDriver(driver),
   mChannel(NO_CHANNEL)
{   
}

OpenALSound::~OpenALSound()
{
}

/**
 * @author JoSch
 * @date 07-23-2005
 */
void OpenALSound::play(bool destroyWhenDone)
{
    if (!isValid())
    {
        load();
    }

    setVolume(1.0);
    setPosition(mPosition);
    setDirection(mDirection);
    setVelocity(mVelocity); 
    pause(false);

    // OpenAL Driver doesn't handle destroyWhenDone yet, so warn about it
    if (destroyWhenDone)
    {
        LOG_WARNING(Logger::MULTIMEDIA, "Sound zum automatischen löschen regisitert, aber\
 OpenAL-Treiber unterstützt diese Option nicht. Memory-Leak.");
    }
}

float OpenALSound::getLength() const
{
	return 0.0;
}

/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& OpenALSound::getMovableType() const
{
    return msMovableType;
}


/**
 * @return Die aktuelle Richtung der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Quaternion OpenALSound::getDirection() const
{
    return mDirection;
}

/**
 * @param direction Die neue Richtung der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void OpenALSound::setDirection (const Quaternion& direction)
{
    mDirection = direction;
}

/**
 * @return Ist der Kanal gueltig?
 * @author JoSch
 * @date 08-05-2005
 */
bool OpenALSound::isValid() const throw (RuntimeException)
{
    return true;
}

/**
 * @return Spielt die Soundquelle noch?
 * @author JoSch
 * @date 07-04-2005
 */
const bool OpenALSound::isPlaying() const
{
    return mIsPlaying;
}


/**
 * @return Die aktuelle Position der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 OpenALSound::getPosition() const
{
    return mPosition;
}

/**
 * @param position Die neue Position der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void OpenALSound::setPosition(const Vector3& position)
{
    mPosition = position;
}

/**
 * @return Die aktuelle Geschwindigkeit der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 OpenALSound::getVelocity() const
{
    return mVelocity;
}

/**
 * @param velocity Die neue Geschwindigkeit der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void OpenALSound::setVelocity(const Vector3& velocity)
{
    mVelocity = velocity;
}

/**
 * @return Die aktuelle Lautstaerke der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const float OpenALSound::getVolume() const
{
    return mVolume;
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void OpenALSound::setVolume(const float gain)
{
    mVolume = gain;
}

/**
 * @param pausing TRUE laesst den Sound unterbrechen.
 * @author JoSch
 * @date 07-04-2005
 */
void OpenALSound::pause(bool pausing)
{
    mIsPlaying = !pausing;
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void OpenALSound::stop()
{
    mIsPlaying = false;
}

/**
 * @return TRUE wenn der Sound unterbrochen wurde.
 * @author JoSch
 * @date 07-04-2005
 */
bool OpenALSound::isPaused()
{
    return !mIsPlaying;
}

};
