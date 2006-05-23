/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#include "DialogWindow.h"

#include "Creature.h"
#include "DebugWindow.h"
#include "DialogCharacter.h"
#include "DialogResponse.h"
#include "DialogCharacterController.h"
#include "GameLoggerWindow.h"
#include "InputManager.h"
#include "ListboxWrappedTextItem.h"
#include "UiSubsystem.h"


namespace rl {

using namespace Ogre;
using namespace CEGUI;

const CEGUI::colour DialogWindow::COLOR_PLAYER_CHARACTER(	 0xFFFF7F7F);
const CEGUI::colour DialogWindow::COLOR_NON_PLAYER_CHARACTER(0xFFFFFF7F);

const CeGuiString DialogWindow::DIALOG_START = "1";
const CeGuiString DialogWindow::DIALOG_END = "DIALOG BEENDET";

DialogWindow::DialogWindow(DialogCharacter* bot, GameLoggerWindow* gamelogger, DialogCharacterController* controller)
  : CeGuiWindow("dialogwindow.xml", WND_MOUSE_INPUT),
	mBot(bot), 
	mCurrentResponse(NULL),
	mGameLogger(gamelogger),
	mController(controller)
{
	initialize();
}

DialogWindow::~DialogWindow()
{
	delete mCurrentResponse;
	mCurrentResponse = NULL;
}

void DialogWindow::initialize()
{
	mImage = getStaticImage("DialogWindow/Image");
	mName = getStaticText("DialogWindow/Name");
	mQuestion = getListbox("DialogWindow/Question");
	mDialogOptions = getListbox("DialogWindow/OptionList");

	mDialogOptions->subscribeEvent(
		Listbox::EventSelectionChanged, 
		boost::bind(&DialogWindow::handleSelectOption, this));
	mDialogOptions->moveToFront();
	mDialogOptions->setClippedByParent(true);
	mDialogOptions->setShowHorzScrollbar(false);
	mDialogOptions->setShowVertScrollbar(false);
	
	// Add 2 ListboxItems, one for the nsc responses, 
	// one for the player selections
	ListboxWrappedTextItem* item = new ListboxWrappedTextItem("");
	item->setTextColours(COLOR_PLAYER_CHARACTER);
	mQuestion->addItem(item);
	item = new ListboxWrappedTextItem("");
	item->setTextColours(COLOR_NON_PLAYER_CHARACTER);
	mQuestion->addItem(item);
	item = NULL;

	mWindow->subscribeEvent(FrameWindow::EventCloseClicked, // Verstecken, falls Close geklickt wird
		boost::bind(&DialogWindow::handleClose, this)); //TODO: als Abbrechen werten 

    getResponse(DIALOG_START);
	mName->setText(mBot->getName());
}

void DialogWindow::getResponse(const CeGuiString& msg)
{
	delete mCurrentResponse;
	mCurrentResponse = mBot->createResponse(msg);
	
	ListboxWrappedTextItem* item = NULL;

	if(mCurrentResponse == NULL)
	{
		mQuestion->getListboxItemFromIndex(0)->setText(DIALOG_END);
		mQuestion->getListboxItemFromIndex(1)->setText("");
//		handleClose();
		return;
	}
	CeGuiString responseText = mCurrentResponse->getResponse();
	mController->response(mBot->getDialogPartner()->getActor(), responseText, "");

	mQuestion->getListboxItemFromIndex(1)->
		setText(mBot->getName() + ": " + responseText);
	mQuestion->handleUpdatedItemData();

	mGameLogger->logDialogEvent(mBot->getName(), responseText);
	
	mResponses = mCurrentResponse->getOptions();
	
	if(mResponses.empty())
	{
		mQuestion->getListboxItemFromIndex(0)->setText(DIALOG_END);
		mQuestion->getListboxItemFromIndex(1)->setText("");
		handleClose();
		return;
	}
	
	NaturalLanguageProcessor::Responses::iterator itr = mResponses.begin();

	unsigned int i = 0;
	CeGuiString currentResponse;
	for(; itr != mResponses.end(); ++itr)
	{			
		if(i < mDialogOptions->getItemCount())
		{
			item = static_cast <ListboxWrappedTextItem*>(mDialogOptions->getListboxItemFromIndex(i));
			currentResponse = itr->second;
			Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, currentResponse.c_str());
			item->setText(currentResponse);
			item->setTextFormatting(CEGUI::WordWrapLeftAligned);
			mDialogOptions->handleUpdatedItemData();
		}
		else
		{
			currentResponse = itr->second;
			item = new ListboxWrappedTextItem(currentResponse);
			item->setTextFormatting(CEGUI::WordWrapLeftAligned);
			mDialogOptions->addItem(item);
		}
		item->setID(itr->first);

		++i;
	}
	while(i < mDialogOptions->getItemCount())
	{
		mDialogOptions->removeItem(mDialogOptions->getListboxItemFromIndex(i));
	}
	mResponses.clear();
}

unsigned int DialogWindow::count()
{
	return mDialogOptions->getItemCount();
}

void DialogWindow::setCallback(string function)
{
	// TO DO: DialogWindow::setCallback(string function)
}

int DialogWindow::getSelectedOption()
{
	// TO DO: DialogWindow::getSelectedOption()
	return 0;
}

void DialogWindow::showNextText()
{
	//TODO: Umschalten zwischen Antwort und Optionen hier her
}

bool DialogWindow::handleSelectOption()
{
	DebugWindow::getSingleton().setMessageText(StringConverter::toString(getSelectedOption()));
	ListboxWrappedTextItem* item = 
		reinterpret_cast<ListboxWrappedTextItem*>(mDialogOptions->getFirstSelectedItem());
	int id = item->getID();
	std::pair<int, CeGuiString> selectedOption 
		= mCurrentResponse->getSelectedOption(id);
	if(selectedOption.first != 0 && selectedOption.first != 666)
	{
		id = selectedOption.first;
		if(!selectedOption.second.empty())
		{
			mGameLogger->logDialogEvent("Held", selectedOption.second);
			mQuestion->getListboxItemFromIndex(0)->setText("Held: " + selectedOption.second);	
			mController->response(mBot->getDialogCharacter()->getActor(), selectedOption.second, "");
		}
	}
	getResponse(StringConverter::toString(id));	
	
	return true;
}

bool DialogWindow::handleClose()
{
	setVisible(false);
	UiSubsystem::getSingleton().setCharacterController(CharacterController::CTRL_MOVEMENT);
	return true;
}

void DialogWindow::setImage(string imageset, string image)
{
	mImage->setImage(imageset, image);
}

void DialogWindow::setName(string name)
{
	mName->setText(name);
}

}
