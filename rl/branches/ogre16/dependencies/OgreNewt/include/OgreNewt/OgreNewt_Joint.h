/* 
	OgreNewt Library

	Ogre implementation of Newton Game Dynamics SDK

	OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.

		by Walaber

*/
#ifndef _INCLUDE_OGRENEWT_JOINT
#define _INCLUDE_OGRENEWT_JOINT

#include <Newton.h>
#include "OgreNewt_World.h"
#include "OgreNewt_Body.h"

// OgreNewt namespace.  all functions and classes use this namespace.
namespace OgreNewt
{

//! base class for all joints.
/*!
	this class is inherited by all other specific joint types.
*/
class _OgreNewtExport Joint
{
public:

	//! constructor
	Joint();

	//! destructor
	virtual ~Joint();

	//! returns collision state
	/*!
		The collision state determines whether collision should be calculated between the parent and child bodies of the joint.
		\return integer value. 1 = collision on, 0 = collision off.
	*/
	int getCollisionState() const { return NewtonJointGetCollisionState( m_joint ); }

	//! sets the collision state
	/*!
		The collision state determines whether collision should be calculated between the parent and child bodies of the joint.
		\param state integer value. 1 = collision on, 0 = collision off.
	*/		
	void setCollisionState( int state ) const { NewtonJointSetCollisionState( m_joint, state ); }


	//! get joint stiffness
	/*!
		Joint stiffness adjusts how much "play" the joint can have.  high stiffness = very small play, but more likely to become unstable.
		\return float representing joint stiffness in range [0,1]
	*/
	Ogre::Real getStiffness() const { return (Ogre::Real)NewtonJointGetStiffness( m_joint ); }

	//! set joint stiffness
	/*!
		Joint stiffness adjusts how much "play" the joint can have.  high stiffness = very small play, but more likely to become unstable.
		\param stiffness float representing joint stiffness in range [0,1]
	*/
	void setStiffness( Ogre::Real stiffness ) const { NewtonJointSetStiffness( m_joint, stiffness ); }


	//! set user data for this joint
	/*!
		user data can be used to connect this class to other user classes through the use of this general pointer.
	*/
	void setUserData( void* ptr ) { m_userdata = ptr; }

	//! get user data for this joint
	/*!
		user data can be used to connect this class to other user classes through the use of this general pointer.
	*/
	void* getUserData() const { return m_userdata; }

		
protected:

	NewtonJoint* m_joint;
	const OgreNewt::World* m_world;

	void* m_userdata;

	static void _CDECL destructor( const NewtonJoint* me );

};






//! CustomJoint 
/*!
	this class represents a basic class for creating user-defined joints.  this class must be inherited to create discreet joints.
*/
class _OgreNewtExport CustomJoint : public Joint
{
 
public:

	//! constructor
	CustomJoint( unsigned int maxDOF, const Body* body0, const Body* body1 );

	//! destructor
	virtual ~CustomJoint();

	//! must be over-written for a functioning joint.
	virtual void submitConstraint() = 0;


	//! find the local orientation and position of the joint with regards to the 2 bodies in the joint.
	void pinAndDirToLocal( const Ogre::Vector3& pinpt, const Ogre::Vector3& pindir, Ogre::Quaternion& localOrient0, Ogre::Vector3& localPos0, Ogre::Quaternion& localOrient1, Ogre::Vector3& localPos1 ) const;

	//! find the global orientation and position of the joint with regards to the a body in the joint.
	void localToGlobal( const Ogre::Quaternion& localOrient, const Ogre::Vector3& localPos, Ogre::Quaternion& globalOrient, Ogre::Vector3& globalPos, int bodyIndex = 0 ) const;

	//! add a linear row to the constraint.
	void addLinearRow( const Ogre::Vector3& pt0, const Ogre::Vector3& pt1, const Ogre::Vector3& dir ) const;

	//! add an angular row to the constraint.
	void addAngularRow( Ogre::Radian relativeAngleError, const Ogre::Vector3& dir ) const;

	//! set the general jacobian rows directly.
	void addGeneralRow( const Ogre::Vector3& linear0, const Ogre::Vector3& angular0, const Ogre::Vector3& linear1, const Ogre::Vector3& angular1 ) const;

	//! set row minimum friction
	void setRowMinimumFriction( Ogre::Real friction ) const;

	//! set row maximum friction
	void setRowMaximumFriction( Ogre::Real friction ) const;

	//! set row acceleration
	void setRowAcceleration( Ogre::Real accel ) const;

	//! set row stiffness
	void setRowStiffness( Ogre::Real stiffness ) const;

	//! apply a spring to this row, allowing for joints with spring behaviour in 1 or more DoF's
	/*!
		\param springK float spring constant.
		\param springD float natural rest state distance of the spring.
	*/		
	void setRowSpringDamper( Ogre::Real springK, Ogre::Real springD ) const;

	//! retrieve the force acting on the current row.
	Ogre::Real getRowForce( int row ) const { return NewtonUserJointGetRowForce( m_joint, row ); }

	//! pin vector to arbitrary quaternion utility function.
	Ogre::Quaternion grammSchmidt( const Ogre::Vector3& pin ) const;

protected:

	unsigned int m_maxDOF;

	const OgreNewt::Body* m_body0;
	const OgreNewt::Body* m_body1;

	//! newton callback.  used internally.
	static void _CDECL newtonSubmitConstraint( const NewtonJoint* me );

};



}	// end NAMESPACE OgreNewt

#endif
// _INCLUDE_OGRENEWT_JOINT

