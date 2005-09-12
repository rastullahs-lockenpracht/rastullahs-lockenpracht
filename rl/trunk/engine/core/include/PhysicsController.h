#ifndef __PHYSICSCONTROLLER_H__
#define __PHYSICSCONTROLLER_H__

#include "CorePrerequisites.h"
#include "PhysicsGenericContactCallback.h"
#include "PhysicalThing.h"

namespace rl
{
    class PhysicsController : public PhysicsGenericContactCallback
    {
    public:
        virtual void OnApplyForceAndTorque(PhysicalThing* thing) = 0;
    };
}

#endif