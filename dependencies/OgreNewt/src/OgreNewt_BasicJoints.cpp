#include <Newton.h>
#include <OgreNewt_BasicJoints.h>
#include <OgreNewt_World.h>
#include <OgreNewt_Body.h>

namespace OgreNewt
{

namespace BasicJoints
{
	
BallAndSocket::BallAndSocket( const World* world, const OgreNewt::Body* child, const OgreNewt::Body* parent, const Ogre::Vector3& pos ) : Joint()
{
	m_world = world;

	if (parent)
		m_joint = NewtonConstraintCreateBall( world->getNewtonWorld(), &pos.x, child->getNewtonBody(), parent->getNewtonBody() );
	else
		m_joint = NewtonConstraintCreateBall( world->getNewtonWorld(), &pos.x, child->getNewtonBody(), NULL );


	// all constructors inherited from Joint MUST call these 2 functions to make the joint function properly.
	NewtonJointSetUserData( m_joint, this );
	NewtonJointSetDestructor( m_joint, destructor );
										
}

BallAndSocket::~BallAndSocket()
{
	// nothing, the ~Joint() function will take care of us.
}

Ogre::Vector3 BallAndSocket::getJointAngle() const
{
	Ogre::Vector3 ret;

	NewtonBallGetJointAngle( m_joint, &ret.x );

	return ret;
}


Ogre::Vector3 BallAndSocket::getJointOmega() const
{
	Ogre::Vector3 ret;

	NewtonBallGetJointOmega( m_joint, &ret.x );

	return ret;
}


Ogre::Vector3 BallAndSocket::getJointForce() const
{
	Ogre::Vector3 ret;

	NewtonBallGetJointForce( m_joint, &ret.x );

	return ret;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


Hinge::Hinge( const World* world, const OgreNewt::Body* child, const OgreNewt::Body* parent, const Ogre::Vector3& pos, const Ogre::Vector3& pin ) : Joint()
{
	m_world = world;

	if (parent)
	{
		m_joint = NewtonConstraintCreateHinge( world->getNewtonWorld(), &pos.x, &pin.x,
												child->getNewtonBody(), parent->getNewtonBody() );
	}
	else
	{
		m_joint = NewtonConstraintCreateHinge( world->getNewtonWorld(), &pos.x, &pin.x,
												child->getNewtonBody(), NULL );
	}

	NewtonJointSetUserData( m_joint, this );
	NewtonJointSetDestructor( m_joint, destructor );
	NewtonHingeSetUserCallback( m_joint, newtonCallback );

	m_callback = NULL;
}

Hinge::~Hinge()
{
}


Ogre::Vector3 Hinge::getJointForce() const
{
	Ogre::Vector3 ret;

	NewtonHingeGetJointForce( m_joint, &ret.x );

	return ret;
}

unsigned _CDECL Hinge::newtonCallback( const NewtonJoint* hinge, NewtonHingeSliderUpdateDesc* desc )
{
	Hinge* me = (Hinge*)NewtonJointGetUserData( hinge );

	me->m_desc = desc;
	me->m_retval = 0;

	if (me->m_callback)
		(*me->m_callback)( me );

	me->m_desc = NULL;

	return me->m_retval;
}

/////// CALLBACK FUNCTIONS ///////
void Hinge::setCallbackAccel( Ogre::Real accel )
{
	if (m_desc)
	{
		m_retval = 1;
		m_desc->m_accel = (float)accel;
	}
}

void Hinge::setCallbackFrictionMin( Ogre::Real min )
{
	if (m_desc)
	{
		m_retval = 1;
		m_desc->m_minFriction = (float)min;
	}
}

void Hinge::setCallbackFrictionMax( Ogre::Real max )
{
	if (m_desc)
	{
		m_retval = 1;
		m_desc->m_maxFriction = (float)max;
	}
}

Ogre::Real Hinge::getCallbackTimestep() const
{
	if (m_desc)
		return (Ogre::Real)m_desc->m_timestep;
	else
		return 0.0;
}

Ogre::Real Hinge::calculateStopAlpha( Ogre::Radian angle ) const
{
	if (m_desc)
		return (Ogre::Real)NewtonHingeCalculateStopAlpha( m_joint, m_desc, (float)angle.valueRadians() );
	else
		return 0.0;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////



Slider::Slider( const World* world, const OgreNewt::Body* child, const OgreNewt::Body* parent, const Ogre::Vector3& pos, const Ogre::Vector3& pin ) : Joint()
{
	m_world = world;

	if (parent)
	{
		m_joint = NewtonConstraintCreateSlider( world->getNewtonWorld(), &pos.x, &pin.x,
												child->getNewtonBody(), parent->getNewtonBody() );
	}
	else
	{
		m_joint = NewtonConstraintCreateSlider( world->getNewtonWorld(), &pos.x, &pin.x,
												child->getNewtonBody(), NULL );
	}

	NewtonJointSetUserData( m_joint, this );
	NewtonJointSetDestructor( m_joint, destructor );
	NewtonSliderSetUserCallback( m_joint, newtonCallback );

	m_callback = NULL;
}

Slider::~Slider()
{
}

Ogre::Vector3 Slider::getJointForce() const
{
	Ogre::Vector3 ret;

	NewtonSliderGetJointForce( m_joint, &ret.x );

	return ret;
}

unsigned _CDECL Slider::newtonCallback( const NewtonJoint* slider, NewtonHingeSliderUpdateDesc* desc )
{
	Slider* me = (Slider*)NewtonJointGetUserData( slider );

	me->m_desc = desc;
	me->m_retval = 0;

	if (me->m_callback)
		(*me->m_callback)( me );

	me->m_desc = NULL;

	return me->m_retval;
}


/////// CALLBACK FUNCTIONS ///////
void Slider::setCallbackAccel( Ogre::Real accel )
{
	if (m_desc)
	{
		m_retval = 1;
		m_desc->m_accel = (float)accel;
	}
}

void Slider::setCallbackFrictionMin( Ogre::Real min )
{
	if (m_desc)
	{
		m_retval = 1;
		m_desc->m_minFriction = (float)min;
	}
}

void Slider::setCallbackFrictionMax( Ogre::Real max )
{
	if (m_desc)
	{
		m_retval = 1;
		m_desc->m_maxFriction = (float)max;
	}
}

Ogre::Real Slider::getCallbackTimestep() const
{
	if (m_desc)
		return (Ogre::Real)m_desc->m_timestep;
	else
		return 0.0;
}

Ogre::Real Slider::calculateStopAccel( Ogre::Real dist ) const
{
	if (m_desc)
		return (Ogre::Real)NewtonSliderCalculateStopAccel( m_joint, m_desc, (float)dist );
	else
		return 0.0;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////



Universal::Universal( const World* world, const OgreNewt::Body* child, const OgreNewt::Body* parent, const Ogre::Vector3& pos, const Ogre::Vector3& pin0, const Ogre::Vector3& pin1 ) : Joint()
{
	m_world = world;

	if (parent)
	{
		m_joint = NewtonConstraintCreateUniversal( world->getNewtonWorld(), &pos.x, &pin0.x, &pin1.x,
												child->getNewtonBody(), parent->getNewtonBody() );
	}
	else
	{
		m_joint = NewtonConstraintCreateUniversal( world->getNewtonWorld(), &pos.x, &pin0.x, &pin1.x,
												child->getNewtonBody(), NULL );
	}

	NewtonJointSetUserData( m_joint, this );
	NewtonJointSetDestructor( m_joint, destructor );
	NewtonUniversalSetUserCallback( m_joint, newtonCallback );

	m_callback = NULL;
}

Universal::~Universal()
{
}

Ogre::Vector3 Universal::getJointForce() const
{
	Ogre::Vector3 ret;

	NewtonUniversalGetJointForce( m_joint, &ret.x );

	return ret;
}

unsigned _CDECL Universal::newtonCallback( const NewtonJoint* universal, NewtonHingeSliderUpdateDesc* desc )
{
	Universal* me = (Universal*)NewtonJointGetUserData( universal );

	me->m_desc = desc;
	me->m_retval = 0;

	if (me->m_callback)
		(*me->m_callback)( me );

	me->m_desc = NULL;

	return me->m_retval;
}


/////// CALLBACK FUNCTIONS ///////
void Universal::setCallbackAccel( Ogre::Real accel, unsigned int axis )
{
	if (axis > 1) { return; }

	if (m_desc)
	{
		m_retval |= axis;
		m_desc[axis].m_accel = (float)accel;
	}
}

void Universal::setCallbackFrictionMax( Ogre::Real max, unsigned int axis )
{
	if (axis > 1) { return; }

	if (m_desc)
	{
		m_retval |= axis;
		m_desc[axis].m_maxFriction = (float)max;
	}
}

void Universal::setCallbackFrictionMin( Ogre::Real min, unsigned int axis )
{
	if (axis > 1) { return; }

	if (m_desc)
	{
		m_retval |= axis;
		m_desc[axis].m_minFriction = (float)min;
	}
}

Ogre::Real Universal::getCallbackTimestep() const
{
	if (m_desc)
		return (Ogre::Real)m_desc->m_timestep;
	else
		return 0.0;
}

Ogre::Real Universal::calculateStopAlpha0( Ogre::Real angle ) const
{
	if (m_desc)
		return (Ogre::Real)NewtonUniversalCalculateStopAlpha0( m_joint, m_desc, (float)angle );
	else
		return 0.0;
}

Ogre::Real Universal::calculateStopAlpha1( Ogre::Real angle ) const
{
	if (m_desc)
		return (Ogre::Real)NewtonUniversalCalculateStopAlpha1( m_joint, m_desc, (float)angle );
	else
		return 0.0;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////



UpVector::UpVector( const World* world, const Body* body, const Ogre::Vector3& pin )
{
	m_world = world;

	m_joint = NewtonConstraintCreateUpVector( world->getNewtonWorld(), &pin.x, body->getNewtonBody() );

	NewtonJointSetUserData( m_joint, this );
	NewtonJointSetDestructor( m_joint, destructor );

}

UpVector::~UpVector()
{
}

Ogre::Vector3 UpVector::getPin() const
{
	Ogre::Vector3 ret;

	NewtonUpVectorGetPin( m_joint, &ret.x );

	return ret;
}





}	// end NAMESPACE BasicJoints




///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


namespace PrebuiltCustomJoints
{

Custom2DJoint::Custom2DJoint(const OgreNewt::Body* body, const Ogre::Vector3& pin ) : CustomJoint( 4, body, NULL )
{
	mPin = pin;
	Ogre::Quaternion bodyorient;
	Ogre::Vector3 bodypos;

	body->getPositionOrientation( bodypos, bodyorient );

	pinAndDirToLocal( bodypos, pin, mLocalOrient0, mLocalPos0, mLocalOrient1, mLocalPos1 );

	// initialize variables
	mMin = mMax = Ogre::Degree(0);
	mLimitsOn = false;
	mAccel = 0.0f;

}


void Custom2DJoint::submitConstraint()
{
	// get the global orientations.
	Ogre::Quaternion globalOrient0, globalOrient1;
	Ogre::Vector3 globalPos0, globalPos1;

	localToGlobal( mLocalOrient0, mLocalPos0, globalOrient0, globalPos0, 0 );
	localToGlobal( mLocalOrient1, mLocalPos1, globalOrient1, globalPos1, 1 );

	// calculate all main 6 vectors.
	Ogre::Vector3 bod0X = globalOrient0 * Ogre::Vector3( Ogre::Vector3::UNIT_X );
	Ogre::Vector3 bod0Y = globalOrient0 * Ogre::Vector3( Ogre::Vector3::UNIT_Y );
	Ogre::Vector3 bod0Z = globalOrient0 * Ogre::Vector3( Ogre::Vector3::UNIT_Z );
	
	Ogre::Vector3 bod1X = globalOrient1 * Ogre::Vector3( Ogre::Vector3::UNIT_X );
	Ogre::Vector3 bod1Y = globalOrient1 * Ogre::Vector3( Ogre::Vector3::UNIT_Y );
	Ogre::Vector3 bod1Z = globalOrient1 * Ogre::Vector3( Ogre::Vector3::UNIT_Z );

	Ogre::LogManager::getSingletonPtr()->logMessage(" Submit Constraint   bod0X:"+Ogre::StringConverter::toString( bod0X )+
		"   bod1X:"+Ogre::StringConverter::toString( bod1X ) );
	
	// ---------------------------------------------------------------
	// first add a linear row to keep the body on the plane.
	addLinearRow( globalPos0, globalPos1, bod0X );

	// have we strayed from the plane along the normal?
	Ogre::Plane thePlane( bod0X, globalPos0 );
	Ogre::Real stray = thePlane.getDistance( globalPos1 );
	if (stray > 0.0001f)
	{
		// we have strayed, apply acceleration to move back to 0 in one timestep.
		Ogre::Real accel = (stray / m_body0->getWorld()->getTimeStep());
		if (thePlane.getSide( globalPos1 ) == Ogre::Plane::NEGATIVE_SIDE) { accel = -accel; }

		setRowAcceleration( accel );
	}

	// see if the main axis (pin) has wandered off.
	Ogre::Vector3 latDir = bod0X.crossProduct( bod1X );
	Ogre::Real latMag = latDir.squaredLength();

	if (latMag > 1.0e-6f)
	{
		// has wandered a bit, we need to correct.  first find the angle off.
		latMag = Ogre::Math::Sqrt( latMag );
		latDir.normalise();
		Ogre::Radian angle = Ogre::Math::ASin( latMag );

		// ---------------------------------------------------------------
		addAngularRow( angle, latDir );

		// ---------------------------------------------------------------
		// secondary correction for stability.
		Ogre::Vector3 latDir2 = latDir.crossProduct( bod1X );
		addAngularRow( Ogre::Radian(0.0f), latDir2 );
	}
	else
	{
		// ---------------------------------------------------------------
		// no major change, just add 2 simple constraints.
		addAngularRow( Ogre::Radian(0.0f), bod1Y );
		addAngularRow( Ogre::Radian(0.0f), bod1Z );
	}

	// calculate the current angle.
	Ogre::Real cos = bod0Y.dotProduct( bod1Y );
	Ogre::Real sin = (bod0Y.crossProduct( bod1Y )).dotProduct( bod0X );

	mAngle = Ogre::Math::ATan2( sin, cos );

	if (mLimitsOn)
	{
		if (mAngle > mMax)
		{
			Ogre::Radian diff = mAngle - mMax;

			addAngularRow( diff, bod0X );
			setRowStiffness( 1.0f );
		}
		else if (mAngle < mMin)
		{
			Ogre::Radian diff = mAngle - mMin;

			addAngularRow( diff, bod0X );
			setRowStiffness( 1.0f );
		}
	}
	else
	{
		if (mAccel != 0.0f)
		{
			addAngularRow( Ogre::Radian(0.0f), bod0X );
			setRowAcceleration( mAccel );

			mAccel = 0.0f;
		}
	}
	
}


CustomRigidJoint::CustomRigidJoint(OgreNewt::Body *child, OgreNewt::Body *parent, Ogre::Vector3 dir, Ogre::Vector3 pos) : OgreNewt::CustomJoint(6,child,parent)
{
	// calculate local offsets.
	pinAndDirToLocal( pos, dir, mLocalOrient0, mLocalPos0, mLocalOrient1, mLocalPos1 );
}

CustomRigidJoint::~CustomRigidJoint()
{
}

void CustomRigidJoint::submitConstraint()
{
	// get globals.
	Ogre::Vector3 globalPos0, globalPos1;
	Ogre::Quaternion globalOrient0, globalOrient1;

	localToGlobal( mLocalOrient0, mLocalPos0, globalOrient0, globalPos0, 0 );
	localToGlobal( mLocalOrient1, mLocalPos1, globalOrient1, globalPos1, 1 );

	// apply the constraints!
	addLinearRow( globalPos0, globalPos1, globalOrient0 * Ogre::Vector3::UNIT_X );
	addLinearRow( globalPos0, globalPos1, globalOrient0 * Ogre::Vector3::UNIT_Y );
	addLinearRow( globalPos0, globalPos1, globalOrient0 * Ogre::Vector3::UNIT_Z );

	// now find a point off 10 units away.
	globalPos0 = globalPos0 + (globalOrient0 * (Ogre::Vector3::UNIT_X * 10.0f));
	globalPos1 = globalPos1 + (globalOrient1 * (Ogre::Vector3::UNIT_X * 10.0f));

	// apply the constraints!
	addLinearRow( globalPos0, globalPos1, globalOrient0 * Ogre::Vector3::UNIT_Y );
	addLinearRow( globalPos0, globalPos1, globalOrient0 * Ogre::Vector3::UNIT_Z );

	Ogre::Vector3 xdir0 = globalOrient0 * Ogre::Vector3::UNIT_X;
	Ogre::Vector3 xdir1 = globalOrient1 * Ogre::Vector3::UNIT_X;

	Ogre::Radian angle = Ogre::Math::ACos( xdir0.dotProduct( xdir1 ) );
	addAngularRow( angle, globalOrient0 * Ogre::Vector3::UNIT_X );
}


CustomPulleyJoint::CustomPulleyJoint( Ogre::Real gearRatio, const Body* parent, const Body* child, const Ogre::Vector3& parentPin, const Ogre::Vector3& childPin ) : OgreNewt::CustomJoint(1, parent, child)
{
	mGearRatio = gearRatio;

	Ogre::Vector3 dummyPos;
	Ogre::Quaternion dummyOrient;
	Ogre::Vector3 pivot(0.0f, 0.0f, 0.0f);

	// calculate local matrices.  in this case we have 2 pins, so we call twice, once for each body.
	pinAndDirToLocal( pivot, parentPin, mLocalOrient0, mLocalPos0, dummyOrient, dummyPos );
	pinAndDirToLocal( pivot, childPin, dummyOrient, dummyPos, mLocalOrient1, mLocalPos1 );	
}

void CustomPulleyJoint::submitConstraint()
{
	Ogre::Real w0, w1;
	Ogre::Real deltat;
	Ogre::Real relAccel, relVel;
	Ogre::Vector3 vel0, vel1;

	Ogre::Vector3 globalPos0, globalPos1;
	Ogre::Quaternion globalOrient0, globalOrient1;

	Ogre::Vector3 xdir0, xdir1;

	// get global matrices.
	localToGlobal( mLocalOrient0, mLocalPos0, globalOrient0, globalPos0, 0);
	localToGlobal( mLocalOrient1, mLocalPos1, globalOrient1, globalPos1, 1);

	xdir0 = globalOrient0 * Ogre::Vector3(Ogre::Vector3::UNIT_X);
	xdir1 = globalOrient1 * Ogre::Vector3(Ogre::Vector3::UNIT_X);

	// velocities for both bodies.
	vel0 = m_body0->getVelocity();
	vel1 = m_body1->getVelocity();

	// relative angular velocity.
	w0 = vel0.dotProduct( xdir0 );
	w1 = vel1.dotProduct( xdir1 );

	// relative velocity.
	relVel = w0 + (mGearRatio * w1);

	deltat = m_body0->getWorld()->getTimeStep();
	relAccel = relVel / deltat;

	addGeneralRow( xdir0, Ogre::Vector3::ZERO, xdir1, Ogre::Vector3::ZERO );
	setRowAcceleration( relAccel );
}


CustomGearJoint::CustomGearJoint(Ogre::Real gearRatio, const OgreNewt::Body *parent, const OgreNewt::Body *child, const Ogre::Vector3& parentPin, const Ogre::Vector3& childPin) : OgreNewt::CustomJoint(1, parent, child)
{
	mGearRatio = gearRatio;

	Ogre::Vector3 dummyPos;
	Ogre::Quaternion dummyOrient;
	Ogre::Vector3 pivot(0.0f, 0.0f, 0.0f);

	// calculate local matrices.  in this case we have 2 pins, so we call twice, once for each body.
	pinAndDirToLocal( pivot, parentPin, mLocalOrient0, mLocalPos0, dummyOrient, dummyPos );
	pinAndDirToLocal( pivot, childPin, dummyOrient, dummyPos, mLocalOrient1, mLocalPos1 );	
}

void CustomGearJoint::submitConstraint()
{
	Ogre::Real w0, w1;
	Ogre::Real deltat;
	Ogre::Real relAccel, relOmega;
	Ogre::Vector3 omega0, omega1;

	Ogre::Vector3 globalPos0, globalPos1;
	Ogre::Quaternion globalOrient0, globalOrient1;

	Ogre::Vector3 xdir0, xdir1;

	// get global matrices.
	localToGlobal( mLocalOrient0, mLocalPos0, globalOrient0, globalPos0, 0);
	localToGlobal( mLocalOrient1, mLocalPos1, globalOrient1, globalPos1, 1);

	xdir0 = globalOrient0 * Ogre::Vector3(Ogre::Vector3::UNIT_X);
	xdir1 = globalOrient1 * Ogre::Vector3(Ogre::Vector3::UNIT_X);

	// velocities for both bodies.
	omega0 = m_body0->getOmega();
	omega1 = m_body1->getOmega();

	// relative angular velocity.
	w0 = omega0.dotProduct( xdir0 );
	w1 = omega1.dotProduct( xdir1 );

	// relative velocity.
	relOmega = w0 + (mGearRatio * w1);

	deltat = m_body0->getWorld()->getTimeStep();
	relAccel =  - relOmega / deltat;

	addGeneralRow( Ogre::Vector3::ZERO, xdir0, Ogre::Vector3::ZERO, xdir1 );
	setRowAcceleration( relAccel );
}





}	// end NAMESPACE PrebuiltCustomJoints




}	// end NAMESPACE OgreNewt
