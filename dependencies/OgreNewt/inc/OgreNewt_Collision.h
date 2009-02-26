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
#include "OgreNewt_CollisionSerializer.h"

// OgreNewt namespace.  all functions and classes use this namespace.
namespace OgreNewt
{

class World;

enum _OgreNewtExport CollisionPrimitive
{
	BoxPrimitive				=	SERIALIZE_ID_BOX,
	ConePrimitive				=	SERIALIZE_ID_CONE,
	EllipsoidPrimitive			=	SERIALIZE_ID_SPHERE,
	CapsulePrimitive			=	SERIALIZE_ID_CAPSULE,
	CylinderPrimitive			=	SERIALIZE_ID_CYLINDER,
	CompoundCollisionPrimitive	=	SERIALIZE_ID_COMPOUND,
	ConvexHullPrimitive			=	SERIALIZE_ID_CONVEXHULL,
	ConvexHullModifierPrimitive	=	SERIALIZE_ID_CONVEXMODIFIER,
	ChamferCylinderPrimitive	=	SERIALIZE_ID_CHAMFERCYLINDER,
	TreeCollisionPrimitive		=	SERIALIZE_ID_TREE,
	NullPrimitive				=	SERIALIZE_ID_NULL,
	HeighFieldPrimitive			=	SERIALIZE_ID_HEIGHTFIELD,
	ScenePrimitive				=	SERIALIZE_ID_SCENE
};

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
	void setUserID( unsigned id ) const { NewtonCollisionSetUserID( m_col, id); }

	//! get user ID, for collision callback identification
	unsigned getUserID() const { return NewtonCollisionGetUserID( m_col ); }

	//! make unique
	void makeUnique() { NewtonCollisionMakeUnique( m_world->getNewtonWorld(), m_col ); }

	//! get the Axis-Aligned Bounding Box for this collision shape.
	Ogre::AxisAlignedBox getAABB( const Ogre::Quaternion& orient = Ogre::Quaternion::IDENTITY, const Ogre::Vector3& pos = Ogre::Vector3::ZERO ) const;

	//! Returns the Collisiontype for this Collision
	CollisionPrimitive getCollisionPrimitiveType() const { return getCollisionPrimitiveType( m_col ); } 

	//! Returns the Collisiontype for the given Newton-Collision
	static CollisionPrimitive getCollisionPrimitiveType(const NewtonCollision *col);

	//! friend functions for the Serializer
	friend void CollisionSerializer::exportCollision(const Collision* collision, const Ogre::String& filename);
	friend void CollisionSerializer::importCollision(Ogre::DataStreamPtr& stream, Collision* pDest);

protected:

	NewtonCollision* m_col;
	const World* m_world;

};

//typedef Ogre::SharedPtr<Collision> CollisionPtr;
typedef Collision* CollisionPtr;

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

        //! returns true, if the collision is a trigger-volume
        bool isTriggerVolume() const { return NewtonCollisionIsTriggerVolume(m_col) != 0; }

        //! set collision as trigger-volume
        /*!
                if a collision is marked as a trigger-volume, there's no calculation of contacts, so
                this acts like an accurate aabb test
        */
        void setAsTriggerVolume(bool trigger) { NewtonCollisionSetAsTriggerVolume(m_col, (int)trigger); }
};



//! represents a scalable collision shape.
class _OgreNewtExport ConvexModifierCollision : public Collision
{
public:
	//! constructor
	ConvexModifierCollision( const OgreNewt::World* world, const OgreNewt::Collision* col );

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

