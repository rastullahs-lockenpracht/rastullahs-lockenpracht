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
#include <boost/bind.hpp>
#include "UiPrerequisites.h"
#include "Exception.h"
#include "CEGUI.h"
#include "Fmod3Config.h"
#include "MultimediaSubsystem.h"
#include "SoundDriver.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl
{

Fmod3Config::Fmod3Config()
    : CeGuiWindow("fmod3.xml", WND_ALL_INPUT, true),
      mDriver(0),
	  mDevice(0)
{
    bindCloseToCloseButton();
    bindClickToCloseWindow(getPushButton("Fmod3Config/Cancel"));

    getWindow("Fmod3Config/OK")->subscribeEvent(
            Window::EventMouseClick, 
            boost::bind(&Fmod3Config::handleOK, this));

	mDriver = getComboDropList("Fmod3Config/Driver");
	mDriver->subscribeEvent(
		Combobox::EventTextChanged,
		boost::bind(&Fmod3Config::handleDriverChanged, this));

	mDevice = getComboDropList("Fmod3Config/Device");
	mDevice->subscribeEvent(
		Combobox::EventTextChanged,
		boost::bind(&Fmod3Config::handleDeviceChanged, this));
	
    centerWindow();
}

Fmod3Config::~Fmod3Config()
{
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

}

#endif
