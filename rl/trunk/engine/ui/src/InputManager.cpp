#include <OgreKeyEvent.h>
#include <OgreRoot.h>

#include <CEGUI.h>

#include "InputManager.h"
#if OGRE_PLATFORM != PLATFORM_WIN32
#include "SDL/SDL.h"
#endif

#include "CeConsole.h"
#include "DebugWindow.h"
#include "GameLoop.h"

#include "Actor.h"
#include "ActorManager.h"

template<> rl::InputManager* Singleton<rl::InputManager>::ms_Singleton = 0;
using namespace Ogre;
using CEGUI::System;

namespace rl {

    InputManager& InputManager::getSingleton(void)
	{
		return Singleton<InputManager>::getSingleton();
	}

	InputManager* InputManager::getSingletonPtr(void)
	{
		return Singleton<InputManager>::getSingletonPtr();
	}

	InputManager::InputManager(void):mEventInitialized(false)
	{
		switchMouseToUnbuffered();
		mEventProcessor = new EventProcessor();
		GameLoop::getSingleton().addSynchronizedTask(this);
		for(int i=0; i<NUM_KEYS; i++)
			mKeyDown[i] = false;

		mScreenX = Root::getSingleton().getAutoCreatedWindow()->getWidth();
		mScreenY = Root::getSingleton().getAutoCreatedWindow()->getHeight();
		
#if OGRE_PLATFORM != PLATFORM_WIN32 
        SDL_WM_GrabInput(SDL_GRAB_OFF);
#endif
	}

	InputManager::~InputManager()
	{
		mEventQueue.activateEventQueue(false);
		GameLoop::getSingleton().removeSynchronizedTask(this);
//		Root::getSingleton().removeFrameListener(this);

		mInputReader->useBufferedInput(NULL, false, false);
		mInputReader->setBufferedInput(false, false);
		delete mEventProcessor;
	}


	void InputManager::addKeyListener(KeyListener *l)
	{
		mKeyListeners.insert(l);
	}

	void InputManager::removeKeyListener(KeyListener *l)
	{
		mKeyListeners.erase(l);
	}

	void InputManager::run(Real elapsedTime)
	{
		if (!isCeguiActive())
		{
			mInputReader->capture();
			while (mEventQueue.getSize() > 0)
			{
				InputEvent* ie = mEventQueue.pop();
				if(ie->getID() == KeyEvent::KE_KEY_PRESSED)
					keyPressed(static_cast<KeyEvent*>(ie));
				else if(ie->getID() == KeyEvent::KE_KEY_RELEASED)
					keyReleased(static_cast<KeyEvent*>(ie));
				else if(ie->getID() == KeyEvent::KE_KEY_CLICKED)
					keyClicked(static_cast<KeyEvent*>(ie));
			}			
		}
	}

	void InputManager::mouseClicked(MouseEvent* e) {}
	void InputManager::mouseEntered(MouseEvent* e) {}
	void InputManager::mouseExited(MouseEvent* e)  {}

	void InputManager::mousePressed(MouseEvent* e)
	{
		if (isCeguiActive())
		{
			System::getSingleton().injectMouseButtonDown(
				convertOgreButtonToCegui(e->getButtonID()));
			e->consume();
		}
	}

	void InputManager::mouseReleased(MouseEvent* e)
	{
		if (isCeguiActive())
		{
			System::getSingleton().injectMouseButtonUp(
				convertOgreButtonToCegui(e->getButtonID()));
			e->consume();
		}	
	}

	void InputManager::mouseMoved(MouseEvent* e)
	{
		if (isCeguiActive())
		{			
			CEGUI::Renderer* renderer  = System::getSingleton().getRenderer();
			System::getSingleton().injectMouseMove(
				e->getRelX() * renderer->getWidth(), 
				e->getRelY() * renderer->getHeight());			

			if (mPickObjects)
				updatePickedObject(e->getX(), e->getY());

			e->consume();
		}
	}

	void InputManager::keyPressed(KeyEvent* e)
	{
		if (isCeguiActive()) 
		{   // Send all events to CEGUI
			CEGUI::System& cegui = CEGUI::System::getSingleton();
			cegui.injectKeyDown(e->getKey());
			cegui.injectChar(getKeyChar(e));
			e->consume();
		}
		else
		{
			if (e->getKey() == KC_F11)
				CeConsole::getSingleton().show();
			else
			{
				mKeyDown[e->getKey()]=true;
				std::set<KeyListener*>::iterator i;
				for(i=mKeyListeners.begin(); i!=mKeyListeners.end(); i++)
					(*i)->keyPressed(e);
			}
			e->consume();
		}

	}
	void InputManager::keyReleased(KeyEvent* e)
	{
		if (isCeguiActive()) 
		{   // Send all events to CEGUI
			CEGUI::System& cegui = CEGUI::System::getSingleton();
			cegui.injectKeyUp(e->getKey());
			e->consume();
		}
		else
		{
			mKeyDown[e->getKey()]=false;
			std::set<KeyListener*>::iterator i;
			for(i=mKeyListeners.begin(); i!=mKeyListeners.end(); i++)
				(*i)->keyReleased(e);
			e->consume();
		}		
	}

	void InputManager::keyClicked(KeyEvent* e) {}

	void InputManager::mouseDragged(MouseEvent* e)
	{
		mouseMoved(e);
	}

	CEGUI::MouseButton InputManager::convertOgreButtonToCegui(int ogre_button_id)
	{
		switch (ogre_button_id)
		{
			default:
			case MouseEvent::BUTTON0_MASK:
				return CEGUI::LeftButton;
			case MouseEvent::BUTTON1_MASK:
				return CEGUI::RightButton;
			case MouseEvent::BUTTON2_MASK:
				return CEGUI::MiddleButton;
			case MouseEvent::BUTTON3_MASK:
				return CEGUI::X1Button;			
		}
	}

	void InputManager::addMouseMotionListener(MouseMotionListener *l)
	{
		if (mEventInitialized)
			mEventProcessor->addMouseMotionListener(l);

		mMouseMotionListeners.insert(l);
	}

	void InputManager::removeMouseMotionListener(MouseMotionListener *l)
	{
		if (mEventInitialized)
			mEventProcessor->removeMouseMotionListener(l);

		mMouseMotionListeners.erase(l);
	}

	void InputManager::registerCeguiWindow(CeGuiWindow* window)
	{
		if (!isCeguiActive())
		{
			switchMouseToBuffered();
			CEGUI::MouseCursor::getSingleton().show();
		}
		mActiveWindows.insert(window);
	}

	void InputManager::unregisterCeguiWindow(CeGuiWindow* window)
	{
		if (isCeguiActive())
		{

			mActiveWindows.erase(window);

			if (!isCeguiActive())
			{
				CEGUI::MouseCursor::getSingleton().hide();
				switchMouseToUnbuffered();		
			}
		}
	}

	void InputManager::switchMouseToBuffered()
	{
		 mBuffered = true;



		// Check to see if input has been initialized

		if (mInputInitialized) {



			// Destroy the input reader.

			//Ogre::Root::getSingleton().removeFrameListener(this);

			//mEventQueue.activateEventQueue(false);

			//mInputReader->useBufferedInput(NULL, false, false);			

			//PlatformManager::getSingleton().destroyInputReader( mInputReader );

			mInputInitialized = false;

		}



		mEventProcessor->initialise(Ogre::Root::getSingleton().getAutoCreatedWindow());

		mInputReader = mEventProcessor->getInputReader();

		mEventProcessor->addKeyListener(this);
		mEventProcessor->addMouseListener(this);
		mEventProcessor->addMouseMotionListener(this);
		mEventProcessor->startProcessingEvents();

		mEventInitialized = true; 
	}

	void InputManager::switchMouseToUnbuffered()
	{
		 mBuffered = false;


		// Check to see if even has been initialized

		if (mEventInitialized) {
			// Stop buffering events

		//	mEventProcessor->stopProcessingEvents();

			mEventProcessor->removeKeyListener(this);
			mEventProcessor->removeMouseListener(this);
			mEventProcessor->removeMouseMotionListener(this);

			mEventInitialized = false;

		}

		mEventQueue.activateEventQueue(true);

		mInputReader = Ogre::PlatformManager::getSingleton().createInputReader();
		mInputReader->useBufferedInput(&mEventQueue, true, false);
		mInputReader->setBufferedInput(true, false);
		mInputReader->initialise(Ogre::Root::getSingleton().getAutoCreatedWindow(), true, true);
		mInputInitialized = true; 
	}

	bool InputManager::isCeguiActive()
	{
		return !mActiveWindows.empty();
	}

	char InputManager::getKeyChar(KeyEvent* ke)
	{
		switch (ke->getKey())
		{
		case KC_2:
			if (ke->isShiftDown())
				return '"';
		}

		return ke->getKeyChar();
	}

	void InputManager::setObjectPickingActive(bool active)
	{
		mPickObjects = active;
		if (!mPickObjects)
			mTargetedObject = NULL;
	}

	void InputManager::updatePickedObject(float mouseRelX, float mouseRelY)
	{
		Actor* a = ActorManager::getSingleton().getActorAt(mouseRelX, mouseRelY);
			DebugWindow::getSingleton().setText(
			"X="+StringConverter::toString(mouseRelX)+
			"   Y="+StringConverter::toString(mouseRelY)+
			"   - Object("+(a==NULL?"null":a->getName())+")");		

		//mTargetedObject = a->getGameObject();		
	}

}
