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

#ifndef __InputManager_H__
#define __InputManager_H__

#include "UiPrerequisites.h"

#ifdef __APPLE__
#include <OIS/OISJoyStick.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>
#else
#include <OISJoyStick.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#endif

#include <stack>

#include "FixRubyHeaders.h"
#include "GameTask.h"
#include "XmlProcessor.h"

namespace rl
{

    class AbstractWindow;
    class ControlState;
    class CommandMapper;
    class Dialog;
    class GameObject;

    class _RlUiExport InputManager : public Ogre::Singleton<InputManager>,
                                     public GameTask,
                                     private XmlProcessor,
                                     public OIS::KeyListener,
                                     public OIS::MouseListener
    {
    public:
        InputManager(Ogre::RenderWindow* window);
        ~InputManager();

        bool isMouseButtonDown(OIS::MouseButtonID buttonID);
        bool isKeyDown(OIS::KeyCode key);

        Ogre::Real getMouseRelativeX(void) const;
        Ogre::Real getMouseRelativeY(void) const;
        Ogre::Real getMouseRelativeZ(void) const;

        void run(Ogre::Real elapsedTime);

        CeGuiString getKeyName(int scancode, int syskeys);
        CeGuiString getKeyName(int combinedKeyCode);
        int getScanCode(const CeGuiString& name);
        int getSystemCode(const CeGuiString& name);
        void loadKeyMapping(const Ogre::String& filename);
        void buildCommandMapping(const Ogre::NameValuePairList& keylist);
        typedef std::map<int, CeGuiString::value_type> KeyCharMap;
        typedef std::map<int, CeGuiString> KeyNameMap;

        virtual const Ogre::String& getName() const;

        void linkKeyToRubyCommand(const CeGuiString& key, const CeGuiString& command);

        // State management

        /// Empties the control state stack and sets the control state to the requested state.
        void setControlState(ControlStateType);
        /// Put a new state onto the stack, this state is used for input handling from then on.
        void pushControlState(ControlStateType);
        /// Remove the current state from the stack and give control back to the former state.
        void popControlState();
        /// Remove all control states.
        void clearControlStates();

        ControlState* getControlState() const;

        int getModifierCode() const;
        const CEGUI::utf8& getKeyChar(int keycode, int modifiers) const;

        virtual bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
        virtual bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
        virtual bool mouseMoved(const OIS::MouseEvent& evt);
        virtual bool keyPressed(const OIS::KeyEvent& evt);
        virtual bool keyReleased(const OIS::KeyEvent& evt);

    private:
        enum
        {
            NUM_MOUSE_BUTTON = 4,
            NUM_KEYS = 256
        };
        enum Modifiers
        {
            ALT_MASK = 1,
            CTRL_MASK = 2,
            SHIFT_MASK = 4,
            SUPER_MASK = 8
        };

        void initializeOis(Ogre::RenderWindow* wnd);
        bool startDialog(Dialog* dialog);

        Ogre::Vector3 mSavedMouseState;

        OIS::Mouse* mMouse;
        OIS::Keyboard* mKeyboard;
        OIS::InputManager* mInputManager;

        unsigned short mScreenX;
        unsigned short mScreenY;

        CommandMapper* mCommandMapper;

        KeyCharMap mKeyMapNormal;
        KeyCharMap mKeyMapShift;
        KeyCharMap mKeyMapAlt;
        KeyNameMap mKeyNames;

        typedef std::map<int, CeGuiString> KeyCommandMap;
        KeyCommandMap mKeyRubyCommand;

        typedef std::stack<ControlState*> ControlStateStack;
        typedef std::vector<ControlState*> ControlStateVector;
        ControlStateStack mControlStates;
        /// Stored for delay deletion, since else a state can be deleted while
        /// still in its run() function.
        ControlStateVector mFinishedControlStates;
    };
}

#endif
