#include <boost/bind.hpp>

#include <CEGUI.h>

#include "MainMenuWindow.h"

#include "UiSubsystem.h"
#include "CoreSubsystem.h"
#include "WindowManager.h"

using namespace CEGUI;

namespace rl {

	MainMenuWindow::MainMenuWindow() :
		CeGuiWindow("mainmenuwindow.xml", WND_KEYBOARD_INPUT),
		mActiveModule("")
	{
		getWindow("MainMenuWindow/Modules")->subscribeEvent(
			Window::EventMouseClick, 
			boost::bind(&MainMenuWindow::handleChooseModules, this));

		getWindow("MainMenuWindow/Start")->subscribeEvent(
			Window::EventMouseClick, 
			boost::bind(&MainMenuWindow::handleStart, this));
		getWindow("MainMenuWindow/Start")->subscribeEvent(
			Window::EventMouseClick, 
			boost::bind(&WindowManager::destroyWindow, WindowManager::getSingletonPtr(), this));

		getWindow("MainMenuWindow/GraphicOptions")->subscribeEvent(
			Window::EventMouseClick, 
			boost::bind(&MainMenuWindow::handleGraphicOptions, this));

		getWindow("MainMenuWindow/Quit")->subscribeEvent(
			Window::EventMouseClick, 
			boost::bind(&WindowManager::destroyWindow, WindowManager::getSingletonPtr(), this));
		getWindow("MainMenuWindow/Quit")->subscribeEvent(
			Window::EventMouseClick, 
			boost::bind(&MainMenuWindow::handleQuit, this));
		
		centerWindow();
		addToRoot(mWindow);		
	}

	bool MainMenuWindow::handleChooseModules()
	{
		MainMenuChooseModulesWindow* w = 
			new MainMenuChooseModulesWindow(
				this, 
				CoreSubsystem::getSingleton().getActivatableModules(), 
				mActiveModule);
		w->setVisible(true);
		return true;
	}

	bool MainMenuWindow::handleStart()
	{
		if (mActiveModule.length() == 0)
			mActiveModule = *(CoreSubsystem::getSingleton().getActivatableModules().begin());

		CoreSubsystem::getSingleton().setActiveModule(mActiveModule.c_str());

		return true;
	}

	bool MainMenuWindow::handleQuit()
	{
		UiSubsystem::getSingleton().requestExit();
		return true;
	}

	bool MainMenuWindow::handleGraphicOptions()
	{
		return true;
	}

	void MainMenuWindow::setActiveModule(const CeGuiString& module)
	{
		mActiveModule = module;
	}

	MainMenuChooseModulesWindow::MainMenuChooseModulesWindow(
		MainMenuWindow* parent, 
		const Ogre::StringVector& modules, 
		const CeGuiString& activeModule) 
		: CeGuiWindow("mainmenuchoosemoduleswindow.xml", WND_KEYBOARD_INPUT)
	{
		mModulesList = getListbox("MainMenuChooseModules/ModulesList");
		mModulesList->setMultiselectEnabled(false);
		mModulesList->setEnabled(true);
		mParent = parent;

		for (Ogre::StringVector::const_iterator mod = modules.begin();
			mod != modules.end(); mod++)
		{
			ListboxTextItem* it = new ListboxTextItem(*mod);
			mModulesList->addItem(it);
			if ((*mod).compare(activeModule.c_str()) == 0)
				mModulesList->setItemSelectState(it, true);
		}

		getWindow("MainMenuChooseModules/OkayButton")->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&MainMenuChooseModulesWindow::handleChooseOkay, this));
		getWindow("MainMenuChooseModules/OkayButton")->subscribeEvent(
			Window::EventMouseClick, 
			boost::bind(&WindowManager::destroyWindow, WindowManager::getSingletonPtr(), this));
		getWindow("MainMenuChooseModules/CancelButton")->subscribeEvent(
			Window::EventMouseClick, 
			boost::bind(&WindowManager::destroyWindow, WindowManager::getSingletonPtr(), this));

		centerWindow();
		addToRoot(mWindow);		
	}

	bool MainMenuChooseModulesWindow::handleChooseOkay()
	{
		ListboxItem* item = mModulesList->getFirstSelectedItem();
		if (item != NULL)
			mParent->setActiveModule(item->getText());
		return true;
	}
}
