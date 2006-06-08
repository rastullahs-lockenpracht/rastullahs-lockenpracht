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
#ifndef FMOD3CONFIG_H_
#define FMOD3CONFIG_H_

#include "SoundDriverConfigWindow.h"
#include "Fmod3Driver.h"

namespace rl
{
	class Fmod3Config : public SoundDriverConfigWindow
	{
	public:
		Fmod3Config(Fmod3Driver* driver);
		virtual ~Fmod3Config();

	protected:
		Fmod3Driver* getDriver();

	private:
		bool handleOK();
		bool handleDriverChanged();
		bool handleOutputChanged();
		bool handleSpeakerChanged();
		
		// Die Combobox, aus der gewählt wird.
		CEGUI::Combobox* mOutput;
		CEGUI::Combobox* mDriver;
		CEGUI::Combobox* mSpeaker;

		// War der Treiber vorher aktiv.
		bool mWasActive;

		Fmod3Driver* mFmod3Driver;
	};

}

#endif /* FMOD3CONFIG_H_*/
