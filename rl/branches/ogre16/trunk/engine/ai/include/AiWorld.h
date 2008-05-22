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
#ifndef __RlAI_World_H__
#define __RlAI_World_H__

#include "OpenSteer/Obstacle.h"
#include "DebugVisualisable.h"
#include "SimpleVehicle.h"

#ifdef __APPLE__
#include <OgreNewt/OgreNewt.h>
#else
#include <OgreNewt.h>
#endif
#include <vector>

namespace OpenSteer
{
    class AbstractVehicle;
}

namespace rl
{
    typedef OpenSteer::AbstractProximityDatabase<OpenSteer::AbstractVehicle*> ProximityDatabase;
	/** Representation of the GameWorld for Steering and Pathing.
	 * Is a container for obstacles and vehicles that can be avoided.
     * Obstacles are static geometry or not moving entities,
     * Vehicles are dynamically moveable creatures / agents
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
		/** Adds an obstacle to the internal list of obstacles.
         * Agents will avoid these obstacles.
		 */
		void addObstacle(OpenSteer::Obstacle* obstacle);
		/** Removes and deletes all internally stored obstacle objects.
		 */
		void removeAllObstacles();

        /**
         * adds a Vehicle to the AiWorld. 
         * Vehicles are moving ojects in the world which Agents should avoid.
         */
        void addVehicle(SimpleVehicle* vehicle);
	private:
		//! list of obstacles
		OpenSteer::ObstacleGroup mObstacles;
        ProximityDatabase* mProximityCheck;
	};


    /** Wrapper für Newton-World
	 */
    class NewtonWorldAsObstacle : public OpenSteer::Obstacle,
        public DebugVisualisable
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
        
        // derived from debugvisualisable
        virtual DebugVisualisableFlag getFlag() const;
        virtual void updatePrimitive();
    protected:
        // derived from debugvisualisable
        virtual void doCreatePrimitive();
        typedef std::pair<Ogre::Vector3,Ogre::Vector3> Vector3Pair;
        mutable std::vector< Vector3Pair > mDebugRaycasts;

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
        std::vector<const OgreNewt::MaterialID *> mMaterialsToConsider;
		//! newton world
        OgreNewt::World *mNewtonWorld;
    };
}
#endif
