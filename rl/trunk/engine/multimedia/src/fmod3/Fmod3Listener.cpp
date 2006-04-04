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
#ifdef WITH_FMOD3

#include "Fmod3Listener.h"
#include "MultimediaSubsystem.h"
extern "C" {
    #include <fmod.h>
    #include <fmod_errors.h>
}


using namespace Ogre;

Ogre::String rl::Fmod3Listener::msMovableType = "Fmod3Listener";

namespace rl {
 

/**
 * @param name Der Name des Zuhoerers.
 * @author JoSch
 * @date 03-12-2005
 */
Fmod3Listener::Fmod3Listener(const String &name):
    ListenerMovable(name)
{
}
 
/**
 * @author JoSch
 * @date 03-12-2005
 */
Fmod3Listener::~Fmod3Listener()
{
}


/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& Fmod3Listener::getMovableType() const
{
    return msMovableType;
}



/**
 * @param orientation Die neue Richtung der Soundquelle.
 * @author JoSch
 * @date 03-16-2005
 */
void Fmod3Listener::setOrientation(const Quaternion &orientation)
{
    ListenerMovable::setOrientation(orientation);
    if (isActive())
    {
		Vector3 at = orientation * Vector3::NEGATIVE_UNIT_Z;
		Vector3 up = orientation * Vector3::UNIT_Y;
        FSOUND_3D_Listener_SetAttributes(0, 0,
            at[0], at[1], at[2],
            up[0], up[1], up[2]);
    }
}


/**
 * @param position Die neue Position der Soundquelle.
 * @author JoSch
 * @date 03-16-2005
 */
void Fmod3Listener::setPosition(const Vector3& position)
{
    ListenerMovable::setPosition(position);
    if (isActive())
    {
        float fx, fy, fz, tx, ty, tz;
        FSOUND_3D_Listener_GetAttributes(0, 0,
            &fx, &fy, &fz, &tx, &ty, &tz);
        float newpos[] = {position[0], position[1], position[2]};
        FSOUND_3D_Listener_SetAttributes(newpos,
            0, fx, fy, fz, tx, ty, tz); 
    }
}

/**
 * @param velocity Die neue Geschwindigkeit der Soundquelle.
 * @author JoSch
 * @date 03-16-2005
 */
void Fmod3Listener::setVelocity(const Vector3& velocity)
{
    mVelocity = velocity;
    if (isActive())
    {
        float fx, fy, fz, tx, ty, tz;
        FSOUND_3D_Listener_GetAttributes(0, 0,
            &fx, &fy, &fz, &tx, &ty, &tz);
        float newvel[] = {velocity[0], velocity[1], velocity[2]};
        FSOUND_3D_Listener_SetAttributes(0, &newvel[0],
            fx, fy, fz, tx, ty, tz);
    }
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 03-16-2005
 */
void Fmod3Listener::setGain(const int gain)
{
    ListenerMovable::setGain(gain);
    if (isActive())
    {
        FSOUND_SetSFXMasterVolume(gain);
    }
}

}
#endif
