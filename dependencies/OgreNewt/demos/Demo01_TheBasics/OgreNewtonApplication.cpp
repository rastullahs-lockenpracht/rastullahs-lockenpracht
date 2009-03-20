/*
	OgreNewt library - connecting Ogre and Newton!

	Demo01_TheBasics - basic demo that shows a simple OgreNewt world, and how
	to setup basic rigid bodies.
*/

#include "OgreNewtonApplication.h"
#include "OgreNewtonFrameListener.h"

#include <OgreNewt.h>
#include <OgreNewt_BasicFrameListener.h>

using namespace Ogre;


OgreNewtonApplication::OgreNewtonApplication(void)
{
	// create the OgreNewt world.  this must be done before any other commands can be called.
	m_World = new OgreNewt::World();

}

OgreNewtonApplication::~OgreNewtonApplication(void)
{
	// delete the world when we're done.
	delete m_World;
}



void OgreNewtonApplication::createScene()
{
	// sky box.
	mSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox");


	// this will be a static object that we can throw objects at.  we'll use a simple cylinder primitive.
	// first I load the visual mesh that represents it.  I have some simple primitive shaped .mesh files in
	// the "primitives" directory to make this simple... all of them have a basic size of "1" so that they
	// can easily be scaled to fit any size primitive.
    Entity* floor;
	SceneNode* floornode;
	floor = mSceneMgr->createEntity("Floor", "cylinder.mesh" );
	//floor->setNormaliseNormals(true);
	floornode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "FloorNode" );
	floornode->attachObject( floor );
	floor->setMaterialName( "Simple/BeachStones" );
	floor->setCastShadows( false );
	

	// okay, the basic mesh is loaded.  now let's decide the size of the object, and scale the node.
	Ogre::Vector3 siz(5,2.5,2.5);
	floornode->setScale( siz );

	// here's where we make a collision shape for the physics.  note that we use the same size as
	// above.
	OgreNewt::Collision* col = new OgreNewt::CollisionPrimitives::Cylinder(m_World, 2.5, 5);

	// now we make a new rigid body based on this collision shape.
	OgreNewt::Body* bod = new OgreNewt::Body( m_World, col );

	// we`re done with the collision shape, we can delete it now.
	delete col;

	// now we "attach" the rigid body to the scene node that holds the visual object, and set it's
	// original position and orientation.  all rigid bodies default to mass=0 (static, immobile), so
	// that's all we'll need to do for this object.  dynamic objects have a few more steps, so look
	// at the code in the FrameListener for more.
	bod->attachNode( floornode );
	bod->setPositionOrientation( Ogre::Vector3(0.0,-10.0,-20.0), Ogre::Quaternion::IDENTITY );


	// position camera
	mCamera->setPosition(0.0, -10.0, 0.0);


	//make a light
	Ogre::Light* light;
	light = mSceneMgr->createLight( "Light1" );
	light->setType( Ogre::Light::LT_POINT );
	light->setPosition( Ogre::Vector3(0.0, 100.0, 100.0) );


}


void OgreNewtonApplication::createFrameListener()
{
	// this is our custom frame listener for this app, that lets us shoot cylinders with the space bar, move
	// the camera, etc.
	mFrameListener = new OgreNewtonFrameListener( mWindow, mCamera, mSceneMgr, m_World);
	mRoot->addFrameListener(mFrameListener);

	// this is a basic frame listener included with OgreNewt that does nothing but update the
	// physics at a set framerate for you.  complex project will want more control, but this
	// works for simple demos like this.  feel free to look at the source to see how it works.
	mNewtonListener = new OgreNewt::BasicFrameListener( mWindow, m_World, 60 );
	mRoot->addFrameListener(mNewtonListener);
}
