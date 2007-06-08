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
#include "GameSettings.h"

#include <boost/bind.hpp>
#include <elements/CEGUIFrameWindow.h>
#include <elements/CEGUIListboxTextItem.h>
#include <iostream>

#include "SoundManager.h"
#include "SoundDriver.h"

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

        centerWindow();
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
        return true;
    }

    bool GameSettings::onCancel()
    {
        return true;
    }

    bool GameSettings::onSoundDriverChanged()
    {
        return true;
    }
}
