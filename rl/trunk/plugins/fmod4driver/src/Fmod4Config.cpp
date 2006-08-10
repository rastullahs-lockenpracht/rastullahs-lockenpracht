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
#include "Fmod4Config.h"

#include <boost/bind.hpp>
#include <fmod.h>

#include "Exception.h"
#include "Fmod4Driver.h"
#include "SoundDriver.h"
#include "SoundManager.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl
{
    Fmod4Config::Fmod4Config(Fmod4Driver *driver)
      : SoundDriverConfigComponent("fmod4.xml"),
        mFmod4Driver(driver)
    {
    }

    Fmod4Config::~Fmod4Config()
    {
    }

    Fmod4Driver* Fmod4Config::getDriver() const
    {
        return mFmod4Driver;
    }

    void Fmod4Config::apply()
    {
    }

    void Fmod4Config::resetToDefaults()
    {
    }

}
