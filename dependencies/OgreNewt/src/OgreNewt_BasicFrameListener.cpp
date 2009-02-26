#include "OgreNewt_BasicFrameListener.h"
#include "OgreNewt_Debugger.h"
#include "OgreNewt_World.h"

namespace OgreNewt
{

	BasicFrameListener::BasicFrameListener( Ogre::RenderWindow* win, Ogre::SceneManager* mgr, OgreNewt::World* W, int update_framerate) :
		FrameListener()
{
	m_World = W;
	desired_framerate = update_framerate;

	m_update = (Ogre::Real)(1.0f / (Ogre::Real)desired_framerate);
	m_elapsed = 0.0f;

	// add the standard debug viewer.
	W->getDebugger().init( mgr );

	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	win->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	//Create all devices (We only catch joystick exceptions here, as, most people have Key/Mouse)
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, false ));

}

BasicFrameListener::~BasicFrameListener(void)
{
}

bool BasicFrameListener::frameStarted(const Ogre::FrameEvent &evt)
{
	m_elapsed += evt.timeSinceLastFrame;

#ifdef _DEBUG
	Ogre::LogManager::getSingleton().logMessage("   Newton Frame Listener... m_elapsed: "+Ogre::StringConverter::toString(m_elapsed)+
		"  m_update:"+Ogre::StringConverter::toString(m_update));
#endif

	int count = 0;

	// loop through and update as many times as necessary (up to 10 times maximum).
	if ((m_elapsed > m_update) && (m_elapsed < (m_update * 10)) )
	{
		while (m_elapsed > m_update)
		{
			m_World->update( m_update );
			m_elapsed -= m_update;
			count++;
		}
	}
	else
	{
		if (m_elapsed < (m_update))
		{
			// not enough time has passed this loop, so ignore for now.
		}
		else
		{
			// too much time has passed (would require more than 10 updates!), so just update once and reset.
			// this often happens on the first frame of a game, where assets and other things were loading, then
			// the elapsed time since the last drawn frame is very long.
			m_World->update( m_elapsed );
			count++;
			m_elapsed = 0.0f; // reset the elapsed time so we don't become "eternally behind".
		}
	}

#ifdef _DEBUG
	Ogre::LogManager::getSingleton().logMessage("   Newton updates this loop: "+Ogre::StringConverter::toString(count));
#endif

	/////////////////////////////////////////////////////////////
	//		DEBUGGER
	mKeyboard->capture();

	if (mKeyboard->isKeyDown(OIS::KC_F3))
	{
		m_world->getDebugger().showDebugInformation( m_World );
	}
	else
	{
		m_world->getDebugger().hideDebugInformation();
	}

	
	return true;
}


}	// end NAMESPACE OgreNewt
