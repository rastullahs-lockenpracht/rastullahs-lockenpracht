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

#ifndef __ScriptObjectMarker_H__
#define __ScriptObjectMarker_H__

#include "ScriptPrerequisites.h"

#include <OgreSingleton.h>

#include <map>

#include <OgreNoMemoryMacros.h>
#include "FixRubyHeaders.h"
#include <ruby.h>
#include "FixRubyHeaders.h"
#include <OgreMemoryMacros.h>

#include "ScriptWrapper.h"


namespace rl {
    class _RlScriptExport ScriptObjectMarker : protected Ogre::Singleton<ScriptObjectMarker>, 
        protected ScriptWrapperInstance
    {
    public:
        ScriptObjectMarker();
        ~ScriptObjectMarker();

        static ScriptObjectMarker& getSingleton();
        static ScriptObjectMarker* getSingletonPtr();

        virtual void owned( void* ptr );
        virtual void disowned( void* ptr );
        virtual void deleted( void* ptr );
    private:
        typedef std::map<VALUE,unsigned int> ValueCountMap;
        typedef std::pair<VALUE,unsigned int> ValueCountPair;

        ValueCountMap m_RubyRefCountMap;
        VALUE mRubyArray;
    };
}

#endif
