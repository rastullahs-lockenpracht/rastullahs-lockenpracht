/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#include "CeGuiWindow.h"

#include "InputManager.h"
#include "UiSubsystem.h"
#include "WindowManager.h"

using namespace std;
using namespace CEGUI;

namespace rl
{

int CeGuiWindow::sNumWindows = 0;

CeGuiWindow::CeGuiWindow(const char* xmlfile, bool withInput)
{
	mNamePrefix = StringConverter::toString(sNumWindows);
	sNumWindows++;

    mWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout(
			(utf8*)"modules/common/gui/windows/"+CeGuiString((utf8*)xmlfile), 
			mNamePrefix);
	assert(mWindow != 0);

	mIsVisible = true;
    mWithInput = withInput;

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
        {
            show();
        }
        else
        {
            hide();
        }
    }
}

void CeGuiWindow::show()
{
	if (!mIsVisible)
	{
		if (!beforeShow())
			return;

		if (isInputWindow())
			InputManager::getSingleton().registerCeguiWindow(this);
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
		if (isInputWindow())
			InputManager::getSingleton().unregisterCeguiWindow(this);
        mIsVisible = false;
	}
}

bool CeGuiWindow::isInputWindow()
{
	return mWithInput;
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

Window* CeGuiWindow::getWindow(const char* name)
{
	return CEGUI::WindowManager::getSingleton().getWindow(mNamePrefix + (utf8*)name);
}

Editbox* CeGuiWindow::getEditbox(const char* name)
{
	return reinterpret_cast<Editbox*>(getWindow(name));
}

Listbox* CeGuiWindow::getListbox(const char* name)
{
	return reinterpret_cast<Listbox*>(getWindow(name));
}

StaticText* CeGuiWindow::getStaticText(const char* name)
{
	return reinterpret_cast<StaticText*>(getWindow(name));
}

StaticImage* CeGuiWindow::getStaticImage(const char* name)
{
	return reinterpret_cast<StaticImage*>(getWindow(name));
}

MultiColumnList* CeGuiWindow::getMultiColumnList(const char* name)
{
	return reinterpret_cast<MultiColumnList*>(getWindow(name));
}

MultiLineEditbox* CeGuiWindow::getMultiLineEditbox(const char* name)
{
	return reinterpret_cast<MultiLineEditbox*>(getWindow(name));
}

const CeGuiString& CeGuiWindow::getName() const
{
	return mName;
}

void CeGuiWindow::centerWindow()
{
	Point pos(mWindow->getRelativeWidth()/2, mWindow->getRelativeHeight()/2);
	mWindow->setPosition(Relative, pos);
}

}

