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
#include "OalListener.h"

using namespace Ogre;

Ogre::String rl::OalListener::msMovableType = "OalListener";

namespace rl {
 

/**
 * @param name Der Name des Zuhoerers.
 * @author JoSch
 * @date 03-12-2005
 */
OalListener::OalListener(const String &name):
    ListenerMovable(name)
{
}
 
/**
 * @author JoSch
 * @date 03-12-2005
 */
OalListener::~OalListener()
{
}


/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& OalListener::getMovableType() const
{
    return msMovableType;
}



/**
 * @param orientation Die neue Richtung der Soundquelle.
 * @author JoSch
 * @date 03-16-2005
 */
void OalListener::setOrientation(const Quaternion &orientation)
{
    ListenerMovable::setOrientation(orientation);
    if (isActive())
    {
    }
}


/**
 * @param position Die neue Position der Soundquelle.
 * @author JoSch
 * @date 03-16-2005
 */
void OalListener::setPosition(const Vector3& position)
{
    ListenerMovable::setPosition(position);
    if (isActive())
    {
    }
}

/**
 * @param velocity Die neue Geschwindigkeit der Soundquelle.
 * @author JoSch
 * @date 03-16-2005
 */
void OalListener::setVelocity(const Vector3& velocity)
{
    mVelocity = velocity;
    if (isActive())
    {
    }
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 03-16-2005
 */
void OalListener::setGain(const int gain)
{
    ListenerMovable::setGain(gain);
    if (isActive())
    {
    }
}

}
