#include "DebugWindow.h"
#include "InputManager.h"

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

	DebugWindow::DebugWindow() : CeGuiWindow("debugwindow.xml")
	{
		mText = getStaticText("DebugWindow/Text");
		mText->moveToFront();
		
		addToRoot(mWindow);

		Root::getSingleton().addFrameListener(this);
	}

	DebugWindow::~DebugWindow()
	{
		Root::getSingleton().removeFrameListener(this);
    }

    void DebugWindow::setText(const Ogre::String& text)
    {
        CEGUI::String o(text.c_str());
        mText->setText(o);
    }
    
	bool DebugWindow::isVisible() const
    {
        return mWindow->isVisible();
    }

	void DebugWindow::keyReleased(KeyEvent* e)
	{
		if(e->getKey() == KC_F5 )
		{
			if (isVisible())
				hide();
			else
				show();
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
		Window
			*textStats = getWindow("DebugWindow/Statistics");

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
