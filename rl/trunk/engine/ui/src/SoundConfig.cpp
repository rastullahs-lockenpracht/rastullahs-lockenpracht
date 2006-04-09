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

#include <boost/bind.hpp>
#include "UiPrerequisites.h"
#include "Exception.h"
#include "CEGUI.h"
#include "SoundConfig.h"
#include "MultimediaSubsystem.h"
#include "SoundDriver.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl
{

SoundConfig::SoundConfig()
    : CeGuiWindow("soundconfig.xml", WND_ALL_INPUT, true),
      mBox(NULL)
{
    bindCloseToCloseButton();
    bindClickToCloseWindow(getPushButton("SoundConfig/Cancel"));

    getWindow("SoundConfig/OK")->subscribeEvent(
            Window::EventMouseClick, 
            boost::bind(&SoundConfig::handleOK, this));

	mVolumeSound = getSlider("SoundConfig/VolumeSound");
	mVolumeSound->setMaxValue(100);
	mVolumeSound->setCurrentValue(	
		MultimediaSubsystem::getSingleton()
			.getActiveDriver()
				->getDefaultSoundVolume());
	mVolumeSound->subscribeEvent(
		Slider::EventValueChanged,
		boost::bind(&SoundConfig::handleVolumeSoundChanged, this));

	mVolumeMusic = getSlider("SoundConfig/VolumeMusic");
	mVolumeMusic->setMaxValue(100);
	mVolumeMusic->setCurrentValue(	
		MultimediaSubsystem::getSingleton()
			.getActiveDriver()
				->getDefaultMusicVolume());
	mVolumeMusic->subscribeEvent(
		Slider::EventValueChanged,
		boost::bind(&SoundConfig::handleVolumeMusicChanged, this));
	
	mVolumeMaster = getSlider("SoundConfig/VolumeMaster");
	mVolumeMaster->setMaxValue(100);
	mVolumeMaster->setCurrentValue(	
		MultimediaSubsystem::getSingleton()
			.getActiveDriver()
				->getMasterVolume());
	mVolumeMaster->subscribeEvent(
		Slider::EventValueChanged,
		boost::bind(&SoundConfig::handleVolumeMasterChanged, this));
	
	mBox = getListbox("SoundConfig/Table");
    DriverList list = 
        MultimediaSubsystem::getSingleton().getSoundDriverList();
    DriverList::const_iterator it;
    for (it = list.begin(); it != list.end(); it++)
    {
        CeGuiString name = (*it)->getName();
        mBox->addItem(new ListboxTextItem(name));
    }
    centerWindow();
}

SoundConfig::~SoundConfig()
{
}

bool SoundConfig::handleOK()
{
    ListboxTextItem *item = 
        dynamic_cast<ListboxTextItem*>(mBox->getFirstSelectedItem());
    if (item != 0)
    {
        SoundDriver *activeDriver = MultimediaSubsystem::getSingleton().getActiveDriver();        
        if (item->getText() != activeDriver->getName())
        {
            // Nicht der aktive Treiber, also ändern.
            DriverList list = MultimediaSubsystem::getSingleton().getSoundDriverList();
            DriverList::const_iterator it;
            SoundDriver *searched = NULL;
            for (it = list.begin(); it != list.end(); it++)
            {
                if (item->getText() == (*it)->getName())
                {
                    searched = *it;
                    break;
                }
            }
            if (searched)
            {
                activeDriver->deInit();
                searched->deInit();
                MultimediaSubsystem::getSingleton().setActiveDriver(searched);
            }
        }
    }
    setVisible(false);
    destroyWindow();
    return true;
}

bool SoundConfig::handleVolumeMusicChanged()
{
	MultimediaSubsystem::getSingleton()
		.getActiveDriver()
			->setDefaultMusicVolume(mVolumeMusic->getCurrentValue());
	return true;
}

bool SoundConfig::handleVolumeSoundChanged()
{
	MultimediaSubsystem::getSingleton()
		.getActiveDriver()
			->setDefaultSoundVolume(mVolumeSound->getCurrentValue());
	return true;
}

bool SoundConfig::handleVolumeMasterChanged()
{
	MultimediaSubsystem::getSingleton()
		.getActiveDriver()
			->setMasterVolume(mVolumeMaster->getCurrentValue());
	return true;
}

}
