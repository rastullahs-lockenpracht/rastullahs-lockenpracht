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

#ifndef __GameLoggerWindow_H__
#define __GameLoggerWindow_H__

#include "UiPrerequisites.h"

#include "AbstractWindow.h"
#include "MessagePump.h"
#include "RulesConstants.h"

#include <CEGUI/Colour.h>

namespace rl
{

    class _RlUiExport GameLoggerWindow : public AbstractWindow
    {
    public:
        GameLoggerWindow();

        /// Message handler for RLMSG_GAMEEVENTLOG_EVENT_ADDED
        bool onLogEntryAdded(GameEventType evt, const CeGuiString& msg);

    private:
        static CEGUI::Colour COLOR_DEFAULT;
        static CEGUI::Colour COLOR_COMBAT;
        static CEGUI::Colour COLOR_DIALOG;
        static CEGUI::Colour COLOR_QUEST;

        MessagePump::ScopedConnection mLogEntryAddedConnection;
        CEGUI::Listbox* mLog;

        void logEvent(const CeGuiString& text, const CEGUI::Colour color);
    };
}

#endif
