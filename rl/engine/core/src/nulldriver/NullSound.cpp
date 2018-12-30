/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h"

#include "NullDriver.h"
#include "NullSound.h"

#include "SoundManager.h"
#include "SoundResource.h"

using namespace Ogre;

Ogre::String rl::NullSound::msMovableType = "NullSound";

namespace rl
{

    /**
     * @param name Der Name des Sounds.
     * @author JoSch
     * @date 07-04-2005
     */
    NullSound::NullSound(const SoundResourcePtr& soundres, SoundDriver* creator)
        : Sound(soundres, creator)
    {
    }

    /**
     * @author JoSch
     * @date 07-04-2005
     */
    NullSound::~NullSound()
    {
        unload();
    }

    /**
     * @author JoSch
     * @date 07-12-2005
     */
    void NullSound::load()
    {
    }

    /**
     * @author JoSch
     * @date 07-22-2005
     */
    void NullSound::unload()
    {
    }

    /**
     * @return TRUE wenn der Sound gueltig ist.
     * @author JoSch
     * @date 07-12-2005
     */
    bool NullSound::isValid() const
    {
        return true;
    }

    /**
     * @return Die gesamte Spiellaenge des Sounds
     * @author JoSch
     * @date 03-18-2005
     */
    float NullSound::getLength() const
    {
        return 0.0;
    }

    /**
     * @author JoSch
     * @date 07-23-2005
     */
    void NullSound::play(bool destroyWhenDone)
    {
        if (destroyWhenDone)
        {
            // We're not going to play anything, so we can self-destroy right away...
            mCreator->destroySound(this);
        }
    }

    /**
     * @author JoSch
     * @date 03-11-2005
     * @return Den Objekttypen
     */
    const String& NullSound::getMovableType() const
    {
        return msMovableType;
    }

    /**
     * @return Die aktuelle Richtung der Soundquelle
     * @author JoSch
     * @date 07-23-2004
     */
    const Quaternion NullSound::getDirection() const
    {
        return mDirection;
    }

    /**
     * @param direction Die neue Richtung der Soundquelle.
     * @author JoSch
     * @date 07-23-2004
     */
    void NullSound::setDirection(const Quaternion& direction)
    {
        mDirection = direction;
    }

    /**
     * @return Spielt die Soundquelle noch?
     * @author JoSch
     * @date 07-04-2005
     */
    const bool NullSound::isPlaying() const
    {
        return false;
    }

    /**
     * @return Die aktuelle Position der Soundquelle
     * @author JoSch
     * @date 07-04-2005
     */
    const Vector3 NullSound::getPosition() const
    {
        return mPosition;
    }

    /**
     * @param position Die neue Position der Soundquelle.
     * @author JoSch
     * @date 07-04-2005
     */
    void NullSound::setPosition(const Vector3& position)
    {
        mPosition = position;
    }

    /**
     * @return Die aktuelle Geschwindigkeit der Soundquelle
     * @author JoSch
     * @date 07-04-2005
     */
    const Vector3 NullSound::getVelocity() const
    {
        return mVelocity;
    }

    /**
     * @param velocity Die neue Geschwindigkeit der Soundquelle.
     * @author JoSch
     * @date 07-04-2005
     */
    void NullSound::setVelocity(const Vector3& velocity)
    {
        mVelocity = velocity;
    }

    /**
     * @return Die aktuelle Lautstaerke der Soundquelle
     * @author JoSch
     * @date 07-04-2005
     */
    const Ogre::Real NullSound::getVolume() const
    {
        return mVolume;
    }

    /**
     * @param gain Die neue Lautstarke der Soundquelle.
     * @author JoSch
     * @date 07-04-2005
     */
    void NullSound::setVolume(const Ogre::Real gain)
    {
        mVolume = gain;
    }

    /**
     * @param pausing TRUE laesst den Sound unterbrechen.
     * @author JoSch
     * @date 07-04-2005
     */
    void NullSound::pause(bool pausing)
    {
    }

    /**
     * @author JoSch
     * @date 07-23-2004
     */
    void NullSound::stop()
    {
    }

    /**
     * @return TRUE wenn der Sound unterbrochen wurde.
     * @author JoSch
     * @date 07-04-2005
     */
    bool NullSound::isPaused()
    {
        return true;
    }

    /**
     * @author JoSch
     * @date 14-03-2007
     * @version 1.0
     * @param priority The new priority of this sound
     */
    void NullSound::setPriority(const int priority)
    {
        mPriority = priority;
    }

    /**
     * @author JoSch
     * @date 14-03-2007
     * @version 1.0
     * @return The new priority of this sound
     */
    const int NullSound::getPriority() const
    {
        return mPriority;
    }

} // Namespace
