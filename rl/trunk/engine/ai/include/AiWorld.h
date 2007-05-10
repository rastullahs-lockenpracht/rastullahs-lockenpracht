/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#ifndef __RlAI_World_H__
#define __RlAI_World_H__

#include "OpenSteer/Obstacle.h"
#include <OgreNewt.h>

namespace rl
{
	/** Representation of the GameWorld for Steering and Pathing.
	 * Is a container for obstacles that have to be avoided when
	 * creating a path.
	 */
	class AiWorld
	{
	public:
		//! default constructor
		AiWorld(void);
		//! default destructor
		~AiWorld(void);

		/** Returns a list of obstacles for OpenSteer.
		 */
		const OpenSteer::ObstacleGroup& getSteeringObstacles();
		/** Adds an obstacle to the internal list of obstacles
		 */
		void addObstacle(OpenSteer::Obstacle* obstacle);
		/** Removes and deletes all internally stored obstacle objects.
		 */
		void removeAllObstacles();
	private:
		//! list of obstacles
		OpenSteer::ObstacleGroup mObstacles;
	};


    /** Wrapper für Newton-World
	 */
    class NewtonWorldAsObstacle : public OpenSteer::Obstacle
    {
    public:
		/** default constructor.
		 * Retrieves Material of newton level and the newton world.
		 */
        NewtonWorldAsObstacle(void);
		/** Searchs for pathintersections by ray casting.
		 * natoka: should be thought over
		 */
        virtual void findIntersectionWithVehiclePath (const OpenSteer::AbstractVehicle& vehicle,
                                              PathIntersection& pi) const;
    private:
		//! different types of raycasting directions
        enum RaycastType
        {
            NONE = -1,
            MIDDLE = 0,
            RIGHT,
            LEFT,
            TOP,
            BOTTOM
        };
		//! newton material id
        const OgreNewt::MaterialID *mLevelMaterial;
		//! newton world
        OgreNewt::World *mNewtonWorld;
    };
}
#endif
