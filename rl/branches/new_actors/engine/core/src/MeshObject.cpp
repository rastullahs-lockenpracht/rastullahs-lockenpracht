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

#include "MeshObject.h"
#include "Actor.h"
#include "CoreSubsystem.h"
#include "World.h"
#include "AnimationManager.h"

using namespace Ogre;

namespace rl {
    MeshObject::MeshObject(const String& name, const String& meshname)
    {
        mMovableObject = CoreSubsystem::getSingletonPtr()->getWorld()
            ->getSceneManager()->createEntity(name, meshname);
    }

    Entity* MeshObject::getEntity()
    {
        return reinterpret_cast<Entity*>(mMovableObject);
    }

    ///@todo Sehr, sehr viel Optmimierungsspielraum :)
    Vector3 MeshObject::getSize()
    {
        Vector3 rval = Vector3::ZERO;

        const AxisAlignedBox& aab = getEntity()->getBoundingBox();
        Vector3 x = aab.getMaximum();
        Vector3 n = aab.getMinimum();
        Vector3 s(1.0, 1.0, 1.0);
        if (getActor() && getActor()->_getSceneNode())
        {
            s = getActor()->_getSceneNode()->getScale();
        }      
        rval.x = (x.x - n.x) * s.x;
        rval.y = (x.y - n.y) * s.y;
        rval.z = (x.z - n.z) * s.z;

        return rval;
    }

    void MeshObject::startAnimation(const Ogre::String& anim)
    {
        AnimationState* animState = getEntity()->getAnimationState(anim);
        AnimationManager::getSingleton().addAnimation(animState);
    }

    void MeshObject::stopAnimation(const Ogre::String& anim)
    {
        AnimationState* animState = getEntity()->getAnimationState(anim);
        AnimationManager::getSingleton().removeAnimation(animState);
    }
}
