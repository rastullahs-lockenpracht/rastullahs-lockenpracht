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

#include "InfoPopup.h"
#include "WindowManager.h"

using namespace CEGUI;

namespace rl
{

    InfoPopup::InfoPopup()
        : AbstractWindow("infopopup.xml", AbstractWindow::WIT_NONE, false)
    {
        mErrorIcon = getWindow("InfoPopup/ErrorIcon");
        mQuestIcon = getWindow("InfoPopup/QuestIcon");
        mWindow->removeChild(mErrorIcon);
        mWindow->removeChild(mQuestIcon);
    }

    InfoPopup::~InfoPopup()
    {
    }

    void InfoPopup::showError()
    {
        showMessageIcon(mErrorIcon);
    }

    void InfoPopup::showQuestBookChange()
    {
        showMessageIcon(mQuestIcon);
    }

    void InfoPopup::showMessageIcon(Window* image)
    {
        int iconWidth = 0;
        for (std::vector<Window*>::iterator it = mActiveIcons.begin(); it != mActiveIcons.end(); it++)
        {
            Window* cur = *it;
            iconWidth += cur->getPixelSize().d_width;
        }
        iconWidth += ICON_SPACING * mActiveIcons.size();

        mWindow->addChild(image);
        image->setXPosition(cegui_absdim(iconWidth));

        mWindow->setWidth(cegui_absdim(iconWidth + ICON_SPACING + image->getPixelSize().d_width));

        mActiveIcons.push_back(image);
        setVisible(true);
    }

    void InfoPopup::windowHid()
    {
        for (std::vector<Window*>::iterator it = mActiveIcons.begin(); it != mActiveIcons.end(); it++)
        {
            Window* cur = *it;
            mWindow->removeChild(cur);
        }

        mActiveIcons.clear();
    }

    void InfoPopup::setVisible(bool visible, bool destroyAfterHide)
    {
        AbstractWindow::setVisible(visible, destroyAfterHide);
        ///@todo implement the following, when timed jobs are possible.
        // if (visible && !isVisible())
        //{
        //	WindowManager::getSingleton()._fadeInOut(this, 0.25f, 2.0f);
        //}
    }

    void InfoPopup::questStateChanged(QuestEvent* anEvent)
    {
        showQuestBookChange();
    }

    void InfoPopup::questPartsDoneChanged(QuestEvent* anEvent)
    {
        showQuestBookChange();
    }

    void InfoPopup::questKnownChanged(QuestEvent* anEvent)
    {
        showQuestBookChange();
    }

    void InfoPopup::questSubquestAdded(QuestEvent* anEvent)
    {
        showQuestBookChange();
    }

    void InfoPopup::journalEntryAdded(JournalEvent* anEvent)
    {
        showQuestBookChange();
    }

    void InfoPopup::journalEntryDeleted(JournalEvent* anEvent)
    {
        showQuestBookChange();
    }
}
