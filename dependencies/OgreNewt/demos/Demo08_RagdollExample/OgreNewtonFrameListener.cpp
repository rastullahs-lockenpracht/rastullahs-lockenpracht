#include "OgreNewtonFrameListener.h"



OgreNewtonFrameListener::OgreNewtonFrameListener(RenderWindow* win, Camera* cam, SceneManager* mgr, OgreNewt::World* W) :
		ExampleFrameListener(win,cam)
{
	m_World = W;
	mSceneMgr = mgr;

    timer = 0.0f;
    OgreNewt::Debugger &debug(m_World->getDebugger());
    debug.init(mgr);

	//////////////////////////////
	// setup the ragdoll model here.
	mRagEntity = mSceneMgr->createEntity( "RAGDOLL_ENTITY", "zombie.mesh" );
	mRagNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mRagNode->attachObject( mRagEntity );
	mRagEntity->getAnimationState( "LOOP" )->setLoop( true );
	mRagEntity->getAnimationState( "LOOP" )->setEnabled( true );
	mRagNode->setPosition( Ogre::Vector3(0.0f, -4.0f, 0.0f) );

	mRagdoll = NULL;

	mK1 = mK2 = false;

}

OgreNewtonFrameListener::~OgreNewtonFrameListener(void)
{
}

bool OgreNewtonFrameListener::frameStarted(const FrameEvent &evt)
{
	Vector3 trans, strafe, vec;
	Quaternion quat;

	quat = mCamera->getOrientation();

	vec = Vector3(0.0,0.0,-3 * evt.timeSinceLastFrame);
	trans = quat * vec;

	vec = Vector3(3*evt.timeSinceLastFrame,0.0,0.0);
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


	if (mKeyboard->isKeyDown(OIS::KC_SPACE))
	{
		if (timer <= 0.0)
		{
			Ogre::Vector3 dir, vec;
			Ogre::Quaternion camorient = mCamera->getDerivedOrientation();
			vec = Ogre::Vector3(0,0,-1);

			dir = camorient * vec;

			Entity* ent;
			SceneNode* node;
			Ogre::String name;
			Ogre::Vector3 pos = mCamera->getDerivedPosition();

			name = "Body "+Ogre::StringConverter::toString( count++ );

			ent = mSceneMgr->createEntity( name, "ellipsoid.mesh" );
			node = mSceneMgr->getRootSceneNode()->createChildSceneNode( name );
			node->attachObject( ent );

			ent->setMaterialName( "Simple/dirt01" );

			OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid( m_World, Ogre::Vector3(1,1,1), 0 ));
			OgreNewt::Body* body = new OgreNewt::Body( m_World, col );

			Ogre::Vector3 inertia, offset;
            col->calculateInertialMatrix(inertia, offset);
			body->setMassMatrix( 10.0, 10.0*inertia );
            body->setCenterOfMass(offset);
			body->attachNode( node );
			body->setStandardForceCallback();
			body->setPositionOrientation( pos, camorient );
			body->setVelocity( (dir * 50.0) );

			timer = 0.2;

#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
            delete col;
#endif
		}
	}
	timer -= evt.timeSinceLastFrame;


	//////////////////////////////////////////////////////
	// RAGDOLL CODE
	if (mRagdoll)
	{
		// ragdoll exists, so pressing "R" Key will remove ragdoll, and reset the entity.
		if ((mKeyboard->isKeyDown( OIS::KC_1) && (!mK1)) || (mKeyboard->isKeyDown( OIS::KC_2 ) && (!mK2) ))
		{
			mK1 = true;
			mK2 = true;

			// remove the ragdoll.
			delete mRagdoll;
			mRagdoll = NULL;

			mRagNode->setPosition( Ogre::Vector3(0.0f, -4.0f, 0.0f) );
			mRagNode->setOrientation( Ogre::Quaternion::IDENTITY );

			mRagEntity->getAnimationState( "LOOP" )->setEnabled( true );
			mRagEntity->getAnimationState( "LOOP" )->setTimePosition( 0.0f );
		}
	}
	else
	{
		// no ragdoll, so "R" key spawns the ragdoll. otherwise, we add time to the animation.
		mRagEntity->getAnimationState( "LOOP" )->addTime( evt.timeSinceLastFrame );

		if (mKeyboard->isKeyDown( OIS::KC_1 ) && (!mK1))
		{
			mK1 = true;

			// spawn the ragdoll.  this is the version that is made up of simple primitives.
			mRagdoll = new RagDoll( "../media/models/zombie_rag_primitives.xml", m_World, mRagNode );
			mRagEntity->getAnimationState( "LOOP" )->setEnabled( false );

		}
		else if (mKeyboard->isKeyDown( OIS::KC_2 ) && (!mK2))
		{
			mK2 = true;

			// spawn the ragdoll.  this is the version that uses auto-hull generation for very accurate collision shapes.
			mRagdoll = new RagDoll( "../media/models/zombie_rag_hull.xml", m_World, mRagNode );
			mRagEntity->getAnimationState( "LOOP" )->setEnabled( false );

		}
	}

	if (!mKeyboard->isKeyDown( OIS::KC_1 )) { mK1 = false; }
	if (!mKeyboard->isKeyDown( OIS::KC_2 )) { mK2 = false; }


	if (mKeyboard->isKeyDown( OIS::KC_F2 )) 
	{ 
		mRagEntity->setVisible( false );
	} 
	else 
	{ 
		mRagEntity->setVisible( true ); 
	}

	
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



    if (mKeyboard->isKeyDown(OIS::KC_T))
        m_World->setThreadCount( m_World->getThreadCount() % 2 + 1);

	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;

	return true;
}

