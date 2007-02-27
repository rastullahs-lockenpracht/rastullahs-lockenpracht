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
 
 #include "PhysicalObstacle.h"
 #include "PhysicalThing.h"
 #include "Exception.h"
 #include <OpenSteer/Obstacle.h>
 #include <OgreNewt_Body.h>
 #include <OgreNewt_Collision.h>
 
using namespace Ogre;

namespace rl {
    PhysicalObstacle::PhysicalObstacle(PhysicalThing *thing)
    {
        setPhysicalThing(thing);
    }
     
    PhysicalObstacle::~PhysicalObstacle()
    {
    }
     
    OpenSteer::Obstacle *PhysicalObstacle::getObstacle() const
    {
        return mObstacle;
    }
     
    void PhysicalObstacle::setObstacle(OpenSteer::Obstacle *obstacle)
    {
        mObstacle = obstacle; // TODO Need update?
    }
    
    PhysicalThing *PhysicalObstacle::getPhysicalThing() const
    {
        return mPhysicalThing;
    }
    
    void PhysicalObstacle::setPhysicalThing(PhysicalThing *thing)
    {
        mPhysicalThing = thing;
        mObstacle = NULL;
        _update();
    }
    
    void PhysicalObstacle::_update()
    {
    	OgreNewt::Body *body = mPhysicalThing->_getBody();
    	RlAssert(body, "PhysicalThing has no body yet!");
        Vector3 position;
        Quaternion orientation;
        body->getPositionOrientation(position, orientation);

    	OgreNewt::CollisionPtr collision = body->getCollision();
    	RlAssert(collision.getPointer(), "Body has no collision!");
    	AxisAlignedBox box = collision->getAABB();
    	Ogre::Vector3 dims = box.getMaximum() - box.getMinimum();
    	OpenSteer::BoxObstacle *obstacle = new OpenSteer::BoxObstacle(dims[0], dims[1], dims[2]);
    	obstacle->setForward(0,0,-1);
    	obstacle->setPosition(position[0], position[1], position[2]);
    	//obstacle->setOrientation(orient[0], orient[1], orient[2]);
    	setObstacle(obstacle);
    }
}
