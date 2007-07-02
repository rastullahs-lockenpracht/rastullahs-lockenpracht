/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include "AbstractWindow.h"
#include "ControlState.h"
#include "CombatControlState.h"
#include "CommandMapper.h"
#include "ConfigurationManager.h"
#include "Console.h"
#include "CoreSubsystem.h"
#include "CutsceneControlState.h"
#include "DebugWindow.h"
#include "DialogControlState.h"
#include "FreeflightControlState.h"
#include "GameLoop.h"
#include "GameObject.h"
#include "MovementControlState.h"
#include "RubyInterpreter.h"
#include "UiSubsystem.h"
#include "WindowFactory.h"

using namespace Ogre;
using namespace OIS;
using CEGUI::System;
using namespace std;

template<> rl::InputManager* Singleton<rl::InputManager>::ms_Singleton = 0;

namespace rl {

    InputManager::InputManager(Ogre::RenderWindow* win) :
        mKeyMapNormal(),
        mKeyMapShift(),
        mKeyMapAlt(),
        mKeyNames(),
        mCommandMapper(NULL),
        mInputManager(NULL)
    {
        initializeOis(win);

        loadKeyMapping(ConfigurationManager::getSingleton().getKeymap());
        LOG_MESSAGE2(Logger::UI, "Keymap geladen", "UiSubsystem::initializeUiSubsystem");

        mCommandMapper = new CommandMapper();

        GameLoop::getSingleton().addTask(this, GameLoop::TG_INPUT);
    }

    InputManager::~InputManager()
    {
        clearControlStates();

        GameLoop::getSingleton().removeTask(this);
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = NULL;
        }
    }

    void InputManager::initializeOis(RenderWindow* win)
    {
        // BEGIN INPUT INITIALIZATION
        LOG_DEBUG(Logger::UI, "Initializing input manager.");

        OIS::ParamList pl;
        size_t windowHnd = 0;
        LOG_DEBUG(Logger::UI, "Initializing input manager: Render window parameters");
        win->getCustomAttribute("WINDOW", &windowHnd);

        #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_EXCLUSIVE")));
            pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
        #elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
        //    pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
        //    pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
        #endif

        std::ostringstream windowHndStr;
        windowHndStr << windowHnd;
        pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

        LOG_DEBUG(Logger::UI, "Initializing input manager: Create input manager");
        mInputManager = OIS::InputManager::createInputSystem(pl);
        LOG_DEBUG(Logger::UI, "Initializing input manager: Create Keyboard input.");
        mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
        mKeyboard->setTextTranslation(OIS::Keyboard::Unicode);
        LOG_DEBUG(Logger::UI, "Initializing input manager: Create Mouse Input.");
        mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

        LOG_DEBUG(Logger::UI, "Initializing input manager: Set mouse paremeters.");
        unsigned int width, height, depth;
        int left, top;
        win->getMetrics(width, height, depth, left, top);
        mMouse->getMouseState().width = width;
        mMouse->getMouseState().height = height;

        LOG_DEBUG(Logger::UI, "Done initializing input manager.");
    }

    void InputManager::run(Ogre::Real elapsedTime)
    {
        mMouse->capture();
        mKeyboard->capture();

		System::getSingleton().injectTimePulse(elapsedTime);

        // delete finished control states
        for (ControlStateVector::iterator it = mFinishedControlStates.begin();
            it != mFinishedControlStates.end(); ++it)
        {
            delete *it;
        }
        mFinishedControlStates.clear();
   
        if (!mControlStates.empty())
        {
            mControlStates.top()->run(elapsedTime);
        }
    }

    bool InputManager::isMouseButtonDown( OIS::MouseButtonID buttonID )
    {
        OIS::MouseState ms = mMouse->getMouseState();
        return ms.buttonDown( buttonID );
    }

    Ogre::Real InputManager::getMouseRelativeX() const
    {
        return (float)mMouse->getMouseState().X.rel;
    }

    Ogre::Real InputManager::getMouseRelativeY() const
    {
        return (float)mMouse->getMouseState().Y.rel;
    }

    Ogre::Real InputManager::getMouseRelativeZ() const
    {
        return (float)mMouse->getMouseState().Z.rel;
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

    const Ogre::String& InputManager::getName() const
    {
        static Ogre::String NAME = "InputManager";

        return NAME;
    }

    void InputManager::linkKeyToRubyCommand(const CeGuiString &keyStr, const CeGuiString &command)
    {
        std::ostringstream ss;

        ss << "InputManager::linkKeyToRubyCommand called: ";
        int key = getScanCode(keyStr);
        if( command.length() == 0 ) // delete
        {
            KeyCommandMap::iterator it = mKeyRubyCommand.find(key);
            ss << "Requesting to delete link from Key '" << keyStr << "'... ";
            if( it != mKeyRubyCommand.end() )
            {
                ss << "Link to command '" << it->second << "' deleted.";
                mKeyRubyCommand.erase(it);
            }
        }
        else
        {
            KeyCommandMap::iterator it = mKeyRubyCommand.find(key);
            if( it == mKeyRubyCommand.end() )
            {
                ss << "New linking Key '";
                mKeyRubyCommand.insert(make_pair(key, command));
            }
            else
            {
                ss << "Setting link from key '";
                it->second = command;
            }
            ss << keyStr << "' to command '" << command << "'.";

        }

        LOG_MESSAGE(Logger::UI, ss.str());
    }

    void InputManager::setControlState(ControlStateType controlStateType)
    {
        while (!mControlStates.empty())
        {
            popControlState();
        }
        pushControlState(controlStateType);
    }

    void InputManager::pushControlState(ControlStateType controlStateType)
    {
        Actor* camera = ActorManager::getSingleton().getActor("DefaultCamera");
        Person* character = UiSubsystem::getSingleton().getActiveCharacter();

        ControlState* controller = NULL;
        switch (controlStateType)
        {
        case CST_CUTSCENE:
            controller = new CutsceneControlState(mCommandMapper, camera);
            break;
        case CST_MOVEMENT:
            controller = new MovementControlState(mCommandMapper, camera, character);
            break;
        case CST_FREEFLIGHT:
            controller = new FreeflightControlState(mCommandMapper, camera, character);
            break;
        case CST_DIALOG:
            controller = new DialogControlState(mCommandMapper, camera, character);
            break;
        case CST_COMBAT:
            controller = new CombatControlState(mCommandMapper, camera, character);
            break;
        default:
            Throw(IllegalStateException, "Unknown controller type.");
        }

        if (!mControlStates.empty())
        {
            mControlStates.top()->pause();
        }

        mKeyboard->setEventCallback(controller);
        mMouse->setEventCallback(controller);

        mControlStates.push(controller);
        mControlStates.top()->resume();
    }

    void InputManager::popControlState()
    {
        ControlState* controller = mControlStates.top();
        mControlStates.pop();
        controller->pause();
        mFinishedControlStates.push_back(controller);

        if (!mControlStates.empty())
        {
            ControlState* newController = mControlStates.top();
            mKeyboard->setEventCallback(newController);
            mMouse->setEventCallback(newController);
            newController->resume();
        }
        else
        {
            mKeyboard->setEventCallback(NULL);
            mMouse->setEventCallback(NULL);
        }
    }

    void InputManager::clearControlStates()
    {
        while (!mControlStates.empty())
        {
            popControlState();
        }
    }

    ControlState* InputManager::getCharacterController() const
    {
        if (!mControlStates.empty())
        {
            return mControlStates.top();
        }
        else
        {
            return NULL;
        }
    }
}
