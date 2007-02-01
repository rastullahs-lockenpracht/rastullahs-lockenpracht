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

#include "SoundObject.h"
#include "Actor.h"
#include "SoundResource.h"
#include "SoundManager.h"
#include "Sound.h"
#include "SoundDriver.h"
#include "SoundManager.h"
#include "Logger.h"
#include "CoreSubsystem.h"
#include "World.h"

using namespace Ogre;

namespace rl {
   
/**
 * @param sound. Der Sound, Besitz wird übernommen.
 * @author JoSch
 * @date 03-11-2005
 */   
SoundObject::SoundObject(Sound *sound, const Ogre::String &name)
    : ActorControlledObject(sound),
	PlaylistObject(),
	EventListener<SoundEvent>()
{
    sound->addEventListener(this);
    // The movable has to be registered with the Ogre scene manager, since it is not
    // created by it.
    CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->injectMovableObject(sound);
}

/**
 * @author JoSch
 * @date 03-11-2005
 */   
SoundObject::~SoundObject()
{
    if (mMovableObject != NULL)
    {
        if (getSound()->isPlaying())
        {
            getSound()->stop();
        }
        getSound()->removeEventListener(this);
		SoundManager::getSingleton().getActiveDriver()->remove(getSound());
        delete mMovableObject;
        mMovableObject = NULL;
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
    Actor *actor = getActor();

    if (mMovableObject == NULL || actor == NULL) 
    {
        return;
    }

    if (!getSound()->isValid())
    {
        return;
    }

    if (isAttached())
    {
        getSound()->setPosition(actor->getWorldPosition());
        getSound()->setDirection(actor->getWorldOrientation()); 

       LOG_DEBUG(Logger::CORE, "Pos SoundObject: "
        + StringConverter::toString(actor->getWorldPosition().x) + " "
        + StringConverter::toString(actor->getWorldPosition().y) + " "
        + StringConverter::toString(actor->getWorldPosition().z));
    }
}

void SoundObject::play( )
{
    getSound()->play();
    if (is3d())
    {
	   _update();
    }
}

void SoundObject::pause(bool pausing)
{
    getSound()->pause(pausing);
}

bool SoundObject::isPaused()
{
    return getSound()->isPaused();
}

void SoundObject::stop()
{
	PlaylistObject::stop();
    getSound()->stop();
}


bool SoundObject::isLooping() const
{
    return getSound()->isLooping();
}

void SoundObject::setLooping( bool looping )
{
    getSound()->setLooping( looping );
}

void SoundObject::setVolume(Ogre::Real volume)
{
	getSound()->setVolume(volume);
}

bool SoundObject::is3d() const
{
	return getSound()->is3d();
}

void SoundObject::set3d( bool is3d )
{
	getSound()->set3d(is3d);
}

void SoundObject::load()
{
	PlaylistObject::load();
	getSound()->load();
}

void SoundObject::unload()
{
	PlaylistObject::unload();
    getSound()->unload();
}

/**
 * @param Der bewegbare Sound
 * @author JoSch
 * @date 03-11-2005
 */   
Sound* SoundObject::getSound() const
{
    return static_cast<Sound*>(mMovableObject);
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
	int newReason = PlaylistEvent::NOPEVENT;
	switch(event->getReason())
	{
		case SoundPlayEvent::PAUSEEVENT:
			newReason = PlaylistEvent::PAUSEEVENT;
			break;
		case SoundPlayEvent::STARTEVENT:
			newReason = PlaylistEvent::STARTEVENT;
			break;
		case SoundPlayEvent::STOPEVENT:
			newReason = PlaylistEvent::STOPEVENT;
			break;
		default:
			break;
	}
	PlaylistEvent newEvent = PlaylistEvent(event->getSource(), newReason);
    dispatchEvent(&newEvent);
    return true;
}

void SoundObject::pause()
{
	PlaylistObject::pause();
    pause(true);
}

void SoundObject::start()
{
	PlaylistObject::start();
    pause(false);
}

float SoundObject::getLength() const
{
	return getSound()->getLength();
}

}
