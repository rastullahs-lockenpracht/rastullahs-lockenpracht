/* SoundMovable.cpp - Diese Klassse kapselt einen Sound.
 * (C) 2004. Team Pantheon. www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#include "SoundMovable.h"
#include "SoundManager.h"
#include <boost/thread.hpp>


using namespace Ogre;
using namespace boost;

namespace rl {
 
String SoundMovable::msMovableType = "SoundMovable";
AxisAlignedBox SoundMovable::msAABox = AxisAlignedBox(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5);

/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 03-12-2005
 */
SoundMovable::SoundMovable(const String &name):
    MovableObject(),
    EventListener<SoundEvent>(),
    EventSource(), 
    EventCaster<SoundEvent>(),
    mChannel(NO_CHANNEL),
    mIs3d(true)
{
    mName = name;
    mSoundResource = SoundResourcePtr(
        SoundManager::getSingleton().getByName(name));

    // ein paar Verknuepfungen
    addEventListener(this);
}
 
/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 03-12-2005
 */
SoundMovable::SoundMovable(const SoundResourcePtr &soundres):
    MovableObject(),
    EventListener<SoundEvent>(),
    EventSource(), 
    EventCaster<SoundEvent>(),
    mSoundResource(soundres),
    mChannel(NO_CHANNEL),
    mIs3d(true)
{
    if (!soundres.isNull())
    {
        mName = soundres->getName();
    }

    // ein paar Verknuepfungen
    addEventListener(this);
}

/**
 * @author JoSch
 * @date 03-12-2005
 */
SoundMovable::~SoundMovable()
{
    // Listener entfernen.
    removeEventListener(this);
    mSoundResource->unload();
}

/**
 * @author JoSch
 * @date 07-23-2005
 */
void SoundMovable::play() throw (RuntimeException)
{
    if (!isValid())
    {
        load();
    }
    pause(false);
}

/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Namen
 */
const String& SoundMovable::getName() const
{
    return mName;
}

/**
 * @author JoSch
 * @date 07-04-2005
 * @return Der Soundkanal
 */
const signed int SoundMovable::getChannel() const
{
    return mChannel;
}

/**
 * @author JoSch
 * @date 07-21-2005
 * @param channel Der Soundkanal
 */
void SoundMovable::setChannel(signed int channel)
{
    mChannel = channel;
}

/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& SoundMovable::getMovableType() const
{
    return msMovableType;
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
void SoundMovable::_notifyCurrentCamera(Camera *cam)
{
    // Brauchen wir nicht zu wissen.
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
const AxisAlignedBox& SoundMovable::getBoundingBox() const 
{
    return msAABox;
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
Real SoundMovable::getBoundingRadius (void) const
{
    return 0.0;
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
void SoundMovable::_updateRenderQueue(RenderQueue *queue)
{
    // Brauchen wir nicht
}

/**
 * @return Wenn false, wird das Ereignis nicht mehr weitergereicht.
 * @author JoSch
 * @date 10-14-2004
 */
bool SoundMovable::eventRaised(SoundEvent *anEvent)
{
#if 0
    SoundSubsystem::log("Event raised. Reason: " + StringConverter::toString(anEvent->getReason()));
    SoundSubsystem::log("Event raised. Source: " + StringConverter::toString((unsigned long int)anEvent->getSource()));
    SoundTimingEvent *e = dynamic_cast<SoundTimingEvent*>(anEvent);
    if (e)
    {
        SoundSubsystem::log("Timing: " + StringConverter::toString((float)e->mTime));
    }
#endif
    return true;
}

/**
 * @return Die aktuelle Richtung der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Vector3 SoundMovable::getDirection() const throw (RuntimeException)
{
    Vector3 result;
    return result;
}

/**
 * @param direction Die neue Richtung der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundMovable::setDirection (const Vector3& direction) throw (RuntimeException)
{
    // TODO
}

/**
 * @return Spielt die Soundquelle noch?
 * @author JoSch
 * @date 07-04-2005
 */
const bool SoundMovable::isPlaying() const
{
    if (isValid())
    {
        return FSOUND_IsPlaying(getChannel());
    }
    return false;
}

/**
 * @return Die aktuelle Position der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 SoundMovable::getPosition() const throw (RuntimeException)
{
    float pos[3];
    if (isValid())
    {
        FSOUND_3D_GetAttributes(getChannel(), pos, 0);
    }
    Vector3 result(pos);
    return result;
}

class SoundSampleMovable;
/**
 * @param position Die neue Position der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void SoundMovable::setPosition(const Vector3& position) throw (RuntimeException)
{
    if (isValid())
    {
        float pos[] = {position[0], position[1], position[2]};
        FSOUND_3D_SetAttributes(getChannel(), pos, 0);
    }
}

/**
 * @return Die aktuelle Geschwindigkeit der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const Vector3 SoundMovable::getVelocity() const throw (RuntimeException)
{
    float vel[3];
    if (isValid())
    {
        FSOUND_3D_GetAttributes(getChannel(), 0, vel);
    }
    Vector3 result(vel);
    return result;
}

/**
 * @param velocity Die neue Geschwindigkeit der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void SoundMovable::setVelocity(const Vector3& velocity) throw (RuntimeException)
{
    if (isValid())
    {
        float vel[] = {velocity[0], velocity[1], velocity[2]};
        FSOUND_3D_SetAttributes(getChannel(), 0, vel);
    }
}

/**
 * @return Die aktuelle Lautstaerke der Soundquelle
 * @author JoSch
 * @date 07-04-2005
 */
const int SoundMovable::getGain() const throw (RuntimeException)
{
    if (isValid())
    {
        return FSOUND_GetVolume(getChannel());
    }
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 07-04-2005
 */
void SoundMovable::setGain(const int gain) throw (RuntimeException)
{
    if (isValid())
    {
        FSOUND_SetVolume(getChannel(), gain);
    }
}

/**
 * @param pausing TRUE lässt den Sound unterbrechen.
 * @author JoSch
 * @date 07-04-2005
 */
void SoundMovable::pause(bool pausing) throw (RuntimeException)
{
    if (isValid())
    {
        FSOUND_SetPaused(getChannel(), pausing);
    }
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void SoundMovable::stop() throw (RuntimeException)
{
    if (isValid())
    {
        FSOUND_StopSound(getChannel());
    }
}

/**
 * @return TRUE wenn der Sound unterbrochen wurde.
 * @author JoSch
 * @date 07-04-2005
 */
bool SoundMovable::isPaused() throw (RuntimeException)
{
    if (isValid())
    {
        return FSOUND_GetPaused(getChannel());
    }
}

/**
 * @return TRUE wenn der Sound 3D sein soll.
 * @author JoSch
 * @date 07-12-2005
 */
bool SoundMovable::is3d() const
{
    return mIs3d;
}

/**
 * @param is3d, ob der Sound 3D sein soll.
 * @author JoSch
 * @date 07-12-2005
 */
void SoundMovable::set3d(bool is3d)
{
    mIs3d = is3d;
}

/**
 * @return der zugeordneten SoundResource.
 * @author JoSch
 * @date 07-12-2005
 */
SoundResourcePtr SoundMovable::getSoundResource() const
{
    return mSoundResource;
}


/**
 * @author JoSch
 * @date 07-23-2004
 */
/*void SoundMovable::rewind() throw (RuntimeException)
{
    // TODO
} */


void SoundMovablePtr::destroy()
{
    SharedPtr<SoundMovable>::destroy();
}

} // Namespace
