#include "PhysicsGenericContactCallback.h"

namespace rl
{
    int PhysicsGenericContactCallback::userBegin()
    {
        return 1;
    }

    int PhysicsGenericContactCallback::userProcess()
    {
        return 1;
    }

    void PhysicsGenericContactCallback::userEnd()
    {
    }
}
