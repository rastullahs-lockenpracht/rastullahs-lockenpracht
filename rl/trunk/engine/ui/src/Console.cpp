#include <boost/bind.hpp>

#include <CEGUI.h>
#include "CoreSubsystem.h"
#include "InputManager.h"
#include "Interpreter.h"

#include "Console.h"
#include "DebugWindow.h"
#include "ListboxWrappedTextItem.h"

template<> rl::Console* Singleton<rl::Console>::ms_Singleton = 0;

using CEGUI::WindowManager; using CEGUI::utf8; using CEGUI::ListboxTextItem;
using CEGUI::KeyEventArgs; using CEGUI::Key; using CEGUI::colour;
using CEGUI::ListboxWrappedTextItem; using CEGUI::TextFormatting;

namespace rl 
{
	Console& Console::getSingleton()
    {
        return Singleton<Console>::getSingleton();
    }
	Console* Console::getSingletonPtr()
    {
        return Singleton<Console>::getSingletonPtr();
    }

	Console::Console() : CeGuiWindow("console.xml", true)
	{
		using namespace CEGUI;
		
		mDisplay = getListbox("Console/Display");
		mCommandLine = getEditbox("Console/Inputbox");

		mWindow->subscribeEvent(
			FrameWindow::EventKeyDown, 
			boost::bind(&Console::handleKeyDown, this, _1));
		mCommandLine->subscribeEvent(
			Editbox::EventKeyDown, 
			boost::bind(&Console::handleKeyDown, this, _1));
		mCommandLine->subscribeEvent(
			Editbox::EventTextAccepted, 
			boost::bind(&Console::handleEnter, this, _1));
		mDisplay->moveToFront();

		mHistory.clear();
        setVisible(false);
		addToRoot(mWindow);	
	}

	bool Console::beforeShow()
	{
		mCommandLine->activate();
		return true;
	}

	bool Console::handleKeyDown(const CEGUI::EventArgs& e)
	{
		KeyEventArgs ke = static_cast<const KeyEventArgs&>(e);
        if (ke.scancode == Key::ArrowDown)
		{
			cycleHistory(1);
			return true;
		}
		else if (ke.scancode == Key::ArrowUp)
		{
			cycleHistory(-1);
			return true;
		}
		
		return false;		
	}

	bool Console::handleEnter(const CEGUI::EventArgs& e)
	{	
		CeGuiString command = mCommandLine->getText();
		CeGuiString printCommand = ">" + command;
		appendTextRow(printCommand, 0xFF7FFF7F);
				
        mPrompt = CoreSubsystem::getSingleton().getInterpreter()->execute(command.c_str());

		mHistory.push_back(command.c_str());
		mHistoryMarker = mHistory.size();
		mCommandLine->setText((utf8*)"");

		return true;
	}

	void Console::write(String output)
	{
        CeGuiString temp = CeGuiString(output);		
		appendTextRow(temp, 0xFF7F7F7F);
		LogManager::getSingleton().logMessage(output);
	}

	void Console::appendTextRow(CeGuiString& text, const colour color)
	{
		/*const float MIN_SPACE_POS = 0.5;

		CeGuiString textLeft = CeGuiString(text);
		const CEGUI::Font* font = mDisplay->getFont();
		unsigned int width = mDisplay->getAbsoluteWidth()*0.95;

		while (textLeft.length() > 0)
		{
			CeGuiString textLine;

			if (font->getTextExtent(textLeft) > width)
			{
				unsigned int numLastChar = font->getCharAtPixel(textLeft, width);
				unsigned int numSpace = textLeft.find_last_of(" \t\n", numLastChar);

				if (numSpace == CeGuiString::npos || numSpace < MIN_SPACE_POS*numLastChar)
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
		}*/
		ListboxWrappedTextItem* item = new ListboxWrappedTextItem(text);
		item->setTextColours(color);
		item->setTextFormatting(CEGUI::WordWrapLeftAligned);
		mDisplay->addItem(item);
		mDisplay->ensureItemIsVisible(item); // scroll to bottom;*/
	}
	
	void Console::setInterpreter(Interpreter* interpreter)
	{
		mInterpreter = interpreter;
	}

	Interpreter* Console::getInterpreter()
	{
		return mInterpreter;
	}

	void Console::cycleHistory(int skip)
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

