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
#include "stdinc.h" //precompiled header

#include "LinearSoundFadeFunctor.h"

namespace rl
{
    LinearSoundFadeFunctor::LinearSoundFadeFunctor(Ogre::Real duration, bool up)
        : SoundFadeFunctor(duration)
        , mUp(up)
    {
    }

    Ogre::Real LinearSoundFadeFunctor::operator()(Ogre::Real time)
    {
        Ogre::Real factor = time / mDuration;
        if (factor > 1.0)
        {
            return -1; // Done
        }
        else
        {
            return (mUp ? factor : 1.0 - factor);
        }
    }
}
