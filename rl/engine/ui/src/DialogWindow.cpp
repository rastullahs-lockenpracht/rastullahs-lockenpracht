#include "DialogWindow.h"

namespace rl {

using namespace CEGUI;
using namespace std;

DialogWindow::DialogWindow()
{
	Window* rootWindow = WindowManager::getSingleton().getWindow((utf8*)"root_wnd");
	//rootWindow->addChildWindow(mConsoleWindow);	
}

DialogWindow::~DialogWindow()
{
}

void DialogWindow::addLine(string text)
{
	mDialogOptions->addItem(new ListboxTextItem(text));
	mTextLines.push_back(text);
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
}

void DialogWindow::setCallback(string function)
{
}

void DialogWindow::show()
{
}

void DialogWindow::hide()
{
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
			int pos = text.find("%"+(*var).first+"%");
			int size = 2+(*var).first.length();
			if (pos != CEGUI::String::npos)
				text = text.replace(pos, size, (*var).second);
		}
		mDialogOptions->getListboxItemFromIndex(line)->setText(text);
	}
	
	
}

}