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
#include "MeshObject.h"
#include "PhysicalThing.h"

using namespace rl;

SteeringVehicle::SteeringVehicle(Agent* parent, Actor* character)
	: _maxForce(1.0f),
      _maxSpeed(1.0f),
      mMass(),
      mRadius(),
      mSpeed(1.0f),
	  mCurrentForce(Vector3::ZERO), 
	  mCurrentVelocity(Vector3::ZERO),
	  mForwardVector(Vector3::NEGATIVE_UNIT_Z),
      mYaw(115),
	  mParent(parent),
	  mActor(character)
{
	initialize();
}

SteeringVehicle::~SteeringVehicle(void)
{
}

void SteeringVehicle::resetLocalSpace()
{
	setForward(Vec3(mForwardVector.x, mForwardVector.y, mForwardVector.z));
	setSide(localRotateForwardToSide(forward()));
	setUp(OpenSteer::Vec3(0, 1, 0));
	Vector3 pos = mActor->getPosition();
	setPosition(Vec3(pos.x, pos.y, pos.z));
	Vector3 src = mActor->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
	Quaternion orientation = mActor->getOrientation();
	mYaw = orientation.getYaw();

//  regenerate local space (by default: align vehicle's forward axis with
//  new velocity, but this behavior may be overridden by derived classes.)
	regenerateOrthonormalBasisUF ( Vec3(src.x, src.y, src.z) );
}

void SteeringVehicle::initialize(void)
{
//  reset LocalSpace state
	resetLocalSpace();
	
//	mActor->_getSceneNode()->setOrientation(Ogre::Quaternion::IDENTITY);
	Vector3 inertia;
	mActor->getPhysicalThing()->_getBody()->getMassMatrix(mMass, inertia);

//	mSpeed = (float)mCreature->getWert(WERT_GS) / (float)Date::ONE_KAMPFRUNDE * 1000.0f;

//  reset SteerLibraryMixin state
	SimpleVehicle_2::reset ();

	setRadius (0.5f);     // size of bounding sphere

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
	OgreNewt::Body* body = mActor->getPhysicalThing()->_getBody();
//  get the charater mass
	Vector3 inertia;
	body->getMassMatrix(mMass, inertia);

//  apply gravity
	Vector3 force = mMass * PhysicsManager::getSingleton().getGravity();

//  Get the velocity vector
	mCurrentVelocity = body->getVelocity();

//  setSpeed(mCurrentVelocity.length());
	Vec3 newVelocity(mCurrentVelocity.x, mCurrentVelocity.y, mCurrentVelocity.z);
//  enforce speed limit
//  newVelocity = newVelocity.truncateLength(maxSpeed ());
//  update speed 
    setSpeed (newVelocity.length());

//  Gravity is applied above, so not needed here
//  prevent adding a counter force against gravity
	if (mCurrentVelocity.y < 0.0f) mCurrentVelocity.y = 0.0f;


//	Vec3 aforce = adjustRawSteeringForce(Vec3(mCurrentForce.x, mCurrentForce.y, mCurrentForce.z)); 
//	aforce = aforce.truncateLength (maxForce());
//	aforce *=-1; // need for newton ?
//	mCurrentForce = Vector3(aforce.x, aforce.y, aforce.z);
	Vector3 temp;
	Quaternion orientation;
	body->getPositionOrientation(temp, orientation);
	setPosition(Vec3(temp.x, temp.y, temp.z));
	
//	if(mCurrentForce.x != 0.0f)
//	{
	// Calculate angular velocity
		mYaw -= Degree(mCurrentForce.x * 60.0f * elapsedTime);

		while (mYaw.valueDegrees() > 360.0f) mYaw -= Degree(360.0f);
		while (mYaw.valueDegrees() < -360.0f) mYaw += Degree(360.0f);
	//  We first need the yaw rotation from actual yaw to desired yaw
		Vector3 src = orientation*Vector3::NEGATIVE_UNIT_Z;
		src.y = 0.0f;
		temp = Quaternion(mYaw, Vector3::UNIT_Y)*Vector3::NEGATIVE_UNIT_Z;
		temp.y = 0.0f;
		Radian yaw = src.getRotationTo(temp).getYaw();

	//  Calculate omega in order to go this rotation in mMaxDelay seconds.
		Real newOmega = yaw.valueRadians() / (1.0f/30.0f);
		body->setOmega(Vector3(0.0f, newOmega, 0.0f));

		mCurrentForce.x = 0.0f;
//	}
	if(elapsedTime > 0.0f)
	{
		force += mMass*(orientation * mCurrentForce - mCurrentVelocity) / elapsedTime;
	}
	body->setForce(force);

//  regenerate local space (by default: align vehicle's forward axis with
//  new velocity, but this behavior may be overridden by derived classes.)
	if (speed() > 0) regenerateOrthonormalBasisUF ( Vec3(temp.x, temp.y, temp.z) );

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
	rVal.z=0;
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

void SteeringVehicle::setAnimation(const CeGuiString& name)
{
	MeshObject* mesh = dynamic_cast<MeshObject*>(mActor->getControlledObject());
	mesh->stopAllAnimations();
    try
    {
	    mesh->startAnimation(name.c_str());
	    mActor->getPhysicalThing()->fitToPose(name.c_str());
    }
    catch( ... ) { }
}


AVGroup SteeringVehicle::getNeighbors()
{
	AVGroup group;
	AgentManager::VehicleList list = AgentManager::getSingleton().getNeighbors(NULL);
	AgentManager::VehicleList::iterator itr = list.begin();
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

ObstacleGroup SteeringVehicle::getObstacles()
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

