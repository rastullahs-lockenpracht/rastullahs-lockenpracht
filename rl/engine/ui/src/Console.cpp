/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h" //precompiled header

#include "ConfigurationManager.h"
#include "Console.h"

#include <CEGUI/widgets/FrameWindow.h>
#include <CEGUI/widgets/ListboxTextItem.h>

#include "CoreSubsystem.h"
#include "InputManager.h"
#include "Job.h"
#include "JobScheduler.h"
#include "RubyInterpreter.h"

using namespace Ogre;

using CEGUI::Colour;
using CEGUI::Key;
using CEGUI::KeyEventArgs;
using CEGUI::ListboxTextItem;

namespace rl
{
    Console::Console()
        : AbstractWindow("console.xml", WIT_KEYBOARD_INPUT)
    {
        using namespace CEGUI;

        mDisplay = getListbox("Console/Display");
        mDisplay->setShowVertScrollbar(true);
        mCommandLine = getEditbox("Console/Inputbox");

        mWindow->subscribeEvent(FrameWindow::EventKeyDown, &Console::handleKeyDown, this);
        mCommandLine->subscribeEvent(Editbox::EventKeyDown, &Console::handleKeyDown, this);
        mWindow->subscribeEvent(FrameWindow::EventKeyUp, &Console::handleKeyUp, this);
        mCommandLine->subscribeEvent(Editbox::EventKeyUp, &Console::handleKeyUp, this);
        mWindow->subscribeEvent(
            FrameWindow::EventCloseClicked, &AbstractWindow::hideWindow, static_cast<AbstractWindow*>(this));
        mWindow->subscribeEvent(FrameWindow::EventActivated, &Console::handleActivated, this);

        mWindow->setAlwaysOnTop(true);

        // load history from file
        if (ConfigurationManager::getSingleton().getIntSetting("General", "Save Console History") > 0)
        {
            // load file
            std::ifstream historyFile;
            std::string filename;
            // compare with ConfigurationManager
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
            filename = Ogre::String(::getenv("HOME")) + "/.rastullah/console_history";
#else
            filename = "./modules/config/console_history";
#endif
            historyFile.open(filename.c_str());
            if (!historyFile.good())
            {
                LOG_MESSAGE(Logger::UI, "could not open history file");
            }
            else
            {
                // parse history file
                while (!historyFile.eof())
                {
                    std::string str;
                    std::getline(historyFile, str);
                    if (str != "")
                        mHistory.push_back(str);
                }
            }
        }
        setVisible(false);
    }

    Console::~Console()
    {
        // save history to file
        int lines = ConfigurationManager::getSingleton().getIntSetting("General", "Save Console History");
        if (lines > 0)
        {
            // file
            std::ofstream historyFile;
            std::string filename;
            // compare with ConfigurationManager
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
            filename = Ogre::String(::getenv("HOME")) + "/.rastullah/console_history";
#else
            filename = "./modules/config/console_history";
#endif
            historyFile.open(filename.c_str());
            if (!historyFile.good())
            {
                LOG_MESSAGE(Logger::UI, "could not open history file for writing");
            }
            else
            {
                std::vector<CeGuiString>::reverse_iterator it = mHistory.rbegin();
                while (lines > 0 && it != mHistory.rend())
                {
                    lines--;
                    if ((*it) != "")
                        historyFile << (*it) << std::endl;
                    it++;
                }
            }
        }
    }

    void Console::setVisible(bool visible, bool destroy)
    {
        if (visible)
        {
            mCommandLine->activate();
        }

        AbstractWindow::setVisible(visible, destroy);
    }

    bool Console::handleKeyDown(const CEGUI::EventArgs& e)
    {
        InputManager* im = InputManager::getSingletonPtr();
        static const CEGUI::utf8 NO_CHAR = 0;
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
        else if (ke.scancode == Key::Return)
        {
            CeGuiString command = mCommandLine->getText();
            CeGuiString printCommand = ">" + command;
            appendTextRow(printCommand, 0xFF7FFF7F);

            mPrompt = CoreSubsystem::getSingleton().getRubyInterpreter()->execute(command.c_str());

            mHistory.push_back(command);
            mHistoryMarker = mHistory.size();
            mCommandLine->setText("");
            return true;
        }

        return false;
    }

    bool Console::wantsKeyToRepeat(const int& key)
    {
        InputManager* im = InputManager::getSingletonPtr();
        static const CEGUI::utf8 NO_CHAR = 0;
        if (im->getKeyChar(key, im->getModifierCode()) != NO_CHAR || // keys that should be repeated
            key == CEGUI::Key::ArrowDown || key == CEGUI::Key::ArrowUp || key == CEGUI::Key::Return
            || key == CEGUI::Key::ArrowLeft || key == CEGUI::Key::ArrowRight || key == CEGUI::Key::Backspace
            || key == CEGUI::Key::Delete)
            return true;

        return false;
    }

    bool Console::handleKeyUp(const CEGUI::EventArgs& e)
    {
        // return true for keys handled in keyup
        InputManager* im = InputManager::getSingletonPtr();
        static const CEGUI::utf8 NO_CHAR = 0;
        KeyEventArgs ke = static_cast<const KeyEventArgs&>(e);
        if (im->getKeyChar(ke.scancode, im->getModifierCode()) != NO_CHAR || ke.scancode == CEGUI::Key::ArrowDown
            || ke.scancode == CEGUI::Key::ArrowUp || ke.scancode == CEGUI::Key::Return)
            return true;

        return false;
    }

    void Console::write(const CeGuiString& output)
    {
        CeGuiString temp;
        if (output.substr(output.length() - 2).compare("\n") == 0)
            temp = output.substr(0, output.length() - 1);
        else
            temp = output;

        appendTextRow(temp, 0xFF7F7F7F);
        LOG_MESSAGE2(Logger::UI, output.c_str(), "Console");
    }

    void Console::appendTextRow(const CeGuiString& text, const Colour color)
    {
        const float MIN_SPACE_POS = 0.5;

        CeGuiString textLeft = CeGuiString(text);
        CEGUI::Font* font = const_cast<CEGUI::Font*>(mDisplay->getFont());
        unsigned int width = mDisplay->getPixelSize().d_width * 0.95f;

        while (textLeft.length() > 0)
        {
            CeGuiString textLine;

            if (font->getTextExtent(textLeft) > width)
            {
                unsigned int numLastChar = font->getCharAtPixel(textLeft, width);
                unsigned int numSpace = textLeft.find_last_of(" \t\n", numLastChar);

                if (numSpace == CeGuiString::npos || numSpace < MIN_SPACE_POS * numLastChar)
                {
                    textLine = textLeft.substr(0, numLastChar);
                    textLeft = textLeft.substr(numLastChar);
                }
                else
                {
                    textLine = textLeft.substr(0, numSpace);
                    textLeft = textLeft.substr(numSpace + 1);
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
        // ListboxWrappedTextItem* item = new ListboxWrappedTextItem(text);
        // item->setTextColours(color);
        // item->setTextFormatting(CEGUI::WordWrapLeftAligned);
        // mDisplay->addItem(item);
        // mDisplay->ensureItemIsVisible(item); // scroll to bottom;*/
    }

    void Console::setRubyInterpreter(RubyInterpreter* RubyInterpreter)
    {
        mRubyInterpreter = RubyInterpreter;
    }

    void Console::cycleHistory(int skip)
    {
        if (mHistory.size() == 0)
            return;

        if (mHistoryMarker + skip < 0)
            mHistoryMarker = 0;
        else if (mHistoryMarker + skip > mHistory.size())
            mHistoryMarker = mHistory.size();
        else
            mHistoryMarker = (unsigned int)(mHistoryMarker + skip);

        if (mHistoryMarker == mHistory.size())
            mCommandLine->setText("");
        else
            mCommandLine->setText(mHistory[mHistoryMarker]);
    }

    bool Console::handleActivated(const CEGUI::EventArgs&)
    {
        mCommandLine->activate();
        return false;
    }
}
