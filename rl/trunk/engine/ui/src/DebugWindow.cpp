#include "DebugWindow.h"

template<> rl::DebugWindow* Ogre::Singleton<rl::DebugWindow>::ms_Singleton = 0;

using namespace CEGUI;

namespace rl 
{
  DebugWindow& DebugWindow::getSingleton(void)
    {
        return Ogre::Singleton<DebugWindow>::getSingleton();
    }
	DebugWindow* DebugWindow::getSingletonPtr(void)
    {
        return Ogre::Singleton<DebugWindow>::getSingletonPtr();
    }

	DebugWindow::DebugWindow()
	{
		mWindow = WindowManager::getSingleton().loadWindowLayout((utf8*)"debugwindow.xml");

		mText = reinterpret_cast<StaticText*>(
			WindowManager::getSingleton().getWindow(
			(utf8*)"DebugWindow/Text"));
		mText->moveToFront();
		
		Window* rootWindow = WindowManager::getSingleton().getWindow((utf8*)"root_wnd");
		rootWindow->addChildWindow(mWindow);

		Root::getSingleton().addFrameListener(this);
	}

	DebugWindow::~DebugWindow()
	{
		Root::getSingleton().removeFrameListener(this);
		WindowManager::getSingleton().destroyWindow(mWindow);
    }

    void DebugWindow::setText(const Ogre::String& text)
    {
        CEGUI::String o(text.c_str());
        mText->setText(o);
    }
    
    void DebugWindow::setVisible(bool visible)
	{
        if (visible)
        {
		    mWindow->show();
        }
        else
        {
            mWindow->hide();
        }
    }

    bool DebugWindow::isVisible() const
    {
        return mWindow->isVisible();
    }

	void DebugWindow::keyReleased(KeyEvent* e)
	{
		if(e->getKey() == KC_F5 )
		{
            setVisible(!isVisible());
		}
    }

	bool DebugWindow::frameStarted(const FrameEvent& evt)
	{
		updateFps();
		return true;
	}

	

	void DebugWindow::updateFps()
	{
		const RenderTarget::FrameStats& stats = Root::getSingleton().getAutoCreatedWindow()->getStatistics();
		const WindowManager& winman = WindowManager::getSingleton();

		Window
			*textStats = winman.getWindow((CEGUI::utf8*)"DebugWindow/Statistics");

		textStats->setText("Current FPS: " + 
			StringConverter::toString(stats.lastFPS)+
			"\nBest/worst/avg FPS: " + 
			StringConverter::toString(stats.bestFPS) + "/" + 
			StringConverter::toString(stats.worstFPS) + "/" + 
			StringConverter::toString(stats.avgFPS)+
			"\nBest/worst Frame times: " + 
			StringConverter::toString(stats.bestFPS) + "/" + 
			StringConverter::toString(stats.worstFPS)+
			"\nTriangle Count: " + 
			StringConverter::toString(stats.triangleCount));
	}
}
