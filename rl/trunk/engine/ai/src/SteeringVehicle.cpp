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
	: mSpeed(1.0f),
	  mCurrentForce(Vector3::ZERO), 
	  mCurrentVelocity(Vector3::ZERO),
	  mForwardVector(Vector3::NEGATIVE_UNIT_Z),
	  mParent(parent),
	  mActor(character)
{
	init();
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
}

void SteeringVehicle::init(void)
{
//  reset LocalSpace state
	resetLocalSpace();
	
	mActor->_getSceneNode()->setOrientation(Ogre::Quaternion::IDENTITY);
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

void SteeringVehicle::applySteeringForce(PhysicalThing* thing, const float elapsedTime)
{
	OgreNewt::Body* body = thing->_getBody();
	Vector3 position;
	Quaternion orientation;
	body->getPositionOrientation(position, orientation);
	setPosition(Vec3(position.x, position.y, position.z));
	// get the charater mass
	Vector3 inertia;
	body->getMassMatrix(mMass, inertia);

	// apply gravity
	Vector3 force = mMass * Vector3(0.0f, -9.81f, 0.0f);

	// Get the velocity vector
	mCurrentVelocity = body->getVelocity();
	mSpeed = mCurrentVelocity.length();
	// Gravity is applied above, so not needed here
	// prevent adding a counter force against gravity
	if (mCurrentVelocity.y < 0.0f) mCurrentVelocity.y = 0.0f;

	if(elapsedTime > 0.0f)
	{
		force += mMass*(orientation * mCurrentForce - mCurrentVelocity) / elapsedTime;
	}
	body->setForce(force);
	// Calculate angular velocity
	mYaw += Degree(mCurrentForce.x * 60.0 * elapsedTime);
	// We first need the yaw rotation from actual yaw to desired yaw
	Vector3 src = orientation*Vector3::NEGATIVE_UNIT_Z;
	src.y = 0;
	Vector3 dst = Quaternion(mYaw, Vector3::UNIT_Y)*Vector3::NEGATIVE_UNIT_Z;
	dst.y = 0;
	Radian yaw = src.getRotationTo(dst).getYaw();

	// Calculate omega in order to go this rotation in mMaxDelay seconds.
	Real newOmega = yaw.valueRadians() / (1.0/30.0);
	body->setOmega(Vector3(0, newOmega, 0));
	
	mCurrentForce = Ogre::Vector3::ZERO;
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

void SteeringVehicle::update(const float currentTime, const float elapsedTime)
{
	OgreNewt::Body* body = mActor->getPhysicalThing()->_getBody();
//	Vec3 aforce = adjustRawSteeringForce(Vec3(mCurrentForce.x, mCurrentForce.y, mCurrentForce.z)); 
//	aforce = aforce.truncateLength (maxForce());
//	aforce *=-1; // need for newton ?
//	mCurrentForce = Vector3(aforce.x, aforce.y, aforce.z);
	Vector3 position;
	Quaternion orientation;
	body->getPositionOrientation(position, orientation);
	setPosition(Vec3(position.x, position.y, position.z));
	// get the charater mass
	Vector3 inertia;
	body->getMassMatrix(mMass, inertia);

	// apply gravity
	Vector3 force = mMass * Vector3(0.0f, -9.81f, 0.0f);

	// Get the velocity vector
	mCurrentVelocity = body->getVelocity();
	setSpeed(mCurrentVelocity.length());
	Vec3 newVelocity(mCurrentVelocity.x, mCurrentVelocity.y, mCurrentVelocity.z);
	// enforce speed limit
//    newVelocity = newVelocity.truncateLength (maxSpeed ());
    // update Speed
//    setSpeed (newVelocity.length());
	// regenerate local space (by default: align vehicle's forward axis with
    // new velocity, but this behavior may be overridden by derived classes.)
    if (speed() > 0) regenerateOrthonormalBasisUF (newVelocity / speed());

	// Gravity is applied above, so not needed here
	// prevent adding a counter force against gravity
	if (mCurrentVelocity.y < 0.0f) mCurrentVelocity.y = 0.0f;

	//if (speed() > 0) regenerateOrthonormalBasisUF (Vec3(mCurrentVelocity.x, mCurrentVelocity.y, mCurrentVelocity.z));
//	mCurrentForce *= speed();

	if(elapsedTime > 0.0f)
	{
		force += mMass*(orientation * mCurrentForce - mCurrentVelocity) / elapsedTime;
	}
	body->setForce(force);
	// Calculate angular velocity
	mYaw -= Degree(mCurrentForce.x * 60.0 * elapsedTime);

	while (mYaw.valueDegrees() > 360.0f) mYaw -= Degree(360.0f);
	while (mYaw.valueDegrees() < -360.0f) mYaw += Degree(360.0f);
	// We first need the yaw rotation from actual yaw to desired yaw
	Vector3 src = orientation*Vector3::NEGATIVE_UNIT_Z;
	src.y = 0;
	Vector3 dst = Quaternion(mYaw, Vector3::UNIT_Y)*Vector3::NEGATIVE_UNIT_Z;
	dst.y = 0;
	Radian yaw = src.getRotationTo(dst).getYaw();

	// Calculate omega in order to go this rotation in mMaxDelay seconds.
	Real newOmega = yaw.valueRadians() / (1.0/30.0);
	body->setOmega(Vector3(0, newOmega, 0));
	
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
	rVal *= -0.1; //adjustment for newton
	return Vector3(rVal.x, rVal.y, rVal.z);
}

Vector3 SteeringVehicle::calcFlee(const Vector3& target)
{
	Vec3 rVal = steerForFlee(Vec3(target.x, target.y, target.z)).setYtoZero();
	rVal *= -0.1;
	return Vector3(rVal.x, rVal.y, rVal.z);
}

Vector3 SteeringVehicle::calcAvoidObstacles(const float minTimeToCollision)
{
	Vec3 rVal = steerToAvoidObstacles(minTimeToCollision, getObstacles());
	rVal *= -0.1;
	return Vector3(rVal.x, rVal.y, rVal.z);
}

Vector3 SteeringVehicle::calcAvoidNeighbors(const float minTimeToCollision)
{
	Vec3 rVal = steerToAvoidNeighbors(minTimeToCollision, getNeighbors()).setYtoZero();
	rVal *= -0.1;
	return Vector3(rVal.x, rVal.y, rVal.z);
}
		
Vector3 SteeringVehicle::calcSteerTargetSpeed(const float targetSpeed)
{
	return Vector3();
}

void SteeringVehicle::setAnimation(const CeGuiString& name)
{
	MeshObject* mesh = dynamic_cast<MeshObject*>(mActor->getControlledObject());
	mesh->stopAllAnimations();
	mesh->startAnimation(name.c_str());
	mActor->getPhysicalThing()->fitToPose(name.c_str());
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

/*Vec3 SteeringVehicle::adjustRawSteeringForce(const Vec3& force,
                                                  const float elapsedTime)
{
    const float maxAdjustedSpeed = 0.2f * maxSpeed ();

    if ((speed () > maxAdjustedSpeed) || (force == Vec3::zero))
    {
        return force;
    }
    else
    {
        const float range = speed() / maxAdjustedSpeed;
        const float cosine = interpolate (pow (range, 20), 1.0f, -1.0f);
        return limitMaxDeviationAngle (force, cosine, forward());
    }
}

void SteeringVehicle::applySteeringForce(const Vec3& force, const float elapsedTime)
{
	const Vec3 adjustedForce = adjustRawSteeringForce (force, elapsedTime);

    // enforce limit on magnitude of steering force
    const Vec3 clippedForce = adjustedForce.truncateLength (maxForce ());

    // compute acceleration and velocity
    Vec3 newAcceleration = (clippedForce / mass());
    Vec3 newVelocity = velocity();

    // damp out abrupt changes and oscillations in steering acceleration
    // (rate is proportional to time step, then clipped into useful range)
    if (elapsedTime > 0)
    {
        const float smoothRate = clip (9 * elapsedTime, 0.15f, 0.4f);
        blendIntoAccumulator (smoothRate,
                              newAcceleration,
                              _smoothedAcceleration);
    }

    // Euler integrate (per frame) acceleration into velocity
    newVelocity += _smoothedAcceleration * elapsedTime;

    // enforce speed limit
    newVelocity = newVelocity.truncateLength (maxSpeed ());

    // update Speed
    setSpeed (newVelocity.length());

    // Euler integrate (per frame) velocity into position
    setPosition (position() + (newVelocity * elapsedTime));

    // regenerate local space (by default: align vehicle's forward axis with
    // new velocity, but this behavior may be overridden by derived classes.)
    regenerateLocalSpace (newVelocity, elapsedTime);

    // maintain path curvature information
    measurePathCurvature (elapsedTime);

    // running average of recent positions
    blendIntoAccumulator (elapsedTime * 0.06f, // QQQ
                          position (),
                          _smoothedPosition);
}
*/

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
/*
void SteeringVehicle::regenerateLocalSpace (const Vec3& newVelocity,
                                                const float elapsedTime)
{
    // adjust orthonormal basis vectors to be aligned with new velocity
    if (speed() > 0) regenerateOrthonormalBasisUF (newVelocity / speed());
}
*/
