/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#include "UiPrerequisites.h"

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
	addToRoot(mWindow);
}

void GameLoggerWindow::logEvent(const CeGuiString& text, const CEGUI::colour color)
{
	CEGUI::ListboxWrappedTextItem* item = new CEGUI::ListboxWrappedTextItem(text);
	item->setTextColours(color);
	mLog->insertItem(item, NULL);
}

void GameLoggerWindow::logDialogEvent(const CeGuiString& text)
{
	logEvent("'"+text+"'", COLOR_DIALOG);
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
