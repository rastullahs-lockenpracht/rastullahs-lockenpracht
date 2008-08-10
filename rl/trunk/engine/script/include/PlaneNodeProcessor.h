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
#ifndef __PlaneNodeProcessor_H__
#define __PlaneNodeProcessor_H__

#include <xercesc/dom/DOMElement.hpp>

#include "ScriptPrerequisites.h"

#ifdef __APPLE__
#   include <OgreNewt/OgreNewt.h>
#else
#   include <OgreNewt.h>
#endif

#include "AbstractMapNodeProcessor.h"

#include <OgreMaterial.h>

namespace rl
{
    class PlaneNodeProcessor : public AbstractMapNodeProcessor
    {
    public:
        PlaneNodeProcessor();

        virtual bool processNode(XERCES_CPP_NAMESPACE::DOMElement* nodeElem, bool loadGameObjects);

    private:

        void createCollision(Ogre::Entity* entity, XERCES_CPP_NAMESPACE::DOMElement* physicsProxyElem);
		void createRenderToTextures(Ogre::Entity* entity, XERCES_CPP_NAMESPACE::DOMElement* rttElem);
	};
}

#endif // __PlaneNodeProcessor_H__
