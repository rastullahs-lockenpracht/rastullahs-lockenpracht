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
#ifndef FMOD3CONFIG_H_
#define FMOD3CONFIG_H_

#include "CeGuiWindow.h"

namespace rl
{
	class Fmod3Driver;

	class Fmod3Config : public rl::CeGuiWindow
	{
	private:
		bool handleOK();
		bool handleDriverChanged();
		bool handleDeviceChanged();
		bool handleSpeakerChanged();
		
		// Die Combobox, aus der gewählt wird.
		CEGUI::Combobox* mDriver;
		CEGUI::Combobox* mDevice;
		CEGUI::Combobox* mSpeaker;

		// Der Fmod3-Driver
		Fmod3Driver *mFmod3Driver;
		// War der Treiber vorher aktiv.
		bool mWasActive;

	public:
		Fmod3Config();
		virtual ~Fmod3Config();
	};

}

#endif /* FMOD3CONFIG_H_*/
#endif
