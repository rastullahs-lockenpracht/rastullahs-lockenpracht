/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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
#include "PhysicsManager.h"

namespace rl
{
	/**
	 * Representation of the GameWorld for Steering and Pathing
	 */
	class AiWorld
	{
	public:
		AiWorld(void);
		~AiWorld(void);

		OpenSteer::ObstacleGroup getSteeringObstacles();
		void addObstacle(OpenSteer::Obstacle* obstacle);
		void removeAllObstacles();
	private:
		OpenSteer::ObstacleGroup mObstacles;
	};





    // Wrapper für Newton-World
    class NewtonWorldAsObstacle : public OpenSteer::Obstacle
    {
    public:
        NewtonWorldAsObstacle(void)
        {
            mLevelMaterial = PhysicsManager::getSingleton()._getLevelMaterialID();
            mNewtonWorld = PhysicsManager::getSingleton()._getNewtonWorld();
        }
        virtual void findIntersectionWithVehiclePath (const OpenSteer::AbstractVehicle& vehicle,
                                              PathIntersection& pi) const;
    private:
        enum RaycastType
        {
            NONE = -1,
            MIDDLE = 0,
            RIGHT,
            LEFT,
            TOP,
            BOTTOM
        };
        OgreNewt::MaterialID *mLevelMaterial;
        OgreNewt::World *mNewtonWorld;
    };
}
#endif
