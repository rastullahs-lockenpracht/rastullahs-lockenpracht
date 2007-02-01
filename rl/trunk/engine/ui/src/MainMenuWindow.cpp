/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include <boost/bind.hpp>

#include "UiPrerequisites.h"

#include "WindowFactory.h"
#include "CoreSubsystem.h"
#include "ConfigurationManager.h"
#include "ContentModule.h"

#include "MainMenuEngineWindow.h"
#include "MainMenuWindow.h"
#include "SoundConfig.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl {

	MainMenuWindow::MainMenuWindow(MainMenuEngineWindow* enginewindow) :
		CeGuiWindow("mainmenuwindow.xml", WND_MOUSE_INPUT, false, false),
		mActiveModule(NULL),
		mEngineWindow(enginewindow)
	{
		getWindow("MainMenu/Game/Start")->subscribeEvent(
			MenuItem::EventClicked, 
			boost::bind(&MainMenuWindow::handleStart, this));

		getWindow("MainMenu/Game/Quit")->subscribeEvent(
			MenuItem::EventClicked, 
			boost::bind(&MainMenuWindow::handleQuit, this));
            
        getWindow("MainMenu/Options/Sound")->subscribeEvent(
            MenuItem::EventClicked,
            boost::bind(&MainMenuWindow::handleSoundOptions, this));
		
		fillModules();

        mWindow->moveToBack();
	}

    MainMenuWindow::~MainMenuWindow()
    {
        delete mEngineWindow;
    }

	void MainMenuWindow::fillModules()
	{
		MenuBase* modulesMenu = getMenu("MainMenu/Modules/Menu");

		ModuleMap modules = CoreSubsystem::getSingleton().getAllModules();
		mActiveModule = CoreSubsystem::getSingleton().getActiveAdventureModule();

		for(ModuleMap::iterator modIt = modules.begin();	
			modIt != modules.end(); modIt++)
		{
			ContentModule* mod = (*modIt).second;

			if (!mod->isCommon())
			{
				if (mActiveModule == NULL)
					mActiveModule = mod;

				MenuItem* it = static_cast<MenuItem*>(
					CEGUI::WindowManager::getSingleton().createWindow("RastullahLook/MenuItem", 
					getNamePrefix()+"MainMenu/Modules/" + mod->getId()));
				
				if (mod == mActiveModule)
					it->setText(mod->getName() + " *");
				else
					it->setText(mod->getName());
				modulesMenu->addItem(it);
				
				it->subscribeEvent(
					MenuItem::EventClicked,
					boost::bind(&MainMenuWindow::handleChooseModule, this, it, mod));
			}
		}
	}

	bool MainMenuWindow::handleChooseModule(MenuItem* it, ContentModule* module)
	{
		MenuBase* modulesMenu = getMenu("MainMenu/Modules/Menu");
		
		ItemEntry* itOld = NULL;
		for (size_t i=0; i<modulesMenu->getItemCount(); i++)
		{
			ItemEntry* curr = modulesMenu->getItemFromIndex(i);
			if (curr->getText().compare(mActiveModule->getName()+" *") == 0)
			{
				itOld = curr;
				break;
			}
		}
		itOld->setText(mActiveModule->getName());

		mActiveModule = module;
		it->setText(module->getName()+" *");	

		return true;
	}

	bool MainMenuWindow::handleStart()
	{
		setVisible(false);
		destroyWindow();
		CoreSubsystem::getSingleton().startAdventureModule(mActiveModule);

        this->setVisible( false );
		return true;
	}

	bool MainMenuWindow::handleQuit()
	{
		WindowFactory::getSingleton().showExitConfirmation();
		return true;
	}

	bool MainMenuWindow::handleGraphicOptions()
	{
		return true;
	}

	void MainMenuWindow::setVisible(bool visible, bool destroyAfterHide)
	{
		CeGuiWindow::setVisible(visible, destroyAfterHide);
		mEngineWindow->setVisible(visible, destroyAfterHide);
	}
	
	bool MainMenuWindow::handleSoundOptions()
	{
		WindowFactory::getSingleton().showSoundConfig();
		return true;
	}

	
}
