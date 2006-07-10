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

#include "SoundChannel.h"
#include <OgreMovableObject.h>
#include <OgreAxisAlignedBox.h>
#include <OgreVector3.h>
#include "Sound.h"
#include "SoundDriver.h"

Ogre::AxisAlignedBox rl::SoundChannel::msAABox = Ogre::AxisAlignedBox(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5);

using namespace Ogre; 

namespace rl
{

SoundChannel::SoundChannel(SoundDriver* driver, Sound *sound, const Ogre::String &name)
 : MovableObject(),
   EventSource(),
   EventCaster<SoundEvent>(),
   mDriver(driver),
   mSound(sound),
   mName(name),
   mVolume(100)
{
}

SoundChannel::~SoundChannel()
{
    delete mSound;
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
    return 0.866f; // SQRT(3*SQR(0.5)) to fit with AABB
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
 * @return Ist der Kanal gueltig?
 * @author JoSch
 * @date 08-05-2005
 */
bool SoundChannel::isValid() const
{
    return mSound->isValid();
}


bool SoundChannel::isLooping() const
{
    return mSound->isLooping();
}

void SoundChannel::setLooping( bool looping )
{
    mSound->setLooping( looping );
}

Sound *SoundChannel::getSound() const
{
    return mSound;
}

void SoundChannel::load()
{
	mSound->load();
}

void SoundChannel::unload()
{
    mSound->unload();
}

bool SoundChannel::is3d() const
{
	return mSound->is3d();
}

void SoundChannel::set3d( bool is3d )
{
	mSound->set3d(is3d);
}

SoundDriver* SoundChannel::getDriver() const
{
	return mDriver;
}

};
