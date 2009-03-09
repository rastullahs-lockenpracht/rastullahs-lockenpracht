/*
	OgreNewt library - connecting Ogre and Newton!

	Demo04_Raycasting - how raycasting in Newton works, implemented with a simple way to drag objects around.
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

	// setup CEGUI
	mGUIRenderer = new CEGUI::OgreCEGUIRenderer( mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr );
	new CEGUI::System( mGUIRenderer );

	// load up CEGUI stuff...
	try
	{
		using namespace CEGUI;
		CEGUI::Logger::getSingleton().setLoggingLevel( CEGUI::Informative );

		CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
        CEGUI::System::getSingleton().setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
        CEGUI::System::getSingleton().setDefaultFont((CEGUI::utf8*)"BlueHighway-10");

		CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow( (CEGUI::utf8*)"DefaultWindow", (CEGUI::utf8*)"root_wnd" );
		CEGUI::System::getSingleton().setGUISheet( sheet );

		//makeGUI();
		//setupGUI();
	
	}
	catch (CEGUI::Exception)
	{}


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
	OgreNewt::Collision* col = new OgreNewt::CollisionPrimitives::TreeCollision( m_World, floor, true );
	OgreNewt::Body* bod = new OgreNewt::Body( m_World, col );
	delete col;
	
	//floornode->setScale( siz );
	bod->attachNode( floornode );
	bod->setPositionOrientation( Ogre::Vector3(0.0,-10.0,0.0), Ogre::Quaternion::IDENTITY );


	// make a simple rope.
	Ogre::Vector3 size(3,1.0,1.0);
	Ogre::Vector3 pos(0,1,0);
	Ogre::Quaternion orient = Ogre::Quaternion::IDENTITY;

	// loop through, making bodies and connecting them.
	OgreNewt::Body* parent = NULL;
	OgreNewt::Body* child = NULL;

	for (int x=0;x<8;x++)
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

	for (int i=0; i<15;i++)
	{
		pos = Ogre::Vector3( 10-rand()%20, 4+rand()%2, 10-rand()%20 );
		size = Ogre::Vector3( 1+rand()%3, 1+rand()%3, 1+rand()%3 );

		OgreNewt::Body* bod = makeSimpleBox( size, pos, orient );
	}

	
	// position camera
	mCamera->setPosition(0.0, -3.0, 23.0);

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

	mNewtonListener = new OgreNewt::BasicFrameListener( mWindow, mSceneMgr, m_World, 60 );
	mRoot->addFrameListener(mNewtonListener);
}

void OgreNewtonApplication::destroyScene()
{
	CEGUI::System* sys = CEGUI::System::getSingletonPtr();
	delete sys;

	// CEGUI Cleanup
	delete mGUIRenderer;
}


OgreNewt::Body* OgreNewtonApplication::makeSimpleBox( Ogre::Vector3& size, Ogre::Vector3& pos, Ogre::Quaternion& orient )
{
	Entity* box1;
	SceneNode* box1node;

	box1 = mSceneMgr->createEntity( "Entity"+Ogre::StringConverter::toString(mEntityCount++), "box.mesh" );
	box1node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	box1node->attachObject( box1 );
	box1node->setScale( size );

	OgreNewt::ConvexCollision* col = new OgreNewt::CollisionPrimitives::Box( m_World, size );
	OgreNewt::Body* bod = new OgreNewt::Body( m_World, col );


	// base mass on the size of the object.
	Ogre::Real mass = size.x * size.y * size.z * 2.5;
	// calculate the inertia based on box formula and mass
	Ogre::Vector3 inertia, offset;
    col->calculateInertialMatrix(inertia, offset);

	delete col;
				
	bod->attachNode( box1node );
	bod->setMassMatrix( mass, mass*inertia );
	bod->setStandardForceCallback();

	box1->setMaterialName( "Simple/BumpyMetal" );


	bod->setPositionOrientation( pos, orient );

	return bod;
}

