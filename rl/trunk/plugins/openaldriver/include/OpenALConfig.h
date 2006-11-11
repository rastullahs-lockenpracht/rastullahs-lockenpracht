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
#ifndef _OpenALConfig_H_
#define _OpenALConfig_H

#include "SoundDriverConfigComponent.h"
#include "OpenALDriver.h"

namespace rl {

	class OpenALConfig : public SoundDriverConfigComponent
	{
	public:
		OpenALConfig(OpenALDriver* driver);
		~OpenALConfig();

		virtual OpenALDriver* getDriver() const;

        virtual void apply();
        virtual void resetToDefaults();
        virtual void readDriverData();

	private:
		OpenALDriver* mDriver;
	};
}

#endif //_OpenALConfig_H_
