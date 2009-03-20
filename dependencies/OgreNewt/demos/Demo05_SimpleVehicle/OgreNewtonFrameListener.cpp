#include "OgreNewtonFrameListener.h"


OgreNewtonFrameListener::OgreNewtonFrameListener(RenderWindow* win, Camera* cam, SceneManager* mgr, OgreNewt::World* W, SimpleVehicle* car) :
		ExampleFrameListener(win,cam)
{
	m_World = W;
	mSceneMgr = mgr;

	mCar = car;

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

			OgreNewt::Collision* col = new OgreNewt::CollisionPrimitives::Ellipsoid( m_World, Ogre::Vector3(1,1,1) );
			OgreNewt::Body* body = new OgreNewt::Body( m_World, col );

			Ogre::Vector3 inertia = OgreNewt::MomentOfInertia::CalcSphereSolid( 10.0, 1.0 );
			body->setMassMatrix( 10.0, inertia );
			body->attachToNode( node );
			body->setStandardForceCallback();
			body->setPositionOrientation( pos, camorient );
			body->setVelocity( (dir * 50.0) );

			timer = 0.2;
		}
	}

	timer -= evt.timeSinceLastFrame;

	// ---------------------------------------------------------
	// -- VEHICLE CONTORLS
	// ---------------------------------------------------------
	Ogre::Real torque = 0.0;
	Ogre::Degree steering(0.0);

	if (mKeyboard->isKeyDown(OIS::KC_W))
		torque += 600.0;

	if (mKeyboard->isKeyDown(OIS::KC_S))
		torque -= 600.0;

	if (mKeyboard->isKeyDown(OIS::KC_A))
		steering += Ogre::Degree(30);

	if (mKeyboard->isKeyDown(OIS::KC_D))
		steering -= Ogre::Degree(30);

	//update the vehicle!
	mCar->setTorqueSteering( torque, steering );

	if ((mKeyboard->isKeyDown(OIS::KC_R)) && (!mR))
	{
		mR = true;
		// rebuild the vehicle
		if (mCar)
		{
			delete mCar;
			mCar = new SimpleVehicle( mSceneMgr, m_World, Ogre::Vector3(0,Ogre::Math::UnitRandom() * 10.0,0), Ogre::Quaternion(Ogre::Quaternion::IDENTITY) );
		}
	}
	if (!mKeyboard->isKeyDown( OIS::KC_R )) { mR = false; }


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
	{
		if (mCar)
			delete mCar;

		return false;
	}

	return true;
}

