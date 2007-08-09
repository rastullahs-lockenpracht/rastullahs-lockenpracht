/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
