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

#ifndef __PHYSICSGENERICCONTACTCALLBACK_H__
#define __PHYSICSGENERICCONTACTCALLBACK_H__

#include "CorePrerequisites.h"

#ifdef __APPLE__
#   include <OgreNewt/OgreNewt.h>
#else
#   include <OgreNewt.h>
#endif


namespace rl {

    class _RlCoreExport PhysicsGenericContactCallback : public OgreNewt::ContactCallback
    {
    public:
        int userBegin();
        int userProcess( Ogre::Real timeStep, int threadid);
        /// This function calls the contact listeners. So any specialised listener overriding
        /// this one, has to call it explicitly.
        void userEnd();
    private:
    };
}

#endif
