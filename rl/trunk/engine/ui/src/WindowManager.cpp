#include "WindowManager.h"
#include "CeGuiWindow.h"

template<> rl::WindowManager* Ogre::Singleton<rl::WindowManager>::ms_Singleton = 0;

namespace rl {

	WindowManager::WindowManager()
	{
		mWindows.clear();
	}

	bool WindowManager::destroyWindow(CeGuiWindow* window)
	{
		WindowSet::iterator iter = mWindows.find(window);
		if (iter == mWindows.end())
			return false;

		//mWindows.erase(iter);
		//TODO: Fenster löschen scheint nicht zu gehen
		//delete window; 
		window->setVisible(false);

		return true;
	}

	void WindowManager::registerWindow(CeGuiWindow* window)
	{
		mWindows.insert(window);
	}

	WindowManager& WindowManager::getSingleton()
	{
		return Singleton<WindowManager>::getSingleton();
	}

	WindowManager* WindowManager::getSingletonPtr()
	{
		return Singleton<WindowManager>::getSingletonPtr();
	}
}