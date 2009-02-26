/*  #include <assert.h>  why do we need this here? */
#include "OgreNewt_Joint.h"
#include "OgreNewt_Body.h"
#include "OgreNewt_World.h"


namespace OgreNewt
{

	
Joint::Joint()
{
	// nothing here
}

Joint::~Joint()
{

	if(m_joint)
	{
		if (NewtonJointGetUserData(m_joint))
		{
			NewtonJointSetDestructor( m_joint, NULL );
			NewtonDestroyJoint( m_world->getNewtonWorld(), m_joint );
		}
	}

}


void _CDECL Joint::destructor( const NewtonJoint* me )
{
	Joint* jnt;

	jnt = (Joint*)NewtonJointGetUserData( me );

	NewtonJointSetDestructor( me, NULL );
	NewtonJointSetUserData( me, NULL );

	delete jnt;
}




CustomJoint::CustomJoint( unsigned int maxDOF, const Body* body0, const Body* body1 ) : Joint()
{
	m_maxDOF = maxDOF;

	m_body0 = body0;
	m_body1 = body1;

	m_world = m_body0->getWorld();

	if (body1)
		m_joint = NewtonConstraintCreateUserJoint( m_world->getNewtonWorld(), m_maxDOF, 
													CustomJoint::newtonSubmitConstraint, CustomJoint::newtonGetInfo,
													m_body0->getNewtonBody(), m_body1->getNewtonBody() );
	else
		m_joint = NewtonConstraintCreateUserJoint( m_world->getNewtonWorld(), m_maxDOF, 
													CustomJoint::newtonSubmitConstraint, CustomJoint::newtonGetInfo,
													m_body0->getNewtonBody(), NULL );

	NewtonJointSetUserData (m_joint, this);
	NewtonJointSetDestructor (m_joint, destructor);

}

CustomJoint::~CustomJoint()
{
}


void CustomJoint::pinAndDirToLocal( const Ogre::Vector3& pinpt, const Ogre::Vector3& pindir, 
								   Ogre::Quaternion& localOrient0, Ogre::Vector3& localPos0, Ogre::Quaternion& localOrient1, Ogre::Vector3& localPos1 ) const
{
	localOrient0 = localOrient1 =  Ogre::Quaternion::IDENTITY;
	localPos0 = localPos1 = Ogre::Vector3::ZERO;

	Ogre::Quaternion bodyOrient0 = Ogre::Quaternion::IDENTITY;
	Ogre::Quaternion bodyOrient1 = Ogre::Quaternion::IDENTITY;
	Ogre::Vector3 bodyPos0 = Ogre::Vector3::ZERO;
	Ogre::Vector3 bodyPos1 = Ogre::Vector3::ZERO;

	Ogre::Quaternion pinOrient = grammSchmidt(pindir);

	m_body0->getPositionOrientation( bodyPos0, bodyOrient0 );

	if (m_body1)
		m_body1->getPositionOrientation( bodyPos1, bodyOrient1 );

	localPos0 = bodyOrient0.Inverse() * (pinpt - bodyPos0);
	localOrient0 = pinOrient * bodyOrient0.Inverse();

	localPos1 = bodyOrient1.Inverse() * (pinpt - bodyPos1);
	localOrient1 = pinOrient * bodyOrient1.Inverse();

}


void CustomJoint::localToGlobal( const Ogre::Quaternion& localOrient, const Ogre::Vector3& localPos, Ogre::Quaternion& globalOrient, Ogre::Vector3& globalPos, int bodyIndex ) const
{
	globalOrient = Ogre::Quaternion::IDENTITY;
	globalPos= Ogre::Vector3::ZERO;

	const Body* bdy = NULL;
	if (bodyIndex == 0)
		bdy = m_body0;
	else if (m_body1)
		bdy = m_body1;

	Ogre::Quaternion bodyOrient = Ogre::Quaternion::IDENTITY;
	Ogre::Vector3 bodyPos = Ogre::Vector3::ZERO;

	if (bdy)
		bdy->getPositionOrientation( bodyPos, bodyOrient );

	globalPos = (bodyOrient * localPos) + bodyPos;
	globalOrient = bodyOrient * localOrient;
}

	

void CustomJoint::addLinearRow( const Ogre::Vector3& pt0, const Ogre::Vector3& pt1, const Ogre::Vector3& dir ) const
{
	NewtonUserJointAddLinearRow( m_joint, &pt0.x, &pt1.x, &dir.x );
}

void CustomJoint::addAngularRow( Ogre::Radian relativeAngleError, const Ogre::Vector3& dir ) const
{
	NewtonUserJointAddAngularRow( m_joint, relativeAngleError.valueRadians(), &dir.x );
}

void CustomJoint::addGeneralRow(const Ogre::Vector3& linear0, const Ogre::Vector3& angular0, const Ogre::Vector3& linear1, const Ogre::Vector3& angular1) const
{
	float jacobian0[6], jacobian1[6];

	jacobian0[0] = linear0.x;
	jacobian0[1] = linear0.y;
	jacobian0[2] = linear0.z;
	jacobian0[3] = angular0.x;
	jacobian0[4] = angular0.y;
	jacobian0[5] = angular0.z;

	jacobian1[0] = linear1.x;
	jacobian1[1] = linear1.y;
	jacobian1[2] = linear1.z;
	jacobian1[3] = angular1.x;
	jacobian1[4] = angular1.y;
	jacobian1[5] = angular1.z;

	NewtonUserJointAddGeneralRow( m_joint, jacobian0, jacobian1 );
}


void CustomJoint::setRowMinimumFriction( Ogre::Real friction ) const
{
	NewtonUserJointSetRowMinimumFriction( m_joint, friction );
}


void CustomJoint::setRowMaximumFriction( Ogre::Real friction ) const
{
	NewtonUserJointSetRowMaximumFriction( m_joint, friction );
}


void CustomJoint::setRowAcceleration( Ogre::Real accel ) const
{
	NewtonUserJointSetRowAcceleration( m_joint, accel );
}

void CustomJoint::setRowStiffness( Ogre::Real stiffness ) const
{
	NewtonUserJointSetRowStiffness( m_joint, stiffness );
}

void CustomJoint::setRowSpringDamper(Ogre::Real springK, Ogre::Real springD) const
{
	NewtonUserJointSetRowSpringDamperAcceleration( m_joint, springK, springD );
}


void _CDECL CustomJoint::newtonSubmitConstraint( const NewtonJoint* me, float timeStep, int threadIndex )
{
	CustomJoint* j = (CustomJoint*)NewtonJointGetUserData( me );

	j->submitConstraint( (Ogre::Real)timeStep, threadIndex );
}


void _CDECL CustomJoint::newtonGetInfo(const NewtonJoint *me, NewtonJointRecord *info)
{
	CustomJoint* j = (CustomJoint*)NewtonJointGetUserData( me );
}


Ogre::Quaternion CustomJoint::grammSchmidt( const Ogre::Vector3& pin ) const
{
	Ogre::Vector3 front, up, right;
	front = pin;

	front.normalise();
	if (Ogre::Math::Abs( front.z ) > 0.577f)
		right = front.crossProduct( Ogre::Vector3(-front.y, front.z, 0.0f) );
	else
		right = front.crossProduct( Ogre::Vector3(-front.y, front.x, 0.0f) );
	right.normalise();
	up = right.crossProduct( front );

	Ogre::Matrix3 ret;
	ret.FromAxes( front, up, right );

	Ogre::Quaternion quat;
	quat.FromRotationMatrix( ret );

	return quat;
}




}	// end NAMESPACE OgreNewt

