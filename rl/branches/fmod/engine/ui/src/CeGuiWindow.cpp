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
#include "Exception.h"

#include "WindowManager.h"
#include "UiSubsystem.h"
#include "CeGuiWindow.h"
#include "InputManager.h"

using namespace std;
using namespace CEGUI;
using namespace Ogre;

namespace rl
{

CeGuiWindow::CeGuiWindow(const char* xmlfile, WindowType type, bool modal)
{
   	mWindow = WindowManager::getSingleton().loadWindow(xmlfile, &mNamePrefix);
	if (mWindow == NULL)
	{
		Throw(rl::IllegalStateException, std::string("Could not load window '")+xmlfile+"'.");
	}
	mWindow->hide();
	
	if (modal)
		mWindow->setAlwaysOnTop(true);

	mIsVisible = false;
	mWindowType = type;
	
	mName = mWindow->getName();
	WindowManager::getSingleton().registerWindow(this);
}

CeGuiWindow::~CeGuiWindow()
{
	CEGUI::WindowManager::getSingleton().destroyWindow(mWindow);
}

bool CeGuiWindow::isVisible()
{
    return mIsVisible;
}

void CeGuiWindow::setVisible(bool visible)
{
    if(mIsVisible != visible)
    {
        if (visible)
            show();
        else
            hide();
    }
}

void CeGuiWindow::show()
{
	if (!mIsVisible)
	{
		if (!beforeShow())
			return;

		InputManager::getSingleton().registerCeGuiWindow(this);

		mWindow->show();
        mIsVisible = true;
    }
}

void CeGuiWindow::hide()
{
	if (mIsVisible)
	{
		if (!beforeHide())
			return;
	
		mWindow->hide();
		
		InputManager::getSingleton().unregisterCeGuiWindow(this);

		mIsVisible = false;
	}
}

CeGuiWindow::WindowType CeGuiWindow::getWindowType()
{
	return mWindowType;
}

bool CeGuiWindow::beforeHide()
{
	return true;
}

bool CeGuiWindow::beforeShow()
{
	return true;
}

void CeGuiWindow::addToRoot(Window* window)
{
	CEGUI::WindowManager::getSingleton().getWindow((utf8*)UiSubsystem::CEGUI_ROOT)->addChildWindow(window);
}

CEGUI::Window* CeGuiWindow::getRoot()
{
	return CEGUI::WindowManager::getSingleton().getWindow((utf8*)UiSubsystem::CEGUI_ROOT);
}

Window* CeGuiWindow::getWindow(const char* name)
{
	return CEGUI::WindowManager::getSingleton().getWindow(mNamePrefix + (utf8*)name);
}

Editbox* CeGuiWindow::getEditbox(const char* name)
{
	return static_cast<Editbox*>(getWindow(name));
}

TabPane* CeGuiWindow::getTabPane(const char* name)
{
	return static_cast<TabPane*>(getWindow(name));
}

Listbox* CeGuiWindow::getListbox(const char* name)
{
	return static_cast<Listbox*>(getWindow(name));
}

StaticText* CeGuiWindow::getStaticText(const char* name)
{
	return static_cast<StaticText*>(getWindow(name));
}

StaticImage* CeGuiWindow::getStaticImage(const char* name)
{
	return static_cast<StaticImage*>(getWindow(name));
}

MultiColumnList* CeGuiWindow::getMultiColumnList(const char* name)
{
	return static_cast<MultiColumnList*>(getWindow(name));
}

MultiLineEditbox* CeGuiWindow::getMultiLineEditbox(const char* name)
{
	return static_cast<MultiLineEditbox*>(getWindow(name));
}

ProgressBar* CeGuiWindow::getProgressBar(const char* name)
{
	return static_cast<ProgressBar*>(getWindow(name));
}

//MenuBase* CeGuiWindow::getMenu(const char* name)
//{
//	return static_cast<MenuBase*>(getWindow(name));
//}

const CeGuiString& CeGuiWindow::getName() const
{
	return mName;
}

void CeGuiWindow::centerWindow()
{
	CEGUI::Point pos(
		(1.0-mWindow->getRelativeWidth())/2, 
		(1.0-mWindow->getRelativeHeight())/2);
	mWindow->setPosition(Relative, pos);
}

void CeGuiWindow::bindClickToCloseWindow(CEGUI::Window* button)
{
	button->subscribeEvent(Window::EventMouseClick,
		boost::bind(&WindowManager::destroyWindow, WindowManager::getSingletonPtr(), this));
}

}
