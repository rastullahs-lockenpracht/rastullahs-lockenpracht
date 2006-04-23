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

#ifndef __PhysicalObject_H__
#define __PhysicalObject_H__

#include "CorePrerequisites.h"

#include "ActorControlledObject.h"

namespace rl {

	class _RlCoreExport PhysicalObject : public ActorControlledObject
    {
    public:
		virtual Ogre::Vector3 getDefaultSize() const = 0;
		virtual Ogre::Vector3 getPoseSize(const Ogre::String& animationName) = 0;
		virtual Ogre::Entity* getEntity() const = 0;
    };
}
#endif
