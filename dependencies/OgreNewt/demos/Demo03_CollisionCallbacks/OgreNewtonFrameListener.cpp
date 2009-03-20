#include "OgreNewtonFrameListener.h"



OgreNewtonFrameListener::OgreNewtonFrameListener(RenderWindow* win, Camera* cam, SceneManager* mgr, OgreNewt::World* W) :
		ExampleFrameListener(win,cam)
{
	m_World = W;
	mSceneMgr = mgr;

    timer = 0.0f;
    OgreNewt::Debugger &debug(m_World->getDebugger());
    debug.init(mgr);
}

OgreNewtonFrameListener::~OgreNewtonFrameListener(void)
{
}

bool OgreNewtonFrameListener::frameStarted(const FrameEvent &evt)
{
	Vector3 trans, strafe, vec;
	Quaternion quat;

	quat = mCamera->getOrientation();

	vec = Vector3(0.0,0.0,-0.5);
	trans = quat * vec;

	vec = Vector3(0.5,0.0,0.0);
	strafe = quat * vec;

	mKeyboard->capture();
	mMouse->capture();

	mCamera->pitch( Degree(mMouse->getMouseState().Y.rel * -0.5) );
    mCamera->setFixedYawAxis(true);
	mCamera->yaw( Degree(mMouse->getMouseState().X.rel * -0.5) );

	if (mKeyboard->isKeyDown(OIS::KC_UP))
		mCamera->moveRelative(trans);

	if (mKeyboard->isKeyDown(OIS::KC_DOWN))
		mCamera->moveRelative(trans * -1.0);

	if (mKeyboard->isKeyDown(OIS::KC_LEFT))
		mCamera->moveRelative(strafe * -1.0);

	if (mKeyboard->isKeyDown(OIS::KC_RIGHT))
		mCamera->moveRelative(strafe);


	if (timer <= 0.0)
	{
		Ogre::Vector3 size( 0.2 + rand() % 2, 0.2 + rand() % 2, 0.2 + rand() % 2 );
		Ogre::Real mass = size.length();
		Entity* ent;
		SceneNode* node;
		Ogre::String name;
		Ogre::Vector3 pos = mCamera->getDerivedPosition();

		name = "Body "+Ogre::StringConverter::toString( count++ );

		ent = mSceneMgr->createEntity( name, "box.mesh" );
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode( name );
		node->attachObject( ent );
		node->setScale( size );

		ent->setMaterialName( "Simple/BumpyMetal" );

		OgreNewt::ConvexCollision* col = new OgreNewt::CollisionPrimitives::Box( m_World, size );
		OgreNewt::Body* body = new OgreNewt::Body( m_World, col );

        Ogre::Vector3 inertia, offset;
        col->calculateInertialMatrix(inertia, offset);
		delete col;

		body->setMassMatrix( mass, mass*inertia );
		body->attachNode( node );
		body->setStandardForceCallback();
		body->setPositionOrientation( Ogre::Vector3(-5,8,0), Ogre::Quaternion::IDENTITY );

		timer = 1.5;
	}
	

	timer -= evt.timeSinceLastFrame;

    OgreNewt::Debugger& debug(m_World->getDebugger());
    if (mKeyboard->isKeyDown(OIS::KC_F3))
    {
        debug.showDebugInformation();
        debug.startRaycastRecording();
        debug.clearRaycastsRecorded();
    }
    else
    {
        debug.hideDebugInformation();
        debug.clearRaycastsRecorded();
        debug.stopRaycastRecording();
    }



	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;

	return true;
}
