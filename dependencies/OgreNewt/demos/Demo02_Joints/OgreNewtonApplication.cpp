/*
	OgreNewt library - connecting Ogre and Newton!

	Demo02_Joints - basic demo that shows how to connect rigid bodies via joints.
*/

#include "OgreNewtonApplication.h"
#include "OgreNewtonFrameListener.h"

#include <OgreNewt.h>
#include <OgreNewt_BasicFrameListener.h>



OgreNewtonApplication::OgreNewtonApplication(void)
{
	// create OgreNewt world.
	m_World = new OgreNewt::World();

	mEntityCount = 0;

}

OgreNewtonApplication::~OgreNewtonApplication(void)
{
	// destroy world.
	delete m_World;
}



void OgreNewtonApplication::createScene()
{
	// sky box.
	mSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox");
	
	// shadows on!
	mSceneMgr->setShadowTechnique( Ogre::SHADOWTYPE_STENCIL_ADDITIVE );


	// floor object!
	Entity* floor;
	SceneNode* floornode;
	floor = mSceneMgr->createEntity("Floor", "simple_terrain.mesh" );
	floornode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "FloorNode" );
	floornode->attachObject( floor );
	floor->setMaterialName( "Simple/BeachStones" );

	floor->setCastShadows( false );

	//-------------------------------------------------------------
	// add some other objects.
	Entity* floor2;
	SceneNode* floornode2;
	floor2 = mSceneMgr->createEntity("Floor2", "simple_terrain.mesh" );
	floornode2 = floornode->createChildSceneNode( "FloorNode2" );
	floornode2->attachObject( floor2 );
	floor2->setMaterialName( "Simple/BeachStones" );
	floor2->setCastShadows( false );
	floornode2->setPosition( Ogre::Vector3(80.0f, 0.0f, 0.0f) );

	Entity* floor3;
	SceneNode* floornode3;
	floor3 = mSceneMgr->createEntity("Floor3", "simple_terrain.mesh" );
	floornode3 = floornode->createChildSceneNode( "FloorNode3" );
	floornode3->attachObject( floor3 );
	floor3->setMaterialName( "Simple/BeachStones" );
	floor3->setCastShadows( false );
	floornode3->setPosition( Ogre::Vector3(-80.0f, -5.0f, 0.0f) );
	floornode3->setOrientation( Ogre::Quaternion( Ogre::Degree(15.0f), Ogre::Vector3::UNIT_Z ) );
	//-------------------------------------------------------------

	// using the new "SceneParser" TreeCollision primitive.  this will automatically parse an entire tree of
	// SceneNodes (parsing all children), and add collision for all meshes in the tree.
	OgreNewt::CollisionPrimitives::TreeCollisionSceneParser* stat_col = new OgreNewt::CollisionPrimitives::TreeCollisionSceneParser( m_World );
	stat_col->parseScene( floornode, true, 0 );
	OgreNewt::Body* bod = new OgreNewt::Body( m_World, OgreNewt::CollisionPtr(stat_col) );
#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
	delete stat_col;
#endif
	
	bod->attachNode( floornode );
	bod->setPositionOrientation( Ogre::Vector3(0.0,-20.0,0.0), Ogre::Quaternion::IDENTITY );



	// make a simple rope.
	Ogre::Vector3 size(3,1.5,1.5);
	Ogre::Vector3 pos(0,3,0);
	Ogre::Quaternion orient = Ogre::Quaternion::IDENTITY;

	// loop through, making bodies and connecting them.
	OgreNewt::Body* parent = NULL;
	OgreNewt::Body* child = NULL;

	for (int x=0;x<5;x++)
	{
		// make the next box.
		child = makeSimpleBox(size, pos, orient);

		// now make a new joint connecting this to the last box.
		OgreNewt::Joint* joint;

		// make the joint right between the bodies...
		

		if (parent)
		{
			joint = new OgreNewt::BasicJoints::BallAndSocket( m_World, child, parent, pos-Ogre::Vector3(size.x/2,0,0) );
		}
		else
		{
			// no parent, this is the first joint, so just pass NULL as the parent, to stick it to the "world"
			joint = new OgreNewt::BasicJoints::BallAndSocket( m_World, child, NULL, pos-Ogre::Vector3(size.x/2,0,0) );
		}

		// offset pos a little more.
		pos += Ogre::Vector3(size.x,0,0);

		// save the last body for the next loop!
		parent = child;
	}

	
	

	

	// position camera
	mCamera->setPosition(0.0, -3.0, 0.0);

	//make a light
	Ogre::Light* light;

	light = mSceneMgr->createLight( "Light1" );
	light->setType( Ogre::Light::LT_POINT );
	light->setPosition( Ogre::Vector3(0.0, 100.0, 100.0) );



}


void OgreNewtonApplication::createFrameListener()
{
	mFrameListener = new OgreNewtonFrameListener( mWindow, mCamera, mSceneMgr, m_World);
	mRoot->addFrameListener(mFrameListener);

	mNewtonListener = new OgreNewt::BasicFrameListener( mWindow, m_World, 60 );
	mRoot->addFrameListener(mNewtonListener);
}


OgreNewt::Body* OgreNewtonApplication::makeSimpleBox( Ogre::Vector3& size, Ogre::Vector3& pos, Ogre::Quaternion& orient )
{
	Entity* box1;
	SceneNode* box1node;

	box1 = mSceneMgr->createEntity( "Entity"+Ogre::StringConverter::toString(mEntityCount++), "box.mesh" );
	box1node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	box1node->attachObject( box1 );
	box1node->setScale( size );
//	box1->setNormaliseNormals(true);

	OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box( m_World, size, 0 ));
	OgreNewt::Body* bod = new OgreNewt::Body( m_World, col );


	// base mass on the size of the object.
	Ogre::Real mass = size.x * size.y * size.z * 2.5;
		
	// calculate the inertia based on box formula and mass
	Ogre::Vector3 inertia, offset;
    col->calculateInertialMatrix(inertia, offset);

#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
	delete col;
#endif
				
	bod->attachNode( box1node );
	bod->setMassMatrix( mass, mass*inertia );
    bod->setCenterOfMass(offset);
	bod->setStandardForceCallback();

	box1->setMaterialName( "Simple/BumpyMetal" );


	bod->setPositionOrientation( pos, orient );

	return bod;
}
