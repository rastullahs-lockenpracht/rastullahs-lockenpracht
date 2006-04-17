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

#include "SoundObject.h"
#include "Actor.h"
#include "SoundResource.h"
#include "SoundManager.h"
#include <Ogre.h>
#include "Sound.h"
#include "SoundChannel.h"
#include "SoundDriver.h"
#include "MultimediaSubsystem.h"
#include "Logger.h"

using namespace Ogre;

namespace rl {
   
/**
 * @param sound. Der Sound, Besitz wird übernommen.
 * @author JoSch
 * @date 03-11-2005
 */   
SoundObject::SoundObject(Sound *sound, const Ogre::String &name)
    : ActorControlledObject()
{
	SoundDriver *driver = MultimediaSubsystem::getSingleton().getActiveDriver();
	if (driver != 0)
	{
    	SoundChannel *sc = driver->createChannel(sound, name);
        sc->addEventListener(this);
    	mMovableObject = dynamic_cast<MovableObject*>(sc);
	}
}

/**
 * @author JoSch
 * @date 03-11-2005
 */   
SoundObject::~SoundObject()
{
    if (mMovableObject)
    {
        SoundChannel *sc = dynamic_cast<SoundChannel*>(mMovableObject);
        if (sc)
        {
            sc->stop();
            sc->removeEventListener(this);
        }
        delete mMovableObject;
    }
}

/**
 * @return Immer false, weil kein Meshobjekt.
 * @author JoSch
 * @date 03-11-2005
 */   
bool SoundObject::isMeshObject()
{
	return false;
}

/**
 * @author JoSch
 * @date 03-11-2005
 */   
void SoundObject::_update()
{
    ActorControlledObject::_update();
    SoundChannel *channel = getSoundChannel();
    Actor *actor = getActor();
    if (!channel || !actor) // Einer ist Null
    {
        return;
    }
    if (!channel->isValid())
    {
        return;
    }
    if (isAttached())
    {
        channel->setPosition(actor->getWorldPosition());
// TODO        channel->setDirection(actor->getWorldOrientation()); 
       Logger::getSingleton().log("Core", LML_TRIVIAL, "Pos SoundObject: "
        + StringConverter::toString(actor->getWorldPosition().x) + " "
        + StringConverter::toString(actor->getWorldPosition().y) + " "
        + StringConverter::toString(actor->getWorldPosition().z));
    }
}

void SoundObject::play( )
{
    getSoundChannel()->play();
    if (is3d())
    {
	   _update();
    }
}

void SoundObject::pause(bool pausing)
{
    getSoundChannel()->pause(pausing);
}

bool SoundObject::isPaused()
{
    return getSoundChannel()->isPaused();
}

void SoundObject::stop()
{
    getSoundChannel()->stop();
}


bool SoundObject::isLooping() const
{
    return getSoundChannel()->isLooping();
}

void SoundObject::setLooping( bool looping )
{
    getSoundChannel()->setLooping( looping );
}

void SoundObject::setVolume(float volume)
{
	if (volume > 1.0)
		volume = 1.0;
	else if (volume < 0.0)
		volume = 0.0;

	getSoundChannel()->setVolume(int (volume * 100.0));
}

bool SoundObject::is3d() const
{
	return getSoundChannel()->is3d();
}

void SoundObject::set3d( bool is3d )
{
	getSoundChannel()->set3d(is3d);
}

void SoundObject::load()
{
	getSoundChannel()->load();
}

void SoundObject::unload()
{
    getSoundChannel()->unload();
}

/**
 * @param Der bewegbare Sound
 * @author JoSch
 * @date 03-11-2005
 */   
SoundChannel* SoundObject::getSoundChannel() const
{
    return reinterpret_cast<SoundChannel*>(mMovableObject);
}

/**
 * @return Immer "SoundObject"
 * @author JoSch
 * @date 03-11-2005
 */   
String SoundObject::getObjectType()
{
    return "SoundObject";
}

bool SoundObject::eventRaised(SoundEvent *event)
{
    dispatchEvent(event);
    return true;
}

void SoundObject::pause()
{
    pause(true);
}

void SoundObject::start()
{
    pause(false);
}

}
