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
#include "stdinc.h" //precompiled header

#include "Sound.h"
#include "SoundDriver.h"

using namespace Ogre;

namespace rl
{

    AxisAlignedBox Sound::msAABox = Ogre::AxisAlignedBox(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

    Sound::Sound(const SoundResourcePtr& soundres, SoundDriver* creator)
        : MovableObject(soundres->getName())
        , EventSource()
        , EventCaster<SoundEvent>()
        , mCreator(creator)
        , mVolume(1.0)
        , mSoundResource(soundres)
        , mIsLooping(false)
        , mIs3d(false)
        , mTicks(0)
        , mPosition(Vector3::ZERO)
        , mDirection(Quaternion::IDENTITY)
        , mVelocity(Vector3::ZERO)
        , mPriority(128)
        , mRolloffStartDistance(1.0)
        , mRolloffEndDistance(10000.0)
    {
    }

    Sound::~Sound()
    {
    }

    /**
     * @return Ob der Sound wiederholt werden soll
     * @author JoSch
     * @date 08-08-2005
     */
    bool Sound::isLooping() const
    {
        return mIsLooping;
    }

    /**
     * @param looping Ob der Sound wiederholt werden soll
     * @author JoSch
     * @date 08-08-2005
     */
    void Sound::setLooping(bool looping)
    {
        mIsLooping = looping;
    }

    /**
     * @return Ob der Sound ein 3D-Sound ist.
     * @author JoSch
     * @date 08-08-2005
     */
    bool Sound::is3d() const
    {
        return mIs3d;
    }

    /**
     * @param is3d Ob der Sound ein 3D-Sound ist.
     * @author JoSch
     * @date 08-08-2005
     */
    void Sound::set3d(bool is3d)
    {
        mIs3d = is3d;
    }

    /** SoundResource zurueckgeben.
     * @return Der Pointer auf die zugehoerige Soundresource.
     * @author JoSch
     * @date 08-08-2005
     */
    const SoundResourcePtr& Sound::getSoundResource() const
    {
        return mSoundResource;
    }

    /**
     * @author JoSch
     * @date 03-11-2005
     */
    const AxisAlignedBox& Sound::getBoundingBox() const
    {
        return msAABox;
    }

    /**
     * @author JoSch
     * @date 03-11-2005
     */
    Real Sound::getBoundingRadius(void) const
    {
        return 0.0f; // SQRT(3*SQR(0.0)) to fit with AABB
    }

    /**
     * @author JoSch
     * @date 03-11-2005
     */
    void Sound::_updateRenderQueue(RenderQueue* queue)
    {
        // Brauchen wir nicht
    }

    void Sound::setRolloffStartDistance(const Ogre::Real& distance)
    {
        mRolloffStartDistance = distance;
    }

    const Ogre::Real Sound::getRolloffStartDistance() const
    {
        return mRolloffStartDistance;
    }

    void Sound::setRolloffEndDistance(const Ogre::Real& distance)
    {
        mRolloffEndDistance = distance;
    }

    const Ogre::Real Sound::getRolloffEndDistance() const
    {
        return mRolloffEndDistance;
    }

    void Sound::visitRenderables(Renderable::Visitor* visitor, bool debugRenderables)
    {
    }
};
