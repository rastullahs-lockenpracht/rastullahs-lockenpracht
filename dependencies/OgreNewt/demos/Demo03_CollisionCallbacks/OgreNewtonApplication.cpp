/*
	OgreNewt library - connecting Ogre and Newton!

	Demo03_CollisionCallbacks - the material system in Newton is extremely powerful.  by assigning
	materials to bodies, you can define all kinds of specific behavior when objects collide.  you can use
	material callbacks to create hot spots, or make sound effects, or spawn particles, etc.  this example
	uses a conveyor belt as an example.  look at the conveyorMatCallback to see how it's implemented.
*/
#include "OgreNewtonApplication.h"
#include "OgreNewtonFrameListener.h"

#include <OgreNewt.h>
#include <OgreNewt_BasicFrameListener.h>



OgreNewtonApplication::OgreNewtonApplication(void)
{
	m_World = new OgreNewt::World();

}

OgreNewtonApplication::~OgreNewtonApplication(void)
{
	std::vector<ConveyorBelt*>::iterator it;
	for (it=mBelts.begin(); it!=mBelts.end(); it++)
	{
		ConveyorBelt* belt = (*it);
		delete belt;
	}

	delete m_World;
}



void OgreNewtonApplication::createScene()
{

	// sky box.
	mSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox");

	// shadows on.
	mSceneMgr->setShadowTechnique( Ogre::SHADOWTYPE_STENCIL_MODULATIVE );

	createMaterials();

	// lets make a few conveyor belt objects.
	ConveyorBelt* belt = new ConveyorBelt();
    Ogre::String name("ConveyorBelt1");
    Ogre::Vector3 size(15,0.3,3);
    Ogre::Vector3 dir(1,0,0);
    Ogre::Real speed(2.0);
    Ogre::Vector3 pos(0,2,0);
    Ogre::Quaternion orient(Ogre::Quaternion::IDENTITY);
	belt->init( name, mSceneMgr, m_World, size, dir, speed, pos, orient, mMatConveyor, BT_CONVEYOR );
	mBelts.push_back( belt );


	belt = new ConveyorBelt();
    name = Ogre::String("ConveyorBelt2");
    size = Ogre::Vector3(23,0.3,4);
    dir = Ogre::Vector3(-1,0,0);
    speed = Ogre::Real(6.0);
    pos = Ogre::Vector3(3,-1,0);
    orient = Ogre::Quaternion(Ogre::Quaternion::IDENTITY);
	belt->init( name, mSceneMgr, m_World, size, dir, speed, pos, orient, mMatConveyor, BT_CONVEYOR );
	mBelts.push_back( belt );


	// floor object!
	Entity* floor;
	SceneNode* floornode;
	floor = mSceneMgr->createEntity("Floor", "simple_terrain.mesh" );
	floornode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "FloorNode" );
	floornode->attachObject( floor );
	floor->setMaterialName( "Simple/BeachStones" );

	floor->setCastShadows( false );

	//Ogre::Vector3 siz(100.0, 10.0, 100.0);
	OgreNewt::Collision* col = new OgreNewt::CollisionPrimitives::TreeCollision( m_World, floor, true );
	OgreNewt::Body* bod = new OgreNewt::Body( m_World, col );
	
	//floornode->setScale( siz );
	bod->attachNode( floornode );
	bod->setPositionOrientation( Ogre::Vector3(0.0,-10.0,0.0), Ogre::Quaternion::IDENTITY );
	

	// position camera
	mCamera->setPosition(0.0, 2.0, 0.0);

	//make a light
	Ogre::Light* light;

	light = mSceneMgr->createLight( "Light1" );
	light->setType( Ogre::Light::LT_POINT );
	light->setPosition( Ogre::Vector3(0.0, 100.0, 100.0) );



}

void OgreNewtonApplication::createMaterials()
{
	mMatDefault = m_World->getDefaultMaterialID();
	mMatConveyor = new OgreNewt::MaterialID( m_World );

	mMatPairDefaultConveyor = new OgreNewt::MaterialPair( m_World, mMatDefault, mMatConveyor );
	mConveyorCallback = new ConveyorMatCallback( BT_CONVEYOR );
	mMatPairDefaultConveyor->setContactCallback( mConveyorCallback );
	mMatPairDefaultConveyor->setDefaultFriction( 1.5, 1.4 );

}


void OgreNewtonApplication::createFrameListener()
{
	mFrameListener = new OgreNewtonFrameListener( mWindow, mCamera, mSceneMgr, m_World);
	mRoot->addFrameListener(mFrameListener);

	mNewtonListener = new OgreNewt::BasicFrameListener( mWindow, m_World, 60 );
	mRoot->addFrameListener(mNewtonListener);
}

