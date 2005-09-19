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
#ifdef WITH_FMOD
#include "SoundChannel.h"
#endif
#ifdef WITH_OPENAL
#endif

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
#ifdef WITH_FMOD
    SoundChannel *sc = new SoundChannel(sound, name);
    mMovableObject = dynamic_cast<MovableObject*>(sc);
#endif
}

/**
 * @author JoSch
 * @date 03-11-2005
 */   
SoundObject::~SoundObject()
{
    if (mMovableObject)
    {
#ifdef WITH_FMOD
        SoundChannel *sc = dynamic_cast<SoundChannel*>(mMovableObject);
        if (sc)
        {
            sc->stop();
        }
#endif
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
#ifdef WITH_FMOD
    if (!channel->isValid())
    {
        return;
    }
    channel->setPosition(actor->getPosition());
    Vector3 *temp1 = new Vector3();
    Vector3 *temp2 = new Vector3(actor->getPosition());
    Real length = temp2->normalise();
    actor->getOrientation().ToAxes(temp1);
    *temp1 += *temp2;
    *temp1 *= length;
    channel->setDirection(*temp1); 
#endif
}

void SoundObject::play( )
{
#ifdef WITH_FMOD
    getSoundChannel()->play();
#endif
}

bool SoundObject::isLooping() const
{
#ifdef WITH_FMOD
    return getSoundChannel()->isLooping();
#endif
}

void SoundObject::setLooping( bool looping )
{
#ifdef WITH_FMOD    
    getSoundChannel()->setLooping( looping );
#endif
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

}
