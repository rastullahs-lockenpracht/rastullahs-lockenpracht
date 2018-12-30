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

#include "GameLoggerWindow.h"

#include <CEGUI/widgets/ListboxTextItem.h>

#include "Creature.h"
#include "GameEventLog.h"
#include "RulesMessages.h"

namespace rl
{
    CEGUI::Colour GameLoggerWindow::COLOR_QUEST(0xFFFF7F7F);
    CEGUI::Colour GameLoggerWindow::COLOR_COMBAT(0xFFFFFF7F);
    CEGUI::Colour GameLoggerWindow::COLOR_DIALOG(0xFFFF7FFF);
    CEGUI::Colour GameLoggerWindow::COLOR_DEFAULT(0xFFFFFFFF);

    GameLoggerWindow::GameLoggerWindow()
        : AbstractWindow("gameloggerwindow.xml", WIT_NONE)
        , mLogEntryAddedConnection()
        , mLog(NULL)
    {
        mLog = getListbox("GameLogger/Log");
        mLogEntryAddedConnection = MessagePump::getSingleton().addMessageHandler<MessageType_GameEventLog_EventAdded>(
            boost::bind(&GameLoggerWindow::onLogEntryAdded, this, _1, _2));
    }

    bool GameLoggerWindow::onLogEntryAdded(GameEventType evt, const CeGuiString& text)
    {
        CEGUI::Colour col = evt == GET_QUEST
            ? COLOR_QUEST
            : evt == GET_COMBAT ? COLOR_COMBAT : evt == GET_DIALOG ? COLOR_DIALOG : COLOR_DEFAULT;
        logEvent(text, col);
        return true;
    }

    void GameLoggerWindow::logEvent(const CeGuiString& text, const CEGUI::Colour color)
    {
        CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem(text);
        item->setTextColours(color);
        mLog->addItem(item);
        mLog->ensureItemIsVisible(item);
    }
}
