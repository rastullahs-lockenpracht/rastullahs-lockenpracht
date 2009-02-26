/* 
	OgreNewt Library

	Ogre implementation of Newton Game Dynamics SDK

	OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.

		by Walaber

*/
#ifndef _INCLUDE_OGRENEWT_COLLISIONPRIMITIVES
#define _INCLUDE_OGRENEWT_COLLISIONPRIMITIVES

#include <Newton.h>
#include "OgreNewt_World.h"
#include "OgreNewt_Collision.h"
#include "OgreNewt_TreeCollisionSerializer.h"

// OgreNewt namespace.  all functions and classes use this namespace.
namespace OgreNewt
{

	//! set of basic collision shapes
	namespace CollisionPrimitives
	{

		//! null collision (results in no collision)
		class _OgreNewtExport Null : public OgreNewt::Collision
		{
		public:
			//! constructor
			Null( const World* world );

			//! destructor
			~Null() {}
		};


		//! standard primitive Box.
		class _OgreNewtExport Box : public OgreNewt::ConvexCollision
		{
		public:
			//! constructor
			/*!
				\param world pointer to OgreNewt::World
				\param size vector representing width, height, depth
				\param orient orientation offset of the primitive
				\param pos position offset of the primitive
			*/
			Box( const World* world, const Ogre::Vector3& size, 
				const Ogre::Quaternion& orient = Ogre::Quaternion::IDENTITY,
                const Ogre::Vector3& pos = Ogre::Vector3::ZERO );

			//! destructor
			~Box() {}
		};

		//! standard primitive Ellipsoid.  
		class _OgreNewtExport Ellipsoid : public OgreNewt::ConvexCollision
		{
		public:
			//! constructor
			/*!
				for a sphere, pass the same radius for all 3 axis.
				\param world pointer to OgreNewt::World
				\param size vector representing radius for all 3 axis
				\param orient orientation offset of the primitive
				\param pos position offset of the primitive
			*/
			Ellipsoid(const World* world, const Ogre::Vector3& size, 
				const Ogre::Quaternion& orient = Ogre::Quaternion::IDENTITY,
                const Ogre::Vector3& pos = Ogre::Vector3::ZERO );

			//! destructor
			~Ellipsoid() {}
		};

		//! standard primitive cylinder.
		class _OgreNewtExport Cylinder : public OgreNewt::ConvexCollision
		{
		public:
			//! constructor
			/*!
				default aligned along the local X axis (x=height).
				\param world pointer to OgreNewt::World
				\param radius radius of the cylinder (Y and Z axis)
				\param height height of the cylinder (X axis)
				\param orient orientation offset of the primitive
				\param pos position offset of the primitive
			*/
			Cylinder( const World* world, Ogre::Real radius, Ogre::Real height, 
				const Ogre::Quaternion& orient = Ogre::Quaternion::IDENTITY,
                const Ogre::Vector3& pos = Ogre::Vector3::ZERO );

			//! destructor
			~Cylinder() {}
		};

		//! standard primitive capsule.
		class _OgreNewtExport Capsule : public OgreNewt::ConvexCollision
		{
		public:
			//! constructor
			/*!
				default aligned along the local X axis (x=height).
				\param world pointer to OgreNewt::World
				\param radius radius of the capsule (Y and Z axis)
				\param height height of the capsule (X axis)
				\param orient orientation offset of the primitive
				\param pos position offset of the primitive
			*/
			Capsule( const World* world, Ogre::Real radius, Ogre::Real height, 
				const Ogre::Quaternion& orient = Ogre::Quaternion::IDENTITY,
                const Ogre::Vector3& pos = Ogre::Vector3::ZERO );

			//! destructor
			~Capsule() {}
		};

		//! standard primitive cone.
		class _OgreNewtExport Cone : public OgreNewt::ConvexCollision
		{
		public:
			//! constructor
			/*!
				default aligned along the local X axis (x=height).
				\param world pointer to OgreNewt::World
				\param radius radius of the cone (Y and Z axis)
				\param height height of the cone (X axis)
				\param orient orientation offset of the primitive
				\param pos position offset of the primitive
			*/
			Cone( const World* world, Ogre::Real radius, Ogre::Real height, 
				const Ogre::Quaternion& orient = Ogre::Quaternion::IDENTITY,
                const Ogre::Vector3& pos = Ogre::Vector3::ZERO );

			//! destructor
			~Cone() {}
		};

		//! filled-donut shape primitive.
		class _OgreNewtExport ChamferCylinder : public OgreNewt::ConvexCollision
		{
		public:
			//! constructor
			/*!
				default aligned along the local X axis (x=height).
				\param world pointer to OgreNewt::World
				\param radius radius of the chamfer cylinder (Y and Z axis)
				\param height height of the chamfer cylinder (X axis)
				\param orient orientation offset of the primitive
				\param pos position offset of the primitive
			*/
			ChamferCylinder( const World* world, Ogre::Real radius, Ogre::Real height, 
				const Ogre::Quaternion& orient = Ogre::Quaternion::IDENTITY,
                const Ogre::Vector3& pos = Ogre::Vector3::ZERO );

			//! destructor
			~ChamferCylinder() {}
		};	

		//! ConvexHull primitive
		/*!
			 "wrap" around a set cloud of vertices.
             A convex hull is the smallest possible convex shape
             that fully encloses all points supplied.
		 */
		class _OgreNewtExport ConvexHull : public OgreNewt::ConvexCollision
		{
		public:
			//! constructor
			/*!
				Overloaded constructor.  pass an Ogre::Entity*, and it will use its vertex data.
				\param world pointer to the OgreNewt::World
				\param entity pointer to an Ogre::Entity, if this is attached to an node, then
				              the node's scale is applied
                \param useTempBuffer determines whether to use the temporary blend buffer instead
                                     of the mesh's vertex data. This is useful when you want
                                     to create a collision from an intermediate animation state.
				\param orient orientation offset of the primitive
				\param pos position offset of the primitive
			*/
			ConvexHull( const World* world, Ogre::Entity* entity, bool useTempBuffer = false,
				const Ogre::Quaternion& orient = Ogre::Quaternion::IDENTITY,
                const Ogre::Vector3& pos = Ogre::Vector3::ZERO );

			/*!
				Overloaded constructor.
                pass a pointer to an array of vertices and the hull will be made from that.
				\param world pointer to the OgreNewt::World
				\param verts pointer to an array of Ogre::Vector3's that contain vertex position data
				\param vertcount number ot vetices in the array
				\param orient orientation offset of the primitive
				\param pos position offset of the primitive
			*/
			ConvexHull( const World* world, const Ogre::Vector3* verts, int vertcount,
				const Ogre::Quaternion& orient = Ogre::Quaternion::IDENTITY,
                const Ogre::Vector3& pos = Ogre::Vector3::ZERO );

			//! destructor
			~ConvexHull() {}
		};

		



		//! TreeCollision - complex polygonal collision
		/*!
			TreeCollision objects are general polygon collision objects.
            TreeCollisio objects have a requirement that their mass must = 0 
            (aka are have infinite mass)
		*/
		class _OgreNewtExport TreeCollision : public OgreNewt::Collision
		{
		public:
			//! constructor
			/*!
				Create a 'blank' tree collision object.
				Can be used for manual TreeCollision creation,
				or to be used with
			       	TreeCollisionSerializer::importTreeCollision
				\param world pointer to the OgreNewt::World
			*/
			TreeCollision( const World* world);

			//! constructor
			/*!
				Create a tree collision object.
				\param world pointer to the OgreNewt::World
				\param entity pointer to an entity, if it is
			       		      attached to a scenenode, then
					      the node's scale is applied.
				\param useTempBuffer determines whether to use the temporary blend buffer instead
                                     of the mesh's vertex data. This is useful when you want
                                     to create a collision from an intermediate animation state.
				\param optimize bool whether you want to optimize the collision or not.
			*/
			TreeCollision( const World* world, Ogre::Entity* entity, bool optimize,
                bool useTempBuffer = false);

			//! constructor
			/*!
				build a TreeCollision from vertice and index information. 
                This can be used with the dotScene scene manager
				for building TreeCollision objects from each mesh in the scene.
				\param world pointer to OgreNewt::World
				\param numVertices number of vertices passed in the array.
				\param numIndices number of indices passed in the array.
				\param vertices pointer to array of vertices (positions only).
				\param indices pointer to array of indices.
				\param optimize bool whether you want to optimize the collision or not.
			*/
			TreeCollision( const World* world, int numVertices, int numIndices,
                const float *vertices, const int *indices, bool optimize); 

			//! constructor
			/*!
				build a TreeCollision from vertice and index information.
                This can be used with the ogre Paging Landscape SceneManager,
				or other custom solutions.
				\param world pointer to OgreNewt::World
				\param numVertices number of vertices in the array.
				\param vertices pointer to array of Ogre::Vector3 vertices (positions only)
				\param indexData pointer to Ogre::IndexData for the mesh
				\param optimize bool whether you want to optimize the collision or not.
			*/
			TreeCollision( const World* world, int numVertices, Ogre::Vector3* vertices,
                Ogre::IndexData* indexData, bool optimize);

			//! destructor
			virtual ~TreeCollision() {}

			//! start a tree collision creation
			void start();

			//! add a poly to the tree collision
			/*!
				Add a single poly to the tree collision.
				\param polys pointer to an array of 3 Vector3D objects representing the global position of each poly.
				\param ID and identifier to assign to this poly, that can be retrieved later upon collision detection.
			*/
			void addPoly( Ogre::Vector3* polys, unsigned int ID );

			//! finish the tree collision
			void finish( bool optimize = true );


			friend void TreeCollisionSerializer::exportTreeCollision(const TreeCollision* collision, const Ogre::String& filename);
			friend void TreeCollisionSerializer::importTreeCollision(Ogre::DataStreamPtr& stream, TreeCollision* pDest);
		};

		////////////////////////////////////////////////////////
		//! TreeCollision created by parsing a tree of SceneNodes, adding collision data of all meshes.
		/*!
			Users can inherit this class, and inherit the "getID" function to perform their own filtering on the
			IDs to pass to Newton.  IDs are useful during collision callbacks to determine which part of the world
			is being hit.

			By default, the ID is set to an incrementing integer.
		*/
		class _OgreNewtExport TreeCollisionSceneParser : public TreeCollision
		{
		public:
			TreeCollisionSceneParser( OgreNewt::World* world );

			~TreeCollisionSceneParser() {}

			//! parse the scene.
			void parseScene( Ogre::SceneNode* startNode, bool optimize = false );

		protected:

			//! this is a user-inherited function that lets you filter which Entities will be added to the treeCollision.
			/*!
				return true to add this entity, return false to ignore it.
			*/
			virtual bool entityFilter( const Ogre::SceneNode* currentNode, const Ogre::Entity* currentEntity ) { return true; }

			//! user inherit-able function, allows customization of the ID to be assigned to this group of polygons.
			virtual unsigned int getID( const Ogre::SceneNode* currentNode, const Ogre::Entity* currentEntity, unsigned int currentSubMesh ) { return count++; }

		private:
			//! recursive function to parse a single scene node.
			void _parseNode( Ogre::SceneNode* node, const Ogre::Quaternion& curOrient, const Ogre::Vector3& curPos );


			static int count;
		};

		////////////////////////////////////////////////////////
		//	COMPOUND COLLISION!

		//! create a compound from several collision pieces.
		class _OgreNewtExport CompoundCollision : public OgreNewt::Collision
		{
		public:
			//! constructor
			/*!
				creates a compound collision object made from an array of simple primitive parts.  can be used to make very complex
				collision shapes.
				\param world pointer to the OgreNewt::World
				\param col_array std::vector of pointers to existing collision objects.
			*/
			CompoundCollision( const World* world, std::vector<CollisionPtr> col_array );

			//! destructor
			~CompoundCollision() {}
		};


		////////////////////////////////////////////////////////
		// supplemental primitives built from convex hulls
		////////////////////////////////////////////////////////

		//! Pyramid primitive
		/*!
			4-sided base, comes to a single point. base is aligned on XZ plane. made from Convex Hull internally.  supplied as part of the
			OgreNewt library, but not a built-in function of Newton istelf.
		*/
		class _OgreNewtExport Pyramid : public OgreNewt::ConvexCollision
		{
		public:
			//! constructor
			/*!
				\param world pointer to the OgreNewt::World
				\param size Ogre::Vector3 size.
				\param orient orientation offset of the primitive
				\param pos position offset of the primitive
			*/
			Pyramid( const World* world, const Ogre::Vector3& size,
				const Ogre::Quaternion& orient = Ogre::Quaternion::IDENTITY, const Ogre::Vector3& pos = Ogre::Vector3::ZERO );

			//! destructor
			~Pyramid() {}
		};

		//! Wraps a collision in order to make it scaleable.
		/*!
			It lets the user to modify the hull via a world space scale
			matrix without changing the initial collision
		*/
		class _OgreNewtExport HullModifier: public OgreNewt::Collision
		{
		public:
			//! constructor
			HullModifier( World* world, CollisionPtr wrappedCollision );
			//! destructor
			~HullModifier() {};

			//! Sets the scale matrix in world space (of Ogre)
			void setMatrix(const Ogre::Matrix4& matrix);

			//! scale matrix in world space (of Ogre)
			Ogre::Matrix4 getMatrix() const;
		};	

	}	// end namespace CollisionPrimitives

}// end namespace OgreNewt

#endif	// _INCLUDE_OGRENEWT_COLLISIONPRIMITIVES

