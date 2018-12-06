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
    void PhysicsGenericContactCallback::contactsProcess(
        OgreNewt::ContactJoint& contactJoint, Real timestep, int threadid)
    {
        userProcess(contactJoint, timestep, threadid);

        Actor* a1 = NULL;
        Actor* a2 = NULL;

        if (contactJoint.getBody0()->getUserData().getType() == typeid(Actor*))
            a1 = Ogre::any_cast<Actor*>(contactJoint.getBody0()->getUserData());
        if (contactJoint.getBody1()->getUserData().getType() == typeid(Actor*))
            a2 = Ogre::any_cast<Actor*>(contactJoint.getBody1()->getUserData());

        PhysicalThing* pt1 = NULL;
        PhysicalThing* pt2 = NULL;

        if (a1)
            pt1 = a1->getPhysicalThing();
        if (a2)
            pt2 = a2->getPhysicalThing();

        if (pt1 && pt1->getContactListener())
        {
            pt1->getContactListener()->contactOccured(pt1, pt2);
        }
        if (pt2 && pt2->getContactListener())
        {
            pt2->getContactListener()->contactOccured(pt1, pt2);
        }
    }
}
