#ifndef __InputManager_H__
#define __InputManager_H__

#include <list>
#include <OgreEventQueue.h>
#include <OgreFrameListener.h>
#include <OgreEventListeners.h>
#include <OgreInput.h>

#include "UiPrerequisites.h"
#include "SynchronizedTask.h"

#include <CEGUI.h>

namespace CEGUI {
	enum MouseButton;
}

namespace rl {

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
			
			void registerCeguiWindow();
			void unregisterCeguiWindow();
			bool isCeguiActive();

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

		private:
			Ogre::InputReader* mInputReader;
			Ogre::EventQueue mEventQueue; 
			Ogre::EventProcessor* mEventProcessor;

			enum { NUM_MOUSE_BUTTON=16, NUM_KEYS=256 };

			bool mKeyDown[NUM_KEYS];

			std::list<KeyListener*> mKeyListenerList;
			std::list<MouseListener*> mMouseListenerList;
			std::list<MouseMotionListener*> mMouseMotionListenerList;

			unsigned short mScreenX;
			unsigned short mScreenY;
			unsigned short mOpenCeguiWindows;
			bool mBuffered, mEventInitialized, mInputInitialized;			

			CEGUI::MouseButton convertOgreButtonToCegui(int ogre_button_id);

			void switchMouseToUnbuffered();
			void switchMouseToBuffered();

			char getKeyChar(Ogre::KeyEvent* ke);
	};

}

#endif
