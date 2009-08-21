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

	//Ogre::Vector3 siz(100.0, 10.0, 100.0);
	OgreNewt::CollisionPtr col = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::TreeCollision( m_World, floor, true, 0 ));
	OgreNewt::Body* bod = new OgreNewt::Body( m_World, col );
#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
	delete col;
#endif
	
	//floornode->setScale( siz );
	bod->attachNode( floornode );
	bod->setPositionOrientation( Ogre::Vector3(0.0,-4.0,0.0), Ogre::Quaternion::IDENTITY );

	

	

	// position camera
	mCamera->setPosition(0.0, -2.0, 10.0);
	mCamera->setNearClipDistance( 0.01f );

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

