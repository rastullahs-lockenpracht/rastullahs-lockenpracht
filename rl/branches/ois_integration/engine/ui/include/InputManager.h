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

#ifndef __InputManager_H__
#define __InputManager_H__

#include "UiPrerequisites.h"

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISJoyStick.h>
#include <OgreRenderWindow.h>
#include <OgreSingleton.h>

#include "FixRubyHeaders.h"
#include "GameTask.h"

#include <OgreNoMemoryMacros.h>
#include <CEGUI.h>
#include <OgreMemoryMacros.h>

namespace CEGUI {
	enum MouseButton;
}

namespace rl {

	class CeGuiWindow;
	class GameObject;
	class CharacterController;
    class CommandMapper;

    class _RlUiExport InputManager
        :	public Ogre::Singleton<InputManager>, 
            public GameTask,
            public OIS::KeyListener, 
            public OIS::MouseListener
    {
    public:
		InputManager(Ogre::RenderWindow* window);
        ~InputManager();

        static InputManager & getSingleton(void);
        static InputManager * getSingletonPtr(void);

        bool isKeyDown(Ogre::KeyCode kc);
        bool isMouseButtonDown(int iButtonID);

        Ogre::Real getMouseRelativeX(void) const;
        Ogre::Real getMouseRelativeY(void) const;
        Ogre::Real getMouseRelativeZ(void) const;

        void run(Ogre::Real elapsedTime);

        void registerCeGuiWindow(CeGuiWindow* window);
        void unregisterCeGuiWindow(CeGuiWindow* window);
        bool isCeguiActive() const;

        virtual bool mousePressed(const OIS::MouseEvent & arg, OIS::MouseButtonID id);
        virtual bool mouseReleased(const OIS::MouseEvent & arg, OIS::MouseButtonID id);
        virtual bool mouseMoved(const OIS::MouseEvent &arg);
		virtual bool keyPressed(const OIS::KeyEvent &arg);
		virtual bool keyReleased(const OIS::KeyEvent &arg);

        CeGuiString getKeyName(int scancode, int syskeys);
        CeGuiString getKeyName(int combinedKeyCode);
        int getScanCode(const CeGuiString& name);
        int getSystemCode(const CeGuiString& name);
        void loadKeyMapping(const Ogre::String& filename);
        void loadCommandMapping(const Ogre::String& filename);
        typedef std::map<int, CEGUI::utf8> KeyCharMap;
        typedef std::map<int, CeGuiString> KeyNameMap;

        void setCharacterController(CharacterController* controller);

        virtual const Ogre::String& getName() const;

    private:
        static const int TIME_SHOW_DESCRIPTION = 4000;
        enum { NUM_MOUSE_BUTTON=4, NUM_KEYS=256 };
        enum Modifiers {ALT_MASK = 1, CTRL_MASK = 2, SHIFT_MASK = 4, SUPER_MASK = 8};

        void initializeOis(Ogre::RenderWindow* wnd);
        bool sendKeyToCeGui(const OIS::KeyEvent& e) const;
        void resetPressedKeys( bool up );
        const int getModifierCode(const OIS::KeyEvent& e) const;

		Ogre::Vector3 mSavedMouseState;
        bool mKeyDown[NUM_KEYS];
		int mCurrentModifiers;

        OIS::Mouse* mMouse;
        OIS::Keyboard* mKeyboard;

        unsigned short mScreenX;
        unsigned short mScreenY;
        bool mBuffered, mEventInitialized, mInputInitialized;			

        CommandMapper* mCommandMapper;

        KeyCharMap mKeyMapNormal;
        KeyCharMap mKeyMapShift;
        KeyCharMap mKeyMapAlt;
        KeyNameMap mKeyNames;

        int mNumActiveWindowsMouseInput;
        int mNumActiveWindowsKeyboardInput;
        int mNumActiveWindowsAllInput;

        CharacterController* mCharacterController;
    };
}

#endif
