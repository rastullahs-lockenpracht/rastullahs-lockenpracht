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

#include "PhysicsGenericContactCallback.h"

#include "Actor.h"
#include "PhysicalThing.h"


using namespace Ogre;

namespace rl
{
    void PhysicsGenericContactCallback::contactsProcess(OgreNewt::ContactJoint &contactJoint, Real timestep, int threadid)
    {
        userProcess(contactJoint, timestep, threadid);

        Actor* a1 = NULL;
        Actor* a2 = NULL;

#ifdef OGRENEWT_USE_OGRE_ANY
        try
        {
            a1 = Ogre::any_cast<Actor*>(contactJoint.getBody0()->getUserData());
        }
        catch(...)
        {
            LOG_WARNING(Logger::CORE, "Found collision with a OgreNewt::Body that doesn't have an Actor as UserData in PhysicsGenericContactCallback::contactsProcess");
        }
        try
        {
            a2 = Ogre::any_cast<Actor*>(contactJoint.getBody1()->getUserData());
        }
        catch(...)
        {
            LOG_WARNING(Logger::CORE, "Found collision with a OgreNewt::Body that doesn't have an Actor as UserData in PhysicsGenericContactCallback::contactsProcess");
        }
#else
        a1 = static_cast<Actor*>(contactJoint.getBody0()->getUserData());
        a2 = static_cast<Actor*>(contactJoint.getBody1()->getUserData());
#endif
        if (a1 && a1->getPhysicalThing()->getContactListener())
        {
            a1->getPhysicalThing()->getContactListener()->
                contactOccured(a1->getPhysicalThing(), a2->getPhysicalThing());
        }
        if (a2 && a2->getPhysicalThing()->getContactListener())
        {
            a2->getPhysicalThing()->getContactListener()->
                contactOccured(a2->getPhysicalThing(), a1->getPhysicalThing());
        }
    }
}
