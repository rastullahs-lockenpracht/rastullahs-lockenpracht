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
#include "MultimediaSubsystem.h"
extern "C" {
    #include <fmod.h>
}

Ogre::AxisAlignedBox rl::SoundChannel::msAABox = Ogre::AxisAlignedBox(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5);

using namespace Ogre; 

namespace rl
{

SoundChannel::SoundChannel(Sound *sound, const Ogre::String &name)
 : MovableObject(),
   mSound(sound),
   mName(name)
{
    
}

SoundChannel::~SoundChannel()
{
    if (mSound)
    {
        delete mSound;
    }
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
    setGain(255);
    setPosition(Vector3(0.0, 0.0, 0.0));
    setDirection(Vector3(0.0, 0.0, 0.0));
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
 * @return Ist der Kanal gueltig?
 * @author JoSch
 * @date 08-05-2005
 */
bool SoundChannel::isValid() const throw (RuntimeException)
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

bool SoundChannel::is3d() const
{
	return mSound->is3d();
}

void SoundChannel::set3d( bool is3d )
{
	mSound->set3d(is3d);
}



};
