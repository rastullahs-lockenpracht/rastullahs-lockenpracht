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

#include "SoundObject.h"
#include "Actor.h"
#include "SoundResource.h"
#include "Sound.h"
#include "SoundDriver.h"
#include "SoundManager.h"
#include "Logger.h"
#include "CoreSubsystem.h"
#include "World.h"

using namespace Ogre;

namespace rl {

/**
 * @param sound. Der Sound, Besitz wird bernommen.
 * @author JoSch
 * @date 03-11-2005
 */
SoundObject::SoundObject(Sound *sound, const Ogre::String &name)
    : ActorControlledObject(sound)
{
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
		SoundManager::getSingleton().getActiveDriver()->destroySound(getSound());
        mMovableObject = NULL;
    }
}

/**
 * @return Immer false, weil kein Meshobjekt.
 * @author JoSch
 * @date 03-11-2005
 */
bool SoundObject::isMeshObject() const
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

void SoundObject::play(bool destroyWhenDone)
{
    getSound()->play(destroyWhenDone);
    if (is3d())
    {
	   _update();
    }
}

void SoundObject::pause(bool pausing)
{
    getSound()->pause(pausing);
}

bool SoundObject::isPaused() const
{
    return getSound()->isPaused();
}

void SoundObject::stop()
{
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
        _update(); // do we need this here?
}

void SoundObject::load()
{
	getSound()->load();
}

void SoundObject::unload()
{
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
String SoundObject::getObjectType() const
{
    return "SoundObject";
}


void SoundObject::pause()
{
    pause(true);
}

void SoundObject::start()
{
    pause(false);
}

float SoundObject::getLength() const
{
	return getSound()->getLength();
}

// Sets the priority of this sound
void SoundObject::setPriority(const int priority)
{
    getSound()->setPriority(priority);
}

// Gets the priority of this sound
const int SoundObject::getPriority() const
{
    return getSound()->getPriority();
}

void SoundObject::setActive(bool active)
{
    pause(!active);
}

}
