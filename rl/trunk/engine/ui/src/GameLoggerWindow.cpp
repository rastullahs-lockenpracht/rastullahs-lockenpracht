/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
#include "UiPrerequisites.h"

#include "Creature.h"
#include "GameLoggerWindow.h"
#include "ListboxWrappedTextItem.h"

namespace rl {

CEGUI::colour GameLoggerWindow::COLOR_CHARACTER( 0xFFFF7F7F);
CEGUI::colour GameLoggerWindow::COLOR_DIALOG(    0xFFFF7FFF);
CEGUI::colour GameLoggerWindow::COLOR_FIGHT(     0xFFFFFF7F);
CEGUI::colour GameLoggerWindow::COLOR_MISC(      0xFFFFFFFF);

GameLoggerWindow::GameLoggerWindow() : 
	CeGuiWindow("gameloggerwindow.xml", WND_SHOW)
{
	mLog = getListbox ("GameLogger/Log");
	bindCloseToCloseButton();
}

void GameLoggerWindow::logEvent(const CeGuiString& text, const CEGUI::colour color)
{
	CEGUI::ListboxWrappedTextItem* item = new CEGUI::ListboxWrappedTextItem(text);
	item->setTextColours(color);
	mLog->insertItem(item, NULL);
}

void GameLoggerWindow::logDialogEvent(const CeGuiString& speaker, const CeGuiString& text)
{
	logEvent(speaker+": '"+text+"'", COLOR_DIALOG);
}

void GameLoggerWindow::logFightEvent(const CeGuiString& text)
{
	logEvent(text, COLOR_FIGHT);
}

void GameLoggerWindow::logCharacterEvent(const CeGuiString& text)
{
	logEvent(text, COLOR_CHARACTER);
}

void GameLoggerWindow::logMiscEvent(const CeGuiString& text)
{
	logEvent(text, COLOR_MISC);
}

}
