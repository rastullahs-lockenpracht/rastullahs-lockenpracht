/* 
	OgreNewt Library

	Ogre implementation of Newton Game Dynamics SDK

	OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.

		by Walaber

*/
#ifndef _INCLUDE_OGRENEWT_COLLISION
#define _INCLUDE_OGRENEWT_COLLISION

#include "OgreNewt_Prerequisites.h"
#include "OgreNewt_World.h"

// OgreNewt namespace.  all functions and classes use this namespace.
namespace OgreNewt
{


/*
	CLASS DEFINITION:

		Collision

	USE:
		this class represents a NewtonCollision, which is the newton structure
		for collision geometry.
*/
//! represents a shape for collision detection
class _OgreNewtExport Collision
{
public:

	//! constructor
	Collision( const World* world );

	//! destructor
	virtual ~Collision();

	//! retrieve the Newton pointer
	/*!
		retrieves the pointer to the NewtonCollision object.
	*/
	const NewtonCollision* getNewtonCollision() const { return m_col; }

  /*!
    Returns the Newton world this collision belongs to.
  */
  const World* getWorld() const {return m_world;}

	//! set a user ID for collision callback identification
	/*!
		you can set different IDs for each piece in a compound collision object, and then use these IDs in a collision callback to
		determine which part is currently colliding.
	*/
	void setUserID( unsigned id ) const { NewtonConvexCollisionSetUserID( m_col, id); }

	//! get user ID, for collision callback identification
	unsigned getUserID() const { return NewtonConvexCollisionGetUserID( m_col ); }

	//! get the Axis-Aligned Bounding Box for this collision shape.
	Ogre::AxisAlignedBox getAABB( const Ogre::Quaternion& orient = Ogre::Quaternion::IDENTITY, const Ogre::Vector3& pos = Ogre::Vector3::ZERO ) const;

 protected:

	NewtonCollision* m_col;
	const World* m_world;

};

typedef Ogre::SharedPtr<Collision> CollisionPtr;

//! represents a collision shape that is explicitly convex.
class _OgreNewtExport ConvexCollision : public Collision
{
public:
	//! constructor
	ConvexCollision( const World* world );

	//! destructor
	~ConvexCollision();

	//! calculate the volume of the collision shape, useful for buoyancy calculations.
	Ogre::Real calculateVolume() const { return (Ogre::Real)NewtonConvexCollisionCalculateVolume( m_col ); }

	//! calculate the moment of inertia for this collision primitive, along with the theoretical center-of-mass for this shape.
	void calculateInertialMatrix( Ogre::Vector3& inertia, Ogre::Vector3& offset ) const { NewtonConvexCollisionCalculateInertialMatrix( m_col, &inertia.x, &offset.x ); }

};



//! represents a scalable collision shape.
class _OgreNewtExport ConvexModifierCollision : public Collision
{
public:
	//! constructor
	ConvexModifierCollision( const OgreNewt::World* world, const CollisionPtr col );

	//! destructor
	~ConvexModifierCollision();

	//! apply a scalar matrix to the collision
	void setScalarMatrix( const Ogre::Matrix4& mat ) const;

	//! get the scalar matrix for the collision
	Ogre::Matrix4 getScalarMatrix() const;

};


}	// end NAMESPACE OgreNewt

#endif
// _INCLUDE_OGRENEWT_COLLISION

