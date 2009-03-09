#pragma once
/*
	Demo03_CollisionCallback

--------------------OLD NEWTON -------------------
	creating a collision callback is simple.  make a class that inherits from the OgreNewt::ContactCallback class.
	this class has 3 virtual functions that the user can implement:

	int userBegin()

	int userProcess()

	void userEnd()

	these 3 functions work together to create a collision callback.  basically it works like this:  when two rigid bodies are about to collide
	(their bounding boxes have overlapped), the "userBegin()" function gets called.  at this point you can check which bodies are involved,
	and decide if you want to reject or continue with the collision.  return "1" to accept collision, or "0" to ignore it.

	after that, the "userProcess" function gets called for each contact that happens between the 2 bodies.  depending on the timestep for the
	physics, there may be more than one contact.  you can use this function to perform many things, like recording positions and velocities, or
	applying special effects (like in this example).

	finally, after all collisions have been solved, the "userEnd" function gets called.  this can be a "cleanup function".  for example you can
	record all of the collisions and positions in the Process function, and then in the end function you might play a sound effect, etc.
*/

#include <OgreNewt.h>

class ConveyorMatCallback :
	public OgreNewt::ContactCallback
{
public:
	ConveyorMatCallback( int conveyorID );
	~ConveyorMatCallback(void);

    int onAABBOverlap( OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex );

    void contactsProcess( OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadIndex );

private:
	int mConveyorID;
};

