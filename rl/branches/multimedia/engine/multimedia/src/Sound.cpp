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

#include "Sound.h"
#include <OgreMovableObject.h>
#include <OgreAxisAlignedBox.h>
#include <OgreVector3.h>
#include "Sound.h"
#include "SoundSubsystem.h"
extern "C" {
    #include <fmod.h>
}


using namespace Ogre; 

namespace rl
{

Sound::Sound(const String& name)
 : mName(name)
{
    
}

Sound::Sound(const SoundResourcePtr &soundres)
{
}

Sound::~Sound()
{
}



/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Namen
 */
const String& Sound::getName() const
{
    return mName;
}



};
