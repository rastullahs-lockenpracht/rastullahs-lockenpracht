#include "MyCustomBallSocket.h"

// cunstructor.  we must pass the 2 bodies (2nd can be NULL), also the max DOF for the joint, up to 6.
MyCustomBallSocket::MyCustomBallSocket( OgreNewt::Body* child, OgreNewt::Body* parent, Ogre::Vector3 point, Ogre::Vector3 pin ) : OgreNewt::CustomJoint( 3, child, parent )
{
	// init our variables to keep track of the joint, so we get the joint orientation in local orientation of the bodies.
		
	// find local orientations relative to each body, based on the global orientation of the joint.
	pinAndDirToLocal( point, pin, mLocalOrient0, mLocalPos0, mLocalOrient1, mLocalPos1 );
}


MyCustomBallSocket::~MyCustomBallSocket()
{
}

// the important function that applies the joint.
void MyCustomBallSocket::submitConstraint(Ogre::Real timestep, int threadindex)
{
	// find the global orientation of the joint relative to the 2 bodies.
	Ogre::Quaternion globalOrient0;
	Ogre::Vector3 globalPos0;

	Ogre::Quaternion globalOrient1;
	Ogre::Vector3 globalPos1;

	localToGlobal( mLocalOrient0, mLocalPos0, globalOrient0, globalPos0, 0 );
	localToGlobal( mLocalOrient1, mLocalPos1, globalOrient1, globalPos1, 1 );

	// to make the joint, we simply apply a constraint to connect 2 points on the bodies along all 3 major axis
	addLinearRow( globalPos0, globalPos1, globalOrient0 * Ogre::Vector3(Ogre::Vector3::UNIT_X) );
	addLinearRow( globalPos0, globalPos1, globalOrient0 * Ogre::Vector3(Ogre::Vector3::UNIT_Y) );
	addLinearRow( globalPos0, globalPos1, globalOrient0 * Ogre::Vector3(Ogre::Vector3::UNIT_Z) );

}

