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
#ifndef __LandmarkPath_H__
#define __LandmarkPath_H__

#include "AiPrerequisites.h"

namespace rl
{
	class Landmark;

	class _RlAiExport LandmarkPath
	{
	public:
		LandmarkPath(const Ogre::String& name);
		~LandmarkPath();

		void addPoint(const Ogre::String& name, const Ogre::Vector3& position);
		void addPoint(Landmark* lm);

	private:
		std::list<Landmark*> mPoints;
		Ogre::String mName;
	};
}

#endif //__LandmarkPath_H__
