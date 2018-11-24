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

#ifdef __APPLE__
#include <OIS/OISInputManager.h>
#else
#include <OISInputManager.h>
#endif

#include "Exception.h"

#include "XmlProcessor.h"

#include "AbstractWindow.h"
#include "Action.h"
#include "ActionManager.h"
#include "Actor.h"
#include "ActorManager.h"
#include "AiMessages.h"
#include "CombatControlState.h"
#include "CombatManager.h"
#include "CommandMapper.h"
#include "ConfigurationManager.h"
#include "Console.h"
#include "ControlState.h"
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

template <> rl::InputManager* Singleton<rl::InputManager>::msSingleton = 0;

namespace
{
    CEGUI::Key::Scan convert_to_cegui(const OIS::KeyCode keyCode)
    {
        switch (keyCode)
        {
        case KC_UNASSIGNED:
            return CEGUI::Key::Unknown;
        case KC_ESCAPE:
            return CEGUI::Key::Escape;
        case KC_1:
            return CEGUI::Key::One;
        case KC_2:
            return CEGUI::Key::Two;
        case KC_3:
            return CEGUI::Key::Three;
        case KC_4:
            return CEGUI::Key::Four;
        case KC_5:
            return CEGUI::Key::Five;
        case KC_6:
            return CEGUI::Key::Six;
        case KC_7:
            return CEGUI::Key::Seven;
        case KC_8:
            return CEGUI::Key::Eight;
        case KC_9:
            return CEGUI::Key::Nine;
        case KC_0:
            return CEGUI::Key::Zero;
        case KC_MINUS:
            return CEGUI::Key::Minus;
        case KC_EQUALS:
            return CEGUI::Key::Equals;
        case KC_BACK:
            return CEGUI::Key::Backspace;
        case KC_TAB:
            return CEGUI::Key::Tab;
        case KC_Q:
            return CEGUI::Key::Q;
        case KC_W:
            return CEGUI::Key::W;
        case KC_E:
            return CEGUI::Key::E;
        case KC_R:
            return CEGUI::Key::R;
        case KC_T:
            return CEGUI::Key::T;
        case KC_Y:
            return CEGUI::Key::Y;
        case KC_U:
            return CEGUI::Key::U;
        case KC_I:
            return CEGUI::Key::I;
        case KC_O:
            return CEGUI::Key::O;
        case KC_P:
            return CEGUI::Key::P;
        case KC_LBRACKET:
            return CEGUI::Key::LeftBracket;
        case KC_RBRACKET:
            return CEGUI::Key::RightBracket;
        case KC_RETURN:
            return CEGUI::Key::Return;
        case KC_LCONTROL:
            return CEGUI::Key::LeftControl;
        case KC_A:
            return CEGUI::Key::A;
        case KC_S:
            return CEGUI::Key::S;
        case KC_D:
            return CEGUI::Key::D;
        case KC_F:
            return CEGUI::Key::F;
        case KC_G:
            return CEGUI::Key::G;
        case KC_H:
            return CEGUI::Key::H;
        case KC_J:
            return CEGUI::Key::J;
        case KC_K:
            return CEGUI::Key::K;
        case KC_L:
            return CEGUI::Key::L;
        case KC_SEMICOLON:
            return CEGUI::Key::Semicolon;
        case KC_APOSTROPHE:
            return CEGUI::Key::Apostrophe;
        case KC_GRAVE:
            return CEGUI::Key::Grave;
        case KC_LSHIFT:
            return CEGUI::Key::LeftShift;
        case KC_BACKSLASH:
            return CEGUI::Key::Backslash;
        case KC_Z:
            return CEGUI::Key::Z;
        case KC_X:
            return CEGUI::Key::X;
        case KC_C:
            return CEGUI::Key::C;
        case KC_V:
            return CEGUI::Key::V;
        case KC_B:
            return CEGUI::Key::B;
        case KC_N:
            return CEGUI::Key::N;
        case KC_M:
            return CEGUI::Key::M;
        case KC_COMMA:
            return CEGUI::Key::Comma;
        case KC_PERIOD:
            return CEGUI::Key::Period;
        case KC_SLASH:
            return CEGUI::Key::Slash;
        case KC_RSHIFT:
            return CEGUI::Key::RightShift;
        case KC_MULTIPLY:
            return CEGUI::Key::Multiply;
        case KC_LMENU:
            return CEGUI::Key::LeftAlt;
        case KC_SPACE:
            return CEGUI::Key::Space;
        case KC_CAPITAL:
            return CEGUI::Key::Capital;
        case KC_F1:
            return CEGUI::Key::F1;
        case KC_F2:
            return CEGUI::Key::F2;
        case KC_F3:
            return CEGUI::Key::F3;
        case KC_F4:
            return CEGUI::Key::F4;
        case KC_F5:
            return CEGUI::Key::F5;
        case KC_F6:
            return CEGUI::Key::F6;
        case KC_F7:
            return CEGUI::Key::F7;
        case KC_F8:
            return CEGUI::Key::F8;
        case KC_F9:
            return CEGUI::Key::F9;
        case KC_F10:
            return CEGUI::Key::F10;
        case KC_NUMLOCK:
            return CEGUI::Key::NumLock;
        case KC_SCROLL:
            return CEGUI::Key::ScrollLock;
        case KC_NUMPAD7:
            return CEGUI::Key::Numpad7;
        case KC_NUMPAD8:
            return CEGUI::Key::Numpad8;
        case KC_NUMPAD9:
            return CEGUI::Key::Numpad9;
        case KC_SUBTRACT:
            return CEGUI::Key::Subtract;
        case KC_NUMPAD4:
            return CEGUI::Key::Numpad4;
        case KC_NUMPAD5:
            return CEGUI::Key::Numpad5;
        case KC_NUMPAD6:
            return CEGUI::Key::Numpad6;
        case KC_ADD:
            return CEGUI::Key::Add;
        case KC_NUMPAD1:
            return CEGUI::Key::Numpad1;
        case KC_NUMPAD2:
            return CEGUI::Key::Numpad2;
        case KC_NUMPAD3:
            return CEGUI::Key::Numpad3;
        case KC_NUMPAD0:
            return CEGUI::Key::Numpad0;
        case KC_DECIMAL:
            return CEGUI::Key::Decimal;
        case KC_OEM_102:
            return CEGUI::Key::OEM_102;
        case KC_F11:
            return CEGUI::Key::F11;
        case KC_F12:
            return CEGUI::Key::F12;
        case KC_F13:
            return CEGUI::Key::F13;
        case KC_F14:
            return CEGUI::Key::F14;
        case KC_F15:
            return CEGUI::Key::F15;
        case KC_KANA:
            return CEGUI::Key::Kana;
        case KC_ABNT_C1:
            return CEGUI::Key::ABNT_C1;
        case KC_CONVERT:
            return CEGUI::Key::Convert;
        case KC_NOCONVERT:
            return CEGUI::Key::NoConvert;
        case KC_YEN:
            return CEGUI::Key::Yen;
        case KC_ABNT_C2:
            return CEGUI::Key::ABNT_C2;
        case KC_NUMPADEQUALS:
            return CEGUI::Key::NumpadEquals;
        case KC_PREVTRACK:
            return CEGUI::Key::PrevTrack;
        case KC_AT:
            return CEGUI::Key::At;
        case KC_COLON:
            return CEGUI::Key::Colon;
        case KC_UNDERLINE:
            return CEGUI::Key::Underline;
        case KC_KANJI:
            return CEGUI::Key::Kanji;
        case KC_STOP:
            return CEGUI::Key::Stop;
        case KC_AX:
            return CEGUI::Key::AX;
        case KC_UNLABELED:
            return CEGUI::Key::Unlabeled;
        case KC_NEXTTRACK:
            return CEGUI::Key::NextTrack;
        case KC_NUMPADENTER:
            return CEGUI::Key::NumpadEnter;
        case KC_RCONTROL:
            return CEGUI::Key::RightControl;
        case KC_MUTE:
            return CEGUI::Key::Mute;
        case KC_CALCULATOR:
            return CEGUI::Key::Calculator;
        case KC_PLAYPAUSE:
            return CEGUI::Key::PlayPause;
        case KC_MEDIASTOP:
            return CEGUI::Key::MediaStop;
        case KC_VOLUMEDOWN:
            return CEGUI::Key::VolumeDown;
        case KC_VOLUMEUP:
            return CEGUI::Key::VolumeUp;
        case KC_WEBHOME:
            return CEGUI::Key::WebHome;
        case KC_NUMPADCOMMA:
            return CEGUI::Key::NumpadComma;
        case KC_DIVIDE:
            return CEGUI::Key::Divide;
        case KC_SYSRQ:
            return CEGUI::Key::SysRq;
        case KC_RMENU:
            return CEGUI::Key::RightAlt;
        case KC_PAUSE:
            return CEGUI::Key::Pause;
        case KC_HOME:
            return CEGUI::Key::Home;
        case KC_UP:
            return CEGUI::Key::ArrowUp;
        case KC_PGUP:
            return CEGUI::Key::PageUp;
        case KC_LEFT:
            return CEGUI::Key::ArrowLeft;
        case KC_RIGHT:
            return CEGUI::Key::ArrowRight;
        case KC_END:
            return CEGUI::Key::End;
        case KC_DOWN:
            return CEGUI::Key::ArrowDown;
        case KC_PGDOWN:
            return CEGUI::Key::PageDown;
        case KC_INSERT:
            return CEGUI::Key::Insert;
        case KC_DELETE:
            return CEGUI::Key::Delete;
        case KC_LWIN:
            return CEGUI::Key::LeftWindows;
        case KC_RWIN:
            return CEGUI::Key::RightWindows;
        case KC_APPS:
            return CEGUI::Key::AppMenu;
        case KC_POWER:
            return CEGUI::Key::Power;
        case KC_SLEEP:
            return CEGUI::Key::Sleep;
        case KC_WAKE:
            return CEGUI::Key::Wake;
        case KC_WEBSEARCH:
            return CEGUI::Key::WebSearch;
        case KC_WEBFAVORITES:
            return CEGUI::Key::WebFavorites;
        case KC_WEBREFRESH:
            return CEGUI::Key::WebRefresh;
        case KC_WEBSTOP:
            return CEGUI::Key::WebStop;
        case KC_WEBFORWARD:
            return CEGUI::Key::WebForward;
        case KC_WEBBACK:
            return CEGUI::Key::WebBack;
        case KC_MYCOMPUTER:
            return CEGUI::Key::MyComputer;
        case KC_MAIL:
            return CEGUI::Key::Mail;
        case KC_MEDIASELECT:
            return CEGUI::Key::MediaSelect;
        }
        return CEGUI::Key::Unknown;
    }
}

namespace rl
{

    InputManager::InputManager(Ogre::RenderWindow* win)
        : GameTask(false)
        , mKeyMapNormal()
        , mKeyMapShift()
        , mKeyMapAlt()
        , mKeyNames()
        , mCommandMapper(NULL)
        , mInputManager(NULL)
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
        for (ControlStateVector::iterator it = mFinishedControlStates.begin(); it != mFinishedControlStates.end(); ++it)
        {
            delete *it;
        }
        mFinishedControlStates.clear();

        if (mInputManager)
        {
            mInputManager->destroyInputObject(mMouse);
            mInputManager->destroyInputObject(mKeyboard);
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
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX & defined _DEBUG
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
        for (ControlStateVector::iterator it = mFinishedControlStates.begin(); it != mFinishedControlStates.end(); ++it)
        {
            delete *it;
        }
        mFinishedControlStates.clear();

        if (!mControlStates.empty())
        {
            mControlStates.top()->run(elapsedTime);
        }
    }

    bool InputManager::isMouseButtonDown(OIS::MouseButtonID buttonID)
    {
        OIS::MouseState ms = mMouse->getMouseState();
        return ms.buttonDown(buttonID);
    }

    bool InputManager::isKeyDown(OIS::KeyCode key)
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
            name = "Alt+" + name;
        if (syskeys & CTRL_MASK)
            name = "Ctrl+" + name;
        if (syskeys & SHIFT_MASK)
            name = "Shift+" + name;
        if (syskeys & SUPER_MASK)
            name = "Super+" + name;
        return name;
    }

    int InputManager::getScanCode(const CeGuiString& name)
    {
        for (KeyNameMap::iterator it = mKeyNames.begin(); it != mKeyNames.end(); it++)
        {
            if ((*it).second == name)
                return (*it).first;
        }

        Ogre::String msg = Ogre::String("Key ") + name.c_str() + " not found.";
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

        if (mKeyboard->isModifierDown(OIS::Keyboard::Alt))
            rval |= ALT_MASK;
        if (mKeyboard->isModifierDown(OIS::Keyboard::Ctrl))
            rval |= CTRL_MASK;
        if (mKeyboard->isModifierDown(OIS::Keyboard::Shift))
            rval |= SHIFT_MASK;

        return rval;
    }

    void InputManager::loadKeyMapping(const Ogre::String& filename)
    {
        using std::make_pair;

        tinyxml2::XMLDocument* doc = loadDocument(filename);
        tinyxml2::XMLElement* dataDocumentContent = doc->RootElement();

        XmlElementList keymaps = getElementsByTagName(dataDocumentContent, "Key");
        for (unsigned int idx = 0; idx < keymaps.size(); idx++)
        {
            const tinyxml2::XMLElement* key = keymaps[idx];
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

    void InputManager::linkKeyToRubyCommand(const CeGuiString& keyStr, const CeGuiString& command)
    {
        std::ostringstream ss;

        ss << "InputManager::linkKeyToRubyCommand called: ";
        int key = getScanCode(keyStr);
        if (command.length() == 0) // delete
        {
            KeyCommandMap::iterator it = mKeyRubyCommand.find(key);
            ss << "Requesting to delete link from Key '" << keyStr << "'... ";
            if (it != mKeyRubyCommand.end())
            {
                ss << "Link to command '" << it->second << "' deleted.";
                mKeyRubyCommand.erase(it);
            }
        }
        else
        {
            KeyCommandMap::iterator it = mKeyRubyCommand.find(key);
            if (it == mKeyRubyCommand.end())
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
        if (WindowManager::getSingleton().getWindowInputMask() & AbstractWindow::WIT_MOUSE_INPUT)
        {
            if (CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(
                    static_cast<CEGUI::MouseButton>(id)))
                retval = true;
        }

        if (!mControlStates.empty())
            if (mControlStates.top()->mousePressed(evt, id, retval))
                retval = true;
        return true;
        // return retval;
    }

    bool InputManager::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {
        bool retval = false;
        if (WindowManager::getSingleton().getWindowInputMask() & AbstractWindow::WIT_MOUSE_INPUT)
        {
            if (CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(
                    static_cast<CEGUI::MouseButton>(id)))
                retval = true;
        }

        if (!mControlStates.empty())
            if (mControlStates.top()->mouseReleased(evt, id, retval))
                retval = true;
        return true;
        // return retval;
    }

    bool InputManager::mouseMoved(const OIS::MouseEvent& evt)
    {
        bool retval = false;
        if (WindowManager::getSingleton().getWindowInputMask() & AbstractWindow::WIT_MOUSE_INPUT)
        {
            if (CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(evt.state.X.rel, evt.state.Y.rel))
                retval = true;
        }

        if (!mControlStates.empty())
            if (mControlStates.top()->mouseMoved(evt, retval))
                retval = true;
        return true;
        // return retval;
    }

    // job definition for key repeating
    // this job is created when a key was pressed and the processing window wants the key to be repeated
    class KeyRepeatJob : public Job
    {
    public:
        static void createKeyRepeatJob(AbstractWindow* window, OIS::KeyCode key)
        {
            // nur neu anlegen, wenns noch nicht existiert!
            if (mKeyJobMap.find(key) == mKeyJobMap.end())
            {
                KeyRepeatJob* job = new KeyRepeatJob(window, key);
                JobScheduler::getSingleton().addJob(job, JobScheduler::JP_NORMAL, 0.5);
            }
        }
        bool execute(Ogre::Real t)
        {
            bool handled = false;
            rl::Time time
                = TimeSourceManager::getSingleton().getTimeSource(TimeSource::REALTIME_CONTINUOUS)->getClock();
            if (WindowManager::getSingleton().getActiveWindow() == mWindow && // perhaps window was deleted!!
                InputManager::getSingleton().isKeyDown(OIS::KeyCode(mKey)))
            {
                if (time - mLastTime > 50 * mCount)
                {
                    if (CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(convert_to_cegui(mKey)))
                        handled = true;
                    else
                    {
                        static const CEGUI::utf8 NO_CHAR = 0;
                        if (InputManager::getSingleton().getKeyChar(
                                mKey, InputManager::getSingleton().getModifierCode())
                            != NO_CHAR)
                        {
                            if (CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(
                                    InputManager::getSingleton().getKeyChar(
                                        mKey, InputManager::getSingleton().getModifierCode())))
                                handled = true;
                        }
                    }
                    if (CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(convert_to_cegui(mKey)))
                        handled = true;

                    mLastTime = time;
                }

                return false;
            }

            mKeyJobMap.erase(mKey);
            if (mCount > 0)
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

        KeyRepeatJob(AbstractWindow* window, OIS::KeyCode key)
            : Job(false, true)
            , mWindow(window)
            , mKey(key)
            , mLastTime(0)
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
        if (WindowManager::getSingleton().getWindowInputMask() & AbstractWindow::WIT_KEYBOARD_INPUT)
        {
            AbstractWindow* activeWin = WindowManager::getSingleton().getActiveWindow();
            if (activeWin != NULL)
            {
                if (activeWin->wantsKeyToRepeat(evt.key))
                {
                    KeyRepeatJob::createKeyRepeatJob(activeWin, evt.key);
                }
            }

            if (CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(convert_to_cegui(evt.key)))
                retval = true;
            else
            {
                static const CEGUI::utf8 NO_CHAR = 0;
                if (getKeyChar(evt.key, getModifierCode()) != NO_CHAR)
                {
                    if (CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(
                            getKeyChar(evt.key, getModifierCode())))
                        retval = true;
                }
            }
        }

        if (!mControlStates.empty())
            if (mControlStates.top()->keyPressed(evt, retval))
                retval = true;

        if (!retval)
        {
            KeyCommandMap::iterator it = mKeyRubyCommand.find(evt.key);
            if (it != mKeyRubyCommand.end())
                CoreSubsystem::getSingleton().getRubyInterpreter()->execute(it->second.c_str());
        }

        return true;
        // return retval;
    }

    bool InputManager::keyReleased(const OIS::KeyEvent& evt)
    {
        bool retval = false;
        if (WindowManager::getSingleton().getWindowInputMask() & AbstractWindow::WIT_KEYBOARD_INPUT)
        {
            if (CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(convert_to_cegui(evt.key)))
                retval = true;
        }

        if (!mControlStates.empty())
            if (mControlStates.top()->keyReleased(evt, retval))
                retval = true;
        return true;
        // return false;
    }
}
