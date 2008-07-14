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
    int PhysicsGenericContactCallback::userBegin()
    {
        return 1;
    }

    int PhysicsGenericContactCallback::userProcess(Real timeStep, int)
    {
        return 1;
    }

    void PhysicsGenericContactCallback::userEnd()
    {
        Actor* a1 = static_cast<Actor*>(m_body0->getUserData());
        Actor* a2 = static_cast<Actor*>(m_body1->getUserData());
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
