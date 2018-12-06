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
#include "Fmod4ConfigComponent.h"

#include <boost/bind.hpp>

#include "Exception.h"
#include "Fmod4Driver.h"
#include "SoundDriver.h"
#include "SoundManager.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl
{
    Fmod4ConfigComponent::Fmod4ConfigComponent()
      : SoundDriverConfigComponent("fmod4.xml", "FMOD Ex")
    {
    }

    Fmod4ConfigComponent::~Fmod4ConfigComponent()
    {
    }

    void Fmod4ConfigComponent::apply()
    {
    }

    void Fmod4ConfigComponent::resetToDefaults()
    {
    }

    void Fmod4ConfigComponent::readDriverData()
    {
    }
}
