#include <boost/bind.hpp>

#include "DialogWindow.h"
#include "InputManager.h"
#include "DebugWindow.h"
#include "CharacterSheetWindow.h"

namespace rl {

using namespace CEGUI;
using namespace std;

DialogWindow::DialogWindow() : CeGuiWindow("dialogwindow.xml", true)
{
	mImage = getStaticImage("DialogWindow/Image");
	mName = getStaticText("DialogWindow/Name");
	mQuestion = getStaticText("DialogWindow/Question");
	mDialogOptions = getListbox("DialogWindow/OptionList");
	mDialogOptions->subscribeEvent(
		Listbox::SelectionChanged, 
		boost::bind(&DialogWindow::handleSelectOption, this));
	
	addToRoot(mWindow);	
}

DialogWindow::~DialogWindow()
{
	// TO DO: DialogWindow::~DialogWindow()
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
	//DebugWindow::getSingleton().setText("Pnk "+StringConverter::toString(getSelectedOption()));
	//hide();
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
