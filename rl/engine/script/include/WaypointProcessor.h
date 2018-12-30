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
#ifndef __WaypointProcessor_H__
#define __WaypointProcessor_H__

#include "ScriptPrerequisites.h"

#include "AbstractMapNodeProcessor.h"

namespace rl
{
    class WaypointProcessor : private AbstractMapNodeProcessor
    {
    public:
        virtual bool processNode(
            const tinyxml2::XMLElement* nodeElem, const Ogre::String& resourceGroup, bool loadGameObjects);
    };
}

#endif //__WaypointProcessor_H__
