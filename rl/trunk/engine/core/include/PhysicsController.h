#ifndef __PHYSICSCONTROLLER_H__
#define __PHYSICSCONTROLLER_H__

#include "CorePrerequisites.h"
#include "PhysicalThing.h"

namespace rl
{
	/** abstract baseclass for PhysicalObject with an external force/torque control.
	 * Derived objects register with PhysicalManager. Whenever force/torque is
	 * going to be applied to the an object controlled by a PhysicsController, the
	 * associated PhysicsControllers OnApplyForceAndTorque gets executed.
	 * So the applied force/torque can be modified before it is applied to the
	 * associated PhysicalThing.
	 */
    class PhysicsController
    {     
    public:
		/** explicit destructor.
		 */
        virtual ~PhysicsController() {};

		/* abstract function for applying force and torque to a PhysicalThing object.
		 * This function is called from the PhysicsManager whenever force/torque is
		 * to be applied to the associated PhysicalThing object.
		 * @param thing PhysicalThing to apply force and torque to.
		 */
        virtual void OnApplyForceAndTorque(PhysicalThing* thing) = 0;
    };
}

#endif
