#include <Ogre.h>
#include <OgreNewt_Collision.h>
#include <OgreNewt_World.h>
#include <OgreNewt_Tools.h>


namespace OgreNewt
{

	
Collision::Collision( const World* world )
{
	m_world = world;
}

Collision::~Collision()
{
	if (m_world->getNewtonWorld())
		NewtonReleaseCollision( m_world->getNewtonWorld(), m_col );
}


Ogre::AxisAlignedBox Collision::getAABB( const Ogre::Quaternion& orient, const Ogre::Vector3& pos ) const
{
	Ogre::AxisAlignedBox box;
	Ogre::Vector3 min, max;
	float matrix[16];
	OgreNewt::Converters::QuatPosToMatrix( orient, pos, matrix );

	NewtonCollisionCalculateAABB( m_col, matrix, &min.x, &max.x );

	box = Ogre::AxisAlignedBox(min, max);
	return box;
}



ConvexCollision::ConvexCollision( const OgreNewt::World* world ) : Collision( world )
{
}

ConvexCollision::~ConvexCollision()
{
}



ConvexModifierCollision::ConvexModifierCollision(const World* world, CollisionPtr col) : Collision(world)
{
	m_col = NewtonCreateConvexHullModifier( world->getNewtonWorld(), col->getNewtonCollision() );
}

ConvexModifierCollision::~ConvexModifierCollision()
{
}

void ConvexModifierCollision::setScalarMatrix( const Ogre::Matrix4& mat ) const
{
	float matrix[16];
	OgreNewt::Converters::Matrix4ToMatrix( mat, matrix );

	NewtonConvexHullModifierSetMatrix( m_col, matrix );	
}

Ogre::Matrix4 ConvexModifierCollision::getScalarMatrix() const
{
	float matrix[16];
	Ogre::Matrix4 mat;
	
	NewtonConvexHullModifierGetMatrix( m_col, matrix );

	OgreNewt::Converters::MatrixToMatrix4( matrix, mat );

	return mat;
}



}	// end NAMESPACE OgreNewt
