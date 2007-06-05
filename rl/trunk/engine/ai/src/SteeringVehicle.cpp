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
#include "SteeringVehicle.h"
#include "AiSubsystem.h"
#include "AiWorld.h" 
#include "Agent.h"
#include "Actor.h"
#include "Creature.h"
#include "MeshObject.h"
#include "MovingCreature.h"
#include "PhysicalThing.h"

using namespace Ogre;
using namespace OpenSteer;

namespace rl {

SteeringVehicle::SteeringVehicle(Agent* parent, Creature* character)
	: _maxForce(1.0f),
      _maxSpeed(1.0f),
      //mMass(),
      //mRadius(),
      mSpeed(1.0f),
	  mCurrentForce(Vector3::ZERO), 
	  mCurrentVelocity(Vector3::ZERO),
	  mForwardVector(Vector3::NEGATIVE_UNIT_Z),
      //mYaw(115),
	  mParent(parent),
	  mCreature(character),
      mMovingCreature(NULL)
      //mHeight(0)
{
	initialize();

    createMovingCreature();
}

SteeringVehicle::~SteeringVehicle(void)
{
    destroyMovingCreature();
}

void SteeringVehicle::createMovingCreature()
{
    if( mMovingCreature == NULL )
    {
        mMovingCreature = new MovingCreature(mCreature);
    }
}

void SteeringVehicle::destroyMovingCreature()
{
    if( mMovingCreature != NULL )
        delete mMovingCreature;
}

void SteeringVehicle::resetLocalSpace()
{
	setForward(Vec3(mForwardVector.x, mForwardVector.y, mForwardVector.z));
	setSide(localRotateForwardToSide(forward()));
	setUp(OpenSteer::Vec3(0, 1, 0));
	Vector3 pos = mCreature->getActor()->getPosition();
	setPosition(Vec3(pos.x, pos.y, pos.z));
	Vector3 src = mCreature->getActor()->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
	//Quaternion orientation = mCreature->getActor()->getOrientation();
	//mYaw = orientation.getYaw();

//  regenerate local space (by default: align vehicle's forward axis with
//  new velocity, but this behavior may be overridden by derived classes.)
	regenerateOrthonormalBasisUF ( Vec3(src.x, src.y, src.z) );
}

void SteeringVehicle::initialize(void)
{
//  reset LocalSpace state
	resetLocalSpace();
	
//	mCreature->getActor()->_getSceneNode()->setOrientation(Ogre::Quaternion::IDENTITY);
	//Vector3 inertia;
	//mCreature->getActor()->getPhysicalThing()->_getBody()->getMassMatrix(mMass, inertia);

//	mSpeed = (float)mCreature->getWert(Creature::WERT_GS) / (float)Date::ONE_KAMPFRUNDE * 1000.0f;

//  reset SteerLibraryMixin state
	SimpleVehicle_2::reset ();

//	setRadius (0.5f);     // size of bounding sphere

	setMaxForce (1.0f);   // steering force is clipped to this magnitude
	setMaxSpeed (1.0f);   // velocity is clipped to this magnitude

//  reset bookkeeping to do running averages of these quanities
//	resetSmoothedPosition ();
//	resetSmoothedCurvature ();
//	resetSmoothedAcceleration ();
}
/*
PerceptionPool* SteeringVehicle::getPerceptionPool()
{
  return mParent->getPerceptionPool();
}
*/
bool SteeringVehicle::isDialogActive()
{
	return mParent->isDialogActive();
}

void SteeringVehicle::addForce(const Ogre::Vector3& force)
{
	mCurrentForce += force;
}

void SteeringVehicle::update(const float currentTime, const float elapsedTime)
{
// from PlayerVehicle
// since physics schould by handled by movingcreature

	OgreNewt::Body* body = mCreature->getActor()->getPhysicalThing()->_getBody();

    Vector3 position;
	Quaternion orientation;
	body->getPositionOrientation(position, orientation);
	setPosition(Vec3(position.x, position.y, position.z));

    //  Get the velocity vector
	mCurrentVelocity = body->getVelocity();
	//  enforce speed limit
	//  newVelocity = newVelocity.truncateLength (maxSpeed ());
	//  update speed
	setSpeed(mCurrentVelocity.length());
	Vec3 newVelocity(mCurrentVelocity.x, mCurrentVelocity.y, mCurrentVelocity.z);

    //  regenerate local space (by default: align vehicle's forward axis with
    //  new velocity, but this behavior may be overridden by derived classes.)
	if (speed() > 0) regenerateOrthonormalBasisUF (newVelocity / speed());

// end of inserting from playervehicle



// only process if mMovingCreature not NULL
    if( mMovingCreature == NULL )
    {
        mCurrentForce = Vector3::ZERO;
        return;
    }
    
// calculate the result of the force    
    Vector3 result = mCurrentForce;// + mCurrentVelocity;

// @todo remove this
if( mCreature->getAu() <= 6 )
    mCreature->modifyAu(20,true);


    AbstractMovement *mov_drehen = mMovingCreature->getMovementFromId(MovingCreature::MT_DREHEN);
    Real vel_drehen(0);
    Radian max_drehen = Degree(0);
    if( mov_drehen->calculateBaseVelocity(vel_drehen) )
    {
        max_drehen = Degree(vel_drehen * 360 * elapsedTime);
    }

    Ogre::Quaternion future_orientation(mMovingCreature->getYaw(), Ogre::Vector3::UNIT_Y);
    Ogre::Vector3 creatureDirection = future_orientation * Ogre::Vector3::NEGATIVE_UNIT_Z;
    Radian yaw(0);
    creatureDirection.y = result.y = 0;
    yaw = creatureDirection.getRotationTo(result, Ogre::Vector3::UNIT_Y).getYaw();
    if( yaw > Radian(0) && yaw > max_drehen )
        yaw = max_drehen;
    if( yaw < Radian(0) && yaw < -max_drehen )
        yaw = -max_drehen;
    // old version was -Degree(mCurrentForce.x * 60 * elapsedTime);
    // should this really depend from the timestep!?!

    Ogre::Vector3 direction(Ogre::Vector3::ZERO);
    Ogre::Vector3 rotation(0,yaw.valueRadians(),0);
    MovingCreature::MovementType movement = MovingCreature::MT_STEHEN;
    if( result != Ogre::Vector3::ZERO )
    {
        direction.z = -1;
        movement = MovingCreature::MT_GEHEN;
    }


    mMovingCreature->setMovement(movement, direction, rotation);

	mCurrentForce = Ogre::Vector3::ZERO;
}

Vector3 SteeringVehicle::calcWander(const float elapsedTime)
{
	Vec3 rVal(mForwardVector.x, mForwardVector.y, mForwardVector.z);
	rVal += steerForWander(elapsedTime).setYtoZero();
	return Vector3(rVal.x, rVal.y, rVal.z);
}

Vector3 SteeringVehicle::calcSeek(const Vector3& target)
{
	Vec3 rVal = steerForSeek(Vec3(target.x, target.y, target.z)).setYtoZero();
	rVal = rVal.normalize();
	//rVal *= -0.1; //adjustment for newton
	//rVal.z=0;
	return Vector3(rVal.x, rVal.y, rVal.z);
}

Vector3 SteeringVehicle::calcFlee(const Vector3& target)
{
	Vec3 rVal = steerForFlee(Vec3(target.x, target.y, target.z)).setYtoZero();
	rVal *= -0.1;
	return Vector3(rVal.x, rVal.y, rVal.z);
}

Vector3 SteeringVehicle::calcPursuit(Agent* agent)
{
	Vec3 rVal = steerForPursuit(*(agent->getVehicle())).setYtoZero();
	rVal = rVal.normalize();
	rVal.z = 0;
	return Vector3(rVal.x, rVal.y, rVal.z);
}

Vector3 SteeringVehicle::calcAvoidObstacles(const float minTimeToCollision)
{
    ObstacleGroup obstacles = getObstacles();
	Vec3 rVal = steerToAvoidObstacles(minTimeToCollision, obstacles).setYtoZero();
	if(rVal.x != 0.0f || rVal.z != 0.0f)
	{
		rVal = rVal;
	}
	rVal = rVal.normalize();
	//rVal *= 0.0001;
	return Vector3(rVal.x, rVal.y, rVal.z);
}

Vector3 SteeringVehicle::calcAvoidNeighbors(const float minTimeToCollision)
{
	Vec3 rVal = steerToAvoidNeighbors(minTimeToCollision, getNeighbors()).setYtoZero();
	if(rVal.x != 0.0f || rVal.z != 0.0f)
	{
		rVal = rVal;
	}
//	rVal *= -1;
	return Vector3(rVal.x, rVal.y, rVal.z);
}
		
Vector3 SteeringVehicle::calcSteerTargetSpeed(const float targetSpeed)
{
	return Vector3();
}

bool SteeringVehicle::isAhead(Agent* agent, const float threshold)
{
	Vector3 target = agent->getVehicle()->getPosition();
	//target.y = position.y;
	return (SimpleVehicle_2::isAhead(Vec3(target.x, target.y, target.z), threshold));
}

bool SteeringVehicle::needAvoidance(const float minTimeToCollision)
{
	Vector3 rVal = calcAvoidNeighbors(minTimeToCollision) + calcAvoidObstacles(minTimeToCollision);
	if(rVal == Vector3::ZERO)
	{
		return false;
	}
	return true;
}

/*
// already in moveingcreature
void SteeringVehicle::setAnimation(const CeGuiString& name)
{
	MeshObject* mesh = dynamic_cast<MeshObject*>(mCreature->getActor()->getControlledObject());
	mesh->stopAllAnimations();
    try
    {
	    mesh->startAnimation(name.c_str());
	    mCreature->getActor()->getPhysicalThing()->fitToPose(name.c_str());
    }
    catch( ... ) { }
}
*/


AVGroup SteeringVehicle::getNeighbors() const
{
	AVGroup group;
	AgentManager::VehicleList list = AgentManager::getSingleton().getNeighbors(NULL);
	AgentManager::VehicleList::const_iterator itr = list.begin();
	for(; itr != list.end(); ++itr)
	{
		if( (*itr) != this )
		{
			group.push_back( (*itr) );
		}
	}
	return group;
}

float SteeringVehicle::calcDistance(const Vector3& vec1, const Vector3& vec2)
{
	Vector3 vec = vec1-vec2;
	return vec.length();
}

Vector3 SteeringVehicle::getPosition()
{
	return Vector3(position().x, position().y, position().z);
}

float SteeringVehicle::mass (void) const 
{
    return mCreature->getActor()->getPhysicalThing()->getMass();
}

float SteeringVehicle::setMass (float m) 
{
	// don't set mass here TODO: throw exception

	return 1;
} 

float SteeringVehicle::speed (void) const 
{
	return mSpeed;
}

float SteeringVehicle::setSpeed (float s) 
{
	return mSpeed = s;
}

float SteeringVehicle::radius (void) const 
{
	// this is only the radius in x axis, but i think, this is the value that should be used here
    Ogre::AxisAlignedBox aab = mCreature->getActor()->getPhysicalThing()->_getBody()->getCollision()->getAABB();
    return (aab.getMaximum().x - aab.getMinimum().x)/2;
}

float SteeringVehicle::setRadius (float m) 
{
	// don't set mass here TODO: throw exception
	return 1;
}

float SteeringVehicle::height (void) const 
{
    Ogre::AxisAlignedBox aab = mCreature->getActor()->getPhysicalThing()->_getBody()->getCollision()->getAABB();
    return aab.getMaximum().y - aab.getMinimum().y;
}

float SteeringVehicle::setHeight (float h) 
{
	// don't set mass here TODO: throw exception
	return 1;
}

const Actor* SteeringVehicle::getActor(void) const  
{ 
	return mCreature->getActor(); 
}

float SteeringVehicle::maxForce (void) const 
{
	return 10000.0f;
} 

float SteeringVehicle::setMaxForce (float mf) 
{
	 // TODO: should not be set here, throw excpetion or so
	return _maxForce = mf;
}

float SteeringVehicle::maxSpeed (void) const 
{
	return 100000; 
}

float SteeringVehicle::setMaxSpeed (float ms) 
{
	 // TODO: should not be set here, throw excpetion or so
	return _maxSpeed = ms;
}

const ObstacleGroup& SteeringVehicle::getObstacles() const
{
	return AiSubsystem::getSingleton().getWorld()->getSteeringObstacles();
}

Vec3 SteeringVehicle::predictFuturePosition(const float predictionTime) const
{
	//return position() + (velocity() * predictionTime);
	return velocity() * predictionTime;
}

Vec3 SteeringVehicle::adjustRawSteeringForce(const Vec3& force)
{
    const float maxAdjustedSpeed = 0.2f * maxSpeed ();

    if ((speed () > maxAdjustedSpeed) || (force == Vec3::zero))
    {
        return force;
    }
    else
    {
        const float range = speed() / maxAdjustedSpeed;
        // const float cosine = interpolate (pow (range, 6), 1.0f, -1.0f);
        // const float cosine = interpolate (pow (range, 10), 1.0f, -1.0f);
        // const float cosine = interpolate (pow (range, 20), 1.0f, -1.0f);
        // const float cosine = interpolate (pow (range, 100), 1.0f, -1.0f);
        // const float cosine = interpolate (pow (range, 50), 1.0f, -1.0f);
        const float cosine = interpolate (pow (range, 20), 1.0f, -1.0f);
        return limitMaxDeviationAngle (force, cosine, forward());
    }
}
/*
void SteeringVehicle::measurePathCurvature (const float elapsedTime)
{
    if (elapsedTime > 0)
    {
        const Vec3 dP = _lastPosition - position ();
        const Vec3 dF = (_lastForward - forward ()) / dP.length ();
        const Vec3 lateral = dF.perpendicularComponent (forward ());
        const float sign = (lateral.dot (side ()) < 0) ? 1.0f : -1.0f;
        _curvature = lateral.length() * sign;
        blendIntoAccumulator (elapsedTime * 4.0f,
                              _curvature,
                              _smoothedCurvature);
        _lastForward = forward ();
        _lastPosition = position ();
    }
}
*/

}
