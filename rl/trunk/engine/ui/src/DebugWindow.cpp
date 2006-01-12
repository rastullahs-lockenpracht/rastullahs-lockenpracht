/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
#include "UiPrerequisites.h"

#include "UiSubsystem.h"
#include "DebugWindow.h"
#include "InputManager.h"
#include "GameLoop.h"

#include "Person.h"
#include "Actor.h"
#include "ActorManager.h"

using namespace CEGUI;
using namespace Ogre;

template<> rl::DebugWindow* Ogre::Singleton<rl::DebugWindow>::ms_Singleton = 0;

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
		
		bindCloseToCloseButton();

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
    
	void DebugWindow::run(Ogre::Real elapsedTime)
	{
		updateFps();
	}

	void DebugWindow::updateFps()
	{
		const RenderTarget::FrameStats& stats = Root::getSingleton().getAutoCreatedWindow()->getStatistics();
		Window *textStats = getWindow("DebugWindow/Statistics");

		Ogre::String textSt = "Current FPS: " + 
			StringConverter::toString(stats.lastFPS)+
			"\nBest/worst/avg FPS: " + 
			StringConverter::toString(stats.bestFPS) + "/" + 
			StringConverter::toString(stats.worstFPS) + "/" + 
			StringConverter::toString(stats.avgFPS)+
			"\nBest/worst Frame times: " + 
			StringConverter::toString(stats.bestFPS) + "/" + 
			StringConverter::toString(stats.worstFPS)+
			"\nTriangle Count: " + 
			StringConverter::toString(stats.triangleCount);

		if (UiSubsystem::getSingleton().getActiveCharacter() != NULL && 
			UiSubsystem::getSingleton().getActiveCharacter()->getActor() != NULL)
		{
			Ogre::Vector3 pos = UiSubsystem::getSingletonPtr()->
				getActiveCharacter()->getActor()->
				_getSceneNode()->getWorldPosition();

			textSt += "\nPlayer Position [ "	
				+ StringConverter::toString(pos.x,1,0,32,std::ios_base::fixed)+", "
				+ StringConverter::toString(pos.y,1,0,32,std::ios_base::fixed)+", "
				+ StringConverter::toString(pos.z,1,0,32,std::ios_base::fixed)+" ]";
		}

        Actor* camActor = ActorManager::getSingleton().getActor("DefaultCamera");
		if( camActor != 0 && camActor->_getSceneNode() != NULL )
		{
            Ogre::Vector3 pos = camActor->_getSceneNode()->getWorldPosition();

			textSt += "\nCamera Position [ "	
				+ StringConverter::toString(pos.x,1,0,32,std::ios_base::fixed)+", "
				+ StringConverter::toString(pos.y,1,0,32,std::ios_base::fixed)+", "
				+ StringConverter::toString(pos.z,1,0,32,std::ios_base::fixed)+" ]";
		}

		textStats->setText(textSt);
	}

	void DebugWindow::setVisible(bool visible)
	{
		if (visible)
			GameLoopManager::getSingleton().addSynchronizedTask(this, FRAME_ENDED);
		else
			GameLoopManager::getSingleton().removeSynchronizedTask(this);
		CeGuiWindow::setVisible(visible);
	}

}
