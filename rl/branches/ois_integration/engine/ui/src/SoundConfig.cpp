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
#include "SoundConfig.h"

#include <boost/bind.hpp>
#include "Exception.h"
#include "SoundDriver.h"
#include "SoundDriverConfigComponent.h"
#include "SoundManager.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl
{

	SoundConfig::SoundConfig()
		: CeGuiWindow("soundconfig.xml", WND_ALL_INPUT),
		mDriverBox(NULL),
		mVolumeSound(0),
		mVolumeMusic(0),
		mVolumeMaster(0),
		mCurrentConfig(NULL)
	{
		getWindow()->subscribeEvent(
			FrameWindow::EventCloseClicked,
			boost::bind(&SoundConfig::handleClose, this));
		getPushButton("SoundConfig/Cancel")->subscribeEvent(
			PushButton::EventMouseClick,
			boost::bind(&SoundConfig::handleClose, this));

		getWindow("SoundConfig/OK")->subscribeEvent(
			Window::EventMouseClick, 
			boost::bind(&SoundConfig::handleOK, this));
            
		mVolumeSound = getSlider("SoundConfig/VolumeSound");
		mVolumeSound->setMaxValue(1.0);
		mVolumeSound->setCurrentValue(	
			SoundManager::getSingleton()
			.getActiveDriver()
			->getDefaultSoundVolume());
		mVolumeSound->subscribeEvent(
			Slider::EventValueChanged,
			boost::bind(&SoundConfig::handleVolumeSoundChanged, this));

		mVolumeMusic = getSlider("SoundConfig/VolumeMusic");
		mVolumeMusic->setMaxValue(1.0);
		mVolumeMusic->setCurrentValue(	
			SoundManager::getSingleton()
			.getActiveDriver()
			->getDefaultMusicVolume());
		mVolumeMusic->subscribeEvent(
			Slider::EventValueChanged,
			boost::bind(&SoundConfig::handleVolumeMusicChanged, this));

		mVolumeMaster = getSlider("SoundConfig/VolumeMaster");
		mVolumeMaster->setMaxValue(1.0);
		mVolumeMaster->setCurrentValue(	
			SoundManager::getSingleton()
			.getActiveDriver()
			->getMasterVolume());
		mVolumeMaster->subscribeEvent(
			Slider::EventValueChanged,
			boost::bind(&SoundConfig::handleVolumeMasterChanged, this));

		mDriverBox = getCombobox("SoundConfig/Table");
		DriverList list = 
			SoundManager::getSingleton().getSoundDriverList();
		DriverList::const_iterator it;
		for (it = list.begin(); it != list.end(); it++)
		{
			CeGuiString name = (*it)->getName();
			mDriverBox->addItem(new ListboxTextItem(name));
		}
		mDriverBox->subscribeEvent(
			Combobox::EventListSelectionAccepted,
			boost::bind(&SoundConfig::handleConfig, this));

		mDriverConfig = getWindow("SoundConfig/DriverSettings");

		centerWindow();
	}

	SoundConfig::~SoundConfig()
	{
	}

	bool SoundConfig::handleOK()
	{
		ListboxTextItem *item = 
			dynamic_cast<ListboxTextItem*>(mDriverBox->getSelectedItem());
		if (item != 0)
		{
			SoundDriver *activeDriver = SoundManager::getSingleton().getActiveDriver();        
			if (item->getText() != activeDriver->getName())
			{
				// Nicht der aktive Treiber, also ändern.
                SoundDriver *driver = SoundManager::getSingleton().getDriverByName(item->getText().c_str());
                if (driver != NULL)
				{
					SoundManager::getSingleton().setActiveDriver(driver);
				}
			}
		}
		setVisible(false);
		destroyWindow();
		return true;
	}

	bool SoundConfig::handleVolumeMusicChanged()
	{
		SoundManager::getSingleton()
			.getActiveDriver()
			->setDefaultMusicVolume(mVolumeMusic->getCurrentValue());
		return true;
	}

	bool SoundConfig::handleVolumeSoundChanged()
	{
		SoundManager::getSingleton()
			.getActiveDriver()
			->setDefaultSoundVolume(mVolumeSound->getCurrentValue());
		return true;
	}

	bool SoundConfig::handleVolumeMasterChanged()
	{
		SoundManager::getSingleton()
			.getActiveDriver()
			->setMasterVolume(mVolumeMaster->getCurrentValue());
		return true;
	}

	bool SoundConfig::handleConfig()
	{
		ListboxTextItem *item = 
			dynamic_cast<ListboxTextItem*>(mDriverBox->getSelectedItem());
		if (item != 0)
		{
			for (list<SoundDriverConfigComponent*>::iterator it = mDriverConfigs.begin();
				it != mDriverConfigs.end(); it++)
			{
				SoundDriverConfigComponent* curr = *it;
				if (item->getText() == curr->getDriverName()
					&& mCurrentConfig != curr)
				{
					if (mCurrentConfig != NULL)
					{
						mCurrentConfig->setVisible(false);
					}
					curr->setVisible(true);
					mCurrentConfig = curr;
					return true;
				}
			}
		}
		return false;
	}

	void SoundConfig::registerDriverConfig(SoundDriverConfigComponent* wnd)
	{
		mDriverConfigs.push_back(wnd);
        wnd->addTo(mDriverConfig);
	}

	bool SoundConfig::handleClose()
	{
		setVisible(false);
		return true;
	}

}
