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

#include <boost/bind.hpp>

#include "DialogWindow.h"
#include "InputManager.h"
#include "DebugWindow.h"
#include "CharacterSheetWindow.h"

template<> rl::DialogWindow* Ogre::Singleton<rl::DialogWindow>::ms_Singleton = 0;

namespace rl {

using namespace CEGUI;
using namespace std;



DialogWindow& DialogWindow::getSingleton()
{
	return Singleton<DialogWindow>::getSingleton();
}
DialogWindow* DialogWindow::getSingletonPtr()
{
	return Singleton<DialogWindow>::getSingletonPtr();
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

	addToRoot(mWindow);
    mIsVisible = false;
	getResponse("START DIALOG");
}

DialogWindow::~DialogWindow()
{
	// TO DO: DialogWindow::~DialogWindow()
}

void DialogWindow::getResponse(string msg)
{
	ListboxTextItem* item;
	mResponses=mNlp->respond(msg);
	if(mResponses.empty())
	{
		mQuestion->setText(CeGuiString("DIALOG BEENDET"));
		hide();	
		return;
	}
	std::map<int,std::string>::iterator itr=mResponses.begin();
	
	mQuestion->setText(itr->second);
	unsigned int i=0;
	for(itr++;itr!=mResponses.end();itr++)
	{			
			if(i<mDialogOptions->getItemCount())
			{
				item=reinterpret_cast<ListboxTextItem*>(mDialogOptions->getListboxItemFromIndex(i));
				item->setText(itr->second);
			}
			else
			{
				item=new ListboxTextItem(itr->second);
				mDialogOptions->addItem(item);
			}
			item->setID(itr->first);
			i+=1;
	}
	while(i<mDialogOptions->getItemCount())
	{
		mDialogOptions->removeItem(mDialogOptions->getListboxItemFromIndex(i));

	}
	mResponses.clear();

}

void DialogWindow::addLine(string text)
{
	mDialogOptions->addItem(new ListboxTextItem(text));
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
	for (unsigned int line = 0; line < count(); line++)
	{
		CeGuiString text = mTextLines[line];
		for (map<string, string>::iterator var = mVariableValues.begin(); 
				var != mVariableValues.end(); 
				var++)
		{
			int pos = 0;
			
			while (pos != CeGuiString::npos)
			{
				pos = text.find("%"+(*var).first+"%", pos);
				int size = 2+(*var).first.length();
				if (pos != CeGuiString::npos)
					text = text.replace(pos, size, (*var).second);
			}
		}
		mDialogOptions->getListboxItemFromIndex(line)->setText(text);
	}	
}

bool DialogWindow::handleSelectOption()
{
	DebugWindow::getSingleton().setText("Pnk "+StringConverter::toString(getSelectedOption()));
	ListboxTextItem* item=reinterpret_cast<ListboxTextItem*>(mDialogOptions->getFirstSelectedItem());
	getResponse(StringConverter::toString(item->getID()));	
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
