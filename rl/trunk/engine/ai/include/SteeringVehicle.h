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
#ifndef __RlAI_SteeringVehicle_H__
#define __RlAI_SteeringVehicle_H__

#include "AiPrerequisites.h"
#include "OgreVector3.h"

// debuging of steerForPursuit...should be ommited
#include "OpenSteer/Vec3.h"
const OpenSteer::Vec3 gBlack   (0, 0, 0);
const OpenSteer::Vec3 gWhite   (1, 1, 1);

const OpenSteer::Vec3 gRed     (1, 0, 0);
const OpenSteer::Vec3 gYellow  (1, 1, 0);
const OpenSteer::Vec3 gGreen   (0, 1, 0);
const OpenSteer::Vec3 gCyan    (0, 1, 1);
const OpenSteer::Vec3 gBlue    (0, 0, 1);
const OpenSteer::Vec3 gMagenta (1, 0, 1);

const OpenSteer::Vec3 gOrange (1, 0.5f, 0);

inline OpenSteer::Vec3 grayColor (const float g) {return OpenSteer::Vec3 (g, g, g);}

const OpenSteer::Vec3 gGray10 = grayColor (0.1f);
const OpenSteer::Vec3 gGray20 = grayColor (0.2f);
const OpenSteer::Vec3 gGray30 = grayColor (0.3f);
const OpenSteer::Vec3 gGray40 = grayColor (0.4f);
const OpenSteer::Vec3 gGray50 = grayColor (0.5f);
const OpenSteer::Vec3 gGray60 = grayColor (0.6f);
const OpenSteer::Vec3 gGray70 = grayColor (0.7f);
const OpenSteer::Vec3 gGray80 = grayColor (0.8f);
const OpenSteer::Vec3 gGray90 = grayColor (0.9f);
#include "OpenSteer/SteerLibrary.h"


using namespace Ogre;
using namespace OpenSteer;

namespace rl
{
//	SimpleVehicle_1 adds concrete LocalSpace methods to AbstractVehicle
	typedef OpenSteer::LocalSpaceMixin<AbstractVehicle> SimpleVehicle_1;
//	SimpleVehicle_2 adds concrete steering methods to SimpleVehicle_1
	typedef OpenSteer::SteerLibraryMixin<SimpleVehicle_1> SimpleVehicle_2;
	
	class Actor;
	class PhysicalThing;

	/**
	 * Realises steering for NPCs
	 * Provides different steering behaviours through methods that calculate
	 * steering forces
	 *
	 * Maybe this should be used as interface instead of AbstractVehicle
	 */
	class _RlAiExport SteeringVehicle : protected SimpleVehicle_2
	{
	public:
		SteeringVehicle(Actor* character);
		virtual ~SteeringVehicle(void);
		
		/**
		 * Add a force to the current force of the vehicle
		 * @param  force value as force vector
		 */
		void addForce(const Ogre::Vector3& force);

		/**
		 * Apply the current force of the vehicle to a PhysicalThing
		 * @param  thing physic object tthe force is apllied to
		 * @param  elapsedTime time since last call
		 */
		void applySteeringForce(PhysicalThing* thing, const float elapsedTime);

		/**
		 * Random walking behaviour
		 * The steering value is purely tangential (has no Forward component) 
		 * and uses the x-axis only at the moment (2D wandering)
		 * @param  elapsedTime The time step value allows wander rate to be consistent when frame times vary
		 * @return a steering force for wandering behavior. 
		 */
		Vector3 calcWander(const float elapsedTime);

		/**
		 * Causes the vehicle to turn toward a target and move to it. 
		 * If this behavior is used alone and unconditionally, 
		 * it will cause the vehicle to pass through the target 
		 * then turn around for another pass.
		 * @param  target target position to seek for
		 * @return a steering force to seek the given target location. 
		 */
		Vector3 calcSeek(const Vector3& target);

		/**
		 * Causes the vehicle to turn away from a target and move away from it.
		 * @param  target target position to flee for
		 * @return a steering force to flee from the given target location. 
		 */
		Vector3 calcFlee(const Vector3& target);

		/**
		 * Causes the vehicle to turn away from obstacles in space. 
		 * The vehicle will consider all close-by obstacles automatically
		 * @param  minTimeToCollision distance to the obstacle in time at the vehicle's current velocity
		 * @return a steering force to avoid obstacles. 
		 */
		Vector3 calcAvoidObstacles(const float minTimeToCollision);

		/**
		 * Causes the vehicle to turn away from neighbour vehicles.
		 * The vehicle will consider all close-by nieghbours automatically
		 * @param  minTimeToCollision distance to the neighbour in time at the vehicle's current velocity
		 * @return a steering force to avoid neighbours. 
		 */
		Vector3 calcAvoidNeighbours(const float minTimeToCollision);
			
		/**
		 * @returns a steering force to maintain a given target speed. 
		 */
		Vector3 calcSteerTargetSpeed(const float targetSpeed);

		/* TODO:
		calcFollowPath
		calcStayOnPath
		calcAvoidObstacles(Obstacle&)
		Vector3 calcEvasion(AbstractVehicle&);
		Vector3 calcPursuit(AbstractVehicle&);
		Vector3 calcSeperation(AVGroup&);
		Vector3 calcAlignment(AVGroup&);
		Vector3 calcCohesion(AVGroup&);
		*/
	
		void setAnimation(const CeGuiString& name);

		void init();

		// inherited from AbstractVehicle

		/**
		 * predict position of this vehicle at some time in the future
		 * (assumes velocity remains constant)
		 */
		Vec3 predictFuturePosition (const float predictionTime) const;

		// get/set mass
		float mass (void) const {return mMass;}
		float setMass (float m) {return mMass = m;}

		// get velocity of vehicle
		Vec3 velocity (void) const {return forward() * mSpeed;}

		// get/set speed of vehicle  (may be faster than taking mag of velocity)
		float speed (void) const {return mSpeed;}
		float setSpeed (float s) {return mSpeed = s;}

		// size of bounding sphere, for obstacle avoidance, etc.
		float radius (void) const {return mRadius;}
		float setRadius (float m) {return mRadius = m;}

		// get/set maxForce
		float maxForce (void) const {return _maxForce;}
		float setMaxForce (float mf) {return _maxForce = mf;}

		// get/set maxSpeed
		float maxSpeed (void) const {return _maxSpeed;}
		float setMaxSpeed (float ms) {return _maxSpeed = ms;}


		/**
		 * adjust the steering force passed to applySteeringForce.
         * allows a specific vehicle class to redefine this adjustment.
         * default is to disallow backward-facing steering at low speed.
		 */
	/*	virtual Vec3 adjustRawSteeringForce (const Vec3& force,
                                             const float elapsedTime);
*/
		/**
		 * apply a given steering force to our momentum,
		 * adjusting our orientation to maintain velocity-alignment.
		 */
	//	void applySteeringForce(const Vec3& force, const float elapsedTime);
	//	void applySteeringForce(const Ogre::Vector3& force, const float elapsedTime);
		
		

		/**
		 * the default version: keep FORWARD parallel to velocity, change
         * UP as little as possible.
		 */
  /*      virtual void regenerateLocalSpace (const Vec3& newVelocity,
                                           const float elapsedTime);
		*/
		
		// get instantaneous curvature (since last update)
/*		float curvature (void) {return _curvature;}

		// get/reset smoothedCurvature, smoothedAcceleration and smoothedPosition
		float smoothedCurvature (void) {return _smoothedCurvature;}
		
		float resetSmoothedCurvature (float value = 0)
		{
			_lastForward = Vec3::zero;
			_lastPosition = Vec3::zero;
			return _smoothedCurvature = _curvature = value;
		}
		
		Vec3 smoothedAcceleration (void) {return _smoothedAcceleration;}
		
		Vec3 resetSmoothedAcceleration (const Vec3& value = Vec3::zero)
		{
			return _smoothedAcceleration = value;
		}
		
		Vec3 smoothedPosition (void) {return _smoothedPosition;}
		
		Vec3 resetSmoothedPosition (const Vec3& value = Vec3::zero)
		{
			return _smoothedPosition = value;
		}

		// set a random "2D" heading: set local Up to global Y, then effectively
		// rotate about it by a random angle (pick random forward, derive side).
		void randomizeHeadingOnXZPlane (void)
		{
			setUp (Vec3::up);
			setForward (RandomUnitVectorOnXZPlane ());
			setSide (localRotateForwardToSide (forward()));
		}
		*/
	private:
    //    float _mass;       // mass (defaults to unity so acceleration=force)
    //   float _radius;     // size of bounding sphere, for obstacle avoidance, etc.
    //    float _speed;      // speed along Forward direction.  Because local space
                           // is velocity-aligned, velocity = Forward * Speed
        float _maxForce;   // the maximum steering force this vehicle can apply
                           // (steering force is clipped to this magnitude)
        float _maxSpeed;   // the maximum speed this vehicle is allowed to move
                           // (velocity is clipped to this magnitude)

     /*   float _curvature;
		float _smoothedCurvature;

		OpenSteer::Vec3 _lastForward;
        OpenSteer::Vec3 _lastPosition;
        OpenSteer::Vec3 _smoothedPosition;
        OpenSteer::Vec3 _smoothedAcceleration;
	*/	
		Ogre::Real mMass;
		Ogre::Real mRadius;
		Ogre::Real mSpeed;	

		Ogre::Vector3 mCurrentForce;
		Ogre::Vector3 mCurrentVelocity;
		Ogre::Vector3 mForwardVector;
		Ogre::Radian mYaw;

		Actor* mActor;

        // measure path curvature (1/turning-radius), maintain smoothed version
      //  void measurePathCurvature (const float elapsedTime);
	};
}
#endif
