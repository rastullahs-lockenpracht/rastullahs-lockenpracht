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
#include "UiPrerequisites.h"

#include "UiSubsystem.h"
#include "Person.h"
#include "Actor.h"

#include "DebugWindow.h"

#include "InputManager.h"

template<> rl::DebugWindow* Ogre::Singleton<rl::DebugWindow>::ms_Singleton = 0;

using namespace CEGUI;

namespace rl 
{
  DebugWindow& DebugWindow::getSingleton(void)
    {
        return Ogre::Singleton<DebugWindow>::getSingleton();
    }
	DebugWindow* DebugWindow::getSingletonPtr(void)
    {
        return Ogre::Singleton<DebugWindow>::getSingletonPtr();
    }

	DebugWindow::DebugWindow() : CeGuiWindow("debugwindow.xml", WND_SHOW)
	{
		mText = getStaticText("DebugWindow/Text");
		mText->moveToFront();
		
		addToRoot(mWindow);
	}

	DebugWindow::~DebugWindow()
	{		
    }

    void DebugWindow::setText(const Ogre::String& text)
    {
        CeGuiString o(text.c_str());
        mText->setText(o);
    }
    
	bool DebugWindow::frameStarted(const FrameEvent& evt)
	{
		updateFps();
		return true;
	}

	void DebugWindow::updateFps()
	{
		const RenderTarget::FrameStats& stats = Root::getSingleton().getAutoCreatedWindow()->getStatistics();
		Window
			*textStats = getWindow("DebugWindow/Statistics");

		textStats->setText("Current FPS: " + 
			StringConverter::toString(stats.lastFPS)+
			"\nBest/worst/avg FPS: " + 
			StringConverter::toString(stats.bestFPS) + "/" + 
			StringConverter::toString(stats.worstFPS) + "/" + 
			StringConverter::toString(stats.avgFPS)+
			"\nBest/worst Frame times: " + 
			StringConverter::toString(stats.bestFPS) + "/" + 
			StringConverter::toString(stats.worstFPS)+
			"\nTriangle Count: " + 
			StringConverter::toString(stats.triangleCount));

		if (UiSubsystem::getSingleton().getActiveCharacter() != NULL && 
			UiSubsystem::getSingleton().getActiveCharacter()->getActor() != NULL)
		{
			Ogre::Vector3 pos = UiSubsystem::getSingletonPtr()->
			    getActiveCharacter()->getActor()->
			        _getSceneNode()->getWorldPosition();
//			setText("Player-Position "+StringConverter::toString(pos));
		}
	}

	void DebugWindow::setVisible(bool visible)
	{
		if (visible)
			Root::getSingleton().addFrameListener(this);
		else
			Root::getSingleton().removeFrameListener(this);
		CeGuiWindow::setVisible(visible);
	}

}
