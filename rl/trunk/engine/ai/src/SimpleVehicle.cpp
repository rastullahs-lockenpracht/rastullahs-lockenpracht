/* This source file is part of Rastullahs Lockenpracht.
 * Copyright(C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h"

#include "SimpleVehicle.h"
#include "PhysicalThing.h"
#include "Creature.h"
#include "CreatureController.h"
#include "CreatureControllerManager.h"

namespace rl
{

SimpleVehicle::SimpleVehicle()
    : mCreature(NULL)
{
}

SimpleVehicle::SimpleVehicle(Creature* creature)
    : mCreature(creature)
{
}

SimpleVehicle::~SimpleVehicle()
{
    if(mProximityToken != NULL)
    {
        delete mProximityToken;
    }
}

CreatureController* SimpleVehicle::getCreatureController() const
{ 
    return CreatureControllerManager::getSingleton().getCreatureController(mCreature);
}
        
const Actor* SimpleVehicle::getActor() const 
{ 
    return mCreature->getActor(); 
}

void SimpleVehicle::update(const float currentTime, const float elapsedTime)
{
    mProximityToken->updateForNewPosition(mCreature->getPosition());
}

float SimpleVehicle::getMass() const 
{
    return mCreature->getMass();
}

float SimpleVehicle::setMass(float m) 
{
	// don't set mass here TODO: throw exception
	return 1;
}

float SimpleVehicle::getRadius() const 
{
	// this is only the radius in x axis, but i think, this is the value that should be used here
    Ogre::AxisAlignedBox aab = mCreature->getActor()->getPhysicalThing()->_getBody()->getCollision()->getAABB();
    return(aab.getMaximum().x - aab.getMinimum().x)/2;
}

float SimpleVehicle::setRadius(float m) 
{
	// don't set mass here TODO: throw exception
	return 1;
}

Ogre::Vector3 SimpleVehicle::getVelocity() const
{
    return mCreature->getActor()->getPhysicalThing()->_getBody()->getVelocity();
}

float SimpleVehicle::getSpeed() const 
{
	return 1.0f;
}

float SimpleVehicle::setSpeed(float s) 
{
    // TODO: should not be set here, throw excpetion or so
	return 1.0f;
}

Vector3 SimpleVehicle::predictFuturePosition(const float predictionTime) const
{
	return mCreature->getPosition() + (getVelocity() * predictionTime);
}

float SimpleVehicle::getMaxForce() const 
{
	return 10000.0f;
} 

float SimpleVehicle::setMaxForce(float mf) 
{
	 // TODO: should not be set here, throw excpetion or so
	return 1.0f; //_maxForce = mf;
}

float SimpleVehicle::getMaxSpeed() const 
{
	return getCreatureController()->getMaximumSpeed();
}

float SimpleVehicle::setMaxSpeed(float ms) 
{
	 // TODO: should not be set here, throw excpetion or so
	return 1;
}

float SimpleVehicle::getHeight() const 
{
    Ogre::AxisAlignedBox aab = mCreature->getActor()->getPhysicalThing()->_getBody()->getCollision()->getAABB();
    return aab.getMaximum().y - aab.getMinimum().y;
}
/*
float SimpleVehicle::setHeight(float h) 
{
	// don't set mass here TODO: throw exception
	return 1;
}*/

}