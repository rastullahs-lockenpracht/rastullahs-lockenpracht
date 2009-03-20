/*
	OgreNewt library - connecting Ogre and Newton!

	Demo05_SimpleVehicle - how to use the vehicle classes to make a simple vehicle in OgreNewt.
*/

#include "OgreNewtonApplication.h"
#include "OgreNewtonFrameListener.h"

#include <OgreNewt.h>
#include <OgreNewt_BasicFrameListener.h>


OgreNewtonApplication::OgreNewtonApplication(void)
{
	// create OgreNewt world.
	m_World = new OgreNewt::World();

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


	// floor object! this time we'll scale it slightly to make it more vehicle-friendly :P
	Ogre::Vector3 size(2.0,0.5,2.0);
	Entity* floor;
	SceneNode* floornode;
	floor = mSceneMgr->createEntity("Floor", "simple_terrain.mesh" );
	floornode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "FloorNode" );
	floornode->attachObject( floor );
	floor->setMaterialName( "Simple/BeachStones" );
	floornode->setScale(size);

	floor->setCastShadows( false );

	//Ogre::Vector3 siz(100.0, 10.0, 100.0);
	OgreNewt::Collision* col = new OgreNewt::CollisionPrimitives::TreeCollision( m_World, floornode, false );
	OgreNewt::Body* bod = new OgreNewt::Body( m_World, col );
	delete col;
	
	bod->attachNode( floornode );
	bod->setPositionOrientation( Ogre::Vector3(0.0,-2.0,0.0), Ogre::Quaternion::IDENTITY );



	// here's where we make the simple vehicle.  everything is taken care of in the constuctor.
	mCar = new SimpleVehicle( mSceneMgr, m_World, Ogre::Vector3(0,-0.5,0), Ogre::Quaternion(Ogre::Quaternion::IDENTITY) );

	

	// position camera
	msnCam = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	msnCam->attachObject( mCamera );
	mCamera->setPosition(0.0, 0.0, 0.0);
	msnCam->setPosition( 0.0, 1.0, 20.0);

	//make a light
	Ogre::Light* light;

	light = mSceneMgr->createLight( "Light1" );
	light->setType( Ogre::Light::LT_POINT );
	light->setPosition( Ogre::Vector3(0.0, 100.0, 100.0) );



}


void OgreNewtonApplication::createFrameListener()
{
	mFrameListener = new OgreNewtonFrameListener( mWindow, mCamera, mSceneMgr, m_World, msnCam, mCar );
	mRoot->addFrameListener(mFrameListener);

	mNewtonListener = new OgreNewt::BasicFrameListener( mWindow, m_World, 60 );
	mRoot->addFrameListener(mNewtonListener);
}

