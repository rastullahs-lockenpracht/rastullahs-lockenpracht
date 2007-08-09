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
#ifndef __Landmark_H__
#define __Landmark_H__

#include "AiPrerequisites.h"

namespace rl
{
	class _RlAiExport Landmark
	{
	public:
		Landmark(const Ogre::String& name, const Ogre::Vector3& position);
		
		const Ogre::String& getName() const;
		const Ogre::Vector3& getPosition() const;

	private:
		Ogre::String mName;
		Ogre::Vector3 mPosition;
	};
}

#endif //__Landmark_H__
