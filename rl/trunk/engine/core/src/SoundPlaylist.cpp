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

#include "SoundPlaylist.h"
#include "SoundObject.h"
#include "SoundChannel.h"
#include "GameLoop.h"

namespace rl
{

SoundPlaylist::SoundPlaylist()
{
}

SoundPlaylist::~SoundPlaylist()
{
}

void SoundPlaylist::run( Ogre::Real elapsedTime )
{
    bool destroy = false;
    if (!mQueue.empty())
    {
        if (mQueue.front()->getSoundChannel()->isPaused())
        {
            delete mQueue.front();
            mQueue.pop();
            if (!mQueue.empty())
            {
                mQueue.front()->play();
            } else {
                destroy = true;
            }
        }
    } else {
        destroy = true;
    }
    if (destroy)
    {
        GameLoopManager::getSingleton().removeAsynchronousTask(this);
        delete this;
    }
}

void SoundPlaylist::add(SoundObject *object)
{
    mQueue.push(object);
}

}
