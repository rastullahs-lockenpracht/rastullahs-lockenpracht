/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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
#include "LandmarkPath.h"

#include "AiSubsystem.h"

namespace rl
{
	LandmarkPath::LandmarkPath(const Ogre::String& name)
		: mName(name)
	{
	}

	LandmarkPath::~LandmarkPath()
	{
		mPoints.clear();
	}

	void LandmarkPath::addPoint(const Ogre::String &name, const Ogre::Vector3 &position)
	{
		addPoint(AiSubsystem::getSingleton().createLandmark(name, position));
	}

	void LandmarkPath::addPoint(Landmark *lm)
	{
		mPoints.push_back(lm);
	}

    LandmarkPath::LandmarkList LandmarkPath::getPoints()
    {
        return mPoints;
    }
}
