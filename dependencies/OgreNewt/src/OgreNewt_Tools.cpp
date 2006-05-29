#include "OgreNewt_Tools.h"

namespace OgreNewt
{

	namespace Converters
	{
	
	//! Take a Newton matrix and create a Quaternion + Position_vector
	void MatrixToQuatPos( const float* matrix, Ogre::Quaternion& quat, Ogre::Vector3 &pos )
	{
		// this takes a matrix returned by Newton, and creates a Quaternion
		// and position Vector3, which is more meaningful for Ogre.
		quat = Quaternion( Matrix3(	matrix[0], matrix[4], matrix[8],
						matrix[1], matrix[5], matrix[9],
						matrix[2], matrix[6], matrix[10] ) );
	
		pos = Vector3( matrix[12], matrix[13], matrix[14] );
	}

	//! Take a Quaternion and Position Matrix and create a Newton-happy float matrix!
	void QuatPosToMatrix( const Ogre::Quaternion& quat, const Ogre::Vector3 &pos, float* matrix )
	{
		// this takes a Quaternion and a Vector3 and creates a float array
		// which is more meaningful to Newton.
		Matrix3 rot;
		Vector3 xcol, ycol, zcol;
		
		quat.ToRotationMatrix( rot );	// creates a 3x3 rotation matrix from the Quaternion.

		xcol = rot.GetColumn(0);
		ycol = rot.GetColumn(1);
		zcol = rot.GetColumn(2);
	
		// now fill the final matrix with the appropriate data:
		matrix[0] = xcol.x;
		matrix[1] = xcol.y;
		matrix[2] = xcol.z;
		matrix[3] = 0.0f;
	
		matrix[4] = ycol.x;
		matrix[5] = ycol.y;
		matrix[6] = ycol.z;
		matrix[7] = 0.0f;
	
		matrix[8] = zcol.x;
		matrix[9] = zcol.y;
		matrix[10] = zcol.z;
		matrix[11] = 0.0f;
	
		matrix[12] = pos.x;
		matrix[13] = pos.y;
		matrix[14] = pos.z;
		matrix[15] = 1.0;
	}

	void MatrixToMatrix4( const float* matrix_in, Ogre::Matrix4& matrix_out )
	{
		// from Newton to Ogre::Matrix4
		matrix_out = Ogre::Matrix4( matrix_in[0], matrix_in[4], matrix_in[8], matrix_in[12],
			matrix_in[1], matrix_in[5], matrix_in[9], matrix_in[13],
			matrix_in[2], matrix_in[6], matrix_in[10], matrix_in[14],
			matrix_in[3], matrix_in[7], matrix_in[11], matrix_in[15] );
	}

	void Matrix4ToMatrix( const Ogre::Matrix4& matrix_in, float* matrix_out )
	{
		// from Ogre to Newton.
		matrix_out[0] = matrix_in[0][0];
		matrix_out[1] = matrix_in[1][0];
		matrix_out[2] = matrix_in[2][0];
		matrix_out[3] = matrix_in[3][0];

		matrix_out[4] = matrix_in[0][1];
		matrix_out[5] = matrix_in[1][1];
		matrix_out[6] = matrix_in[2][1];
		matrix_out[7] = matrix_in[3][1];

		matrix_out[8] = matrix_in[0][2];
		matrix_out[9] = matrix_in[1][2];
		matrix_out[10] = matrix_in[2][2];
		matrix_out[11] = matrix_in[3][2];

		matrix_out[12] = matrix_in[0][3];
		matrix_out[13] = matrix_in[1][3];
		matrix_out[14] = matrix_in[2][3];
		matrix_out[15] = matrix_in[3][3];
	}


	} // end namespace "converters"

	
	namespace CollisionTools
	{
		//! find the point on a collision primitive closest to a global point.
		
		int CollisionPointDistance( const OgreNewt::World* world, const Ogre::Vector3& globalpt, 
									CollisionPtr col, const Ogre::Quaternion& colorient, const Ogre::Vector3& colpos, 
									Ogre::Vector3& retpt, Ogre::Vector3& retnormal )
		{
			float matrix[16];
			Converters::QuatPosToMatrix( colorient, colpos, matrix );

			return NewtonCollisionPointDistance( world->getNewtonWorld(), &colpos.x, col->getNewtonCollision(), matrix, &retpt.x, &retnormal.x );
		}
		


		
		int CollisionClosestPoint( const OgreNewt::World* world, CollisionPtr colA, const Ogre::Quaternion& colOrientA, const Ogre::Vector3& colPosA,
															CollisionPtr colB, const Ogre::Quaternion& colOrientB, const Ogre::Vector3& colPosB,
															Ogre::Vector3& retPosA, Ogre::Vector3& retPosB, Ogre::Vector3& retNorm )
		{
			float matrixA[16];
			float matrixB[16];

			Converters::QuatPosToMatrix( colOrientA, colPosA, matrixA );
			Converters::QuatPosToMatrix( colOrientB, colPosB, matrixB );

			return NewtonCollisionClosestPoint( world->getNewtonWorld(), colA->getNewtonCollision(), matrixA, colB->getNewtonCollision(), matrixB,
												&retPosA.x, &retPosB.x, &retNorm.x );
		}


		int CollisionCollide(  const OgreNewt::World* world, int maxSize, 
			CollisionPtr colA, const Ogre::Quaternion& colOrientA, const Ogre::Vector3& colPosA,
			CollisionPtr colB, const Ogre::Quaternion& colOrientB, const Ogre::Vector3& colPosB,
			Ogre::Vector3* retContactPts, Ogre::Vector3* retNormals, Ogre::Real* retPenetrations )
		{
			float matrixA[16];
			float matrixB[16];

			Converters::QuatPosToMatrix( colOrientA, colPosA, matrixA );
			Converters::QuatPosToMatrix( colOrientB, colPosB, matrixB );

			return NewtonCollisionCollide( world->getNewtonWorld(), maxSize, colA->getNewtonCollision(), matrixA,
				colB->getNewtonCollision(), matrixB, &retContactPts[0].x, &retNormals[0].x, retPenetrations );
		}


		int CollisionCollideContinue( const OgreNewt::World* world, int maxSize, Ogre::Real timeStep,
			CollisionPtr colA, const Ogre::Quaternion& colOrientA, const Ogre::Vector3& colPosA, const Ogre::Vector3& colVelA, const Ogre::Vector3& colOmegaA,
			CollisionPtr colB, const Ogre::Quaternion& colOrientB, const Ogre::Vector3& colPosB, const Ogre::Vector3& colVelB, const Ogre::Vector3& colOmegaB,
			Ogre::Real& retTimeOfImpact, Ogre::Vector3* retContactPts, Ogre::Vector3* retNormals, Ogre::Real* retPenetrations )
		{
			float matrixA[16];
			float matrixB[16];

			Converters::QuatPosToMatrix( colOrientA, colPosA, matrixA );
			Converters::QuatPosToMatrix( colOrientB, colPosB, matrixB );

			return NewtonCollisionCollideContinue( world->getNewtonWorld(), maxSize, timeStep,
				colA->getNewtonCollision(), matrixA, &colVelA.x, &colOmegaA.x, 
				colB->getNewtonCollision(), matrixB, &colVelB.x, &colOmegaB.x,
				&retTimeOfImpact, &retContactPts[0].x, &retNormals[0].x, retPenetrations );
		}


		Ogre::Real CollisionRayCast( CollisionPtr col, const Ogre::Vector3& startPt, const Ogre::Vector3& endPt, 
			Ogre::Vector3& retNorm, int& retColID )
		{
			return NewtonCollisionRayCast( col->getNewtonCollision(), &startPt.x, &endPt.x, &retNorm.x, &retColID );
		}

		Ogre::AxisAlignedBox CollisionCalculateAABB( CollisionPtr col, const Ogre::Quaternion& orient, const Ogre::Vector3& pos )
		{
			float matrix[16];
			Converters::QuatPosToMatrix( orient, pos, matrix );
			Ogre::Vector3 min, max;

			NewtonCollisionCalculateAABB( col->getNewtonCollision(), matrix, &min.x, &max.x );

			return Ogre::AxisAlignedBox( min, max );
		}
		





	}	// end namespace "CollisionTools"


	namespace MomentOfInertia
	{

		Ogre::Vector3 CalcEllipsoidSolid( Ogre::Real mass, const Ogre::Vector3& size )
		{
			Ogre::Vector3 inertia;

			inertia.x = 0.2f * mass * ((size.y * size.y) + (size.z* size.z));
			inertia.y = 0.2f * mass * ((size.x * size.x) + (size.z* size.z));
			inertia.z = 0.2f * mass * ((size.x * size.x) + (size.y* size.y));

			return inertia;
		}

		Ogre::Vector3 CalcSphereSolid( Ogre::Real mass, Ogre::Real radius )
		{
			Ogre::Vector3 inertia;

			inertia.x = (2.0f * mass * (radius * radius)) / 5.0;
			inertia.y = inertia.z = inertia.x;

			return inertia;
		}

		Ogre::Vector3 CalcSphereHollow(Ogre::Real mass, Ogre::Real radius )
		{
			Ogre::Vector3 inertia;

			inertia.x = (2.0f * mass * (radius * radius)) / 3.0;
			inertia.y = inertia.z = inertia.x;

			return inertia;
		}

		Ogre::Vector3 CalcBoxSolid(Ogre::Real mass, const Ogre::Vector3& size )
		{
			Ogre::Vector3 inertia;

			inertia.x = (mass * ((size.y * size.y) + (size.z * size.z))) / 12.0f;
			inertia.y = (mass * ((size.x * size.x) + (size.z * size.z))) / 12.0f;
			inertia.z = (mass * ((size.y * size.y) + (size.x * size.x))) / 12.0f;

			return inertia;
		}

		Ogre::Vector3 CalcCylinderSolid(Ogre::Real mass, Ogre::Real radius, Ogre::Real height )
		{
			Ogre::Vector3 inertia;

			inertia.x = inertia.y = (mass * ((3.0f*(radius * radius)) + (height * height))) / 12.0f;
			inertia.z = (mass * (radius * radius)) / 2.0f;

			return inertia;
		}


	} // end of namespace "MomentOfInertia"



}
