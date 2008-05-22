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

#include "SoundStitching.h"
#include "SoundDriver.h"

using namespace Ogre;

namespace rl
{

AxisAlignedBox SoundStitching::msAABox = Ogre::AxisAlignedBox(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

SoundStitching::SoundStitching(unsigned int numSlots, const CeGuiString& name, SoundDriver* creator)
:  MovableObject(name.c_str()),
   EventSource(),
   EventCaster<SoundEvent>(),
   mCreator(creator),
   mVolume(1.0),
   mName(name),
   mIsLooping(true),
   mIs3d(false),
   mTicks(0),
   mPosition(Vector3::ZERO),
   mDirection(Quaternion::IDENTITY),
   mVelocity(Vector3::ZERO),
   mPriority(128),
   mRolloffStartDistance(1.0),
   mRolloffEndDistance(10000.0),
   mNumSlots(numSlots)
{
}

SoundStitching::~SoundStitching()
{
}

/**
 * @return Ob der Sound wiederholt werden soll
 * @author JoSch
 * @date 08-08-2005
 */
bool SoundStitching::isLooping() const
{
    return mIsLooping;
}

/**
 * @param looping Ob der Sound wiederholt werden soll
 * @author JoSch
 * @date 08-08-2005
 */
void SoundStitching::setLooping(bool looping)
{
    mIsLooping = looping;
}

/**
 * @return Ob der Sound ein 3D-Sound ist.
 * @author JoSch
 * @date 08-08-2005
 */
bool SoundStitching::is3d() const
{
    return mIs3d;
}

/**
 * @param is3d Ob der Sound ein 3D-Sound ist.
 * @author JoSch
 * @date 08-08-2005
 */
void SoundStitching::set3d(bool is3d)
{
    mIs3d = is3d;
}

/** SoundResource zurückgeben.
 * @return Der Pointer auf die zugehörige Soundresource.
 * @author JoSch
 * @date 08-08-2005
 */
const SoundResourcePtr &SoundStitching::getSoundResource() const
{
    return mSoundResource;
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
const AxisAlignedBox& SoundStitching::getBoundingBox() const
{
    return msAABox;
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
Real SoundStitching::getBoundingRadius (void) const
{
    return 0.0f; // SQRT(3*SQR(0.0)) to fit with AABB
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
void SoundStitching::_updateRenderQueue(RenderQueue *queue)
{
    // Brauchen wir nicht
}

void SoundStitching::visitRenderables(Renderable::Visitor* visitor, 
	bool debugRenderables)
{
    // Brauchen wir nicht
}

void SoundStitching::setRolloffStartDistance(const Ogre::Real& distance)
{
	mRolloffStartDistance = distance;
}

const Ogre::Real SoundStitching::getRolloffStartDistance() const
{
	return mRolloffStartDistance;
}

void SoundStitching::setRolloffEndDistance(const Ogre::Real& distance)
{
	mRolloffEndDistance = distance;
}

const Ogre::Real SoundStitching::getRolloffEndDistance() const
{
	return mRolloffEndDistance;
}


void SoundStitching::addSoundToCache(CeGuiString label, SoundResourcePtr sound)
{
    mSoundCache[label] = sound;
}

unsigned int SoundStitching::getNumSlots()
{
    return mNumSlots;
}

float SoundStitching::getLength() const
{
    return 0.0f;
}

};

