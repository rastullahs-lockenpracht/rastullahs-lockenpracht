#include "OgreNewt_Body.h"
#include "OgreNewt_Tools.h"


namespace OgreNewt
{

	
Body::Body( const World* W, CollisionPtr col, int bodytype ) 
{
	m_world = W;
	m_collision = col;
	m_type = bodytype;
	m_node = NULL;
	m_matid = NULL;
	
	m_userdata = NULL;

	m_offset = Vector3::ZERO;
	m_orientationBias = Quaternion::IDENTITY;

	m_forcecallback = NULL;
	m_transformcallback = NULL;
	m_buoyancycallback = NULL;

	m_body = NewtonCreateBody( m_world->getNewtonWorld(), col->getNewtonCollision() ); 

	NewtonBodySetUserData( m_body, this );
	NewtonBodySetDestructorCallback( m_body, newtonDestructor );
}

Body::~Body()
{
	if (m_body)
	{
		if (NewtonBodyGetUserData(m_body))
		{
			NewtonBodySetDestructorCallback( m_body, NULL );
			NewtonDestroyBody( m_world->getNewtonWorld(), m_body );
		}
	}
}

// destructor callback
void _CDECL Body::newtonDestructor( const NewtonBody* body )
{
	//newton wants to destroy the body.. so first find it.
	OgreNewt::Body* me;

	me = (OgreNewt::Body*)NewtonBodyGetUserData( body );

	// remove destructor callback
	NewtonBodySetDestructorCallback( body, NULL );
	// remove the user data
	NewtonBodySetUserData( body, NULL );

	//now delete the object.
	delete me;
}


// transform callback
void _CDECL Body::newtonTransformCallback( const NewtonBody* body, const float* matrix )
{
	Ogre::Quaternion orient;
	Ogre::Vector3 pos;
	OgreNewt::Body* me;

	me = (OgreNewt::Body*) NewtonBodyGetUserData( body );

	OgreNewt::Converters::MatrixToQuatPos( matrix, orient, pos );

	if (me->m_transformcallback)
		me->m_transformcallback( me, orient, pos );
}

	
void _CDECL Body::newtonForceTorqueCallback( const NewtonBody* body )
{
	OgreNewt::Body* me = (OgreNewt::Body*)NewtonBodyGetUserData( body );

	if (me->m_forcecallback)
		me->m_forcecallback( me );
}

void Body::standardForceCallback( OgreNewt::Body* me )
{
	//apply a simple gravity force.
	Ogre::Real mass;
	Ogre::Vector3 inertia;

	me->getMassMatrix(mass, inertia);
	Ogre::Vector3 force(0,-9.8,0);
	force *= mass;

	me->addForce( force );

}


void Body::standardTransformCallback( OgreNewt::Body* me, const Ogre::Quaternion& orient, const Ogre::Vector3& pos )
{
	me->m_node->setOrientation( me->m_orientationBias * orient);
	me->m_node->setPosition( pos - orient*me->m_offset);
}



int _CDECL Body::newtonBuoyancyCallback(const int collisionID, void *context, const float* globalSpaceMatrix, float* globalSpacePlane)
{
	OgreNewt::Body* me = (OgreNewt::Body*)context;

	
	Ogre::Quaternion orient;
	Ogre::Vector3 pos;

	OgreNewt::Converters::MatrixToQuatPos( globalSpaceMatrix, orient, pos );

	// call our user' function to get the plane.
	Ogre::Plane theplane;
	
	if (me->m_buoyancycallback(collisionID, me, orient, pos, theplane))
	{
		globalSpacePlane[0] = theplane.normal.x;
		globalSpacePlane[1] = theplane.normal.y;
		globalSpacePlane[2] = theplane.normal.z;

		globalSpacePlane[3] = theplane.d;

		return 1;
	}

	return 0;
}


// attachToNode
void Body::attachToNode(Ogre::SceneNode* node, const Vector3& offset,
						const Quaternion& orientationBias)
{
	m_node = node;
	m_offset = offset;
	m_orientationBias = orientationBias;
	if (m_body)
	{
		setCustomTransformCallback( &Body::standardTransformCallback );
	}
}

void Body::setPositionOrientation( const Ogre::Vector3& pos, const Ogre::Quaternion& orient )
{
	if (m_body)
	{
		float matrix[16];

		OgreNewt::Converters::QuatPosToMatrix( orient, pos, &matrix[0] );
		NewtonBodySetMatrix( m_body, &matrix[0] );

		if (m_node)
		{
			m_node->setOrientation(m_orientationBias * orient);
			m_node->setPosition(pos - orient*m_offset);
		}
	}
}

// set mass matrix
void Body::setMassMatrix( Ogre::Real mass, const Ogre::Vector3& inertia )
{
	if (m_body)
		NewtonBodySetMassMatrix( m_body, (float)mass, (float)inertia.x, (float)inertia.y, (float)inertia.z );
}

void Body::setMass(Ogre::Real mass)
{
	if (m_body)
    {
        Vector3 center = Vector3::ZERO;
        Vector3 inertia = Vector3::ZERO;
        NewtonConvexCollisionCalculateInertialMatrix(m_collision->getNewtonCollision(),
            &inertia.x, &center.x);
        NewtonBodySetMassMatrix(m_body, (float)mass,
            (float)inertia.x, (float)inertia.y, (float)inertia.z);
    }
}

// basic gravity callback
void Body::setStandardForceCallback()
{
	setCustomForceAndTorqueCallback( standardForceCallback );
}

// custom user force callback
void Body::setCustomForceAndTorqueCallback( ForceCallback callback )
{
	if (!m_forcecallback)
	{
		m_forcecallback = callback;
		NewtonBodySetForceAndTorqueCallback( m_body, newtonForceTorqueCallback );
	}
	else
	{
		if (m_forcecallback != callback)
			m_forcecallback = callback;
	}

}

// custom user force callback
void Body::setCustomTransformCallback( TransformCallback callback )
{
	if (!m_transformcallback)
	{
		m_transformcallback = callback;
		NewtonBodySetTransformCallback( m_body, newtonTransformCallback );
	}
	else
	{
		if (m_transformcallback != callback)
			m_transformcallback = callback;
	}

}

//set collision
void Body::setCollision(CollisionPtr col)
{
	NewtonBodySetCollision( m_body, col->getNewtonCollision() );
	m_collision = col;
}

//get collision
CollisionPtr Body::getCollision() const
{
	return m_collision;
}

//get material group ID
const OgreNewt::MaterialID* Body::getMaterialGroupID() const
{
	if (m_matid)
		return m_matid;
	else
		return NULL;
}


// get position and orientation
void Body::getPositionOrientation( Ogre::Vector3& pos, Ogre::Quaternion& orient ) const
{
	float matrix[16];

	NewtonBodyGetMatrix( m_body, matrix );
	OgreNewt::Converters::MatrixToQuatPos( matrix, orient, pos );
}

void Body::getMassMatrix( Ogre::Real& mass, Ogre::Vector3& inertia ) const
{
	NewtonBodyGetMassMatrix( m_body, &mass, &inertia.x, &inertia.y, &inertia.z );
}

void Body::getInvMass( Ogre::Real& mass, Ogre::Vector3& inertia ) const
{
	NewtonBodyGetInvMass( m_body, &mass, &inertia.x, &inertia.y, &inertia.z );
}

Ogre::Vector3 Body::getOmega() const
{
	Ogre::Vector3 ret;
	NewtonBodyGetOmega( m_body, &ret.x );
	return ret;
}

Ogre::Vector3 Body::getVelocity() const
{
	Ogre::Vector3 ret;
	NewtonBodyGetVelocity( m_body, &ret.x );
	return ret;
}

Ogre::Vector3 Body::getAngularDamping() const
{
	Ogre::Vector3 ret;
	NewtonBodyGetAngularDamping( m_body, &ret.x );
	return ret;
}

Ogre::Vector3 Body::getCenterOfMass() const
{
	Ogre::Vector3 ret;
	NewtonBodyGetCentreOfMass( m_body, &ret.x );
	return ret;
}

void Body::addBouyancyForce( Ogre::Real fluidDensity, Ogre::Real fluidLinearViscosity, Ogre::Real fluisAngularViscosity, const Ogre::Vector3& gravity, buoyancyPlaneCallback callback )
{
	// call the newton function.
	if (callback)
		m_buoyancycallback = callback;
	else
		m_buoyancycallback = NULL;

	NewtonBodyAddBuoyancyForce( m_body, fluidDensity, fluidLinearViscosity, fluisAngularViscosity,
		&gravity.x, newtonBuoyancyCallback, this );

	m_buoyancycallback = NULL;
}

void Body::addGlobalForce( const Ogre::Vector3& force, const Ogre::Vector3& pos )
{
	Ogre::Vector3 bodypos;
	Ogre::Quaternion bodyorient;
	getPositionOrientation( bodypos, bodyorient );

	Ogre::Vector3 topoint = pos - bodypos;
	Ogre::Vector3 torque = topoint.crossProduct( force );

	addForce( force );
	addTorque( torque );
}

void Body::addLocalForce( const Ogre::Vector3& force, const Ogre::Vector3& pos )
{
	Ogre::Vector3 bodypos;
	Ogre::Quaternion bodyorient;

	getPositionOrientation( bodypos, bodyorient );

	Ogre::Vector3 globalforce = bodyorient * force;
	Ogre::Vector3 globalpoint = (bodyorient * pos) + bodypos;

	addGlobalForce( globalforce, globalpoint );
}

Ogre::Vector3 Body::getOffset() const
{
    return m_offset;
}

void Body::setOffset(const Ogre::Vector3& offset)
{
    m_offset = offset;
}

Ogre::Quaternion Body::getOrientationBias() const
{
	return m_orientationBias;
}

void Body::setOrientationBias(const Ogre::Quaternion& orientationbias)
{
	m_orientationBias = orientationbias;
}

// --------------------------------------------------------------------------------------


}
