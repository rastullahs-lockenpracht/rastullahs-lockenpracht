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
#include "stdinc.h"

#include "DialogWindow.h"

#include <boost/bind.hpp>
#include <elements/CEGUIFrameWindow.h>

#include "Exception.h"
#include "ConfigurationManager.h"
#include "Creature.h"
#include "DebugWindow.h"
#include "DialogCharacter.h"
#include "DialogResponse.h"
#include "DialogOption.h"
#include "DialogControlState.h"
#include "GameLoggerWindow.h"
#include "InputManager.h"
#include "ListboxWrappedTextItem.h"
#include "UiSubsystem.h"


namespace rl {

	using namespace Ogre;
	using namespace CEGUI;

	const CEGUI::colour DialogWindow::COLOR_PLAYER_CHARACTER(	 0xFFFF7F7F);
	const CEGUI::colour DialogWindow::COLOR_NON_PLAYER_CHARACTER(0xFFFFFF7F);

	DialogWindow::DialogWindow(DialogControlState* controller)
	  : AbstractWindow("dialogwindow.xml", WIT_MOUSE_INPUT, false)
	{
		mImage = getWindow("DialogWindow/Image");
		mName = getWindow("DialogWindow/Name");
		mQuestion = getListbox("DialogWindow/Question");
		mDialogOptions = getListbox("DialogWindow/OptionList");

		mWindow->subscribeEvent(
			FrameWindow::EventCloseClicked, // Verstecken, falls Close geklickt wird
			boost::bind(&DialogControlState::requestDialogClose, controller)); //TODO: als Abbrechen werten 

		mDialogOptions->subscribeEvent(
			Listbox::EventSelectionChanged, 
			boost::bind(&DialogControlState::handleDialogSelectOption, controller));

		mDialogOptions->moveToFront();
		mDialogOptions->setClippedByParent(true);
		mDialogOptions->setShowHorzScrollbar(false);
		mDialogOptions->setShowVertScrollbar(false);
		mDialogOptions->setMultiselectEnabled(false);

		// Add 2 ListboxItems, one for the nsc responses, 
		// one for the player selections
		mPlayerText = new ListboxWrappedTextItem("", 6);
		mPlayerText->setTextColours(COLOR_PLAYER_CHARACTER);
		mQuestion->addItem(mPlayerText);

		mNscText = new ListboxWrappedTextItem("", 0);
		mNscText->setTextColours(COLOR_NON_PLAYER_CHARACTER);
		mQuestion->addItem(mNscText);
	}

	void DialogWindow::setImage(const CeGuiString& imageset, const CeGuiString& image)
	{
		mImage->setProperty("Image", "set:" + imageset + " image:" + image);
	}

	void DialogWindow::setName(const CeGuiString& name)
	{
		mName->setText(name);
	}

	void DialogWindow::setDialogEnd()
	{
		setChosenOption("Ende");
		setResponse("");
	}

	void DialogWindow::setAvailableOptions(const CeGuiStringVector& options)
	{
		unsigned int i = 0;
		for(CeGuiStringVector::const_iterator itr = options.begin(); 
			itr != options.end(); ++itr)
		{	
			ListboxWrappedTextItem* item = NULL;
			if(i < mDialogOptions->getItemCount())
			{
				item = static_cast<ListboxWrappedTextItem*>(
					mDialogOptions->getListboxItemFromIndex(i));
				item->setText(*itr);
			}
			else
			{
				item =	new ListboxWrappedTextItem(*itr, 6, true);
				mDialogOptions->addItem(item);
			}

			item->setTextFormatting(CEGUI::WordWrapLeftAligned);
			mDialogOptions->handleUpdatedItemData();

			++i;
		}
		while(i < mDialogOptions->getItemCount())
		{
			mDialogOptions->removeItem(mDialogOptions->getListboxItemFromIndex(i));
		}	
	}

	int DialogWindow::getSelectedOptionIndex() const
	{
		CEGUI::ListboxItem* curr = mDialogOptions->getFirstSelectedItem();
		if (curr)
		{
			return mDialogOptions->getItemIndex(curr);
		}
		else
		{
			return -1;
		}
	}

	void DialogWindow::setChosenOption(const CeGuiString& option)
	{
		mPlayerText->setText("Held: " + option);
	}

	void DialogWindow::setResponse(const CeGuiString& response)
	{
		mNscText->setText(mName->getText() + ": " + response);
		mQuestion->handleUpdatedItemData();
	}

}
