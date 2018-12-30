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
#include "stdinc.h" //precompiled header

#include "MainMenuWindow.h"

#include <CEGUI/WindowManager.h>
#include <boost/bind.hpp>

#include "ConfigurationManager.h"
#include "ContentModule.h"
#include "CoreSubsystem.h"
#include "MainMenuEngineWindow.h"
#include "SoundConfig.h"
#include "WindowFactory.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl
{

    MainMenuWindow::MainMenuWindow(MainMenuEngineWindow* enginewindow)
        : AbstractWindow("mainmenuwindow.xml", WIT_MOUSE_INPUT, false, false)
        , mActiveModule(NULL)
        , mEngineWindow(enginewindow)
    {
        getWindow("MainMenu/Game/Start")
            ->subscribeEvent(MenuItem::EventClicked, boost::bind(&MainMenuWindow::handleStart, this));

        getWindow("MainMenu/Game/Load")
            ->subscribeEvent(MenuItem::EventClicked, boost::bind(&MainMenuWindow::handleLoad, this));

        getWindow("MainMenu/Game/Quit")
            ->subscribeEvent(MenuItem::EventClicked, boost::bind(&MainMenuWindow::handleQuit, this));

        getWindow("MainMenu/Options")
            ->subscribeEvent(MenuItem::EventClicked, boost::bind(&MainMenuWindow::handleSettings, this));

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

        for (ModuleMap::iterator modIt = modules.begin(); modIt != modules.end(); modIt++)
        {
            ContentModule* mod = (*modIt).second;

            if (!mod->isCommon())
            {
                if (mActiveModule == NULL)
                    mActiveModule = mod;

                MenuItem* it = static_cast<MenuItem*>(CEGUI::WindowManager::getSingleton().createWindow(
                    "RastullahLook/MenuItem", getNamePrefix() + "MainMenu/Modules/" + mod->getId()));

                if (mod == mActiveModule)
                    it->setText(mod->getName() + " *");
                else
                    it->setText(mod->getName());
                modulesMenu->addItem(it);

                it->subscribeEvent(
                    MenuItem::EventClicked, boost::bind(&MainMenuWindow::handleChooseModule, this, it, mod));
            }
        }
    }

    bool MainMenuWindow::handleChooseModule(MenuItem* it, ContentModule* module)
    {
        MenuBase* modulesMenu = getMenu("MainMenu/Modules/Menu");

        ItemEntry* itOld = NULL;
        CeGuiString activeName;
        activeName = mActiveModule->getName();
        for (size_t i = 0; i < modulesMenu->getItemCount(); i++)
        {
            ItemEntry* curr = modulesMenu->getItemFromIndex(i);
            CeGuiString currName;
            currName = curr->getText();
            if (currName.compare(activeName + " *") == 0)
            {
                itOld = curr;
                break;
            }
        }
        if (!itOld)
        {
            LOG_ERROR(Logger::UI, "Could not determine last active MenuEntry in MainMenuWindow::handleChooseModule");
        }
        else
            itOld->setText(mActiveModule->getName());

        mActiveModule = module;
        it->setText(module->getName() + " *");

        return true;
    }

    bool MainMenuWindow::handleStart()
    {
        WindowFactory::getSingleton().hideMainMenuLoadWindow();
        setVisible(false);
        destroyWindow();
        CoreSubsystem::getSingleton().startAdventureModule(mActiveModule);
        return true;
    }

    bool MainMenuWindow::handleLoad()
    {
        WindowFactory::getSingleton().showMainMenuLoadWindow(this);
        return true;
    }

    bool MainMenuWindow::handleQuit()
    {
        WindowFactory::getSingleton().showExitConfirmation();
        return true;
    }

    bool MainMenuWindow::handleSettings()
    {
        WindowFactory::getSingleton().showGameSettings();
        return true;
    }

    void MainMenuWindow::setVisible(bool visible, bool destroyAfterHide)
    {
        AbstractWindow::setVisible(visible, destroyAfterHide);
        mEngineWindow->setVisible(visible, destroyAfterHide);
    }
}
