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
#include "stdinc.h"

#include "Fmod4Listener.h"

using namespace Ogre;

namespace rl {

String rl::Fmod4Listener::msMovableType = "Fmod4Listener"; 

Fmod4Listener::Fmod4Listener(const Ogre::String &name, FMOD::System *fmod4system, int listenerId)
: ListenerMovable(name),
  mFmod4System(fmod4system),
  mListenerId(listenerId)
{
}

Fmod4Listener::~Fmod4Listener()
{
}

const String& Fmod4Listener::getMovableType() const
{
    return msMovableType;
}

void Fmod4Listener::setPosition(const Vector3& position)
{
    FMOD_VECTOR pos;
    pos.x = position.x;
    pos.y = position.y;
    pos.z = -position.z;
    mFmod4System->set3DListenerAttributes(mListenerId, &pos, NULL, NULL, NULL);
}

void Fmod4Listener::setOrientation(const Ogre::Quaternion &orientation)
{
    FMOD_VECTOR up, to;

    Vector3 upVec = orientation * Vector3::UNIT_Y;
    up.x = upVec.x;
    up.y = upVec.y;
    up.z = -upVec.z;

    Vector3 toVec = orientation * Vector3::NEGATIVE_UNIT_Z;
    to.x = toVec.x;
    to.y = toVec.y;
    to.z = -toVec.z;

    mFmod4System->set3DListenerAttributes(mListenerId, NULL, NULL, &to, &up);
}

void Fmod4Listener::setVelocity(const Ogre::Vector3& velocity)
{
    FMOD_VECTOR vel;
    vel.x = velocity.x;
    vel.y = velocity.y;
    vel.z = -velocity.z;
    mFmod4System->set3DListenerAttributes(mListenerId, NULL, &vel, NULL, NULL);
}

}
