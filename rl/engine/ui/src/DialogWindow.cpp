#include <boost/bind.hpp>

#include "DialogWindow.h"
#include "InputManager.h"

namespace rl {

using namespace CEGUI;
using namespace std;

DialogWindow::DialogWindow()
{
	Window* dialogWindow = static_cast<Listbox*>(WindowManager::getSingleton().createWindow((utf8*)"Taharez Frame Window", (utf8*)"DialogWindow"));
	dialogWindow->setMetricsMode(Absolute);
	dialogWindow->setPosition(Relative, Point(0.25f, 0.25f));
	dialogWindow->setSize(Absolute, Size(400, 300));

	mImage = static_cast<StaticImage*>(WindowManager::getSingleton().createWindow((utf8*)"Taharez StaticImage", dialogWindow->getName()+(utf8*)"/Image"));
	mImage->setPosition(Absolute, Point(2*28, 2*30));
	mImage->setSize(Absolute, Size(2*64, 2*64));

	mName = static_cast<StaticText*>(WindowManager::getSingleton().createWindow((utf8*)"Taharez StaticText", dialogWindow->getName()+(utf8*)"/Name"));
	mName->setPosition(Absolute, Point(2*12, 2*100));
	mName->setSize(Absolute, Size(2*100, 2*20));
	mName->setHorizontalFormatting(StaticText::HorzCentred);
		
	mQuestion = static_cast<StaticText*>(WindowManager::getSingleton().createWindow((utf8*)"Taharez StaticText", dialogWindow->getName()+(utf8*)"/Question"));
	mQuestion->setPosition(Absolute, Point(2*120, 2*24));
	mQuestion->setSize(Absolute, Size(2*268, 2*20));

	mDialogOptions = static_cast<Listbox*>(WindowManager::getSingleton().createWindow((utf8*)"Taharez Listbox", dialogWindow->getName()+(utf8*)"/OptionList"));
	mDialogOptions->setPosition(Absolute, Point(2*120, 2*50));
	mDialogOptions->setSize(Absolute, Size(2*268, 2*238));
	mDialogOptions->setMultiselectEnabled(false);
	mDialogOptions->setEnabled(true);
	mDialogOptions->subscribeEvent(Listbox::SelectionChanged, boost::bind(&DialogWindow::handleSelectOption, this, _1));
	
	dialogWindow->addChildWindow(mDialogOptions);
    dialogWindow->addChildWindow(mQuestion);
	dialogWindow->addChildWindow(mImage);
	dialogWindow->addChildWindow(mName);

	Window* rootWindow = WindowManager::getSingleton().getWindow((utf8*)"root_wnd");
	rootWindow->addChildWindow(dialogWindow);	
}

DialogWindow::~DialogWindow()
{
}

void DialogWindow::addLine(string text)
{
	mDialogOptions->addItem(new ListboxTextItem(text));
	mTextLines.push_back(text);
	updateValues();
}

void DialogWindow::removeLine(int num)
{
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
}

void DialogWindow::show()
{
	InputManager::getSingleton().registerCeguiWindow();
}

void DialogWindow::hide()
{
	InputManager::getSingleton().unregisterCeguiWindow();
}

int DialogWindow::getSelectedOption()
{
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

void DialogWindow::runTest()
{
	DialogWindow* dialog = new DialogWindow();
	dialog->setQuestion("Wie ist dein Name?");
	dialog->addLine("Hab ich vergessen");
	dialog->addLine("Ich heisse %NAME%");
	dialog->addLine("Mein Name ist %NAME%");
	dialog->addLine("Man nennt mich %NAME%, oder auch %NAME%");
	dialog->addLine("Ich habe nichts zu sagen");

	dialog->setVariableValue("NAME", "Heinz-Peter");
	dialog->setName("Klaus-Dieter");
	dialog->show();

	DebugWindow::getSingleton().setVisible(true);
}

void DialogWindow::handleSelectOption(const CEGUI::EventArgs& e)
{
	DebugWindow::getSingleton().setText("Pönk"+StringConverter::toString(getSelectedOption());
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