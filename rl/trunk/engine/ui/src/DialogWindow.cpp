#include <boost/bind.hpp>

#include "DialogWindow.h"
#include "InputManager.h"
#include "DebugWindow.h"
#include "CharacterSheetWindow.h"

namespace rl {

using namespace CEGUI;
using namespace std;
/*
DialogWindow& DialogWindow::getSingleton()
{
	return Singleton<DialogWindow>::getSingleton();
}
DialogWindow* DialogWindow::getSingletonPtr()
{
	return Singleton<DialogWindow>::getSingletonPtr();
}
*/
DialogWindow::DialogWindow(string dialogFile) : CeGuiWindow("dialogwindow.xml", true),mNlp(new NaturalLanguageProcessor(dialogFile))
{
	mImage = getStaticImage("DialogWindow/Image");
	mName = getStaticText("DialogWindow/Name");
	mQuestion = getMultiLineEditbox("DialogWindow/Question");
	mDialogOptions = getListbox("DialogWindow/OptionList");
	mDialogOptions->subscribeEvent(
		Listbox::SelectionChanged, 
		boost::bind(&DialogWindow::handleSelectOption, this));

	addToRoot(mWindow);	
	mState == CS_CLOSED;
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
		mQuestion->setText(CEGUI::String("DIALOG BEENDET"));
		hide();	
		return;
	}
	std::map<int,std::string>::iterator itr=mResponses.begin();
	
	mQuestion->setText(itr->second);
	int i=0;
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
		CEGUI::String text = mTextLines[line];
		for (map<string, string>::iterator var = mVariableValues.begin(); 
				var != mVariableValues.end(); 
				var++)
		{
			int pos = 0;
			
			while (pos != CEGUI::String::npos)
			{
				pos = text.find("%"+(*var).first+"%", pos);
				int size = 2+(*var).first.length();
				if (pos != CEGUI::String::npos)
					text = text.replace(pos, size, (*var).second);
			}
		}
		mDialogOptions->getListboxItemFromIndex(line)->setText(text);
	}	
}

void DialogWindow::handleSelectOption()
{
	DebugWindow::getSingleton().setText("Pnk "+StringConverter::toString(getSelectedOption()));
	ListboxTextItem* item=reinterpret_cast<ListboxTextItem*>(mDialogOptions->getFirstSelectedItem());
	getResponse(StringConverter::toString(item->getID()));	
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
