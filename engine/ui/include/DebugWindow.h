#ifndef __DEBUGWINDOW_H__
#define __DEBUGWINDOW_H__

#include "UiPrerequisites.h"
#include <OgreSingleton.h>
#include <OgreKeyEvent.h>

#include <CEGUI.h>

namespace rl {

    class _RlUiExport DebugWindow
		: public Ogre::Singleton<DebugWindow>, public Ogre::KeyListener, public Ogre::FrameListener
    {
    public:
        DebugWindow(void);
        ~DebugWindow();
        static DebugWindow& getSingleton(void);
        static DebugWindow* getSingletonPtr(void);

        void setText(const Ogre::String& output);

        bool isVisible(void) const;
        void setVisible(bool visible);

        void keyClicked(Ogre::KeyEvent* e) {};
        void keyPressed(Ogre::KeyEvent* e) {};
        void keyReleased(Ogre::KeyEvent* e);

		bool frameStarted(const Ogre::FrameEvent& evt);

    private:
		void updateFps();
        CEGUI::StaticText* mText;
		CEGUI::Window* mWindow;
    };
}

#endif
