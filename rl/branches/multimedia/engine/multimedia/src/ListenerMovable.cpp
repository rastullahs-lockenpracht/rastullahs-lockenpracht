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
#include "ListenerMovable.h"
#include <OgreVector3.h>
#include "MultimediaSubsystem.h"
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
    setOrientation(Vector3(0.0, 0.0, 1.0),
        Vector3(0.0, 1.0, 0.0));
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
    return mAt;
}

/**
 * @return Die aktuelle Richtung der Soundquelle (up-Vektor)
 * @author JoSch
 * @date 03-16-2005
 */
const Vector3 ListenerMovable::getOrientationUp() const throw (RuntimeException)
{
    return mUp;
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
    mUp = up;
    mAt = at;
    if (isActive())
    {
        float v[] = {at[0], at[1], at[2]},
            w[] =  {up[0], up[1], up[2]};
/* TODO         FSOUND_3D_Listener_SetAttributes(0, 0,
            at[0], at[1], at[2],
            up[0], up[1], up[2]); */
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
        float fx, fy, fz, tx, ty, tz;
/* TODO        FSOUND_3D_Listener_GetAttributes(0, 0,
            &fx, &fy, &fz, &tx, &ty, &tz);
        float newpos[] = {position[0], position[1], position[2]};
        FSOUND_3D_Listener_SetAttributes(newpos,
            0, fx, fy, fz, tx, ty, tz); */
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
        float fx, fy, fz, tx, ty, tz;
/* TODO        FSOUND_3D_Listener_GetAttributes(0, 0,
            &fx, &fy, &fz, &tx, &ty, &tz);
        float newvel[] = {velocity[0], velocity[1], velocity[2]};
        FSOUND_3D_Listener_SetAttributes(0, &newvel[0],
            fx, fy, fz, tx, ty, tz); */
    }
}

/**
 * @return Die aktuelle Lautstaerke der Soundquelle
 * @author JoSch
 * @date 03-16-2005
 */
const int ListenerMovable::getGain() const throw (RuntimeException)
{
    return mGain;
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 03-16-2005
 */
void ListenerMovable::setGain(const int gain) throw (RuntimeException)
{
    mGain = gain;
    if (isActive())
    {
        // TODO FSOUND_SetSFXMasterVolume(gain);
    }
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
