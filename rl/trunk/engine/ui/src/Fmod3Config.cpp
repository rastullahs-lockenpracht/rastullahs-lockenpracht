/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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

#ifdef WITH_FMOD3

#include "Fmod3Config.h"

#include <boost/bind.hpp>
#include <fmod.h>

#include "Exception.h"
#include "Fmod3Driver.h"
#include "NullDriver.h"
#include "SoundDriver.h"
#include "SoundManager.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl
{

	Fmod3Config::Fmod3Config()
		: CeGuiWindow("fmod3.xml", WND_ALL_INPUT, true),
		  mDriver(0),
		  mDevice(0),
		  mWasActive(false),
		  mFmod3Driver(0)
	{
		bindCloseToCloseButton();
		bindClickToCloseWindow(getPushButton("Fmod3Config/Cancel"));

		getWindow("Fmod3Config/OK")->subscribeEvent(
				Window::EventMouseClick, 
				boost::bind(&Fmod3Config::handleOK, this));

		mDriver = getCombobox("Fmod3Config/Driver");
		mDriver->subscribeEvent(
			Combobox::EventTextChanged,
			boost::bind(&Fmod3Config::handleDriverChanged, this));

		mDevice = getCombobox("Fmod3Config/Device");
		mDevice->subscribeEvent(
			Combobox::EventTextChanged,
			boost::bind(&Fmod3Config::handleDeviceChanged, this));
		
		mSpeaker = getCombobox("Fmod3Config/Speaker");
		mSpeaker->subscribeEvent(
			Combobox::EventTextChanged,
			boost::bind(&Fmod3Config::handleSpeakerChanged, this));

		mFmod3Driver = dynamic_cast<Fmod3Driver*>(
			SoundManager::getSingleton().getDriverByName(
				Ogre::String(Fmod3Driver::NAME.c_str()))
		);

		RlAssert(mFmod3Driver != NULL, "Fmod3 Treiber nicht vorhanden");
		if (mFmod3Driver == SoundManager::getSingleton().getActiveDriver())
		{
			SoundManager::getSingleton().setActiveDriver(
				SoundManager::getSingleton().getDriverByName(
					Ogre::String(NullDriver::NAME.c_str()))
			);
			mWasActive = true;
		}
		for (int i = 0; i < FSOUND_GetNumDrivers(); i++)
		{
			ListboxTextItem *item = new ListboxTextItem(
				Ogre::String(FSOUND_GetDriverName(i))
			);
			mDriver->addItem(item);
		}

		centerWindow();
	}

	Fmod3Config::~Fmod3Config()
	{
		if (mWasActive)
		{
			SoundManager::getSingleton().setActiveDriver(mFmod3Driver);
		}
	}

	bool Fmod3Config::handleOK()
	{
		setVisible(false);
		destroyWindow();
		return true;
	}

	bool Fmod3Config::handleDriverChanged()
	{
		return true;
	}

	bool Fmod3Config::handleDeviceChanged()
	{
		return true;
	}

	bool Fmod3Config::handleSpeakerChanged()
	{
		return true;
	}

}

#endif
