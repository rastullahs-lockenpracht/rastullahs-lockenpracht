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

#include "SelectionHelper.h"

#include "Actor.h"
#include "GameObject.h"
#include "PhysicalObject.h"
#include "PhysicsManager.h"

#ifdef __APPLE__
#   include <OgreNewt/OgreNewt.h>
#else
#   include <OgreNewt.h>
#endif

using namespace Ogre;
using namespace OgreNewt;

namespace rl
{
    bool SelectionHelper::checkLineOfSight(GameObject* go1, GameObject* go2)
    {
        // We require both GOs to exist, else LoS makes no sense.
        RlAssert1(go1 != NULL && go2 != NULL);

        if (!((go1->getState() & GOS_IN_SCENE) || (go2->getState() & GOS_IN_SCENE)))
        {
            // No LoS, if either GO is not in the scene.
            return false;
        }
        // Since both are in the scene, both should have Actors.
        Actor* actor1 = go1->getActor();
        Actor* actor2 = go2->getActor();
        RlAssert1(actor1 != NULL && actor2 != NULL);

        // Determine start position based on controlled object's type
        Ogre::Vector3 startPos = actor1->getWorldPosition();
        ActorControlledObject* obj = actor1->getControlledObject();
        PhysicalObject* physical = dynamic_cast<PhysicalObject*>(obj);
        if (physical)
        {
            // Take AABB center as reference point for physicals
            startPos += actor1->getWorldOrientation() *
                physical->getDefaultSize().getCenter();
        }

        // Determine target's position based on controlled object's type
        Ogre::Vector3 targetPos = actor2->getWorldPosition();
        obj = actor2->getControlledObject();
        physical = dynamic_cast<PhysicalObject*>(obj);
        if (physical)
        {
            // Take AABB center as reference point for physicals
            targetPos += actor2->getWorldOrientation() *
                physical->getDefaultSize().getCenter();
        }

        // Setup and execute raycast. Set result to be ordered by distance
        BasicRaycast raycast = BasicRaycast(PhysicsManager::getSingleton()._getNewtonWorld(),
            startPos, targetPos, true);

        // inspect results
        // the reference actor actor1 is filtered out
        // if the first of the other hit bodies is not the actor2, it is not in LoS.
        bool isVisible = true;
        for (int i = 0, num = raycast.getHitCount(); i < num; ++i)
        {
            OgreNewt::BasicRaycast::BasicRaycastInfo info = raycast.getInfoAt(i);
            OgreNewt::Body* body = info.mBody;
            if (body != NULL)
            {
#ifdef OGRENEWT_USE_OGRE_ANY
                Actor* hitActor = Ogre::any_cast<Actor*>(body->getUserData());
#else
                Actor* hitActor = static_cast<Actor*>(body->getUserData());
#endif
                isVisible = hitActor == actor2 || hitActor == actor1;
                if (!isVisible) break;
            }
        }
        return isVisible;
    }
}
