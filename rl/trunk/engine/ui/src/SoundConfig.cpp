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
#include "Fmod3Config.h"
#include "Fmod3Driver.h"
#include "NullDriver.h"
#include "OalDriver.h"
#include "SoundDriver.h"
#include "SoundManager.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl
{

	SoundConfig::SoundConfig()
		: CeGuiWindow("soundconfig.xml", WND_ALL_INPUT, true),
		mBox(0),
		mVolumeSound(0),
		mVolumeMusic(0),
		mVolumeMaster(0)
	{
		bindCloseToCloseButton();
		bindClickToCloseWindow(getPushButton("SoundConfig/Cancel"));

		getWindow("SoundConfig/OK")->subscribeEvent(
			Window::EventMouseClick, 
			boost::bind(&SoundConfig::handleOK, this));
            
		getWindow("SoundConfig/ConfigDriver")->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&SoundConfig::handleConfig, this));	

		mVolumeSound = getSlider("SoundConfig/VolumeSound");
		mVolumeSound->setMaxValue(100);
		mVolumeSound->setCurrentValue(	
			SoundManager::getSingleton()
			.getActiveDriver()
			->getDefaultSoundVolume());
		mVolumeSound->subscribeEvent(
			Slider::EventValueChanged,
			boost::bind(&SoundConfig::handleVolumeSoundChanged, this));

		mVolumeMusic = getSlider("SoundConfig/VolumeMusic");
		mVolumeMusic->setMaxValue(100);
		mVolumeMusic->setCurrentValue(	
			SoundManager::getSingleton()
			.getActiveDriver()
			->getDefaultMusicVolume());
		mVolumeMusic->subscribeEvent(
			Slider::EventValueChanged,
			boost::bind(&SoundConfig::handleVolumeMusicChanged, this));

		mVolumeMaster = getSlider("SoundConfig/VolumeMaster");
		mVolumeMaster->setMaxValue(100);
		mVolumeMaster->setCurrentValue(	
			SoundManager::getSingleton()
			.getActiveDriver()
			->getMasterVolume());
		mVolumeMaster->subscribeEvent(
			Slider::EventValueChanged,
			boost::bind(&SoundConfig::handleVolumeMasterChanged, this));

		mBox = getListbox("SoundConfig/Table");
		DriverList list = 
			SoundManager::getSingleton().getSoundDriverList();
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
			SoundDriver *activeDriver = SoundManager::getSingleton().getActiveDriver();        
			if (item->getText() != activeDriver->getName())
			{
				// Nicht der aktive Treiber, also ändern.
				DriverList list = SoundManager::getSingleton().getSoundDriverList();
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
					SoundManager::getSingleton().setActiveDriver(searched);
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
			dynamic_cast<ListboxTextItem*>(mBox->getFirstSelectedItem());
		if (item != 0)
		{
			if (item->getText() == NullDriver::NAME)
			{
				//showMessage("Hier gibt es noch nichts zu konfigurieren");
				return true;
			}
#ifdef WITH_FMOD3
			if (item->getText() == Fmod3Driver::NAME)
			{
				Fmod3Config *config = new Fmod3Config();
				config->setVisible(true);
				return true;
			}
#endif
#ifdef WITH_OAL
			if (item->getText() == OalDriver::NAME)
			{
				//OalConfig *config = new OalConfig();
				//config->setVisible(true);
				return true;
			}
#endif
			return true;
		}
		return true;
	}

}
