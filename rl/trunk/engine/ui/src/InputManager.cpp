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
#include "InputManager.h"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <OISInputManager.h>

#include "Exception.h"

#include "XmlHelper.h"
#include "XmlResource.h"
#include "XmlResourceManager.h"

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
#include "UiSubsystem.h"
#include "WindowFactory.h"


template<> rl::InputManager* Singleton<rl::InputManager>::ms_Singleton = 0;
using namespace OIS;
using CEGUI::System;

namespace rl {

    InputManager::InputManager(Ogre::RenderWindow* win) :
		mNumActiveWindowsMouseInput(0),
		mNumActiveWindowsKeyboardInput(0),
		mNumActiveWindowsAllInput(0),
        mKeyMapNormal(),
		mKeyMapShift(),
	    mKeyMapAlt(),
		mKeyNames(),
        mCharacterController(NULL),
        mCommandMapper(NULL),
        mInputManager(NULL)
	{
        initializeOis(win);
        GameLoopManager::getSingleton().addSynchronizedTask(this, FRAME_ENDED);
    }

	InputManager::~InputManager()
	{
        GameLoopManager::getSingleton().removeSynchronizedTask(this);
		if( mInputManager )
		{
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );
            OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = NULL;
		}
	}

    InputManager& InputManager::getSingleton()
    {
        return Singleton<InputManager>::getSingleton();
    }

    InputManager* InputManager::getSingletonPtr()
    {
        return Singleton<InputManager>::getSingletonPtr();
    }

    void InputManager::initializeOis(RenderWindow* win)
    {
                // BEGIN INPUT INITIALIZATION
		OIS::ParamList pl;	
		size_t windowHnd = 0;
        #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		    win->getCustomAttribute("WINDOW", &windowHnd);
            pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_EXCLUSIVE")));
            pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
        #elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		    win->getCustomAttribute("GLXWINDOW", &windowHnd);
        //	pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
        //	pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
        #endif

        std::ostringstream windowHndStr;
        windowHndStr << windowHnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		//mInputManager = OIS::InputManager::createInputSystem(windowHnd);
		mInputManager = OIS::InputManager::createInputSystem(pl);
		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
        mKeyboard->setTextTranslation(OIS::Keyboard::Unicode);
		mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
        
        unsigned int width, height, depth;
		int left, top;
		win->getMetrics(width, height, depth, left, top);
        mMouse->getMouseState().width = width;
        mMouse->getMouseState().height = height;

        mKeyboard->setEventCallback(this);
        mMouse->setEventCallback(this);
    }

    void InputManager::run(Ogre::Real elapsedTime)
    {
		mMouse->capture();
        mKeyboard->capture();		
    }

	void InputManager::setCharacterController(CharacterController* controller)
	{
		mCharacterController = controller;
        mCharacterController->setCommandMapper(mCommandMapper);
    }

    bool InputManager::mousePressed(const OIS::MouseEvent & e, MouseButtonID id)
	{
		if (isCeguiActive())
		{
			System::getSingleton().injectMouseButtonDown(
                static_cast<CEGUI::MouseButton>(id));
		}
		else
		{
            if (mCharacterController != NULL)
            {
			    mCharacterController->
                    injectMouseDown(CommandMapper::encodeKey(id, getModifierCode()));
            }
		}
        return true;
	}

    bool InputManager::isMouseButtonDown( OIS::MouseButtonID buttonID )
    {
        OIS::MouseState ms = mMouse->getMouseState();
        return ms.buttonDown( buttonID );
    }


    bool InputManager::mouseReleased(const OIS::MouseEvent & arg, MouseButtonID id)
	{
		if (isCeguiActive())
		{
			System::getSingleton().injectMouseButtonUp(
                static_cast<CEGUI::MouseButton>(id));
            // return true;
		}
        else
            /// {
            /// @todo Furchtbarer Hack. Das Ereignis wird durchgeschliffen, damit
            /// der DialogCharacterController ne Möglichkeit hat den Text abzubrechen.
            /// Verantwortlichkeit zwischen DialogWindow und Controller ist arg durcheinander
            /// und die Tatsache, dass ich das als Kommentar in den InputManager schreibe zeigt,
            /// dass da noch mehr durcheinander ist. ^^
            if (mCharacterController != NULL)
                mCharacterController->injectMouseUp(
					CommandMapper::encodeKey(id, getModifierCode()));
            
		return true;
	}

    bool InputManager::mouseMoved(const OIS::MouseEvent &arg)
	{
		if (isCeguiActive())
		{			
			CEGUI::Renderer* renderer  = System::getSingleton().getRenderer();
			System::getSingleton().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);			

            return true;
		}
        return false;
	}

    Ogre::Real InputManager::getMouseRelativeX() const
    {
        if (isCeguiActive())
        {
            return 0;
            return mSavedMouseState.x;
        }
        return (float)mMouse->getMouseState().X.rel;
    }

    Ogre::Real InputManager::getMouseRelativeY() const
    {
        if (isCeguiActive())
        {
            return 0;
            return mSavedMouseState.y;
        }
        return (float)mMouse->getMouseState().Y.rel;
    }

    Ogre::Real InputManager::getMouseRelativeZ() const
    {
        if (isCeguiActive())
        {
            return 0;
            return mSavedMouseState.z;
        }
        return (float)mMouse->getMouseState().Z.rel;
    }

    bool InputManager::sendKeyToCeGui(const OIS::KeyEvent& e) const
	{
		// Fenster, die alle Inputs wollen
		if (mNumActiveWindowsAllInput > 0)
        {
			return true;
        }

		// Wenn kein Fenster mit Tastatureingabe aktiv ist, kriegt CEGUI keine KeyEvents
		if (mNumActiveWindowsKeyboardInput == 0)
        {
			return false;
        }

		// ---- Tastatureingabe gefordert ----

        // Tasten, die Zeichen liefern sollen an CEGUI gesendet werden
        if (getKeyChar(e.key, getModifierCode()) != 0)
        {
            return true;
        }

        if (e.key == OIS::KC_RETURN 
			|| e.key == OIS::KC_HOME || e.key == OIS::KC_END
			|| e.key == OIS::KC_LEFT || e.key == OIS::KC_RIGHT
			|| e.key == OIS::KC_BACK || e.key == OIS::KC_DELETE
			|| e.key == OIS::KC_UP   || e.key == OIS::KC_DOWN
			|| e.key == OIS::KC_RMENU)
        {
			return true;
        }

		return false;
	}

    bool InputManager::keyPressed(const OIS::KeyEvent& e)
	{
        if (sendKeyToCeGui(e)) 
		{   // Send all events to CEGUI
			CEGUI::System& cegui = CEGUI::System::getSingleton();
			cegui.injectKeyDown(e.key);
			cegui.injectChar(getKeyChar(e.key, getModifierCode()));
		}
        else
        {
            if (mCharacterController != NULL)
            {
                mCharacterController->injectKeyDown(CommandMapper::encodeKey(e.key, getModifierCode()));
            }
        }

        return true;
	}

	bool InputManager::keyReleased(const OIS::KeyEvent& e)
	{
		LOG_MESSAGE2(Logger::UI, "Key released: " + mKeyboard->getAsString(e.key) , "InputManager::keyReleased");
        if (sendKeyToCeGui(e)) 
		{
			CEGUI::System& cegui = CEGUI::System::getSingleton();
			cegui.injectKeyUp(e.key);
			LOG_MESSAGE2(Logger::UI, "    fed to cegui", "InputManager::keyReleased");
		}
        else
        {
            int code = CommandMapper::encodeKey(e.key, getModifierCode());
            Action* action = ActionManager::getSingleton().getInGameGlobalAction(
                mCommandMapper->getAction(code, CMDMAP_KEYMAP_GLOBAL));
            if (action != NULL)
            {
                try
                {
                    action->doAction(NULL, NULL, NULL);
					LOG_MESSAGE2(Logger::UI, "    invoked action " + action->getName(), "InputManager::keyReleased");
                }
                catch( ScriptInvocationFailedException& sife )
		        {
			        LOG_ERROR(Logger::UI, sife.toString() );
		        }
            }

            if (mCharacterController != NULL)
            {
		        mCharacterController->injectKeyUp(e.key);
				LOG_MESSAGE2(Logger::UI, "    fed to char controller", "InputManager::keyReleased");
            }
        }

        return true;
	}

	CeGuiString InputManager::getKeyName(int combinedKeyCode)
	{
		int scancode, syskeys;
		CommandMapper::decodeKey(combinedKeyCode, &scancode, &syskeys);
		return getKeyName(scancode, syskeys);
	}

	CeGuiString InputManager::getKeyName(int scancode, int syskeys)
	{
		CeGuiString name = mKeyNames.find(scancode)->second;
		if (syskeys & ALT_MASK)
			name = "Alt+"+name;
		if (syskeys & CTRL_MASK)
			name = "Ctrl+"+name;
		if (syskeys & SHIFT_MASK)
			name = "Shift+"+name;
        if (syskeys & SUPER_MASK)
			name = "Super+"+name;
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
		if (name == "Alt")
        {
			return ALT_MASK;
        }
		else if (name == "Ctrl")
        {
			return CTRL_MASK;
        }
		else if (name == "Shift")
        {
			return SHIFT_MASK;
        }
		else if (name == "Super")
        {
			return SUPER_MASK;
        }
		return 0;
	}

    int InputManager::getModifierCode() const
    {
        int rval = 0;

        if (mKeyboard->isModifierDown(OIS::Keyboard::Alt)) rval |= ALT_MASK;
        if (mKeyboard->isModifierDown(OIS::Keyboard::Ctrl)) rval |= CTRL_MASK;
        if (mKeyboard->isModifierDown(OIS::Keyboard::Shift)) rval |= SHIFT_MASK;

		return rval;
    }

	void InputManager::registerCeGuiWindow(CeGuiWindow* window)
	{
		if (window->getWindowType() == CeGuiWindow::WND_SHOW)
			return;

		bool active = isCeguiActive();

		if (window->getWindowType() == CeGuiWindow::WND_MOUSE_INPUT)
		{
			mNumActiveWindowsMouseInput++;
        }
		else if (window->getWindowType() == CeGuiWindow::WND_KEYBOARD_INPUT)
		{
			mNumActiveWindowsKeyboardInput++;
        }
		else if (window->getWindowType() == CeGuiWindow::WND_ALL_INPUT)
		{
			mNumActiveWindowsAllInput++;
        }
		
		if (!active && isCeguiActive()) // war nicht aktiv, sollte jetzt aktiv sein -> anschalten
		{
            mSavedMouseState.x = mMouse->getMouseState().X.rel;
            mSavedMouseState.y = mMouse->getMouseState().Y.rel;
            mSavedMouseState.z = mMouse->getMouseState().Z.rel;
            CEGUI::MouseCursor::getSingleton().show();
		}
	}

	void InputManager::unregisterCeGuiWindow(CeGuiWindow* window)
	{
		if (window->getWindowType() == CeGuiWindow::WND_SHOW)
		{
			return;
        }

		bool active = isCeguiActive();

		if (window->getWindowType() == CeGuiWindow::WND_MOUSE_INPUT)
		{
			mNumActiveWindowsMouseInput--;
        }
		else if (window->getWindowType() == CeGuiWindow::WND_KEYBOARD_INPUT)
		{
			mNumActiveWindowsKeyboardInput--;
        }
		else if (window->getWindowType() == CeGuiWindow::WND_ALL_INPUT)
		{
			mNumActiveWindowsAllInput--;
        }

		if (active && !isCeguiActive()) // war aktiv, sollte nicht mehr aktiv sein -> ausschalten
		{
			CEGUI::MouseCursor::getSingleton().hide();
		}
	}

	bool InputManager::isCeguiActive() const
	{
		return 
			mNumActiveWindowsKeyboardInput > 0 || 
			mNumActiveWindowsMouseInput > 0 || 
			mNumActiveWindowsAllInput > 0;
	}

	void InputManager::loadKeyMapping(const Ogre::String& filename)
	{
		using namespace XERCES_CPP_NAMESPACE;
		using XERCES_CPP_NAMESPACE::DOMDocument;
		using std::make_pair;

		XMLPlatformUtils::Initialize();
		XmlHelper::initializeTranscoder();

		XercesDOMParser* parser = new XercesDOMParser();
        parser->setValidationScheme(XercesDOMParser::Val_Auto);    // optional.
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

    const CEGUI::utf8& InputManager::getKeyChar(int scancode, int modifiers) const
    {
        static const CEGUI::utf8 NO_CHAR = 0;

        const KeyCharMap* charmap = NULL;

        if (modifiers == 0)
        {
            charmap = &mKeyMapNormal;
        }
        else if (modifiers == ALT_MASK)
        {
            charmap = &mKeyMapAlt;
        }
        else if (modifiers == SHIFT_MASK)
        {
            charmap = &mKeyMapShift;
        }

        if (charmap != NULL)
        {
            KeyCharMap::const_iterator charIt = charmap->find(scancode);
            if (charIt != charmap->end())
            {
                return (*charIt).second;
            }
        }

        return NO_CHAR;
    }

    void InputManager::loadCommandMapping(const Ogre::String& filename)
    {
        if (mCommandMapper == NULL)
        {
            mCommandMapper = new CommandMapper();
        }
		mCommandMapper->loadCommandMap(filename);
    }

    const Ogre::String& InputManager::getName() const
    {
        static Ogre::String NAME = "InputManager";

        return NAME;
    }

}
