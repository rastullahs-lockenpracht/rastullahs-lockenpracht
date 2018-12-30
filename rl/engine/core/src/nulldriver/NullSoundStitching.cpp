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
#include "NullSoundStitching.h"

#include "SoundManager.h"
#include "SoundResource.h"

using namespace Ogre;

Ogre::String rl::NullSoundStitching::msMovableType = "NullSoundStitching";

namespace rl
{

    /**
     * @param name Der Name des Sounds.
     * @author JoSch
     * @date 07-04-2005
     */
    NullSoundStitching::NullSoundStitching(unsigned int numSlots, const CeGuiString& name, SoundDriver* creator)
        : SoundStitching(numSlots, name, creator)
    {
    }

    /**
     * @author JoSch
     * @date 07-04-2005
     */
    NullSoundStitching::~NullSoundStitching()
    {
        unload();
    }

    /**
     * @author JoSch
     * @date 07-12-2005
     */
    void NullSoundStitching::load()
    {
    }

    /**
     * @author JoSch
     * @date 07-22-2005
     */
    void NullSoundStitching::unload()
    {
    }

    /**
     * @return TRUE wenn der Sound gueltig ist.
     * @author JoSch
     * @date 07-12-2005
     */
    bool NullSoundStitching::isValid() const
    {
        return true;
    }

    /**
     * @author JoSch
     * @date 07-23-2005
     */
    void NullSoundStitching::play(bool destroyWhenDone)
    {
    }

    /**
     * @author JoSch
     * @date 03-11-2005
     * @return Den Objekttypen
     */
    const String& NullSoundStitching::getMovableType() const
    {
        return msMovableType;
    }

    /**
     * @return Die aktuelle Richtung der Soundquelle
     * @author JoSch
     * @date 07-23-2004
     */
    const Quaternion NullSoundStitching::getDirection() const
    {
        return mDirection;
    }

    /**
     * @param direction Die neue Richtung der Soundquelle.
     * @author JoSch
     * @date 07-23-2004
     */
    void NullSoundStitching::setDirection(const Quaternion& direction)
    {
        mDirection = direction;
    }

    /**
     * @return Spielt die Soundquelle noch?
     * @author JoSch
     * @date 07-04-2005
     */
    const bool NullSoundStitching::isPlaying() const
    {
        return false;
    }

    /**
     * @return Die aktuelle Position der Soundquelle
     * @author JoSch
     * @date 07-04-2005
     */
    const Vector3 NullSoundStitching::getPosition() const
    {
        return mPosition;
    }

    /**
     * @param position Die neue Position der Soundquelle.
     * @author JoSch
     * @date 07-04-2005
     */
    void NullSoundStitching::setPosition(const Vector3& position)
    {
        mPosition = position;
    }

    /**
     * @return Die aktuelle Geschwindigkeit der Soundquelle
     * @author JoSch
     * @date 07-04-2005
     */
    const Vector3 NullSoundStitching::getVelocity() const
    {
        return mVelocity;
    }

    /**
     * @param velocity Die neue Geschwindigkeit der Soundquelle.
     * @author JoSch
     * @date 07-04-2005
     */
    void NullSoundStitching::setVelocity(const Vector3& velocity)
    {
        mVelocity = velocity;
    }

    /**
     * @return Die aktuelle Lautstaerke der Soundquelle
     * @author JoSch
     * @date 07-04-2005
     */
    const Ogre::Real NullSoundStitching::getVolume() const
    {
        return mVolume;
    }

    /**
     * @param gain Die neue Lautstarke der Soundquelle.
     * @author JoSch
     * @date 07-04-2005
     */
    void NullSoundStitching::setVolume(const Ogre::Real gain)
    {
        mVolume = gain;
    }

    /**
     * @param pausing TRUE laesst den Sound unterbrechen.
     * @author JoSch
     * @date 07-04-2005
     */
    void NullSoundStitching::pause(bool pausing)
    {
    }

    /**
     * @author JoSch
     * @date 07-23-2004
     */
    void NullSoundStitching::stop()
    {
    }

    /**
     * @return TRUE wenn der Sound unterbrochen wurde.
     * @author JoSch
     * @date 07-04-2005
     */
    bool NullSoundStitching::isPaused()
    {
        return true;
    }

    /**
     * @author JoSch
     * @date 14-03-2007
     * @version 1.0
     * @param priority The new priority of this sound
     */
    void NullSoundStitching::setPriority(const int priority)
    {
        mPriority = priority;
    }

    /**
     * @author JoSch
     * @date 14-03-2007
     * @version 1.0
     * @return The new priority of this sound
     */
    const int NullSoundStitching::getPriority() const
    {
        return mPriority;
    }

    /**
     * @author JoSch
     * @date 08-03-2008
     * @version 1.0
     * @return Assign a sound recource to a slot.
     */
    void NullSoundStitching::putSoundIntoSlot(unsigned int, CeGuiString label)
    {
        // Do nothing
    }

    /**
     * @author JoSch
     * @date 08-03-2008
     * @version 1.0
     * @return Get the slot of the plaing sound.
     */
    unsigned int NullSoundStitching::getPlayingSlot()
    {
        return 0;
    }

} // Namespace
