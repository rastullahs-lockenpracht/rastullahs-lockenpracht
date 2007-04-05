/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

/**
 * \file GameObjectInfoWindow.cpp
 *
 * Implementation of the GameObject window
 */
#include "GameObjectInfoWindow.h"

#include <boost/bind.hpp>
#include <CEGUIWindowManager.h>

#include "Action.h"
#include "GameObject.h"
#include "Item.h"

using namespace CEGUI;

namespace rl {

    //------------------------------------------------------- Constructor

	GameObjectInfoWindow::GameObjectInfoWindow(GameObject* go, Creature* character) 
	 :  AbstractWindow("gameobjectinfowindow.xml", WND_MOUSE_INPUT),
		mGameObject(go),
		mCharacter(character),
		mNumActionButtons(0)
	{
		mDescription = getMultiLineEditbox("GameObjectInfoWindow/Description");
		mIcon = getWindow("GameObjectInfoWindow/Icon");
        mActionButtonsArea = getWindow("GameObjectInfoWindow/ActionButtons");

        centerWindow();

        bindDestroyWindowToXButton();
        //bindDestroyWindowToClick(getWindow("GameObjectInfoWindow/CloseButton"));

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
		try 
		{
			Item* item = dynamic_cast<Item*>(mGameObject);
			if (item->getImageName() != "")
			{
				mIcon->setProperty("Image", item->getImageName());
			}
		}
		catch (std::bad_cast)
		{
			//and forget, mGameObject simply is no item in this case
		}

		
		///@todo: Add Action Buttons
		const ActionVector actions = mGameObject->getValidActions(mCharacter);
		for (ActionVector::const_iterator it = actions.begin(); it != actions.end(); it++)
		{
			addActionButton(*it);
		}
	}

	void GameObjectInfoWindow::addActionButton(Action* action)
	{
		static int BUTTON_HEIGHT = 15;
		Window* btn = CEGUI::WindowManager::getSingleton().createWindow("RastullahLook/Button");
		btn->setSize(UVector2(cegui_reldim(1), cegui_absdim(BUTTON_HEIGHT)));
		btn->setPosition(UVector2(cegui_reldim(0), cegui_absdim((BUTTON_HEIGHT+1) * mNumActionButtons)));
		btn->setText(action->getDescription());
		mActionButtonsArea->addChildWindow(btn);
		mNumActionButtons++;
	}

} // namespace rl