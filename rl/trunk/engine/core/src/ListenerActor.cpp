/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#include "ListenerActor.h"

#include "CoreSubsystem.h"
#include "World.h"
extern "C" {
#include "AL/al.h"
#include "AL/alc.h"
}


template<> rl::ListenerActor* Singleton<rl::ListenerActor>::ms_Singleton = 0;

namespace rl {
 
/**
 * @return Das Singletonobjekt.
 * @author JoSch 
 * @date 07-23-2004
 */
ListenerActor& ListenerActor::getSingleton(void)
{
   return Singleton<ListenerActor>::getSingleton();
}

/**
 * @return Ein Zeiger auf das Singletonobjekt.
 * @author JoSch 
 * @date 07-23-2004
 */
ListenerActor* ListenerActor::getSingletonPtr(void)
{
   return Singleton<ListenerActor>::getSingletonPtr();
}

/**
 * @param name Der Name des Aktors
 * @author JoSch 
 * @date 07-23-2004
 */
ListenerActor::ListenerActor(const String& name)
    : Actor(name)
{
    alListener3f(AL_POSITION, 0.0, 0.0, 0.0);
    alListener3f(AL_VELOCITY, 0.0, 0.0, 0.0);
    ALfloat data[6] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};
    alListenerfv(AL_ORIENTATION, data);
}

/**
 * @param name Der Name des Aktors
 * @param pParentSceneNode Der Szenenknoten, der zu dem Aktor gehoert.
 * @author JoSch
 * @date 07-23-2004
 */
ListenerActor::ListenerActor(const String& name,
        SceneNode* pParentSceneNode)
        : Actor(name, pParentSceneNode)
{
    alListener3f(AL_POSITION, 0.0, 0.0, 0.0);
    alListener3f(AL_VELOCITY, 0.0, 0.0, 0.0);
    ALfloat data[6] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};
    alListenerfv(AL_ORIENTATION, data);
} 

/**
 * @author JoSch
 * @date 07-23-2004
 */
ListenerActor::~ListenerActor()
{
}

/**
 * @param velocity Die neue Geschwindigkeit.
 * @author JoSch
 * @date 07-23-2004
 */
void ListenerActor::setVelocity(const Vector3& velocity)
{
    alListener3f(AL_VELOCITY, velocity[0], velocity[1], velocity[2]);
}

/**
 * @return Die aktuelle Geschwindigkeit
 * @author JoSch
 * @date 07-23-2004
 */
const Vector3 ListenerActor::getVelocity() const
{
    Vector3 velocity;
    alGetListener3f(AL_VELOCITY, &velocity[0],
        &velocity[1], &velocity[2]);
    return velocity;
}

/**
 * @param x, y, z Die neue Position des Listeners.
 * @author JoSch
 * @date 07-23-2004
 */
void ListenerActor::setPosition(Real x, Real y, Real z)
{
    Actor::setPosition(x, y, z);
    alListener3f(AL_POSITION, x, y, z);    
}

/**
 * @param orientation Die neue Orientierung.
 * @author JoSch
 * @date 07-23-2004
 */
void ListenerActor::setOrientation(const Quaternion& orientation)
{
    Actor::setOrientation(orientation);
    
    ViewPoint vp = CoreSubsystem::getSingleton().getWorld()->
                   getSceneManager()->getSuggestedViewpoint(false);

    ALfloat data[6] = {
        orientation.x, orientation.y, orientation.z,
        vp.orientation.x, vp.orientation.y, vp.orientation.z};
    alListenerfv(AL_ORIENTATION, data);
}

bool ListenerActor::isSimpleOgreActor()
{
    return false;
}

 
}
