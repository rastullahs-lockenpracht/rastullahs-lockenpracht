/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#ifndef __InputManager_H__
#define __InputManager_H__

#include <set>
#include <OgreEventQueue.h>
#include <OgreFrameListener.h>
#include <OgreEventListeners.h>
#include <OgreInput.h>

#include "UiPrerequisites.h"
#include "FixRubyHeaders.h"
#include "SynchronizedTask.h"

#include <CEGUI.h>

namespace CEGUI {
	enum MouseButton;
}

namespace rl {

	class CeGuiWindow;
	class GameObject;

	class _RlUiExport InputManager : public SynchronizedTask, public KeyListener, public MouseListener, public MouseMotionListener, public Singleton<InputManager>
	{
		public:
			InputManager(void);
			~InputManager();

			static InputManager & getSingleton(void);
			static InputManager * getSingletonPtr(void);

			bool isKeyDown(KeyCode kc) { return mKeyDown[kc]; };
			bool isMouseButtonDown(int iButtonID) { return mInputReader->getMouseButton(iButtonID); };

			Ogre::Real getMouseRelativeX(void) { return mInputReader->getMouseRelativeX(); };
			Ogre::Real getMouseRelativeY(void) { return mInputReader->getMouseRelativeY(); };
			Ogre::Real getMouseRelativeZ(void) { return mInputReader->getMouseRelativeZ(); };

			void addKeyListener(KeyListener *l);
			void removeKeyListener(KeyListener *l);

			void addMouseListener(MouseListener *l);
			void removeMouseListener(MouseListener *l);

			void addMouseMotionListener(MouseMotionListener *l);
			void removeMouseMotionListener(MouseMotionListener *l);

			void run(Real elapsedTime);
			
			void registerCeguiWindow(CeGuiWindow* window);
			void unregisterCeguiWindow(CeGuiWindow* window);
			bool isCeguiActive();

			void setObjectPickingActive(bool active);
			GameObject* getPickedObject();
			void updatePickedObject(float mouseRelX, float mouseRelY);

			void activateTargetQuery();

			void mouseClicked(MouseEvent* e);
			void mouseEntered(MouseEvent* e);
			void mouseExited(MouseEvent* e);
			void mousePressed(MouseEvent* e);
			void mouseReleased(MouseEvent* e);
			void mouseMoved(MouseEvent* e);
			void mouseDragged(MouseEvent* e);
			void keyPressed(KeyEvent* e);
			void keyReleased(KeyEvent* e);
			void keyClicked(KeyEvent* e);

			void loadKeyMapping(const Ogre::String& filename);
			typedef std::map<int, CEGUI::utf8> KeyCharMap;
			typedef std::map<int, CeGuiString> KeyNameMap;

		private:
			Ogre::InputReader* mInputReader;
			Ogre::EventQueue mEventQueue; 
			Ogre::EventProcessor* mEventProcessor;

			enum { NUM_MOUSE_BUTTON=16, NUM_KEYS=256 };

			bool mKeyDown[NUM_KEYS];

			std::set<KeyListener*> mKeyListeners;
			std::set<MouseListener*> mMouseListeners;
			std::set<MouseMotionListener*> mMouseMotionListeners;
			std::set<CeGuiWindow*> mActiveWindows;

			unsigned short mScreenX;
			unsigned short mScreenY;
			bool mBuffered, mEventInitialized, mInputInitialized;			

			CEGUI::MouseButton convertOgreButtonToCegui(int ogre_button_id);

        	bool processGlobalKeyEvent(KeyEvent* e);
			bool sendKeyToCeGui(KeyEvent* e);

			void switchMouseToUnbuffered();
			void switchMouseToBuffered();

			CEGUI::utf32 getKeyChar(Ogre::KeyEvent* ke);			

			bool mPickObjects;
			GameObject* mTargetedObject;

			KeyCharMap mKeyMapNormal;
			KeyCharMap mKeyMapShift;
			KeyCharMap mKeyMapAlt;
			KeyNameMap mKeyNames;
	};

}

#endif
