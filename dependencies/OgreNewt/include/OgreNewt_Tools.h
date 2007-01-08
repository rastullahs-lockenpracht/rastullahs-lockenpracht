/* 
	OgreNewt Library

	Ogre implementation of Newton Game Dynamics SDK

	OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.

		by Walaber

*/
#ifndef _INCLUDE_OGRENEWT_TOOLS
#define _INCLUDE_OGRENEWT_TOOLS


#include <Ogre.h>
#include <Newton.h>
#include "OgreNewt_World.h"
#include "OgreNewt_Collision.h"
using namespace Ogre;

namespace OgreNewt
{

	//! set of handy convertors.
	namespace Converters
	{
		//! Take a Newton matrix and create a Quaternion + Position_vector
		/*!
			\param matrix input newton matrix (float[16])
			\param quat returned quaternion
			\param pos returned position vector
		*/
		_OgreNewtExport void MatrixToQuatPos( const float* matrix, Quaternion& quat, Vector3 &pos );
	

		//! Take a Quaternion and Position Matrix and create a Newton-happy float matrix!
		/*!
			\param quat input quaternion
			\param pos input position vector
			\param matrix returned matrix (float[16])
		*/
		_OgreNewtExport void QuatPosToMatrix( const Quaternion& quat, const Vector3 &pos, float* matrix );

		//! Take a Newton matrix and make it into an Ogre::Matrix4.
		/*!
			\param matrix_in input matrix from Newton (float[16])
			\param matrix_out output Ogre::Matrix4 object.
		*/
		_OgreNewtExport void MatrixToMatrix4( const float* matrix_in, Ogre::Matrix4& matrix_out );

		//! Take an Ogre::Matrix4 and make it into a Newton-happy matrix.
		/*!
			\param matrix_in Ogre::Matrix4 to be converted.
			\param matrix_out Newton-happy output matrix (float[16])
		*/
		_OgreNewtExport void Matrix4ToMatrix( const Ogre::Matrix4& matrix_in, float* matrix_out );
	}

	//! low-level collision commands.
	namespace CollisionTools
	{

		//! find the point on a collision primitive closest to a global point.
		/*!
			\param world pointer to world
			\param globalpt point to check distance from.
			\param col collision primitive pointer
			\param colorient orientation of the collision primitive
			\param colpos position of the collision primitive
			\param retpos returned position on the collision primitive in global space
			\param retnormal returned normal on the collision primitive in global space
		*/
		_OgreNewtExport int CollisionPointDistance( const OgreNewt::World* world, const Ogre::Vector3& globalpt, 
									CollisionPtr col, const Ogre::Quaternion& colorient, const Ogre::Vector3& colpos, 
									Ogre::Vector3& retpt, Ogre::Vector3& retnormal );
									


		//! find the nearest 2 points between 2 collision shapes.
		/*!
			\param world pointer to world
			\param colA pointer to collision primitive A
			\param colOrientA orientation of collision primitive A
			\param colPosA position of collision primitive A
			\param colB pointer to collision primitive B
			\param colOrientB orientation of collision primitive B
			\param colPosB position of collision primitive B
			\param retPosA returned position on collision primitive A
			\param retPosB returned position on collision primitive B
			\param retNorm returned collision normal
		*/
		_OgreNewtExport int CollisionClosestPoint( const OgreNewt::World* world, const OgreNewt::CollisionPtr colA, const Ogre::Quaternion& colOrientA, const Ogre::Vector3& colPosA,
															CollisionPtr colB, const Ogre::Quaternion& colOrientB, const Ogre::Vector3& colPosB,
															Ogre::Vector3& retPosA, Ogre::Vector3& retPosB, Ogre::Vector3& retNorm );


		//! manual collision between collision primitives.
		/*!
			This function allows you to perform a manual collision check on 2 collision primitives.
			the function returns the number of contacts found.
			\param world pointer to world
			\param maxSize max number of contact points you can receive
			\param colA pointer to Collision primitive A
			\param colOrientA orientation of collision primitive A
			\param colPosA position of collision primitive A
			\param colB pointer to collision primitive B
			\param colOrientB orientation of collision orimitive B
			\param colPosB position of collision primitive B
			\param retContactPts returned array of contact points
			\param retNormals returned normals for each contact
			\param retPenetrations returned penetrations for each contact.
		*/
		_OgreNewtExport int CollisionCollide(  const OgreNewt::World* world, int maxSize, 
			CollisionPtr colA, const Ogre::Quaternion& colOrientA, const Ogre::Vector3& colPosA,
			CollisionPtr colB, const Ogre::Quaternion& colOrientB, const Ogre::Vector3& colPosB,
			Ogre::Vector3* retContactPts, Ogre::Vector3* retNormals, Ogre::Real* retPenetrations );


		//! manual collision between moving primitives.
		/*!
			This is the most advanced collision function, that takes 2 primitives with velocities and omegas,
			and calculates if the will contact with each other over a set timestep.
			\param world pointer to world
			\param maxSize max number of contact points you can receive
			\param colA pointer to Collision primitive A
			\param colOrientA orientation of collision primitive A
			\param colPosA position of collision primitive A
			\param colVelA velocity of collision primitive A
			\param colOmegaA omega of collision primitive A
			\param colB pointer to collision primitive B
			\param colOrientB orientation of collision orimitive B
			\param colPosB position of collision primitive B
			\param colVelB velocity of collision primitive B
			\param colOmegaB omega of collision primitive B
			\param retTimeOfImpact time of collision
			\param retContactPts returned array of contact points
			\param retNormals returned normals for each contact
			\param retPenetrations returned penetrations for each contact.
		*/
		_OgreNewtExport int CollisionCollideContinue( const OgreNewt::World* world, int maxSize, Ogre::Real timeStep,
			const CollisionPtr colA, const Ogre::Quaternion& colOrientA, const Ogre::Vector3& colPosA, const Ogre::Vector3& colVelA, const Ogre::Vector3& colOmegaA,
			const CollisionPtr colB, const Ogre::Quaternion& colOrientB, const Ogre::Vector3& colPosB, const Ogre::Vector3& colVelB, const Ogre::Vector3& colOmegaB,
			Ogre::Real& retTimeOfImpact, Ogre::Vector3* retContactPts, Ogre::Vector3* retNormals, Ogre::Real* retPenetrations );


		//! local raycast on a collision object.
		/*!
			This function performs a local raycast on a single collision primitive shape.  the function returns the distance to the
			point of intersection as a scalar between [0,1].
			\param col pointer to collision shape on which to cast the ray
			\param startPt starting point of the ray, in local space of the collision shape
			\param endPt end point of the ray, in local space of the collision shape
			\param normal returned normal where the ray hit the collision.
			\param colID returned ID of the collision primitive hit.
		*/
		_OgreNewtExport Ogre::Real CollisionRayCast( CollisionPtr col, const Ogre::Vector3& startPt, const Ogre::Vector3& endPt, 
														Ogre::Vector3& retNorm, int& retColID );
				

		//! calculate the AABB of a collision primitive in an arbitrary orientation
		/*!
			Calculates the global AABB of a collision primitive in world space, given an orientation and position.
			\param col collision shape to calculate.
			\param orient world orientation of the collision.
			\param pos world position of the collision.
		*/
		_OgreNewtExport Ogre::AxisAlignedBox CollisionCalculateAABB( CollisionPtr col, const Ogre::Quaternion& orient, const Ogre::Vector3& pos );

	}	// end namespace "ColliionTools"


	//! calculators of MomentOfInertia
	namespace MomentOfInertia
	{

		//! calculate moment of inertia for a solid ellipsoid
		_OgreNewtExport Ogre::Vector3 CalcEllipsoidSolid( Ogre::Real mass, const Ogre::Vector3& size );

		//! calculate moment of inertia for a solid sphere
		_OgreNewtExport Ogre::Vector3 CalcSphereSolid( Ogre::Real mass, Ogre::Real radius );

		//! calculate moment of inertia for a hollow sphere
		_OgreNewtExport Ogre::Vector3 CalcSphereHollor(Ogre::Real mass, Ogre::Real radius );
		
		//! calculate moment of inertia for a solid box
		_OgreNewtExport Ogre::Vector3 CalcBoxSolid(Ogre::Real mass, const Ogre::Vector3& size );

		//! calculate moment of inertia for a solid cylinder
		_OgreNewtExport Ogre::Vector3 CalcCylinderSolid(Ogre::Real mass, Ogre::Real radius, Ogre::Real height );
		
	}

}	// end NAMESPACE OgreNewt

#endif
