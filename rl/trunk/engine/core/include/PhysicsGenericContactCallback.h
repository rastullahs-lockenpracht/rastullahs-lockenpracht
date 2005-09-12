#ifndef __PHYSICSGENERICCONTACTCALLBACK_H__
#define __PHYSICSGENERICCONTACTCALLBACK_H__

#include <OgreNewt.h>
#include "CorePrerequisites.h"

namespace rl {

    class _RlCoreExport PhysicsGenericContactCallback : public OgreNewt::ContactCallback
    {
    public:
        int userBegin();
        int userProcess();
        void userEnd();
    private:
    };
}

#endif
