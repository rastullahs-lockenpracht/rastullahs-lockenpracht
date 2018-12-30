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
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CEGUI/elements/CEGUIFrameWindow.h>
#include <CEGUI/elements/CEGUIListboxTextItem.h>
#else
#include <CEGUI/widgets/FrameWindow.h>
#include <CEGUI/widgets/ListboxTextItem.h>
#endif

#include <iostream>

#include "SoundDriver.h"
#include "SoundManager.h"

using namespace Ogre;
using namespace CEGUI;
using namespace std;

namespace rl
{
    GameSettings::GameSettings()
        : AbstractWindow("gamesettings.xml", WIT_MOUSE_INPUT)
    {
        // Register sound driver components
        std::list<SoundDriver*> soundDriverList = SoundManager::getSingleton().getDriverList();

        for (std::list<SoundDriver*>::iterator it = soundDriverList.begin(); it != soundDriverList.end(); it++)
        {
            if ((*it)->isDriverPlugin())
            {
                // registerSoundDriverConfig((*it)->getConfigComponent());
            }
        }

        // Connect actions to callbacks
        getWindow()->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, boost::bind(&GameSettings::onCancel, this));

        mVideoRenderer = getCombobox("GameOptionsWindow/Video/Renderer");
        mVideoResolution = getCombobox("GameOptionsWindow/Video/Resolution");
        mVideoFullscreen = getCheckbox("GameOptionsWindow/Video/Fullscreen");

        mVideoColorDepth32 = getRadioButton("GameOptionsWindow/Video/ColorDepth/32");
        mVideoColorDepth32->setUserData(new Ogre::String("32"));
        mVideoColorDepth32->setGroupID(1);
        mVideoColorDepth16 = getRadioButton("GameOptionsWindow/Video/ColorDepth/16");
        mVideoColorDepth16->setUserData(new Ogre::String("16"));
        mVideoColorDepth16->setGroupID(1);

        mVideoFsaa0 = getRadioButton("GameOptionsWindow/Video/FSAA/Off");
        mVideoFsaa0->setUserData(new Ogre::String("0"));
        mVideoFsaa0->setGroupID(2);
        mVideoFsaa2 = getRadioButton("GameOptionsWindow/Video/FSAA/2x");
        mVideoFsaa2->setUserData(new Ogre::String("2"));
        mVideoFsaa2->setGroupID(2);
        mVideoFsaa4 = getRadioButton("GameOptionsWindow/Video/FSAA/4x");
        mVideoFsaa4->setUserData(new Ogre::String("4"));
        mVideoFsaa4->setGroupID(2);
        mVideoFsaa8 = getRadioButton("GameOptionsWindow/Video/FSAA/8x");
        mVideoFsaa8->setUserData(new Ogre::String("8"));
        mVideoFsaa8->setGroupID(2);

        mVideoRttModeFBO = getRadioButton("GameOptionsWindow/Video/RTT/FBO");
        mVideoRttModeFBO->setUserData(new Ogre::String("FBO"));
        mVideoRttModeFBO->setGroupID(3);
        mVideoRttModePBuffer = getRadioButton("GameOptionsWindow/Video/RTT/PBuffer");
        mVideoRttModePBuffer->setUserData(new Ogre::String("PBuffer"));
        mVideoRttModePBuffer->setGroupID(3);
        mVideoRttModeCopy = getRadioButton("GameOptionsWindow/Video/RTT/Copy");
        mVideoRttModeCopy->setUserData(new Ogre::String("Copy"));
        mVideoRttModeCopy->setGroupID(3);

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

        getCombobox("GameOptionsWindow/Sound/TableSoundDriver")
            ->addItem(new CEGUI::ListboxTextItem(
                scc->getDriverName(), getCombobox("GameOptionsWindow/Sound/TableSoundDriver")->getItemCount(), scc));
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

        Ogre::RenderSystem* renderer = root->getRenderSystem();

        const RenderSystemList& renderers = root->getAvailableRenderers();
        createElements(mVideoRenderer, renderers.size());

        for (unsigned int i = 0; i < renderers.size(); ++i)
        {
            Ogre::RenderSystem* cur = renderers[i];
            ListboxItem* item = mVideoRenderer->getListboxItemFromIndex(i);
            item->setText(cur->getName());
            if (cur == renderer)
            {
                mVideoRenderer->setItemSelectState(item, true);
            }
        }

        ConfigOptionMap config = renderer->getConfigOptions();

        setOption(config, "Full Screen", mVideoFullscreen);
        std::vector<RadioButton*> videoColorDepth;
        videoColorDepth.push_back(mVideoColorDepth32);
        videoColorDepth.push_back(mVideoColorDepth16);

        setOption(config, "Colour Depth", videoColorDepth);
        std::vector<RadioButton*> videoAntiAliasing;
        videoAntiAliasing.push_back(mVideoFsaa0);
        videoAntiAliasing.push_back(mVideoFsaa2);
        videoAntiAliasing.push_back(mVideoFsaa4);
        videoAntiAliasing.push_back(mVideoFsaa8);
        setOption(config, "FSAA", videoAntiAliasing);

        std::vector<RadioButton*> videoRttMode;
        videoRttMode.push_back(mVideoRttModeFBO);
        videoRttMode.push_back(mVideoRttModePBuffer);
        videoRttMode.push_back(mVideoRttModeCopy);
        setOption(config, "RTT Preferred Mode", videoRttMode);

        setOption(config, "Video Mode", mVideoResolution);
    }

    void GameSettings::setOption(
        const ConfigOptionMap& configuration, const Ogre::String& option, ToggleButton* checkbox)
    {
        ConfigOptionMap::const_iterator cfi = configuration.find(option);
        if (cfi != configuration.end())
        {
            checkbox->setSelected(cfi->second.currentValue == "Yes");
        }
    }

    void GameSettings::setOption(const ConfigOptionMap& configuration, const Ogre::String& option, Combobox* combobox)
    {
        ConfigOptionMap::const_iterator cfi = configuration.find(option);
        if (cfi != configuration.end())
        {
            ConfigOption curOption = cfi->second;
            int delta = curOption.possibleValues.size() - combobox->getItemCount();
            if (delta > 0)
            {
                for (int i = 0; i < delta; ++i)
                {
                    combobox->addItem(new ListboxTextItem(""));
                }
            }
            else if (delta < 0)
            {
                for (int i = 0; i < -delta; ++i)
                {
                    ListboxItem* item = combobox->getListboxItemFromIndex(combobox->getItemCount() - 1);
                    combobox->removeItem(item);
                    delete item;
                }
            }

            for (unsigned int i = 0; i < combobox->getItemCount(); ++i)
            {
                ListboxItem* item = combobox->getListboxItemFromIndex(i);
                item->setText(curOption.possibleValues[i]);
            }
        }
    }

    void GameSettings::setOption(
        const ConfigOptionMap& configuration, const Ogre::String& option, std::vector<RadioButton*> radioGroup)
    {
        ConfigOptionMap::const_iterator cfi = configuration.find(option);
        if (cfi != configuration.end())
        {
            ConfigOption curOption = cfi->second;
            for (std::vector<RadioButton*>::const_iterator it = radioGroup.begin(); it != radioGroup.end(); ++it)
            {
                Ogre::String* value = static_cast<Ogre::String*>((*it)->getUserData());
                if (value && (*value == curOption.currentValue))
                {
                    (*it)->setSelected(true);
                    break;
                }
            }
        }
    }

    void GameSettings::createElements(CEGUI::Combobox* combobox, size_t count)
    {
        int delta = count - combobox->getItemCount();
        if (delta > 0)
        {
            for (int i = 0; i < delta; ++i)
            {
                combobox->addItem(new ListboxTextItem(""));
            }
        }
        else if (delta < 0)
        {
            for (int i = 0; i < -delta; ++i)
            {
                ListboxItem* item = combobox->getListboxItemFromIndex(combobox->getItemCount() - 1);
                combobox->removeItem(item);
                delete item;
            }
        }
    }
}
