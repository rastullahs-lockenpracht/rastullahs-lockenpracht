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
        /*!
         * this function is called, when the aabbs of two bodies overlap, return 0 to indicate, that no contact calculation
         * should be initiated.
         * ATTENTION: if 0 is returned, the contact-listeners cannot be called!
        */
        virtual int onAABBOverlap( OgreNewt::Body* body0, OgreNewt::Body* body1,  int threadindex) { return 1; }

        /*!
         * process user defined contact forces (etc) here
        */
        virtual void userProcess( OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadid) {}

        //! this function must not be overwritten!
        void contactsProcess( OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadid );
    };
}

#endif
