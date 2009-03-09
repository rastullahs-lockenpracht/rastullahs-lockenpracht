#pragma once
/*
	This is the implementation of the OgreNewt vehicle.  when making a vehicle, you must inherit from the "OgreNewt::Vehicle" class.

	then you create a few functions that setup the vehicle properly.  see below.
*/

#include <OgreNewt.h>
#include <Ogre.h>



class SimpleVehicle :
	public OgreNewt::Vehicle
{
public:
	// we also make our own Tire class based on the OgreNewt::Vehicle::Tire class, to store some extra information
	class SimpleTire : public OgreNewt::Vehicle::Tire
	{
	public:
		SimpleTire(OgreNewt::Vehicle* vehicle, Ogre::Quaternion localorient, Ogre::Vector3 localpos, Ogre::Vector3 pin, 
			Ogre::Real mass, Ogre::Real width, Ogre::Real radius, Ogre::Real susShock, Ogre::Real susSpring, Ogre::Real susLength, int ColID, bool steer )
			: Tire(vehicle, localorient, localpos, pin, mass, width, radius, susShock, susSpring, susLength, ColID )
		{
			mSteeringTire = steer;
		}

		~SimpleTire();

		bool mSteeringTire;
	};


	// constructor we will use to create the entire vehicle.
	SimpleVehicle(Ogre::SceneManager* mgr, OgreNewt::World* world, Ogre::Vector3& position, Ogre::Quaternion& orient);

	~SimpleVehicle(void);

	// this is a function created specifically for this vehicle to allow control of it.
	void setTorqueSteering(Ogre::Real torque, Ogre::Degree steering) { mTorque = torque; mSteering = steering; }
private:

	// this is another virtual function used to actually create the vehicle.
	void setup();

	// we must define a callback for the vehicle.  this is where we control 
	// the vehicle, by adding torque to the tires, and steering.
	void userCallback();


	// this is just a helper function to make a simple rigid body. taken from the other examples and implanted
	// in this class.
	OgreNewt::Body* makeSimpleBox( Ogre::Vector3& size, Ogre::Vector3& pos, Ogre::Quaternion& orient );


	// scene manager for creating entities, etc.
	Ogre::SceneManager* mSceneMgr;

	OgreNewt::World* mWorld;

	static int mEntityCount;

	// for steering, etc.
	Ogre::Real mTorque;
	Ogre::Degree mSteering;
};

