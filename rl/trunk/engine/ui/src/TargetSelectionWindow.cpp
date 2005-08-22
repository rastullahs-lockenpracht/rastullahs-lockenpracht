/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#include <boost/bind.hpp>
#include "UiPrerequisites.h"

#include "UiSubsystem.h"
#include "TargetSelectionWindow.h"
#include "InputManager.h"
#include "GameLoop.h"

#include "Person.h"
#include "Actor.h"
#include "GameObject.h"

using namespace CEGUI;
using namespace Ogre;

template<> rl::TargetSelectionWindow* Ogre::Singleton<rl::TargetSelectionWindow>::ms_Singleton = 0;

namespace rl 
{
	TargetSelectionWindow& TargetSelectionWindow::getSingleton(void)
    {
        return Ogre::Singleton<TargetSelectionWindow>::getSingleton();
    }

	TargetSelectionWindow* TargetSelectionWindow::getSingletonPtr(void)
    {
        return Ogre::Singleton<TargetSelectionWindow>::getSingletonPtr();
    }

	TargetSelectionWindow::TargetSelectionWindow() : 
		CeGuiWindow("targetselectionwindow.xml", WND_MOUSE_INPUT)
	{
		mText = getStaticText("TargetSelectionWindow/Text");
		mText->moveToFront();

		mWindow->subscribeEvent(
			Window::EventMouseMove,
			boost::bind(&TargetSelectionWindow::showObjectDescription, this, _1));

		mWindow->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&TargetSelectionWindow::showObjectActionsWindow, this, _1));
		
		addToRoot(mWindow);
	}

	TargetSelectionWindow::~TargetSelectionWindow()
	{		
    }

	void TargetSelectionWindow::setVisible(bool visible)
	{
		mWindow->setMutedState(!visible);
		CeGuiWindow::setVisible(visible);
	}

	GameObject* TargetSelectionWindow::getTargetedObject(float x, float y)
	{
		InputManager::getSingleton().updatePickedObject(
			x/(float)Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth(), 
			y/(float)Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight());
		return InputManager::getSingleton().getPickedObject();
	}

	bool TargetSelectionWindow::showObjectDescription(const CEGUI::EventArgs& e)
	{
		CEGUI::MouseEventArgs me = static_cast<const CEGUI::MouseEventArgs&>(e);

		GameObject* object = getTargetedObject(me.position.d_x, me.position.d_y);

		if (object == NULL)
			setText("");
		else
			setText(object->getName());
		return true;
	}

	bool TargetSelectionWindow::showObjectActionsWindow(const CEGUI::EventArgs& e)
	{
		CEGUI::MouseEventArgs me = static_cast<const CEGUI::MouseEventArgs&>(e);

		if (mAction == NULL)
		{
			GameObject* object = getTargetedObject(me.position.d_x, me.position.d_y);
			if (object != NULL)
			{
				UiSubsystem::getSingleton().showActionChoice(object);
		        setVisible(false);
			}
		}
		return true;
	}

	void TargetSelectionWindow::setAction(Action* action)
	{
		mAction = action;
	}

	void TargetSelectionWindow::setText(const CeGuiString& description)
	{
		mText->setText(description);
	}
}
