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
#ifndef PHYSICALOBSTACLE_H_
#define PHYSICALOBSTACLE_H_

#include "AiPrerequisites.h"

namespace OpenSteer
{
    class Obstacle;
}

namespace rl
{
    class PhysicalThing;

    /** Class connecting OpenSteer::Obstacle and rl::PhysicalThing.
     */
    class _RlAiExport PhysicalObstacle
    {

    public:
        /** Constructor
         * @param thing PhysicalThing object to be connected with a OpenSteer::Obstacle
         */
        PhysicalObstacle(PhysicalThing* thing);
        /** explicit default destructor
         */
        ~PhysicalObstacle();
        /** returns the associated OpenSteer::Obstacle.
         */
        OpenSteer::Obstacle* getObstacle() const;

        /** returns the associated PhysicalThing.
         */
        PhysicalThing* getPhysicalThing() const;
        /** stores an other PhysicalThing internally.
         * @param thing PhysicalThing object to be connected with a OpenSteer::Obstacle
         */
        void setPhysicalThing(PhysicalThing* thing);

    protected:
        /** Initializes the opensteer obstacle by utilizing the PhysicalThing
         * Creates a Box Obstacle representing the PhysicalThings Collision primitiv
         * for OpenSteer.
         */
        void _update();

    private:
        /** Sets the obstacle
         * @param obstacle OpenSteer::Obstacle representing the internally stored
         * PhysicalThing.
         */
        void setObstacle(OpenSteer::Obstacle* obstacle);

        //! collision object for OpenSteer
        OpenSteer::Obstacle* mObstacle;
        //! object used for the collision object for opensteer
        PhysicalThing* mPhysicalThing;
    };
}

#endif /*PHYSICALOBSTACLE_H_*/
