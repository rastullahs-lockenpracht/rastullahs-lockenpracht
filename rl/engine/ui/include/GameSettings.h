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

#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include "AbstractWindow.h"
#include "RenderSystemConfigComponent.h"
#include "SoundDriverConfigComponent.h"

namespace rl
{
    class GameSettings : AbstractWindow
    {
    public:
        /**
         * Constructs a new GameSettings object.
         *
         * Creates the connections between callback functions and CEGUI
         * widgets.
         */
        GameSettings();
        ~GameSettings();

        void registerSoundDriverConfig(SoundDriverConfigComponent* scc);
        void registerRenderSystemConfig(RenderSystemConfigComponent* rcc);

        void setVisible(bool visible, bool destroyAfterHide = false);

    private:
        /**
         * Helper function, which sets the configuration component for a
         * given driver name
         *
         * Searchs both ConfigComponent lists and activates the correct
         * ConfigComponent
         *
         * @param drivername Name of the driver to set the ConfigComponent for
         */
        void setConfigComponent(const CEGUI::String& drivername);

        /**
         * Is called if the user clicks the Apply button
         *
         * Collects all settings and passes those on to the
         * different subsystems like CoreSubsystem or Ogre::RenderSystem.
         */
        bool onApply();

        /**
         * Is called if the user clicks the Cancel button
         *
         * No changes made to any of the configuration pages will be saved.
         */
        bool onCancel();

        /**
         * Is called if the user selects a new sounddriver
         *
         * Initialises the selected sound driver and loads the specific
         * configuration section.
         */
        bool onSoundDriverChanged();

        /**
         * Is called if the user selected a new rendersystem
         *
         * Initialises the selected render system and loads the specific
         * configuration section.
         */
        bool onRenderSystemChanged();

        void update();

        void setOption(
            const Ogre::ConfigOptionMap& configuration, const Ogre::String& option, CEGUI::ToggleButton* checkbox);
        void setOption(
            const Ogre::ConfigOptionMap& configuration, const Ogre::String& option, CEGUI::Combobox* combobox);
        void setOption(const Ogre::ConfigOptionMap& configuration, const Ogre::String& option,
            std::vector<CEGUI::RadioButton*> radioGroup);

        void createElements(CEGUI::Combobox* combobox, size_t count);

        std::list<SoundDriverConfigComponent*>
            mSoundDriverConfigs; //!< List of registered sounddriver configuration components
        SoundDriverConfigComponent*
            mCurrentSoundDriverConfig; //!< Configuration component of currently selected sound driver
        std::list<RenderSystemConfigComponent*>
            mRenderSystemConfigs; //!< List of registered rendersystem configuration components
        RenderSystemConfigComponent*
            mCurrentRenderSystemConfig; //!< Configuration component of currently selected rendersystem

        CEGUI::Combobox* mVideoRenderer;
        CEGUI::Combobox* mVideoResolution;
        CEGUI::ToggleButton* mVideoFullscreen;
        CEGUI::RadioButton *mVideoColorDepth32, *mVideoColorDepth16;
        CEGUI::RadioButton *mVideoFsaa0, *mVideoFsaa2, *mVideoFsaa4, *mVideoFsaa8;
        CEGUI::RadioButton *mVideoRttModeFBO, *mVideoRttModePBuffer, *mVideoRttModeCopy;
    };
}

#endif
