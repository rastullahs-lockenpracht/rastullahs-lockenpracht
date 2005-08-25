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
#include "DialogSubsystem.h"
#include "InputManager.h"
#include "DebugWindow.h"
#include "CharacterSheetWindow.h"
#include "ListboxWrappedTextItem.h"

using namespace Ogre;

template<> rl::DialogWindow* Ogre::Singleton<rl::DialogWindow>::ms_Singleton = 0;

namespace rl {

using namespace CEGUI;
using namespace std;



DialogWindow& DialogWindow::getSingleton()
{
	return Ogre::Singleton<DialogWindow>::getSingleton();
}
DialogWindow* DialogWindow::getSingletonPtr()
{
	return Ogre::Singleton<DialogWindow>::getSingletonPtr();
}

DialogWindow::DialogWindow(string dialogFile) : 
	CeGuiWindow("dialogwindow.xml", WND_MOUSE_INPUT),
	mNlp(new NaturalLanguageProcessor(dialogFile))
{
	mImage = getStaticImage("DialogWindow/Image");
	mName = getStaticText("DialogWindow/Name");
	mQuestion = getMultiLineEditbox("DialogWindow/Question");
	mDialogOptions = getListbox("DialogWindow/OptionList");
	mDialogOptions->subscribeEvent(
		Listbox::EventSelectionChanged, 
		boost::bind(&DialogWindow::handleSelectOption, this));
	mDialogOptions->moveToFront();
	mDialogOptions->setClippedByParent(true);
	mDialogOptions->setShowHorzScrollbar(false);
	mDialogOptions->setShowVertScrollbar(false);

	mWindow->subscribeEvent(FrameWindow::EventCloseClicked, // Verstecken, falls Close geklickt wird
		boost::bind(&DialogWindow::handleClose, this)); //TODO: als Abbrechen werten 

	addToRoot(mWindow);
    getResponse("START DIALOG");
	mName->setText(mNlp->getName());
}

DialogWindow::~DialogWindow()
{
	// TO DO: DialogWindow::~DialogWindow()
	if(mNlp)delete mNlp;
}

void DialogWindow::getResponse(string msg)
{
	ListboxWrappedTextItem* item;
	mResponses = mNlp->respond(msg);
	
	if(mResponses.empty())
	{
		mQuestion->setText(CeGuiString("DIALOG BEENDET"));
		hide();	
		return;
	}
	std::map<int,std::string>::iterator itr = mResponses.begin();
	
	mQuestion->setText(itr->second);
	unsigned int i = 0;
	string currentResponse;
	for(++itr; itr != mResponses.end(); ++itr)
	{			
			if(i < mDialogOptions->getItemCount())
			{
				item = reinterpret_cast<ListboxWrappedTextItem*>(mDialogOptions->getListboxItemFromIndex(i));
				currentResponse = itr->second;
				DialogSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, currentResponse);
				item->setText(currentResponse);
				item->setTextFormatting(CEGUI::WordWrapLeftAligned);
			}
			else
			{
				currentResponse = itr->second;
				item = new ListboxWrappedTextItem(currentResponse);
				item->setTextFormatting(CEGUI::WordWrapLeftAligned);
				mDialogOptions->addItem(item);
			}
			item->setID(itr->first);
			mDialogOptions->ensureItemIsVisible(item);
			++i;
	}
	while(i < mDialogOptions->getItemCount())
	{
		mDialogOptions->removeItem(mDialogOptions->getListboxItemFromIndex(i));

	}
	mResponses.clear();
}

void DialogWindow::addLine(string text)
{
	ListboxWrappedTextItem* item = new ListboxWrappedTextItem(text);
	item->setTextFormatting(CEGUI::WordWrapLeftAligned);
	mDialogOptions->addItem(item);
	mTextLines.push_back(text);
	updateValues();
}

void DialogWindow::removeLine(int num)
{
	// TO DO: DialogWindow::removeLine(int num)
}

unsigned int DialogWindow::count()
{
	return mDialogOptions->getItemCount();
}

void DialogWindow::setVariableValue(string name, string value)
{
	mVariableValues.insert(pair<string, string>(name, value));
	updateValues();
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

void DialogWindow::updateValues()
{
	CeGuiString text;
	for (unsigned int line = 0; line < count(); ++line)
	{
		text = mTextLines[line];
		map<string, string>::iterator itr = mVariableValues.begin();
		for (; itr != mVariableValues.end(); ++itr)
		{
			int pos = 0;
			
			while (pos != CeGuiString::npos)
			{
				pos = text.find("%"+(*itr).first+"%", pos);
				int size = 2 + (*itr).first.length();
				if (pos != CeGuiString::npos)
				{
					text = text.replace(pos, size, (*itr).second);
				}
			}
		}
		mDialogOptions->getListboxItemFromIndex(line)->setText(text);
	}	
}

bool DialogWindow::handleSelectOption()
{
	DebugWindow::getSingleton().setText("Pnk "+StringConverter::toString(getSelectedOption()));
	ListboxWrappedTextItem* item = reinterpret_cast<ListboxWrappedTextItem*>(mDialogOptions->getFirstSelectedItem());
	getResponse(StringConverter::toString(item->getID()));	
	return true;
}

bool DialogWindow::handleClose()
{
	setVisible(false);
	return true;
}

void DialogWindow::setQuestion(string question)
{
	mQuestion->setText(question);
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
