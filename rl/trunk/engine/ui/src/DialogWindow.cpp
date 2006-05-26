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
	mImage = getStaticImage("DialogWindow/Image");
	mName = getStaticText("DialogWindow/Name");
	mQuestion = getListbox("DialogWindow/Question");
	mDialogOptions = getListbox("DialogWindow/OptionList");

	mWindow->subscribeEvent(FrameWindow::EventCloseClicked, // Verstecken, falls Close geklickt wird
		boost::bind(&DialogWindow::handleClose, this)); //TODO: als Abbrechen werten 

	initialize();
}

DialogWindow::~DialogWindow()
{
	delete mCurrentResponse;
	mCurrentResponse = NULL;
}

void DialogWindow::initialize()
{
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
	mName->setText(mBot->getName());

	mCurrentResponse = mBot->createResponse(DIALOG_START);
    getOptions(DIALOG_START);
	mState = CHOOSING_OPTION;
}

void DialogWindow::getResponse(const CeGuiString& msg)
{
	delete mCurrentResponse;
	mCurrentResponse = mBot->createResponse(msg);
	
	if(mCurrentResponse == NULL)
	{
		mQuestion->getListboxItemFromIndex(0)->setText(DIALOG_END);
		mQuestion->getListboxItemFromIndex(1)->setText("");
		mState = CLOSING_DIALOG;
		return;
	}
	CeGuiString responseText = mCurrentResponse->getResponse();

	mQuestion->getListboxItemFromIndex(1)->
		setText(mBot->getName() + ": " + responseText);
	mQuestion->handleUpdatedItemData();

	mGameLogger->logDialogEvent(mBot->getName(), responseText);

	mController->response(mBot->getDialogPartner()->getActor(), responseText, "lachen.ogg"); //TODO: Sounddatei
	setVisible(false);
	mState = TALKING_PARTNER_CHARACTER;
	mCurrentResponseText = msg;
}

void DialogWindow::textFinished()
{
	if (mState == TALKING_PARTNER_CHARACTER)
	{
		getOptions(mCurrentResponseText);
		setVisible(true);
		mState = CHOOSING_OPTION;
	}
	else if (mState == TALKING_PLAYER_CHARACTER)
	{
		getResponse(mCurrentResponseText);
	}
	else if (mState == CLOSING_DIALOG)
	{
		handleClose();
	}
}

void DialogWindow::getOptions(const CeGuiString& question)
{
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
			ListboxWrappedTextItem* item = 
				static_cast <ListboxWrappedTextItem*>(mDialogOptions->getListboxItemFromIndex(i));
			item->setID(itr->first);
			currentResponse = itr->second;
			Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, currentResponse.c_str());
			item->setText(currentResponse);
			item->setTextFormatting(CEGUI::WordWrapLeftAligned);
			mDialogOptions->handleUpdatedItemData();
		}
		else
		{
			currentResponse = itr->second;
			ListboxWrappedTextItem* item = 
				new ListboxWrappedTextItem(currentResponse);
			item->setID(itr->first);
			item->setTextFormatting(CEGUI::WordWrapLeftAligned);
			mDialogOptions->addItem(item);
		}

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
			mController->response(
				mBot->getDialogCharacter()->getActor(), 
				selectedOption.second, 
				"lachen.ogg"); //TODO: Sounddatei
			mState = TALKING_PLAYER_CHARACTER;
			setVisible(false);
		}
	}
	mCurrentResponseText = StringConverter::toString(id);
	
	return true;
}

bool DialogWindow::handleClose()
{
	UiSubsystem::getSingleton().requestCharacterControllerSwitch(CharacterController::CTRL_MOVEMENT);
	destroyWindow();
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
