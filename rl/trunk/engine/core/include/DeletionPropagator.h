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

#ifndef __DeletionPropagator_H__
#define __DeletionPropagator_H__

#include "CorePrerequisites.h"
#include <OgreSingleton.h>

namespace rl {

    class _RlCoreExport DeletionListener 
    {
    public:
        virtual void pointerDeleted( void* ptr ) = 0;
    };

    class _RlCoreExport DeletionPropagator : protected Ogre::Singleton<DeletionPropagator>
    {
    public:
        DeletionPropagator();
        virtual ~DeletionPropagator();

        /// Ermöglicht dem Script-Repository benachrichtigt zu werden, wenn
        /// die Löschung eines Actors bevorsteht.
        void setDeletionListener( DeletionListener* list );

        void notifyPointerDeleted( void* ptr );
        /** Returns the Singleton */
        static DeletionPropagator & getSingleton(void);
        static DeletionPropagator * getSingletonPtr(void);  
    private:
        DeletionListener* m_DeletionListener;
    };

}
#endif
