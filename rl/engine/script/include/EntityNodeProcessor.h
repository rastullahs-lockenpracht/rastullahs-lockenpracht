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
#ifndef __EntityNodeProcessor_H__
#define __EntityNodeProcessor_H__

#include "ScriptPrerequisites.h"

#ifdef __APPLE__
#include <OgreNewt/OgreNewt.h>
#else
#include <OgreNewt.h>
#endif

#include "AbstractMapNodeProcessor.h"

namespace rl
{
    class EntityNodeProcessor : public AbstractMapNodeProcessor
    {
    public:
        EntityNodeProcessor(const Ogre::String& resourcegroup);

        virtual bool processNode(
            const tinyxml2::XMLElement* nodeElem, const Ogre::String& resourceGroup, bool loadGameObjects);

    private:
        Ogre::String mResourceGroup;

        void createCollision(Ogre::Entity* entity, Ogre::String meshFile, const tinyxml2::XMLElement* physicsProxyElem);
        void processAnimation(Ogre::Entity* entity, const tinyxml2::XMLElement* animationElem);
    };
}

#endif // __EntityNodeProcessor_H__
