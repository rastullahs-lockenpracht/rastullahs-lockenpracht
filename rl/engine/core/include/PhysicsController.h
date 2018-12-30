/* (C) 2003-2008. Team Pantheon. www.team-pantheon.de
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
 *  along with this program; if not you can get it here:
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm
 */
#ifndef __PHYSICSCONTROLLER_H__
#define __PHYSICSCONTROLLER_H__

#include "CorePrerequisites.h"

namespace rl
{
    class PhysicalThing;

    /** abstract baseclass for PhysicalObject with an external force/torque control.
     * Derived objects register with PhysicalManager. Whenever force/torque is
     * going to be applied to the object controlled by a PhysicsController, the
     * associated PhysicsControllers OnApplyForceAndTorque gets executed.
     * So the applied force/torque can be modified before it is applied to the
     * associated PhysicalThing.
     */
    class PhysicsController
    {
    public:
        /** explicit destructor.
         */
        virtual ~PhysicsController(){};

        /* abstract function for applying force and torque to a PhysicalThing object.
         * This function is called from the PhysicsManager whenever force/torque is
         * to be applied to the associated PhysicalThing object.
         * @param thing PhysicalThing to apply force and torque to.
         */
        virtual void OnApplyForceAndTorque(PhysicalThing* thing, float timestep) = 0;
    };
}

#endif
