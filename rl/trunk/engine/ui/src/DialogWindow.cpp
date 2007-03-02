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

DialogWindow::DialogWindow(DialogCharacter* bot, GameLoggerWindow* gamelogger,
                           DialogCharacterController* controller)
  : CeGuiWindow("dialogwindow.xml", WND_MOUSE_INPUT, false),
	mBot(bot), 
	mCurrentResponse(NULL),
    mCurrentResponseText(""),
	mGameLogger(gamelogger),
	mController(controller),
    mState( TALKING_PLAYER_CHARACTER )
{
	mImage = getWindow("DialogWindow/Image");
	mName = getWindow("DialogWindow/Name");
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
	ListboxWrappedTextItem* item = new ListboxWrappedTextItem("", 6);
	item->setTextColours(COLOR_PLAYER_CHARACTER);
	mQuestion->addItem(item);
	item = new ListboxWrappedTextItem("", 0);
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
	
    if(mBot->hasExitRequest())
    {
        mQuestion->getListboxItemFromIndex(0)->setText(DIALOG_END);
		mQuestion->getListboxItemFromIndex(1)->setText("");
		mState = CLOSING_DIALOG;
        handleClose();
        return;
    }
	if(mCurrentResponse == NULL)
	{
		mQuestion->getListboxItemFromIndex(0)->setText(DIALOG_END);
		mQuestion->getListboxItemFromIndex(1)->setText("");
		mState = CLOSING_DIALOG;
	//	�bergangsl�sung, wenn gerade kein sprecher aktiv ist wird 
	//	nicht nicht textFinished aufgerufen
    //  hier m�sste wahrscheinlich requestClose auftauchen
		handleClose();
		return;
	}

	DialogResponse::Responses responses = mCurrentResponse->getResponses();
	CeGuiString responseSound = "null.ogg";
	CeGuiString responseText;
	if(!responses.empty())
	{
		responseSound = responses.begin()->first;
		responseText = responses.begin()->second;
		responseText.c_str();
		responseSound.c_str();	
	}

	mController->response(
			mBot->getDialogPartner()->getActor(), 
			responseText, responseSound.c_str());

	if(!responseText.empty())
	{
		mQuestion->getListboxItemFromIndex(1)->
			setText(mBot->getName() + ": " + responseText);
		mQuestion->handleUpdatedItemData();

		mGameLogger->logDialogEvent(mBot->getName(), responseText);
		LOG_MESSAGE(Logger::DIALOG, mBot->getName() + " says: " + responseText);
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

    LOG_DEBUG(Logger::UI, 
				StringConverter::toString(mState)
				+ " bei textFinished" );
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
	    LOG_MESSAGE(Logger::UI, 
			"Player says: " + currentResponse);
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
				new ListboxWrappedTextItem((*itr)->getText(), 6, true);
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

void DialogWindow::setCallback(Ogre::String function)
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
	if(item != NULL)
	{
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
				LOG_MESSAGE(Logger::DIALOG,
                    "Player says: " + selectedOption);
				mQuestion->getListboxItemFromIndex(0)->setText("Held: " + selectedOption);	
				mController->response(
					mBot->getDialogCharacter()->getActor(), 
					selectedOption, 
					option->getId().c_str());			
				setVisible(false);
			}
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

void DialogWindow::setImage(Ogre::String imageset, Ogre::String image)
{
    mImage->setProperty("Image", "set:" + imageset + " image:" + image);
}

void DialogWindow::setName(Ogre::String name)
{
	mName->setProperty("Text", name);
}

}
