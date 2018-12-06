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

#include "Actor.h"
#include "CoreSubsystem.h"
#include "Logger.h"
#include "SoundDriver.h"
#include "SoundManager.h"
#include "SoundStitching.h"
#include "SoundStitchingObject.h"
#include "World.h"

using namespace Ogre;

namespace rl
{

    SoundStitchingObject::SoundStitchingObject(SoundStitching* stitching, const Ogre::String& name)
        : ActorControlledObject(stitching)
    {
        CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->injectMovableObject(stitching);
    }

    SoundStitchingObject::~SoundStitchingObject()
    {
    }

    /**
     * @return Immer false, weil kein Meshobjekt.
     * @author JoSch
     * @date 03-11-2005
     */
    bool SoundStitchingObject::isMeshObject() const
    {
        return false;
    }

    /**
     * @return Ob der Sound wiederholt werden soll
     * @author JoSch
     * @date 08-08-2005
     */
    bool SoundStitchingObject::isLooping() const
    {
        return getSoundStitching()->isLooping();
    }

    /**
     * @param looping Ob der Sound wiederholt werden soll
     * @author JoSch
     * @date 08-08-2005
     */
    void SoundStitchingObject::setLooping(bool looping)
    {
        getSoundStitching()->setLooping(looping);
    }

    /**
     * @return Ob der Sound ein 3D-Sound ist.
     * @author JoSch
     * @date 08-08-2005
     */
    bool SoundStitchingObject::is3d() const
    {
        return getSoundStitching()->is3d();
    }

    /**
     * @param is3d Ob der Sound ein 3D-Sound ist.
     * @author JoSch
     * @date 08-08-2005
     */
    void SoundStitchingObject::set3d(bool is3d)
    {
        getSoundStitching()->set3d(is3d);
    }

    void SoundStitchingObject::addSoundToCache(CeGuiString label, SoundResourcePtr sound)
    {
        getSoundStitching()->addSoundToCache(label, sound);
    }

    unsigned int SoundStitchingObject::getNumSlots()
    {
        return getSoundStitching()->getNumSlots();
    }

    float SoundStitchingObject::getLength() const
    {
        return 0.0f;
    }

    /**
     * @author JoSch
     * @date 09-03-2008
     */
    void SoundStitchingObject::_update()
    {
        ActorControlledObject::_update();
        Actor* actor = getActor();

        if (mMovableObject == NULL || actor == NULL)
        {
            return;
        }

        if (!getSoundStitching()->isValid())
        {
            return;
        }

        if (isAttached())
        {
            getSoundStitching()->setPosition(actor->getWorldPosition());
            getSoundStitching()->setDirection(actor->getWorldOrientation());

            LOG_DEBUG(Logger::CORE,
                "Pos SoundStitchingObject: " + StringConverter::toString(actor->getWorldPosition().x) + " "
                    + StringConverter::toString(actor->getWorldPosition().y) + " "
                    + StringConverter::toString(actor->getWorldPosition().z));
        }
    }

    /**
     * @return Always "SoundStitchingObject"
     * @author JoSch
     * @date 09-03-2008
     */
    String SoundStitchingObject::getObjectType() const
    {
        return "SoundStitchingObject";
    }

    void SoundStitchingObject::setActive(bool active)
    {
        pause(!active);
    }

    void SoundStitchingObject::play(bool destroyWhenDone)
    {
        getSoundStitching()->play(destroyWhenDone);
        if (is3d())
        {
            _update();
        }
    }

    void SoundStitchingObject::stop()
    {
        getSoundStitching()->stop();
    }

    /**
     * @param Die bewegbare Soundsequenz
     * @author JoSch
     * @date 09-03-2008
     */
    SoundStitching* SoundStitchingObject::getSoundStitching() const
    {
        return static_cast<SoundStitching*>(mMovableObject);
    }

    void SoundStitchingObject::pause(bool pausing)
    {
        getSoundStitching()->pause(pausing);
    }

    void SoundStitchingObject::pause()
    {
        pause(true);
    }

    bool SoundStitchingObject::isPaused()
    {
        return getSoundStitching()->isPaused();
    }

    const Ogre::Real SoundStitchingObject::getVolume() const
    {
        return getSoundStitching()->getVolume();
    }

    void SoundStitchingObject::setVolume(const Ogre::Real gain)
    {
        getSoundStitching()->setVolume(gain);
    }

    const int SoundStitchingObject::getPriority() const
    {
        return getSoundStitching()->getPriority();
    }

    void SoundStitchingObject::setPriority(const int priority)
    {
        getSoundStitching()->setPriority(priority);
    }

    void SoundStitchingObject::load()
    {
        getSoundStitching()->load();
    }

    void SoundStitchingObject::unload()
    {
        getSoundStitching()->unload();
    }

    void SoundStitchingObject::start()
    {
        pause(false);
    }
};
