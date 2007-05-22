/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#include "Landmark.h"

namespace rl
{
	Landmark::Landmark(const Ogre::String &name, const Ogre::Vector3 &position)
		: mName(name), mPosition(position)
	{
	}

    const Ogre::String& Landmark::getName() const
    {
        return mName;
    }

    const Ogre::Vector3& Landmark::getPosition() const
    {
        return mPosition;
    }
}
