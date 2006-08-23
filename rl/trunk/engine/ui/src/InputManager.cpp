/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "Exception.h"

#include "XmlHelper.h"
#include "XmlResource.h"
#include "XmlResourceManager.h"

#include <OgreKeyEvent.h>
#include <OgreRoot.h>

#include "Action.h"
#include "ActionManager.h"
#include "Actor.h"
#include "ActorManager.h"
#include "CeGuiWindow.h"
#include "CharacterController.h"
#include "CommandMapper.h"
#include "Console.h"
#include "CoreSubsystem.h"
#include "DebugWindow.h"
#include "DialogWindow.h"
#include "GameLoop.h"
#include "GameObject.h"
#include "InputManager.h"
#include "UiSubsystem.h"
#include "WindowFactory.h"



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

	InputManager::InputManager() :
		mEventInitialized(false),
		mBuffered(false), 
		mInputInitialized(false),
		mNumActiveWindowsMouseInput(0),
		mNumActiveWindowsKeyboardInput(0),
		mNumActiveWindowsAllInput(0),
		mPickObjects(false),
        mTargetedObject(NULL),
		mScheduledInputSwitch(SWITCH_NO_SWITCH),
        mInputReader( NULL ),
        mTargetedObjectTime(0),
        mKeyMapNormal(),
		mKeyMapShift(),
	    mKeyMapAlt(),
		mKeyNames(),
        mCharacterController( NULL ),
        mCommandMapper(NULL)
	{
        for(int i=0; i<NUM_KEYS; i++)
            mKeyDown[i] = false;
		for(int i=0; i<NUM_MOUSE_BUTTON; i++)
			mMouseButtonDown[i] = false;

		mEventQueue = new EventQueue();
		mEventProcessor = new EventProcessor();
		switchMouseToUnbuffered();
		GameLoopManager::getSingleton().addSynchronizedTask(this, FRAME_ENDED);
        
		mScreenX = Root::getSingleton().getAutoCreatedWindow()->getWidth();
		mScreenY = Root::getSingleton().getAutoCreatedWindow()->getHeight();		
	}

	InputManager::~InputManager()
	{
		mEventQueue->activateEventQueue(false);
		GameLoopManager::getSingleton().removeSynchronizedTask(this);

		mInputReader->useBufferedInput(NULL, false, false);
		mInputReader->setBufferedInput(false, false);

		delete mEventProcessor;
        delete mEventQueue;

        delete mCommandMapper;
	}


	void InputManager::setCharacterController(CharacterController* controller)
	{
		mCharacterController = controller;
        mCharacterController->setCommandMapper(mCommandMapper);
	}

	void InputManager::run(Real elapsedTime)
	{
		if (mScheduledInputSwitch == SWITCH_TO_BUFFERED)
		{
			switchMouseToBuffered();
			mScheduledInputSwitch = SWITCH_NO_SWITCH;
			return;
		}
		else if (mScheduledInputSwitch == SWITCH_TO_UNBUFFERED)
		{
			switchMouseToUnbuffered();
			mScheduledInputSwitch = SWITCH_NO_SWITCH;
			return;
		}

		if (mNumActiveWindowsKeyboardInput == 0)
		{
			mInputReader->capture();
			while (mEventQueue->getSize() > 0)
			{
				InputEvent* ie = mEventQueue->pop();
				if(ie->getID() == KeyEvent::KE_KEY_PRESSED)
					keyPressed(static_cast<KeyEvent*>(ie));
				else if(ie->getID() == KeyEvent::KE_KEY_RELEASED)
					keyReleased(static_cast<KeyEvent*>(ie));
				else if(ie->getID() == KeyEvent::KE_KEY_CLICKED)
					keyClicked(static_cast<KeyEvent*>(ie));
                delete ie;
			}			
		}

		if (!mBuffered)
		{
			int pressedButtonMask = 0, releasedButtonMask = 0; 
			checkMouseButton(0, MouseEvent::BUTTON0_MASK, pressedButtonMask, releasedButtonMask);
			checkMouseButton(1, MouseEvent::BUTTON1_MASK, pressedButtonMask, releasedButtonMask);
			checkMouseButton(2, MouseEvent::BUTTON2_MASK, pressedButtonMask, releasedButtonMask);
			checkMouseButton(3, MouseEvent::BUTTON3_MASK, pressedButtonMask, releasedButtonMask);

			if (releasedButtonMask != 0 && mCharacterController!=NULL)
				mCharacterController->injectMouseUp(releasedButtonMask);

			if (pressedButtonMask != 0 && mCharacterController!=NULL)
				mCharacterController->injectMouseDown(pressedButtonMask);
		}		
	}

	void InputManager::checkMouseButton(
		const int button, const int buttonMask, int& pressedButtonMask, int& releasedButtonMask)
	{
		if (mInputReader->getMouseButton(button))
		{
			if (mMouseButtonDown[button] == false)
				pressedButtonMask |= buttonMask;

			mMouseButtonDown[button] = true;
		}
		else
		{
			if (mMouseButtonDown[button] == true)
				releasedButtonMask |= buttonMask;

			mMouseButtonDown[button] = false;
		}
	}

	void InputManager::mouseClicked(MouseEvent* e) 
	{
		if ( ! (isCeguiActive() && mBuffered) )
		{
			e->consume();
            if( mCharacterController != NULL )
            {
			    mCharacterController->injectMouseClicked(
                    CommandMapper::encodeKey(e->getButtonID(), e->getModifiers()));
            }
		}
	}

	void InputManager::mouseEntered(MouseEvent* e) {}
	void InputManager::mouseExited(MouseEvent* e)  {}

	void InputManager::mousePressed(MouseEvent* e)
	{
		e->consume();
		if (isCeguiActive() && mBuffered)
		{
			System::getSingleton().injectMouseButtonDown(
				convertOgreButtonToCegui(e->getButtonID()));
		}
		else
		{
            if (mCharacterController != NULL)
			    mCharacterController->injectMouseDown(CommandMapper::encodeKey(e->getButtonID(), e->getModifiers()));
		}
			
	}

	void InputManager::mouseReleased(MouseEvent* e)
	{
		e->consume();
		if (isCeguiActive() && mBuffered)
		{
			System::getSingleton().injectMouseButtonUp(
				convertOgreButtonToCegui(e->getButtonID()));
		}
            /// else
            /// {
            /// @todo Furchtbarer Hack. Das Ereignis wird durchgeschliffen, damit
            /// der DialogCharacterController ne Möglichkeit hat den Text abzubrechen.
            /// Verantwortlichkeit zwischen DialogWindow und Controller ist arg durcheinander
            /// und die Tatsache, dass ich das als Kommentar in den InputManager schreibe zeigt,
            /// dass da noch mehr durcheinander ist. ^^
            if (mCharacterController != NULL)
                mCharacterController->injectMouseUp(CommandMapper::encodeKey(e->getButtonID(), e->getModifiers()));
            /// }
	}

    void InputManager::mouseMoved(MouseEvent* e)
	{
		if (isCeguiActive() && mBuffered)
		{			
			e->consume();

			CEGUI::Renderer* renderer  = System::getSingleton().getRenderer();
			System::getSingleton().injectMouseMove(
				e->getRelX() * renderer->getWidth(), 
				e->getRelY() * renderer->getHeight());			

			if (mPickObjects)
				updatePickedObject(e->getX(), e->getY());
		}       
	}

	bool InputManager::sendKeyToCeGui(KeyEvent* e)
	{
		// Fenster, die alle Inputs wollen
		if (mNumActiveWindowsAllInput > 0)
			return true;

		// Wenn kein Fenster mit Tastatureingabe aktiv ist, kriegt CEGUI keine KeyEvents
		if (mNumActiveWindowsKeyboardInput == 0)
			return false;

		// Tastatureingabe gefordert
		// Alle Tasten an CEGUI senden, die ein Zeichen erzeugen
		if (getKeyChar(e) != 0)
			return true;

		if (e->getKey() == KC_RETURN || 
			e->getKey() == KC_HOME || e->getKey() == KC_END ||
			e->getKey() == KC_LEFT || e->getKey() == KC_RIGHT ||
			e->getKey() == KC_BACK || e->getKey() == KC_DELETE ||
			e->getKey() == KC_UP || e->getKey() == KC_DOWN ||
			e->getKey() == KC_RMENU)
			return true;

		return false;
	}

	void InputManager::keyPressed(KeyEvent* e)
	{
		e->consume();

		if (sendKeyToCeGui(e)) 
		{   // Send all events to CEGUI
			CEGUI::System& cegui = CEGUI::System::getSingleton();
			cegui.injectKeyDown(e->getKey());
			cegui.injectChar(getKeyChar(e));
			return;
		}

		mKeyDown[e->getKey()]=true;
        if( mCharacterController!=NULL )
		    mCharacterController->injectKeyDown(e->getKey());
	}

	void InputManager::keyReleased(KeyEvent* e)
	{
		e->consume();

		if (sendKeyToCeGui(e)) 
		{
			CEGUI::System& cegui = CEGUI::System::getSingleton();
			cegui.injectKeyUp(e->getKey());

			return;
		}

		mKeyDown[e->getKey()] = false;

        if (mCharacterController != NULL)
        {
		    mCharacterController->injectKeyUp(e->getKey());
        }
	}

	void InputManager::keyClicked(KeyEvent* e) 
	{
		e->consume();
		if (sendKeyToCeGui(e)) 
			return;
		
        int code = CommandMapper::encodeKey(e->getKey(), e->getModifiers());
        Action* action = ActionManager::getSingleton().getInGameGlobalAction(
            mCommandMapper->getAction(code, CMDMAP_KEYMAP_GLOBAL));
        if (action != NULL)
        {
            try
            {
                action->doAction(NULL, NULL, NULL);
            }
            catch( ScriptInvocationFailedException& sife )
		    {
			    LOG_ERROR(Logger::UI, sife.toString() );
		    }
        }
        
        if (mCharacterController != NULL)
        {
		    mCharacterController->injectKeyClicked(code);
        }
	}

	void InputManager::mouseDragged(MouseEvent* e)
	{
		mouseMoved(e);
	}

	CeGuiString InputManager::getKeyName(int combinedKeyCode)
	{
		int scancode, syskeys;
		CommandMapper::decodeKey(combinedKeyCode, &scancode, &syskeys);
		return getKeyName(scancode, syskeys);
	}

	CeGuiString InputManager::getKeyName(int scancode, int syskeys)
	{
		using namespace Ogre; 

		CeGuiString name = mKeyNames.find(scancode)->second;
		if (syskeys & InputEvent::ALT_MASK)
			name = "Alt+"+name;
		if (syskeys & InputEvent::CTRL_MASK)
			name = "Ctrl+"+name;
		if (syskeys & InputEvent::SHIFT_MASK)
			name = "Shift+"+name;
		if (syskeys & InputEvent::META_MASK)
			name = "Meta+"+name;
		return name;
	}

	int InputManager::getScanCode(const CeGuiString& name)
	{
		for(KeyNameMap::iterator it = mKeyNames.begin(); it != mKeyNames.end(); it++)
		{
			if ((*it).second == name)
				return (*it).first;
		}

		Ogre::String msg = Ogre::String("Key ")+ name.c_str()+" not found.";
		Throw(IllegalArgumentException, msg);
	}

	int InputManager::getSystemCode(const CeGuiString& name)
	{
		using namespace Ogre; 

		if (name == "Alt")
			return InputEvent::ALT_MASK;
		else if (name == "Ctrl")
			return InputEvent::CTRL_MASK;
		else if (name == "Shift")
			return InputEvent::SHIFT_MASK;
		else if (name == "Meta")
			return InputEvent::META_MASK;

		return 0;
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

	void InputManager::registerCeGuiWindow(CeGuiWindow* window)
	{
		if (window->getWindowType() == CeGuiWindow::WND_SHOW)
			return;

		bool active = isCeguiActive();

		if (window->getWindowType() == CeGuiWindow::WND_MOUSE_INPUT)
			mNumActiveWindowsMouseInput++;
		else if (window->getWindowType() == CeGuiWindow::WND_KEYBOARD_INPUT)
			mNumActiveWindowsKeyboardInput++;
		else if (window->getWindowType() == CeGuiWindow::WND_ALL_INPUT)
			mNumActiveWindowsAllInput++;
		
		if (!active && isCeguiActive()) // war nicht aktiv, sollte jetzt aktiv sein -> anschalten
		{
			setObjectPickingActive(false);
            CEGUI::MouseCursor::getSingleton().show();
            resetPressedKeys( true );
			if (mScheduledInputSwitch == SWITCH_TO_UNBUFFERED)
				mScheduledInputSwitch = SWITCH_NO_SWITCH;
			else
				mScheduledInputSwitch = SWITCH_TO_BUFFERED;
		}
	}

	void InputManager::unregisterCeGuiWindow(CeGuiWindow* window)
	{
		if (window->getWindowType() == CeGuiWindow::WND_SHOW)
			return;

		bool active = isCeguiActive();

		if (window->getWindowType() == CeGuiWindow::WND_MOUSE_INPUT)
			mNumActiveWindowsMouseInput--;
		else if (window->getWindowType() == CeGuiWindow::WND_KEYBOARD_INPUT)
			mNumActiveWindowsKeyboardInput--;
		else if (window->getWindowType() == CeGuiWindow::WND_ALL_INPUT)
			mNumActiveWindowsAllInput--;

		if (active && !isCeguiActive()) // war aktiv, sollte nicht mehr aktiv sein -> ausschalten
		{
			CEGUI::MouseCursor::getSingleton().hide();
            setObjectPickingActive(true);
            resetPressedKeys( false );
			if (mScheduledInputSwitch == SWITCH_TO_BUFFERED)
				mScheduledInputSwitch = SWITCH_NO_SWITCH;
			else
				mScheduledInputSwitch = SWITCH_TO_UNBUFFERED;		
		}
	}

    void InputManager::resetPressedKeys( bool up )
    {
        for(int i=0; i<NUM_KEYS; i++)
        {
            if( mKeyDown[i] && up )
            {
                if( mCharacterController != NULL )
                    mCharacterController->injectKeyUp( i );
                mKeyDown[i] = false;
            }
            else if( mKeyDown[i] && !up ) 
            {
                if( mCharacterController != NULL )
                    mCharacterController->injectKeyDown( i );
            }
        }
    }

	void InputManager::switchMouseToBuffered()
	{
		 mBuffered = true;

		// Check to see if input has been initialized
		if (mInputInitialized) {

			mEventQueue->activateEventQueue(false);
			mInputReader->useBufferedInput(NULL, false, false);			
			
            // Destroy the input reader.
			PlatformManager::getSingleton().destroyInputReader( mInputReader );

			mInputInitialized = false;
		}

		mEventProcessor->initialise(Ogre::Root::getSingleton().getAutoCreatedWindow());

		mInputReader = mEventProcessor->getInputReader();

		mEventProcessor->addKeyListener(this);
		mEventProcessor->addMouseListener(this);
		mEventProcessor->addMouseMotionListener(this);
		LOG_MESSAGE(Logger::UI, "Start processing events");
		mEventProcessor->startProcessingEvents();

		mEventInitialized = true; 
	}

	void InputManager::switchMouseToUnbuffered()
	{
		 mBuffered = false;

		// Check to see if event has been initialized
		if (mEventInitialized) {
			// Stop buffering events

			LOG_MESSAGE(Logger::UI, "Stop processing events");
			mEventProcessor->stopProcessingEvents();
			mEventInitialized = false;
		}

		while(mEventQueue->getSize() > 0)
			mEventQueue->pop();
		mEventQueue->activateEventQueue(true);
		while(mEventQueue->getSize() > 0)
			mEventQueue->pop();
		
		mInputReader = Ogre::PlatformManager::getSingleton().createInputReader();
		mInputReader->useBufferedInput(mEventQueue, true, false);
		mInputReader->setBufferedInput(true, false);
		mInputReader->initialise(Ogre::Root::getSingleton().getAutoCreatedWindow(), true, true);

		mInputInitialized = true; 
	}

	bool InputManager::isCeguiActive()
	{
		return 
			mNumActiveWindowsKeyboardInput > 0 || 
			mNumActiveWindowsMouseInput > 0 || 
			mNumActiveWindowsAllInput > 0;
	}

	/**
	 * Ermittelt aus einem KeyEvent das zugehörige Zeichen auf der Tastatur
	 * @todo Um andere Sprachen zu ermöglichen, in datengetriebene Lösung umwandeln, 
	 * Locale-Dateien, generischer Ansatz
	 * 
	 * @param ke Ogre-KeyEvent zu verarbeitendes Event
	 * @return Zeichen, das der gedrückten Tastenkombination entspricht
	 */
	CEGUI::utf32 InputManager::getKeyChar(KeyEvent* ke)
	{
		KeyCharMap* keymap;
		if (!ke->isShiftDown() && !ke->isAltDown())
			keymap = &mKeyMapNormal;
		else if (ke->isShiftDown() && !ke->isAltDown())
			keymap = &mKeyMapShift;
		else if (!ke->isShiftDown() && ke->isAltDown())
			keymap = &mKeyMapAlt;
		else
			return ke->getKeyChar();

		KeyCharMap::iterator keyIter = keymap->find(ke->getKey());
		if (keyIter != keymap->end())
			return (*keyIter).second;
			
		return ke->getKeyChar();
	}

	void InputManager::loadKeyMapping(const Ogre::String& filename)
	{
		using namespace XERCES_CPP_NAMESPACE;
		using XERCES_CPP_NAMESPACE::DOMDocument;
		using std::make_pair;

		XMLPlatformUtils::Initialize();
		XmlHelper::initializeTranscoder();

		XercesDOMParser* parser = new XercesDOMParser();
        parser->setValidationScheme(XercesDOMParser::Val_Always);    // optional.
        parser->setDoNamespaces(true);    // optional

/*        ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
        parser->setErrorHandler(errHandler);*/

		XMLCh* ALT = XMLString::transcode("AltChar");
		XMLCh* SHIFT = XMLString::transcode("ShiftChar");
		XMLCh* NORMAL = XMLString::transcode("NormalChar");
		XMLCh* DESCR = XMLString::transcode("KeyDescription");
		XMLCh* CODE = XMLString::transcode("KeyCode");
		XMLCh* KEY = XMLString::transcode("Key");
		
		XmlPtr res = 
			XmlResourceManager::getSingleton().create(
			filename, 
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		res->parseBy(parser);
		DOMDocument* doc = parser->getDocument();
		DOMElement* dataDocumentContent = doc->getDocumentElement();

		DOMNodeList* keymaps = dataDocumentContent->getElementsByTagName(KEY);
		for (unsigned int idx = 0; idx < keymaps->getLength(); idx++)
		{
			DOMElement* key = static_cast<DOMElement*>(keymaps->item(idx));
			int keycode = XMLString::parseInt(key->getAttribute(CODE));

			const XMLCh* xmlch;

			xmlch = key->getAttribute(NORMAL);
			if (xmlch != NULL && XMLString::stringLen(xmlch) > 0)
			{
				CeGuiString s(XmlHelper::transcodeToString(xmlch)); 
				mKeyMapNormal.insert(make_pair(keycode, s[0]));
			}

			xmlch = key->getAttribute(ALT);
			if (xmlch != NULL && XMLString::stringLen(xmlch) > 0)
			{
				CeGuiString s(XmlHelper::transcodeToString(xmlch)); 
				mKeyMapAlt.insert(make_pair(keycode, s[0]));
			}

			xmlch = key->getAttribute(SHIFT);
			if (xmlch != NULL && XMLString::stringLen(xmlch) > 0)
			{
				CeGuiString s(XmlHelper::transcodeToString(xmlch)); 
				mKeyMapShift.insert(make_pair(keycode, s[0]));
			}

			xmlch = key->getAttribute(DESCR);
			mKeyNames.insert(make_pair(keycode, XmlHelper::transcodeToString(xmlch)));
		}

		XMLString::release(&ALT);
		XMLString::release(&SHIFT);
		XMLString::release(&NORMAL);
		XMLString::release(&CODE);
		XMLString::release(&DESCR);
		XMLString::release(&KEY);

		doc->release();
		XMLPlatformUtils::Terminate();

        //XmlResourceManager::getSingleton().remove(filename);
        //res.setNull();
	}

    void InputManager::loadCommandMapping(const Ogre::String& filename)
    {
        if (mCommandMapper == NULL)
        {
            mCommandMapper = new CommandMapper();
        }
		mCommandMapper->loadCommandMap(filename);
    }

	void InputManager::setObjectPickingActive(bool active)
	{
		mPickObjects = active;
		if (!mPickObjects)
		{
            // Altes Picking entfernen
            if (mTargetedObject != NULL && mTargetedObject->getActor() != NULL ) 
				mTargetedObject->getActor()->setHighlighted(false);

			mTargetedObject = NULL;
			WindowFactory::getSingleton().showObjectDescription(NULL);
		}
	}

    void InputManager::updatePickedObject(float mouseRelX, float mouseRelY)
    {
        Actor* actor = ActorManager::getSingleton().getActorAt(mouseRelX, mouseRelY, 30, 7);

        // Keine Highlights in Cutscene oder Dialog
        if( actor != NULL )
		{
            // Altes Highlight entfernen
			if (mTargetedObject != NULL &&
                mTargetedObject->getActor() != NULL &&
                actor != mTargetedObject->getActor() )
            {
				mTargetedObject->getActor()->setHighlighted(false);
            }

            // Nur ein Highlight wenn es auch ein dazugehöriges GameObject gibt
			if( actor->getGameObject() != NULL)
            {
				GameObject* targetedObject = static_cast<GameObject*>(actor->getGameObject());
				if (targetedObject->isHighlightingEnabled())
				{
					if (targetedObject != mTargetedObject)
					{
					    actor->setHighlighted(true);
						mTargetedObject = targetedObject;
						// mTargetedObjectTime = CoreSubsystem::getSingleton().getClock();
						// WindowFactory::getSingleton().showObjectName(targetedObject);
					}
					//else
					//{
					//	if (CoreSubsystem::getSingleton().getClock()
					//		- mTargetedObjectTime 
					//		> TIME_SHOW_DESCRIPTION)
					//	{
					//		WindowFactory::getSingleton().showObjectDescription(mTargetedObject);
					//	}
					//}
				}
            }
		}
        // Nichts mehr angewählt
		else
		{
			if (mTargetedObject != NULL && mTargetedObject->getActor() != NULL ) 
			{
				mTargetedObject->getActor()->setHighlighted(false);
				//mTargetedObjectTime = 0;
				//WindowFactory::getSingleton().showObjectName(NULL);
				//WindowFactory::getSingleton().showObjectDescription(NULL);
			}

			mTargetedObject = NULL;
		}
    }

	GameObject* InputManager::getPickedObject()
	{
		return mTargetedObject;
	}

	bool InputManager::isKeyDown(KeyCode kc) 
	{ 
		return mKeyDown[kc]; 
	}
	
	bool InputManager::isMouseButtonDown(int iButtonID) 
	{ 
		if (mInputInitialized)
			return mInputReader->getMouseButton(iButtonID); 

		return false;
	}


	Ogre::Real InputManager::getMouseRelativeX(void) 
	{ 
		if (mInputInitialized)
			return mInputReader->getMouseRelativeX(); 

		return 0;
	}

	Ogre::Real InputManager::getMouseRelativeY(void) 
	{ 
		if (mInputInitialized)
			return mInputReader->getMouseRelativeY(); 

		return 0;
	}

	Ogre::Real InputManager::getMouseRelativeZ(void) 
	{ 
		if (mInputInitialized)
			return mInputReader->getMouseRelativeZ(); 

		return 0;
	}
}
