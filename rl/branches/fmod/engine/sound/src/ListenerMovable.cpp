/* ListenerMovable.cpp - Diese Klassse kapselt einen Zuhoerer.
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
#include "ListenerMovable.h"
#include <OgreVector3.h>
#include "SoundSubsystem.h"
extern "C" {
    #include <fmod.h>
    #include <fmod_errors.h>
}


using namespace Ogre;

rl::ListenerMovable* rl::ListenerMovable::gActiveListener = 0;


namespace rl {
 
String ListenerMovable::msMovableType = "ListenerMovable";
AxisAlignedBox ListenerMovable::msAABox = AxisAlignedBox(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5);

/**
 * @param name Der Name des Zuhoerers.
 * @author JoSch
 * @date 03-12-2005
 */
ListenerMovable::ListenerMovable(const String &name):
    MovableObject()
{
    mName = name;

    /// Ein paar Standardwerte setzen
    setPosition(Vector3(0.0, 0.0, 0.0));
    setVelocity(Vector3(0.0, 0.0, 0.0));
    setOrientation(Vector3(0.0, 0.0, 0.0),
        Vector3(0.0, 0.0, 0.0));
}
 
/**
 * @author JoSch
 * @date 03-12-2005
 */
ListenerMovable::~ListenerMovable()
{
}

/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Namen
 */
const String& ListenerMovable::getName() const
{
    return mName;
}


/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& ListenerMovable::getMovableType() const
{
    return msMovableType;
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
void ListenerMovable::_notifyCurrentCamera(Camera *cam)
{
    // Brauchen wir nicht zu wissen.
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
const AxisAlignedBox& ListenerMovable::getBoundingBox() const 
{
    return msAABox;
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
Real ListenerMovable::getBoundingRadius (void) const
{
    return 0.0;
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
void ListenerMovable::_updateRenderQueue(RenderQueue *queue)
{
    // Brauchen wir nicht
}

/**
 * @return Die aktuelle Richtung der Soundquelle (at-Vektor)
 * @author JoSch
 * @date 03-16-2005
 */
const Vector3 ListenerMovable::getOrientationAt() const throw (RuntimeException)
{
    return mOrientationAt;
}

/**
 * @return Die aktuelle Richtung der Soundquelle (up-Vektor)
 * @author JoSch
 * @date 03-16-2005
 */
const Vector3 ListenerMovable::getOrientationUp() const throw (RuntimeException)
{
    return mOrientationUp;
}

/**
 * @param at Die neue Richtung der Soundquelle.
 * @param up Die neue Richtung der Soundquelle.
 * @author JoSch
 * @date 03-16-2005
 */
void ListenerMovable::setOrientation(const Vector3 &at,
        const Vector3 &up) throw (RuntimeException)
{
    mOrientationAt = at;
    mOrientationUp = up;
    if (isActive())
    {
        float v[] = {at[0], at[1], at[2],
            up[0], up[1], up[2]};
        //alListenerfv(AL_ORIENTATION, v);
        check();
    }
}

/**
 * @return Die aktuelle Position der Soundquelle
 * @author JoSch
 * @date 03-16-2005
 */
const Vector3 ListenerMovable::getPosition() const throw (RuntimeException)
{
    return mPosition;
}

/**
 * @param position Die neue Position der Soundquelle.
 * @author JoSch
 * @date 03-16-2005
 */
void ListenerMovable::setPosition(const Vector3& position) throw (RuntimeException)
{
    mPosition = position;
    if (isActive())
    {
        //alListener3f(AL_POSITION,
        //    position[0], position[1], position[2]);
        check();
    }
}

/**
 * @return Die aktuelle Geschwindigkeit der Soundquelle
 * @author JoSch
 * @date 03-16-2005
 */
const Vector3 ListenerMovable::getVelocity() const throw (RuntimeException)
{
    return mVelocity;
}

/**
 * @param velocity Die neue Geschwindigkeit der Soundquelle.
 * @author JoSch
 * @date 03-16-2005
 */
void ListenerMovable::setVelocity(const Vector3& velocity) throw (RuntimeException)
{
    mVelocity = velocity;
    if (isActive())
    {
        //alListener3f(AL_VELOCITY,
        //    velocity[0], velocity[1], velocity[2]);
        check();
    }
}

/**
 * @return Die aktuelle Lautstaerke der Soundquelle
 * @author JoSch
 * @date 03-16-2005
 */
const float ListenerMovable::getGain() const throw (RuntimeException)
{
    return mGain;
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 03-16-2005
 */
void ListenerMovable::setGain(const float gain) throw (RuntimeException)
{
    if (isActive())
    {
        //alListenerf(AL_GAIN, gain);
    }
    check();
}

/**
 * Ueberprueft, ob Fehler aufgetreten ist, ansonsten Exception.
 * @author JoSch
 * @date 03-16-2005
 */
void ListenerMovable::check() const throw (RuntimeException)
{
/*    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        String errormsg = (char*)alGetString(error);
        SoundSubsystem::log("Error: " + errormsg);
        Throw(RuntimeException, errormsg);
    } */
}

/**
 * @return TRUE, wenn der aktive Zuhoerer.
 * @author JoSch
 * @date 03-16-2005
 */
bool ListenerMovable::isActive() const
{
    return (this == ListenerMovable::gActiveListener);
}

/**
 * @return Aktiven Listener
 * @author JoSch
 * @date 03-16-2005
 */
ListenerMovable* ListenerMovable::getActiveListener()
{
    return gActiveListener;
}

/**
 * @author JoSch
 * @date 03-16-2005
 */
void ListenerMovable::setActive() throw (RuntimeException)
{
    gActiveListener = this;
    setPosition(getPosition());
    setGain(getGain());
    setVelocity(getVelocity());
    setOrientation(getOrientationAt(), getOrientationUp());
}

}
