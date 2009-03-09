#pragma once
/*
	This is the implementation of a very simple ball and socket joint, created as a custom joint, to demonstrate how to use the
	custom joint interface in OgreNewt.
*/

#include <OgreNewt.h>


class MyCustomBallSocket : public OgreNewt::CustomJoint
{
public:

	// constructor - should be implemented by the user.
	MyCustomBallSocket( OgreNewt::Body* child, OgreNewt::Body* parent, Ogre::Vector3 point, Ogre::Vector3 pin );

	~MyCustomBallSocket();

	// also user MUST implement this function to actually apply the constraint.
	void submitConstraint(Ogre::Real timestep, int threadindex);

private:
	// these are variables I will use to keep track of the joint, to submit the constraint.  this i basically a direct
	// copy of the ballandsocket user joint included with the newton SDK.

	Ogre::Vector3 mLocalPos0;
	Ogre::Quaternion mLocalOrient0;

	Ogre::Vector3 mLocalPos1;
	Ogre::Quaternion mLocalOrient1;

};

