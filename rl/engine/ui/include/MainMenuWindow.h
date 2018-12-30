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
#ifndef __MainMenuWindow_H__
#define __MainMenuWindow_H__

#include "AbstractWindow.h"
#include "UiPrerequisites.h"
#include <CEGUI/widgets/MenuItem.h>

namespace rl
{
    class ContentModule;
    class MainMenuEngineWindow;

    class _RlUiExport MainMenuWindow : public AbstractWindow
    {
    public:
        MainMenuWindow(MainMenuEngineWindow* enginewindow);
        virtual ~MainMenuWindow();

        void setActiveModule(const CeGuiString& module);
        void setVisible(bool visible, bool destroyAfterHide = false);

    private:
        bool handleChooseModule(CEGUI::MenuItem* it, ContentModule* module);
        bool handleStart();
        bool handleLoad();
        bool handleQuit();
        bool handleSettings();

        bool handleKey(const CEGUI::EventArgs& evt);
        void fillModules();

        ContentModule* mActiveModule;
        MainMenuEngineWindow* mEngineWindow;
    };
}

#endif
