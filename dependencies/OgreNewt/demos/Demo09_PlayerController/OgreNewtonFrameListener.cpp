#include "OgreNewtonFrameListener.h"


//#include <OgreNoMemoryMacros.h>
#include <CEGUI/CEGUI.h>
//#include <OgreMemoryMacros.h>


OgreNewtonFrameListener::OgreNewtonFrameListener(RenderWindow* win, Camera* cam, SceneManager* mgr, OgreNewt::World* W, OgreNewt::PlayerController *player) :
		ExampleFrameListener(win,cam)
{
	m_World = W;
	mSceneMgr = mgr;
    mPlayer = player;

    OgreNewt::Debugger &debug(m_World->getDebugger());
    debug.init(mgr);
}

OgreNewtonFrameListener::~OgreNewtonFrameListener(void)
{
}

bool OgreNewtonFrameListener::frameStarted(const FrameEvent &evt)
{
	mKeyboard->capture();
	mMouse->capture();



    // player movement uhjk zi
    Ogre::Real forwardSpeed, sideSpeed;
    Ogre::Radian heading;
    mPlayer->getVelocity(forwardSpeed, sideSpeed, heading);
    forwardSpeed = 0;
    sideSpeed = 0;
    if(mKeyboard->isKeyDown(OIS::KC_U))
        forwardSpeed += 6.0;
    if(mKeyboard->isKeyDown(OIS::KC_J))
        forwardSpeed -= 5.0;
    if(mKeyboard->isKeyDown(OIS::KC_H))
        sideSpeed -= 5.0;
    if(mKeyboard->isKeyDown(OIS::KC_K))
        sideSpeed += 5.0;
    if(mKeyboard->isKeyDown(OIS::KC_Z))
        heading += Degree(90)/60.0;
    if(mKeyboard->isKeyDown(OIS::KC_I))
        heading -= Degree(90)/60.0;

    mPlayer->setVelocity(forwardSpeed, sideSpeed, heading);


    OgreNewt::Debugger& debug(m_World->getDebugger());
    if (mKeyboard->isKeyDown(OIS::KC_F3))
    {
        debug.startRaycastRecording();
        debug.clearRaycastsRecorded();
        debug.showDebugInformation();
    }
    else
    {
        debug.clearRaycastsRecorded();
        debug.stopRaycastRecording();
        debug.hideDebugInformation();
    }



    if (mKeyboard->isKeyDown(OIS::KC_T))
        m_World->setThreadCount( m_World->getThreadCount() % 2 + 1);


	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;

	return true;
}

