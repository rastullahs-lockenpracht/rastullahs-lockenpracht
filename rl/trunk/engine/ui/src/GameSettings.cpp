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

#include "GameSettings.h"

#include <boost/bind.hpp>
#include <elements/CEGUIFrameWindow.h>
#include <elements/CEGUIListboxTextItem.h>
#include <iostream>

#include "SoundManager.h"
#include "SoundDriver.h"

using namespace Ogre;

namespace rl
{
    GameSettings::GameSettings()
        : AbstractWindow("gamesettings.xml", WIT_MOUSE_INPUT)
    {
        // Register sound driver components
        std::list<SoundDriver*> soundDriverList = SoundManager::getSingleton().getDriverList();

        for (std::list<SoundDriver*>::iterator it = soundDriverList.begin();
             it != soundDriverList.end(); it++)
        {
            if ((*it)->isDriverPlugin())
            {
                registerSoundDriverConfig((*it)->getConfigComponent());
            }
        }

        // Connect actions to callbacks
        getWindow()->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,
                                    boost::bind(&GameSettings::onCancel, this));

        mVideoRenderer = getCombobox("GameOptionsWindow/Video/Renderer");
        mVideoResolution = getCombobox("GameOptionsWindow/Video/Resolution");
        mVideoFullscreen = getCheckbox("GameOptionsWindow/Video/Fullscreen");
        centerWindow();
        setVisible(false);
        update();
    }

    GameSettings::~GameSettings()
    {
    }

    void GameSettings::registerSoundDriverConfig(SoundDriverConfigComponent* scc)
    {
        // Add component to list
        mSoundDriverConfigs.push_back(scc);

        getCombobox("GameOptionsWindow/Sound/TableSoundDriver")->addItem(
            new CEGUI::ListboxTextItem(scc->getDriverName(),
            getCombobox("GameOptionsWindow/Sound/TableSoundDriver")->getItemCount(),
            scc));
    }

    void GameSettings::registerRenderSystemConfig(RenderSystemConfigComponent* rcc)
    {
    }

    void GameSettings::setVisible(bool visible, bool destroyAfterHide)
    {
        AbstractWindow::setVisible(visible, destroyAfterHide);
    }

    void GameSettings::setConfigComponent(const CEGUI::String& drivername)
    {
        // Search the sound configuration component list
        for (std::list<SoundDriverConfigComponent*>::iterator it = mSoundDriverConfigs.begin();
             it != mSoundDriverConfigs.end(); it++)
        {
            SoundDriverConfigComponent* cmp = *it;
            if (drivername == cmp->getDriverName() && mCurrentSoundDriverConfig != cmp)
            {
                if (mCurrentSoundDriverConfig != NULL)
                {
                    mCurrentSoundDriverConfig->setVisible(false);
                }
                cmp->setVisible(true);
                mCurrentSoundDriverConfig = cmp;
                return;
            }
        }

        // Search the graphic configuration component list
        for (std::list<RenderSystemConfigComponent*>::iterator it = mRenderSystemConfigs.begin();
             it != mRenderSystemConfigs.end(); it++)
        {
            RenderSystemConfigComponent* cmp = *it;
            if (drivername == cmp->getDriverName() && mCurrentRenderSystemConfig != cmp)
            {
                if (mCurrentRenderSystemConfig != NULL)
                {
                    mCurrentRenderSystemConfig->setVisible(false);
                }
                cmp->setVisible(true);
                mCurrentRenderSystemConfig = cmp;
                return;
            }
        }
    }

    bool GameSettings::onApply()
    {
        hideWindow();
        return true;
    }

    bool GameSettings::onCancel()
    {
        hideWindow();
        return true;
    }

    bool GameSettings::onSoundDriverChanged()
    {
        return true;
    }
    
    void GameSettings::update()
    {
        Root* root = Ogre::Root::getSingletonPtr();
        RenderSystem* renderer = root->getRenderSystem();
        
        ConfigOptionMap config = renderer->getConfigOptions();
        
		ConfigOptionMap::iterator cfi;
		
		cfi = config.find( "Full Screen" );
		if( cfi != config.end() )
		{
			mVideoFullscreen->setSelected(cfi->second.currentValue == "Yes");
		}
        
/*		cfi = config.find( "FSAA" );
		if( cfi != config.end() )
		{
			if( cfi->second.currentValue == "0" )
			{
				SetControlValue( iFSAARef, 1 );
			}
			else if( cfi->second.currentValue == "2" )
			{
				SetControlValue( iFSAARef, 2 );
			}
			else if( cfi->second.currentValue == "4" )
			{
				SetControlValue( iFSAARef, 3 );
			}
			else if( cfi->second.currentValue == "6" )
			{
				SetControlValue( iFSAARef, 4 );
			}
		}
        
		cfi = config.find( "Colour Depth" );
		if( cfi != config.end() )
		{
			if( cfi->second.currentValue == "32" )
			{
				SetControlValue( iColorDepthRef, 1 );
			}
			else
			{
				SetControlValue( iColorDepthRef, 2 );
			}
		}*/
    }
}
