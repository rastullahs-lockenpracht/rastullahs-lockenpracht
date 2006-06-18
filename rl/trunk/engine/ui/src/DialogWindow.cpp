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

#include "Exception.h"
#include "ConfigurationManager.h"
#include "Creature.h"
#include "DebugWindow.h"
#include "DialogCharacter.h"
#include "DialogResponse.h"
#include "DialogOption.h"
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
const CeGuiString DialogWindow::DIALOG_EXIT = "EXIT";

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

	mWindow->subscribeEvent(
		FrameWindow::EventCloseClicked, // Verstecken, falls Close geklickt wird
		boost::bind(&DialogWindow::requestClose, this)); //TODO: als Abbrechen werten 

	mDialogOptions->subscribeEvent(
		Listbox::EventSelectionChanged, 
		boost::bind(&DialogWindow::handleSelectOption, this));
	mDialogOptions->moveToFront();
	mDialogOptions->setClippedByParent(true);
	mDialogOptions->setShowHorzScrollbar(false);
	mDialogOptions->setShowVertScrollbar(false);
	mDialogOptions->setMultiselectEnabled(false);
}

DialogWindow::~DialogWindow()
{
	delete mCurrentResponse;
	mCurrentResponse = NULL;

	Ogre::String voiceFile = mBot->getVoiceFile().c_str();
	ResourceGroupManager::getSingleton().removeResourceLocation(voiceFile, voiceFile);
}

void DialogWindow::initialize()
{
	
	// Add 2 ListboxItems, one for the nsc responses, 
	// one for the player selections
	ListboxWrappedTextItem* item = new ListboxWrappedTextItem("");
	item->setTextColours(COLOR_PLAYER_CHARACTER);
	mQuestion->addItem(item);
	item = new ListboxWrappedTextItem("");
	item->setTextColours(COLOR_NON_PLAYER_CHARACTER);
	mQuestion->addItem(item);
	item = NULL;

	
	Ogre::String voiceFile = mBot->getVoiceFile().c_str();
	if (voiceFile != "")
	{
		if (voiceFile.find(".zip") != Ogre::String::npos)
		{
			ResourceGroupManager::getSingleton().addResourceLocation(
				voiceFile, "Zip", voiceFile);
		}
		else
		{
			ResourceGroupManager::getSingleton().addResourceLocation(
           		voiceFile, "FileSystem", voiceFile);
		}
	}
}

void DialogWindow::start()
{
	initialize();
	mName->setText(mBot->getName());
	getResponse(DIALOG_START);
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
	//	übergangslösung, wenn gerade kein sprecher aktiv ist wird 
	//	nicht nicht textFinished aufgerufen
		handleClose();
		return;
	}

	DialogResponse::Responses responses = mCurrentResponse->getResponses();
	CeGuiString responseText;
	if(!responses.empty())
	{
		responseText = responses.begin()->second;
		responseText.c_str();
		mController->response(
			mBot->getDialogPartner()->getActor(), 
			responseText, 
			responses.begin()->first.c_str());
	}

	if(!responseText.empty())
	{
		mQuestion->getListboxItemFromIndex(1)->
			setText(mBot->getName() + ": " + responseText);
		mQuestion->handleUpdatedItemData();

		mGameLogger->logDialogEvent(mBot->getName(), responseText);
	}


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
	
	if (mState == TALKING_PLAYER_CHARACTER)
	{
		getResponse(mCurrentResponseText);
	}
	
	if (mState == CLOSING_DIALOG)
	{
		handleClose();
	}
}

void DialogWindow::getOptions(const CeGuiString& question)
{
	if(mCurrentResponse == NULL)
	{
		Throw(rl::IllegalStateException, "mCurrentResponse must not be NULL.");
		return;
	}
	
	DialogResponse::DialogOptions options = mCurrentResponse->getDialogOptions();
	
	if(options.empty())
	{
		mQuestion->getListboxItemFromIndex(0)->setText(DIALOG_END);
		mQuestion->getListboxItemFromIndex(1)->setText("");
		handleClose();
		return;
	}
	
	//mDialogOptions->clearAllSelections();

	unsigned int i = 0;
	for(DialogResponse::DialogOptions::const_iterator itr = options.begin(); 
		itr != options.end(); ++itr)
	{	
		CeGuiString currentResponse = (*itr)->getText();
		Logger::getSingleton().log(
			Logger::DIALOG, 
			Logger::LL_MESSAGE, 
			currentResponse);
		if(i < mDialogOptions->getItemCount())
		{
			ListboxWrappedTextItem* item = 
				static_cast <ListboxWrappedTextItem*>(mDialogOptions->getListboxItemFromIndex(i));
			item->setUserData(*itr);
			item->setText(currentResponse);
			item->setTextFormatting(CEGUI::WordWrapLeftAligned);
			mDialogOptions->handleUpdatedItemData();
		}
		else
		{
			ListboxWrappedTextItem* item = 
				new ListboxWrappedTextItem((*itr)->getText());
			item->setUserData(*itr);
			item->setTextFormatting(CEGUI::WordWrapLeftAligned);
			mDialogOptions->addItem(item);
		}

		++i;
	}
	while(i < mDialogOptions->getItemCount())
	{
		mDialogOptions->removeItem(mDialogOptions->getListboxItemFromIndex(i));
	}	
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
		static_cast<ListboxWrappedTextItem*>(mDialogOptions->getFirstSelectedItem());
	int id = item->getID();
	DialogOption* option = static_cast<DialogOption*>(item->getUserData());
	option->processSelection();
	mCurrentResponseText = option->getPattern();
	CeGuiString selectedOption = option->getText();
	if(mCurrentResponseText != "0" && mCurrentResponseText != "666")
	{
		if(!selectedOption.empty())
		{
			mState = TALKING_PLAYER_CHARACTER;
			mGameLogger->logDialogEvent("Held", selectedOption);
			mQuestion->getListboxItemFromIndex(0)->setText("Held: " + selectedOption);	
			mController->response(
				mBot->getDialogCharacter()->getActor(), 
				selectedOption, 
				option->getId().c_str());			
			setVisible(false);
		}
	}
	
	return true;
}

bool DialogWindow::handleClose()
{
	UiSubsystem::getSingleton().requestCharacterControllerSwitch(
		CharacterController::CTRL_MOVEMENT);
	destroyWindow();
	return true;
}

bool DialogWindow::requestClose()
{
//	handleClose is called automatically 
	getResponse(DIALOG_EXIT);
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
