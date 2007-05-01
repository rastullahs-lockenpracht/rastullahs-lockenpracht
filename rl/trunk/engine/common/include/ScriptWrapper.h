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

#ifndef __ScriptWrapper_H__
#define __ScriptWrapper_H__

#include "CommonPrerequisites.h"
#include <OgreSingleton.h>

namespace rl {

    class _RlCommonExport ScriptWrapperInstance 
    {
    public:
        virtual void deleted( void* ptr ) = 0;
		virtual void owned( void* ptr ) = 0;
		virtual void disowned( void* ptr ) = 0;
    };

    class _RlCommonExport ScriptWrapper : public Ogre::Singleton<ScriptWrapper>
    {
    public:
        ScriptWrapper();
        virtual ~ScriptWrapper();

        /// Ermöglicht dem Script-Repository benachrichtigt zu werden, wenn
        /// die Löschung eines Actors bevorsteht.
        void setScriptWrapperInstance( ScriptWrapperInstance* list );
		

		void deleted( void* ptr );
		void owned( void* ptr );
        void disowned( void* ptr );
    private:
        ScriptWrapperInstance* m_ScriptWrapperInstance;
    };

}
#endif
