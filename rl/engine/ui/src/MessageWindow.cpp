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

#include "CEGUI/PropertyHelper.h"
#include "MessageWindow.h"

#include <boost/bind.hpp>

using namespace CEGUI;

namespace rl
{

    MessageWindow::MessageWindow(const CeGuiString& title, const CeGuiString& text, const Buttons buttons)
        : AbstractWindow("messagewindow.xml", WIT_MOUSE_INPUT)
    {
        mTextField = getWindow("MessageWindow/Text");
        centerWindow();

        setTitle(title);
        setText(text);
        setButtons(getButtonTexts(buttons));
    }

    void MessageWindow::setText(const CeGuiString& message)
    {
        mTextField->setText(message);
    }

    void MessageWindow::setTitle(const rl::CeGuiString& title)
    {
        mWindow->setText(title);
    }

    const CeGuiStringVector MessageWindow::getButtonTexts(const MessageWindow::Buttons buttons) const
    {
        CeGuiStringVector rval;
        if (buttons == OK)
        {
            rval.push_back("Okay");
        }
        else if (buttons == YES_NO)
        {
            rval.push_back("Ja");
            rval.push_back("Nein");
        }
        return rval;
    }

    bool MessageWindow::onButtonClicked(int button, const rl::CeGuiString& text)
    {
        return true;
    }

    void MessageWindow::setButtons(const CeGuiStringVector& texts)
    {
        for (unsigned int btnCount = 1; btnCount <= 3; ++btnCount)
        {
            Window* buttonPanel = getWindow(
                ("MessageWindow/ButtonPanel_" + CEGUI::PropertyHelper<unsigned int>::toString(btnCount)).c_str());

            if (btnCount == texts.size())
            {
                buttonPanel->setVisible(true);
                mButtonPanel = buttonPanel;
            }
            else
            {
                buttonPanel->setVisible(false);
            }
        }

        for (size_t btnIdx = 0; btnIdx < texts.size(); ++btnIdx)
        {
            Window* btn = mButtonPanel->getChildAtIdx(btnIdx);
            btn->setText(texts[btnIdx]);
            btn->subscribeEvent(
                Window::EventMouseClick, boost::bind(&MessageWindow::onButtonClicked, this, btnIdx, texts[btnIdx]));
        }
    }
}
