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
#ifndef __RlAI_SteeringVehicle_H__
#define __RlAI_SteeringVehicle_H__

#include "AiPrerequisites.h"
#include "AgentManager.h"
#include "DebugVisualisable.h"
#include "LineSetPrimitive.h"
#include "OgreVector3.h"
#include "OpenSteer/SteerLibrary.h"


namespace rl
{
	/*	SimpleVehicle_1 adds concrete LocalSpace methods to AbstractVehicle.
	 * OpenSteer::LocalSpaceMixin contains functionality to convert from one
	 * coordinate system to an other.
	 */
	typedef OpenSteer::LocalSpaceMixin<OpenSteer::AbstractVehicle> SimpleVehicle_1;
	/*	SimpleVehicle_2 adds concrete steering methods to SimpleVehicle_1.
	 * OpenSteer::SteerLibraryMixin adds the "steering library"
	 * functionality to a given base class.  SteerLibraryMixin assumes its base
     * class supports the AbstractVehicle interface.
	 * The "steering library" itself presents methods to calculate movements
	 * according to behaviours.
	 */
	typedef OpenSteer::SteerLibraryMixin<SimpleVehicle_1> SimpleVehicle_2;
	
	class Actor;
	class Agent;
	class PhysicalThing;
    class MovingCreature;
//	class PerceptionPool;

	/**
	 * Realises steering for NPCs
	 * Provides different steering behaviours through methods that calculate
	 * steering forces
	 *
	 * Maybe this should be used as interface instead of AbstractVehicle
	 */
        class _RlAiExport SteeringVehicle : public SimpleVehicle_2,
                                            public DebugVisualisable
	{
	public:
		/** Constructor.
		 * @param parent Agent owning this vehicle
		 * @param character Actor controlled by Agent
		 */
		SteeringVehicle(Agent* parent, Creature* character);
		/** explicit virtual destructor.
		 */
		virtual ~SteeringVehicle(void);

		/**
		 * Add a force to the current force of the vehicle
		 * @param  force value as force vector
		 */
		void addForce(const Ogre::Vector3& force);

		/**
		 * Random walking behaviour
		 * The steering value is purely tangential (has no Forward component) 
		 * and uses the x-axis only at the moment (2D wandering)
		 * @param  elapsedTime The time step value allows wander rate to be consistent when frame times vary
		 * @return a steering force for wandering behavior. 
		 */
		Ogre::Vector3 calcWander(const float elapsedTime);

		/**
		 * Causes the vehicle to turn toward a target and move to it. 
		 * If this behavior is used alone and unconditionally, 
		 * it will cause the vehicle to pass through the target 
		 * then turn around for another pass.
		 * @param  target target position to seek for
		 * @return a steering force to seek the given target location. 
		 */
		Ogre::Vector3 calcSeek(const Ogre::Vector3& target);

		/**
		 * Causes the vehicle to turn away from a target and move away from it.
		 * @param  target target position to flee for
		 * @return a steering force to flee from the given target location. 
		 */
		Ogre::Vector3 calcFlee(const Ogre::Vector3& target);

		/**
		 * Causes the vehicle to turn to a target and move to it.
		 * @param agent to persue
		 * @return a steering force for persuing the given agent
		 */
		Ogre::Vector3 calcPursuit(Agent* agent);

		/**
		 * Causes the vehicle to turn away from obstacles in space. 
		 * The vehicle will consider all close-by obstacles automatically
		 * @param  minTimeToCollision distance to the obstacle in time at the vehicle's current velocity
		 * @return a steering force to avoid obstacles. 
		 */
		Ogre::Vector3 calcAvoidObstacles(const float minTimeToCollision);

		/**
		 * Causes the vehicle to turn away from neighbor vehicles.
		 * The vehicle will consider all close-by nieghbours automatically
		 * @param  minTimeToCollision distance to the neighbour in time at the vehicle's current velocity
		 * @return a steering force to avoid neighbours. 
		 */
		Ogre::Vector3 calcAvoidNeighbors(const float minTimeToCollision);
			
		/**
		 * @returns a steering force to maintain a given target speed. 
		 */
		Ogre::Vector3 calcSteerTargetSpeed(const float targetSpeed);

		/** tests if the specified Agent is within the threshold.
		 * @param agent Agent to test against
		 * @param threshold specifies maximum distance
		 */
		bool isAhead(Agent* agent, const float threshold);

		/** tests if collision will happen within specified timeframe
		 * @param minTimeToCollision minimum time to next collision
		 */
		bool needAvoidance(const float minTimeToCollision);
		
        /** Sets the animation of the actors mesh to a new keyframe.
		 * @param name of the animation to set the mesh of the actor to.
		 */
		//void setAnimation(const CeGuiString& name);
	
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
				
//		PerceptionPool* getPerceptionPool();

		/** tests if a dialog is active with the creature
		 * @returns true when dialog is active, otherwise false
		 */
		bool isDialogActive();

		/** calculates euklidian distance between two vectors
		 * @param vec1 first vector
		 * @param vec2 second vector
		 */
		float calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2);

		/** returns the position of ?
		 */
		Ogre::Vector3 getPosition();

		// inherited from AbstractVehicle

		/**
		 * update the steering of the vehicle
		 */
		virtual void update(const float currentTime, const float elapsedTime);
		/**
		 * predict position of this vehicle at some time in the future
		 * (assumes velocity remains constant)
		 */
		OpenSteer::Vec3 predictFuturePosition (const float predictionTime) const;

		void resetLocalSpace();

		/** get mass
		 * @returns mass of physical object
		 */
		float mass (void) const;
		/** does not set mass but is necessary for AbstractVehicle.
		 * throws an exception on invocation.
		 * @param m mass
		 */
		float setMass (float m);

		/** retrieve velocity of vehicle
		 * @returns velocity of the vehicle
		 */
		OpenSteer::Vec3 velocity (void) const {return OpenSteer::Vec3(mCurrentVelocity.x, mCurrentVelocity.y, mCurrentVelocity.z);}

		/** retrieves speed of vehicle.
		 * may be faster than taking mag of velocity
		 */
		float speed (void) const;
		/** sets speed of vehicle.
		 * may be faster than taking mag of velocity
		 * @param s new speed to set
		 * @returns float new speed set.
		 */
		float setSpeed (float s);

		/** radius for size of bounding sphere.
		 * used for obstacle avoidance, etc.
		 * TODO: this should be handled by size of NewtonBody
		 * @returns float the radius
	    */
		float radius (void) const;
		/** sets radius for size of bounding sphere.
		 * used for obstacle avoidance, etc.
		 * TODO: this should be handled by size of NewtonBody
		 * @returns float the radius
	    */
		float setRadius (float m);

		/** height for size of bounding sphere.
		 * used for obstacle avoidance, etc.
		 * TODO: this should be handled by height of NewtonBody
		 * @returns float the height
	    */
        float height (void) const;
		/** height for size of bounding sphere.
		 * used for obstacle avoidance, etc.
		 * TODO: this should be handled by height of NewtonBody
		 * @returns float the height
	    */
        float setHeight (float h);

		/** retrieves maximum force.
		 * @returns float containing maximum force.
		 */
		float maxForce (void) const; 
		/** sets maximum force.
		 * TODO: should not be set here, throw excpetion or so
		 * @returns float containing maximum force set.
		 */
		float setMaxForce (float mf);

		/** retrieves maximum speed.
		 * @returns float containing maximum speed
		 */
		float maxSpeed (void) const;
		/** retrieves maximum speed.
		 * TODO: should not be set here, throw excpetion or so
		 * @returns float containing maximum speed
		 */
		float setMaxSpeed (float ms);


		/**
		 * adjust the steering force passed to applySteeringForce.
         * allows a specific vehicle class to redefine this adjustment.
         * default is to disallow backward-facing steering at low speed.
		 */
		virtual OpenSteer::Vec3 adjustRawSteeringForce (const OpenSteer::Vec3& force);
                                            // const float elapsedTime);

		/**
		 * apply a given steering force to our momentum,
		 * adjusting our orientation to maintain velocity-alignment.
		 */
	//	void applySteeringForce(const OpenSteer::Vec3& force, const float elapsedTime);
	//	void applySteeringForce(const Ogre::Vector3& force, const float elapsedTime);
		
		

		/**
		 * the default version: keep FORWARD parallel to velocity, change
         * UP as little as possible.
		 */
  /*      virtual void regenerateLocalSpace (const OpenSteer::Vec3& newVelocity,
                                           const float elapsedTime);
		*/
		
		// get instantaneous curvature (since last update)
/*		float curvature (void) {return _curvature;}

		// get/reset smoothedCurvature, smoothedAcceleration and smoothedPosition
		float smoothedCurvature (void) {return _smoothedCurvature;}
		
		float resetSmoothedCurvature (float value = 0)
		{
			_lastForward = OpenSteer::Vec3::zero;
			_lastPosition = OpenSteer::Vec3::zero;
			return _smoothedCurvature = _curvature = value;
		}
		
		OpenSteer::Vec3 smoothedAcceleration (void) {return _smoothedAcceleration;}
		
		OpenSteer::Vec3 resetSmoothedAcceleration (const OpenSteer::Vec3& value = OpenSteer::Vec3::zero)
		{
			return _smoothedAcceleration = value;
		}
		
		OpenSteer::Vec3 smoothedPosition (void) {return _smoothedPosition;}
		
		OpenSteer::Vec3 resetSmoothedPosition (const OpenSteer::Vec3& value = OpenSteer::Vec3::zero)
		{
			return _smoothedPosition = value;
		}

		// set a random "2D" heading: set local Up to global Y, then effectively
		// rotate about it by a random angle (pick random forward, derive side).
		void randomizeHeadingOnXZPlane (void)
		{
			setUp (OpenSteer::Vec3::up);
			setForward (RandomUnitVectorOnXZPlane ());
			setSide (localRotateForwardToSide (forward()));
		}
		*/

		/** retrieve the controlled Actor
		 * @returns Actor that is controlled by the SteeringVehicle
		 */
        const Actor* getActor(void) const;


        // derived from debugvisualisable
        virtual DebugVisualisableFlag getFlag() const;
        virtual void updatePrimitive();
	protected:
		/** initializes
		 */
		void initialize();

        virtual void createMovingCreature(); // permit PlayerVehicle to override this functions!
        virtual void destroyMovingCreature();

		/** retrieves the neighbours of this SteeringVehicle
		 */
		OpenSteer::AVGroup getNeighbors() const;
		/** retrieves the obstacles
		 */
		const OpenSteer::ObstacleGroup& getObstacles() const;

	//    float _mass;       // mass (defaults to unity so acceleration=force)
    //   float _radius;     // size of bounding sphere, for obstacle avoidance, etc.
    //    float _speed;      // speed along Forward direction.  Because local space
                           // is velocity-aligned, velocity = Forward * Speed
		
		/** the maximum steering force this vehicle can apply.
         * steering force is clipped to this magnitude.
		 */
        float _maxForce;
		/** the maximum speed this vehicle is allowed to move.
         * velocity is clipped to this magnitude.
		 */
        float _maxSpeed;

     /*   float _curvature;
		float _smoothedCurvature;

		OpenSteer::Vec3 _lastForward;
        OpenSteer::Vec3 _lastPosition;
        OpenSteer::Vec3 _smoothedPosition;
        OpenSteer::Vec3 _smoothedAcceleration;
	*/	
		//! mass of the steering vehicle retrieved from Newton body (is this really necessary ?)
		//Ogre::Real mMass;
		//! radius of for the bounding sphere (use newton to get this from)
		//Ogre::Real mRadius;
		//! speed of the vehicle
		Ogre::Real mSpeed;	

		//! height of the vehicle ? (use newton to get this)
        //float mHeight;

		//! current force 
		Ogre::Vector3 mCurrentForce;
		//! current velocity
		Ogre::Vector3 mCurrentVelocity;
		//! direction vector
		Ogre::Vector3 mForwardVector;
		//! the yaw angle in radians
		//Ogre::Radian mYaw;

		//! Agent using this vehicle
		Agent* mParent;
        //! Creature object steered by this vehicle (and controlled by Agent).
        Creature* mCreature;

        MovingCreature* mMovingCreature;

        // measure path curvature (1/turning-radius), maintain smoothed version
      //  void measurePathCurvature (const float elapsedTime);


        // derived from debugvisualisable
        virtual void doCreatePrimitive();

        Ogre::Vector3 mDebugSteer;
        Ogre::Vector3 mDebugWander;
        Ogre::Vector3 mDebugAvoidObstacles;
    };
}
#endif
