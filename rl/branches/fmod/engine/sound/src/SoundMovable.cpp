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
    EventCaster<SoundEvent>()
{
    mName = name;
    mSoundResource = SoundResourcePtr(dynamic_cast<SoundResource*>
        (SoundManager::getSingleton().getByName(name).getPointer()));

    // ein paar Verknuepfungen
    addEventListener(this);

    /// Ein paar Standardwerte setzen
    setGain(1.0f);
    setPosition(Vector3(0.0, 0.0, 0.0));
    setVelocity(Vector3(0.0, 0.0, 0.0));
    setDirection(Vector3(0.0, 0.0, 0.0));
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
    mSoundResource(soundres)
{
    if (!soundres.isNull())
    {
        mName = soundres->getName();
    }

    // ein paar Verknuepfungen
    addEventListener(this);

    /// Ein paar Standardwerte setzen
    setGain(1.0f);
    setPosition(Vector3(0.0, 0.0, 0.0));
    setVelocity(Vector3(0.0, 0.0, 0.0));
    setDirection(Vector3(0.0, 0.0, 0.0));
}

/**
 * @author JoSch
 * @date 03-12-2005
 */
SoundMovable::~SoundMovable()
{
    // Listener entfernen.
    removeEventListener(this);
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
bool SoundMovable::eventRaised(SoundEvent *anEvent) const
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
    // TODO
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
 * @date 07-23-2004
 */
const bool SoundMovable::isPlaying() const
{
    return true;
}

/**
 * @return Die aktuelle Position der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Vector3 SoundMovable::getPosition() const throw (RuntimeException)
{
    Vector3 result;
    // TODO
    return result;
}

/**
 * @param position Die neue Position der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundMovable::setPosition(const Vector3& position) throw (RuntimeException)
{
    // TODO
}

/**
 * @return Die aktuelle Geschwindigkeit der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Vector3 SoundMovable::getVelocity() const throw (RuntimeException)
{
    Vector3 result;
    // TODO
    return result;
}

/**
 * @param velocity Die neue Geschwindigkeit der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundMovable::setVelocity(const Vector3& velocity) throw (RuntimeException)
{
    // TODO
}

/**
 * @return Die aktuelle Lautstaerke der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const float SoundMovable::getGain() const throw (RuntimeException)
{
    float result;
    // TODO
    return result;
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundMovable::setGain(const float gain) throw (RuntimeException)
{
    // TODO
}

/**
 * @author JoSch
 * @date 09-15-2004
 */
void SoundMovable::play(unsigned int msec) throw (RuntimeException)
{
    // TODO
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void SoundMovable::pause() throw (RuntimeException)
{
    // TODO
}

/**
 * Abspielen stoppen.
 * @author JoSch
 * @date 07-27-2004
 */
void SoundMovable::stop(unsigned int msec) throw (RuntimeException)
{
    // TODO
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void SoundMovable::rewind() throw (RuntimeException)
{
    // TODO
}

void SoundMovablePtr::destroy()
{
    SharedPtr<SoundMovable>::destroy();
}

} // Namespace
