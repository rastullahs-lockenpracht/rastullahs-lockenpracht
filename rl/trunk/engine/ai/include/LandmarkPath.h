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
#ifndef __LandmarkPath_H__
#define __LandmarkPath_H__

#include "AiPrerequisites.h"

namespace rl
{
	class Landmark;

	class _RlAiExport LandmarkPath
	{
	public:
        typedef std::deque<Landmark*> LandmarkList;

		LandmarkPath(const Ogre::String& name);
		~LandmarkPath();

		void addPoint(const Ogre::String& name, const Ogre::Vector3& position);
		void addPoint(Landmark* lm);
        void removePoint(const Ogre::String& name);
        void removePoint(Landmark* lm);
        
        LandmarkList getPoints() const;
        bool isEmpty() const;
	private:
		LandmarkList mPoints;
		Ogre::String mName;
	};
}

#endif //__LandmarkPath_H__
