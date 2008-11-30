/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h" //precompiled header

#include "InputManager.h"

#include <xercesc/dom/DOM.hpp>

#ifdef __APPLE__
#   include <OIS/OISInputManager.h>
#else
#   include <OISInputManager.h>
#endif

#include "Exception.h"

#include "XmlProcessor.h"

#include "AbstractWindow.h"
#include "Action.h"
#include "ActionManager.h"
#include "Actor.h"
#include "ActorManager.h"
#include "AiMessages.h"
#include "ControlState.h"
#include "CombatControlState.h"
#include "CombatManager.h"
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
#include "Job.h"
#include "JobScheduler.h"
#include "MovementControlState.h"
#include "PartyManager.h"
#include "RubyInterpreter.h"
#include "UiSubsystem.h"
#include "WindowFactory.h"
#include "WindowManager.h"

using namespace Ogre;
using namespace OIS;
using CEGUI::System;
using namespace std;

template<> rl::InputManager* Singleton<rl::InputManager>::ms_Singleton = 0;

namespace rl {

    InputManager::InputManager(Ogre::RenderWindow* win) 
      : GameTask(false),
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

        // delete finished control states
        for (ControlStateVector::iterator it = mFinishedControlStates.begin();
            it != mFinishedControlStates.end(); ++it)
        {
            delete *it;
        }
        mFinishedControlStates.clear();

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
        #elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX & defined DEBUG
            pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
            pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
        #endif

        std::ostringstream windowHndStr;
        windowHndStr << windowHnd;
        pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

        LOG_DEBUG(Logger::UI, "Initializing input manager: Create input manager");
        mInputManager = OIS::InputManager::createInputSystem(pl);
        LOG_DEBUG(Logger::UI, "Initializing input manager: Create Keyboard input.");
        mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
        mKeyboard->setTextTranslation(OIS::Keyboard::Unicode);
        mKeyboard->setEventCallback(this);
        LOG_DEBUG(Logger::UI, "Initializing input manager: Create Mouse Input.");
        mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
        mMouse->setEventCallback(this);

        LOG_DEBUG(Logger::UI, "Initializing input manager: Set mouse paremeters.");
        unsigned int width, height, depth;
        int left, top;
        win->getMetrics(width, height, depth, left, top);
        mMouse->getMouseState().width = width;
        mMouse->getMouseState().height = height;

        MessagePump::getSingleton().addMessageHandler<MessageType_DialogStarted>(
            boost::bind(&InputManager::startDialog, this, _1));

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

    bool InputManager::isKeyDown( OIS::KeyCode key )
    {
        return mKeyboard->isKeyDown(key);
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

        initializeXml();

        DOMDocument* doc = loadDocument(filename);
        DOMElement* dataDocumentContent = doc->getDocumentElement();

        DOMNodeList* keymaps
            = dataDocumentContent->getElementsByTagName(AutoXMLCh("Key").data());
        for (unsigned int idx = 0; idx < keymaps->getLength(); idx++)
        {
            DOMElement* key = static_cast<DOMElement*>(keymaps->item(idx));
            int keycode = getAttributeValueAsInteger(key, "KeyCode");

            CeGuiString s;
            s = getAttributeValueAsString(key, "NormalChar");
            if (!s.empty())
            {
                mKeyMapNormal.insert(make_pair(keycode, s[0]));
            }

            s = getAttributeValueAsString(key, "AltChar");
            if (!s.empty())
            {
                mKeyMapAlt.insert(make_pair(keycode, s[0]));
            }

            s = getAttributeValueAsString(key, "ShiftChar");
            if (!s.empty())
            {
                mKeyMapShift.insert(make_pair(keycode, s[0]));
            }

            s = getAttributeValueAsString(key, "KeyDescription");
            mKeyNames.insert(make_pair(keycode, s));
        }

        shutdownXml();
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
        Creature* character = PartyManager::getSingleton().getActiveCharacter();

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
            newController->resume();
        }
    }

    void InputManager::clearControlStates()
    {
        while (!mControlStates.empty())
        {
            ControlState* controller = mControlStates.top();
            mControlStates.pop();
            controller->pause();
            mFinishedControlStates.push_back(controller);
        }
    }

    ControlState* InputManager::getControlState() const
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

    bool InputManager::startDialog(Dialog* dialog)
    {
        pushControlState(CST_DIALOG);
        dynamic_cast<DialogControlState*>(mControlStates.top())->start(dialog);
        return true;
    }

    bool InputManager::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        bool retval = false;
        if( WindowManager::getSingleton().getWindowInputMask() & AbstractWindow::WIT_MOUSE_INPUT )
        {
            if( CEGUI::System::getSingleton().injectMouseButtonDown(
                static_cast<CEGUI::MouseButton>(id)) )
                retval = true;
        }

        if( !mControlStates.empty() )
            if( mControlStates.top()->mousePressed(evt, id, retval) )
                retval = true;
        return true;
        //return retval;
    }

    bool InputManager::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        bool retval = false;
        if( WindowManager::getSingleton().getWindowInputMask() & AbstractWindow::WIT_MOUSE_INPUT )
        {
            if( CEGUI::System::getSingleton().injectMouseButtonUp(
                static_cast<CEGUI::MouseButton>(id)) )
                retval = true;
        }

        if( !mControlStates.empty() )
            if( mControlStates.top()->mouseReleased(evt, id, retval) )
                retval = true;
        return true;
        //return retval;
    }

    bool InputManager::mouseMoved(const OIS::MouseEvent& evt)
    {
        bool retval = false;
        if( WindowManager::getSingleton().getWindowInputMask() & AbstractWindow::WIT_MOUSE_INPUT )
        {
            if( CEGUI::System::getSingleton().injectMouseMove(
                evt.state.X.rel, evt.state.Y.rel ) )
                retval = true;
        }

        if( !mControlStates.empty() )
            if( mControlStates.top()->mouseMoved(evt, retval) )
                retval = true;
        return true;
        //return retval;
    }

    // job definition for key repeating
    // this job is created when a key was pressed and the processing window wants the key to be repeated
    class KeyRepeatJob : public Job
    {
    public:
        static void createKeyRepeatJob(AbstractWindow* window, OIS::KeyCode key)
        {
            // nur neu anlegen, wenns noch nicht existiert!
            if( mKeyJobMap.find(key) == mKeyJobMap.end() )
            {
                KeyRepeatJob *job = new KeyRepeatJob(window, key);
                JobScheduler::getSingleton().addJob(job, JobScheduler::JP_NORMAL, 0.5);
            }
        }
        bool execute(Ogre::Real t)
        {
            bool handled = false;
            rl::Time time = TimeSourceManager::getSingleton().getTimeSource(TimeSource::REALTIME_CONTINUOUS)->getClock();
            if ( WindowManager::getSingleton().getActiveWindow() == mWindow && // perhaps window was deleted!!
                InputManager::getSingleton().isKeyDown(OIS::KeyCode(mKey)) )
            {
                if ( time - mLastTime > 50*mCount )
                {
                    if( CEGUI::System::getSingleton().injectKeyDown(mKey) )
                        handled = true;
                    else
                    {
                        static const CEGUI::utf8 NO_CHAR = 0;
                        if( InputManager::getSingleton().getKeyChar(mKey, InputManager::getSingleton().getModifierCode()) != NO_CHAR )
                        {
                            if( CEGUI::System::getSingleton().injectChar(InputManager::getSingleton().getKeyChar(mKey, InputManager::getSingleton().getModifierCode())) )
                                handled = true;
                        }
                    }
                    if( CEGUI::System::getSingleton().injectKeyUp(mKey) )
                        handled = true;

                    mLastTime = time;
                }

                return false;
            }

            mKeyJobMap.erase(mKey);
            if( mCount > 0 )
                mCount--;
            return true;
        }
    private:
        rl::Time mLastTime;
        AbstractWindow* mWindow;
        OIS::KeyCode mKey;
        typedef std::map<OIS::KeyCode, KeyRepeatJob*> KeyJobMap;
        static KeyJobMap mKeyJobMap;
        static int mCount;


        KeyRepeatJob(AbstractWindow* window, OIS::KeyCode key) :
          Job(false, true),
          mWindow(window),
          mKey(key),
          mLastTime(0)
        {
            mKeyJobMap[key] = this;
            mCount++;
        }
    };
    KeyRepeatJob::KeyJobMap KeyRepeatJob::mKeyJobMap;
    int KeyRepeatJob::mCount = 0;

    bool InputManager::keyPressed(const OIS::KeyEvent& evt)
    {
        bool retval = false;
        if( WindowManager::getSingleton().getWindowInputMask() & AbstractWindow::WIT_KEYBOARD_INPUT )
        {
            AbstractWindow *activeWin = WindowManager::getSingleton().getActiveWindow();
            if( activeWin != NULL )
            {
                if( activeWin->wantsKeyToRepeat(evt.key) )
                {
                    KeyRepeatJob::createKeyRepeatJob(activeWin, evt.key);
                }
            }

            if( CEGUI::System::getSingleton().injectKeyDown( evt.key ) )
                retval = true;
            else
            {
                static const CEGUI::utf8 NO_CHAR = 0;
                if( getKeyChar(evt.key, getModifierCode()) != NO_CHAR )
                {
                    if( CEGUI::System::getSingleton().injectChar(getKeyChar(evt.key, getModifierCode())) )
                        retval = true;
                }
            }
        }

        if( !mControlStates.empty() )
            if( mControlStates.top()->keyPressed(evt, retval) )
                retval = true;

        if( !retval )
        {
            KeyCommandMap::iterator it = mKeyRubyCommand.find(evt.key);
            if( it != mKeyRubyCommand.end() )
                CoreSubsystem::getSingleton().getRubyInterpreter()->execute(it->second.c_str());
                
        }

        return true;
        //return retval;
    }

    bool InputManager::keyReleased(const OIS::KeyEvent& evt)
    {
        bool retval = false;
        if( WindowManager::getSingleton().getWindowInputMask() & AbstractWindow::WIT_KEYBOARD_INPUT )
        {
            if( CEGUI::System::getSingleton().injectKeyUp( evt.key ) )
                retval = true;
        }

        if( !mControlStates.empty() )
            if( mControlStates.top()->keyReleased(evt, retval) )
                retval = true;
        return true;
        //return false;
    }

}
