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

/**
 * \file GameObjectInfoWindow.cpp
 *
 * Implementation of the GameObject window
 */
#include "GameObjectInfoWindow.h"

#include <CEGUI/WindowManager.h>
#include <boost/bind.hpp>

#include "Action.h"
#include "GameObject.h"
#include "Item.h"

using namespace CEGUI;

namespace rl
{

    //------------------------------------------------------- Constructor

    GameObjectInfoWindow::GameObjectInfoWindow(GameObject* go, Creature* character)
        : AbstractWindow("gameobjectinfowindow.xml", WIT_MOUSE_INPUT)
        , mGameObject(go)
        , mCharacter(character)
        , mNumActionButtons(0)
    {
        mDescription = getMultiLineEditbox("GameObjectInfoWindow/Description");
        mIcon = getWindow("GameObjectInfoWindow/Icon");
        mActionButtonsArea = getWindow("GameObjectInfoWindow/ActionButtons");

        centerWindow();

        bindDestroyWindowToXButton();
        // bindDestroyWindowToClick(getWindow("GameObjectInfoWindow/CloseButton"));

        initialize();
    }

    //------------------------------------------------------- Destructor

    GameObjectInfoWindow::~GameObjectInfoWindow()
    {
    }

    //------------------------------------------------------- initialize
    void GameObjectInfoWindow::initialize()
    {
        mWindow->setProperty("Text", "Eigenschaften von " + mGameObject->getName());
        mDescription->setText(mGameObject->getDescription());

        Item* item = dynamic_cast<Item*>(mGameObject);
        if (item != NULL && item->getImageName() != "")
        {
            mIcon->setProperty("Image", item->getImageName());
        }

        ///@todo: Add Action Buttons
        const ActionVector actions = mGameObject->getValidActions(mCharacter);
        for (ActionVector::const_iterator it = actions.begin(); it != actions.end(); it++)
        {
            if ((*it)->getName() != GameObject::DEFAULT_VIEW_OBJECT_ACTION
                && (*it)->getName() != GameObject::DEFAULT_VIEW_OBJECT_ACTION_DEBUG)
            {
                addActionButton(*it);
            }
        }
    }

    void GameObjectInfoWindow::addActionButton(Action* action)
    {
        static int BUTTON_HEIGHT = 15;
        Window* btn = CEGUI::WindowManager::getSingleton().createWindow("RastullahLook/Button");
        btn->setSize(USize(cegui_reldim(1), cegui_absdim(BUTTON_HEIGHT)));
        btn->setPosition(UVector2(cegui_reldim(0), cegui_absdim((BUTTON_HEIGHT + 1) * mNumActionButtons)));
        btn->setText(action->getDescription());
        btn->setTooltipText(action->getDescription());
        mActionButtonsArea->addChild(btn);
        mNumActionButtons++;
        btn->subscribeEvent(
            Window::EventMouseClick, boost::bind(&GameObjectInfoWindow::handleActivateAction, this, action));
    }

    bool GameObjectInfoWindow::handleActivateAction(Action* action) const
    {
        if (action->canDo(mGameObject, mCharacter))
        {
            action->doAction(mGameObject, mCharacter);
            return true;
        }
        return false;
    }

} // namespace rl
