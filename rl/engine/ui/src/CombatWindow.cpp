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

#include "CombatWindow.h"
#include "ContentModule.h"
#include "CoreSubsystem.h"
#include "MessagePump.h"
#include "UiMessages.h"

#include <CEGUI/WindowManager.h>

using namespace CEGUI;
using namespace Ogre;

namespace rl
{

    CombatWindow::CombatWindow()
        : AbstractWindow("combatwindow.xml", WIT_MOUSE_INPUT, false)
        , mButtons()
        , mNextHandle(0)
        , mSetSize()
        , mButtonPadding(3)
    {
        int width = 0, height = 0;
        Window* w = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("buttons/attack.xml");
        auto size = w->getPixelSize();
        height = std::max((int)size.d_height, height);
        width += (int)size.d_width + mButtonPadding;
        CEGUI::WindowManager::getSingleton().destroyWindow(w);
        w = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("buttons/parry.xml");
        size = w->getPixelSize();
        height = std::max((int)size.d_height, height);
        width += (int)size.d_width;
        CEGUI::WindowManager::getSingleton().destroyWindow(w);
        mSetSize = SetSize(width, height);

        getWindow("CombatWindow/EndTurnButton")
            ->subscribeEvent(CEGUI::Window::EventMouseClick, &CombatWindow::endTurnButtonClicked, this);
    }

    CombatWindow::~CombatWindow()
    {
        for (ButtonMap::iterator it = mButtons.begin(); it != mButtons.end(); ++it)
        {
            for (size_t i = 0; i < it->second.size(); ++i)
            {
                CEGUI::WindowManager::getSingleton().destroyWindow(it->second[i]);
            }
        }
    }

    CombatWindow::SetSize CombatWindow::getButtonSetSize()
    {
        return mSetSize;
    }

    int CombatWindow::addEnemyButtonSet()
    {
        CeGuiString prefix = CeGuiString("_") + StringConverter::toString(mNextHandle).c_str();
        ButtonVector buttons;
        Window* attackButton = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("buttons/attack.xml", prefix);
        mWindow->addChild(attackButton);
        buttons.push_back(attackButton);
        Window* parryButton = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("buttons/parry.xml", prefix);
        mWindow->addChild(parryButton);
        buttons.push_back(parryButton);
        mButtons.insert(std::make_pair(mNextHandle, buttons));

        attackButton->subscribeEvent(CEGUI::Window::EventMouseClick,
            boost::bind(&CombatWindow::enemyButtonClicked, this, mNextHandle, ATTACK_BUTTON));
        parryButton->subscribeEvent(CEGUI::Window::EventMouseClick,
            boost::bind(&CombatWindow::enemyButtonClicked, this, mNextHandle, PARRY_BUTTON));

        return mNextHandle++;
    }

    void CombatWindow::removeEnemyButtonSet(int handle)
    {
        ButtonMap::iterator it = mButtons.find(handle);
        if (it != mButtons.end())
        {
            for (size_t i = 0; i < it->second.size(); ++i)
            {
                CEGUI::WindowManager::getSingleton().destroyWindow(it->second[i]);
            }
            mButtons.erase(it);
        }
    }

    void CombatWindow::placeEnemyButtonSet(int handle, int xstart, int y)
    {
        ButtonMap::iterator it = mButtons.find(handle);
        if (it != mButtons.end())
        {
            int x = xstart;
            for (size_t i = 0; i < it->second.size(); ++i)
            {
                it->second[i]->setPosition(UVector2(UDim(0, x), UDim(0, y)));
                x += it->second[i]->getPixelSize().d_width + mButtonPadding;
            }
        }
    }

    void CombatWindow::enableEnemyButtonSet(int handle, bool enabled)
    {
        ButtonMap::iterator it = mButtons.find(handle);
        if (it != mButtons.end())
        {
            for (size_t i = 0; i < it->second.size(); ++i)
            {
                it->second[i]->setVisible(enabled);
            }
        }
    }

    void CombatWindow::enableAllEnemyButtonSets(bool enabled)
    {
        for (ButtonMap::iterator it = mButtons.begin(); it != mButtons.end(); ++it)
        {
            enableEnemyButtonSet(it->first, enabled);
        }
    }

    bool CombatWindow::enemyButtonClicked(int handle, int buttonIndex)
    {
        MessagePump::getSingleton().sendMessage<MessageType_CombatIoEnemyButtonClicked>(handle, buttonIndex);
        return true;
    }

    bool CombatWindow::endTurnButtonClicked()
    {
        MessagePump::getSingleton().sendMessage<MessageType_CombatIoEndTurnButtonClicked>();
        return true;
    }

    void CombatWindow::setAttackedOpponentName(const CeGuiString& name)
    {
        getWindow("CombatWindow/AttackedOpponent")->setText(name);
    }

    void CombatWindow::setParriedOpponentName(const CeGuiString& name)
    {
        getWindow("CombatWindow/ParriedOpponent")->setText(name);
    }
}
