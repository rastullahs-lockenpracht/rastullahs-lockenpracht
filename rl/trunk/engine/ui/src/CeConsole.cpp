#include <boost/bind.hpp>

#include <CEGUI.h>
#include "UiSubsystem.h"
#include "InputManager.h"
#include "Interpreter.h"
#include "CeConsole.h"
#include "DebugWindow.h"

template<> rl::CeConsole* Singleton<rl::CeConsole>::ms_Singleton = 0;

using CEGUI::WindowManager; using CEGUI::utf8; using CEGUI::ListboxTextItem;
using CEGUI::KeyEventArgs; using CEGUI::Key; using CEGUI::colour;

namespace rl 
{
	CeConsole& CeConsole::getSingleton()
    {
        return Singleton<CeConsole>::getSingleton();
    }
	CeConsole* CeConsole::getSingletonPtr()
    {
        return Singleton<CeConsole>::getSingletonPtr();
    }

	CeConsole::CeConsole() : CeGuiWindow("console.xml", true)
	{
		using namespace CEGUI;
		
		mDisplay = getListbox("Console/Display");
		mCommandLine = getEditbox("Console/Inputbox");

		mWindow->subscribeEvent(
			FrameWindow::KeyDownEvent, 
			boost::bind(&CeConsole::handleKeyDown, this, _1));
		mCommandLine->subscribeEvent(
			Editbox::KeyDownEvent, 
			boost::bind(&CeConsole::handleKeyDown, this, _1));
		mCommandLine->subscribeEvent(
			Editbox::TextAcceptedEvent, 
			boost::bind(&CeConsole::handleEnter, this, _1));
		mDisplay->moveToFront();

		mHistory.clear();
		mWindow->hide();
		mState = CS_CLOSED;
		
		addToRoot(mWindow);	
	}

	bool CeConsole::beforeShow()
	{
		mCommandLine->activate();
		return true;
	}


	bool CeConsole::isOpen() const
	{
		return mState == CS_OPEN || mState == CS_OPENING;
	}

	void CeConsole::handleKeyDown(const CEGUI::EventArgs& e)
	{
		KeyEventArgs ke = static_cast<const KeyEventArgs&>(e);
		if (ke.scancode == Key::Escape || ke.scancode == Key::F11)
		{
			hide();
		}
		else if (ke.scancode == Key::ArrowDown)
			cycleHistory(1);
		else if (ke.scancode == Key::ArrowUp)
			cycleHistory(-1);
	}

	void CeConsole::handleEnter(const CEGUI::EventArgs& e)
	{	
		CEGUI::String command = mCommandLine->getText();
		CEGUI::String printCommand = ">" + command;
		appendTextRow(printCommand, 0xFF7FFF7F);
				
		if(mInterpreter != 0)
			mPrompt = mInterpreter->execute(command.c_str());
			//if (mPrompt)...

		mHistory.push_back(command.c_str());
		mHistoryMarker = mHistory.size();
		mCommandLine->setText((utf8*)"");
	}

	void CeConsole::write(String output)
	{
        CEGUI::String temp = CEGUI::String(output);		
		appendTextRow(temp, 0xFF7F7F7F);
		LogManager::getSingleton().logMessage(output);
	}

	void CeConsole::appendTextRow(CEGUI::String& text, const colour color)
	{
		const float MIN_SPACE_POS = 0.5;

		CEGUI::String textLeft = CEGUI::String(text);
		const CEGUI::Font* font = mDisplay->getFont();
		unsigned int width = /*mDisplay->getAbsoluteWidth()*/500*0.95;

		while (textLeft.length() > 0)
		{
			CEGUI::String textLine;

			if (font->getTextExtent(textLeft) > width)
			{
				unsigned int numLastChar = font->getCharAtPixel(textLeft, width);
				unsigned int numSpace = textLeft.find_last_of(" \t\n", numLastChar);

				if (numSpace == CEGUI::String::npos || numSpace < MIN_SPACE_POS*numLastChar)
				{
					textLine = textLeft.substr(0, numLastChar);
					textLeft = textLeft.substr(numLastChar);
				}
				else
				{
					textLine = textLeft.substr(0, numSpace);
					textLeft = textLeft.substr(numSpace+1);
				}
			}
			else
			{
				textLine = textLeft;
				textLeft = "";
			}

			ListboxTextItem* item = new ListboxTextItem(textLine);
			item->setTextColours(color);
			mDisplay->addItem(item);
			mDisplay->ensureItemIsVisible(item); // scroll to bottom;
		}
	}
	
	void CeConsole::setInterpreter(Interpreter* interpreter)
	{
		mInterpreter = interpreter;
	}

	Interpreter* CeConsole::getInterpreter()
	{
		return mInterpreter;
	}

	void CeConsole::cycleHistory(int skip)
	{
		if (mHistory.size() == 0)
			return;
		
		mHistoryMarker += skip;

		if (mHistoryMarker < 0)
			mHistoryMarker = 0;
		else if (mHistoryMarker > mHistory.size())
			mHistoryMarker = mHistory.size();

		if (mHistoryMarker == mHistory.size())
			mCommandLine->setText((utf8*)"");
		else
			mCommandLine->setText(mHistory[mHistoryMarker]);
	}
}

