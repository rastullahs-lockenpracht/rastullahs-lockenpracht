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

#ifndef __SCRIPTOBJECTREPOSITORY_H__
#define __SCRIPTOBJECTREPOSITORY_H__

#include "CorePrerequisites.h"

#include <OgreSingleton.h>

#include <map>

#include <OgreNoMemoryMacros.h>
#include "FixRubyHeaders.h"
#include <ruby.h>
#include "FixRubyHeaders.h"
#include <OgreMemoryMacros.h>

#include "DeletionPropagator.h"


namespace rl {
    class _RlCoreExport ScriptObjectRepository : protected Ogre::Singleton<ScriptObjectRepository>, 
        protected DeletionListener
    {
    public:
        ScriptObjectRepository();
        ~ScriptObjectRepository();

        static ScriptObjectRepository& getSingleton();
        static ScriptObjectRepository* getSingletonPtr();

        /** Gibt das zugehörige RubyObject zu einem eingetragenen
         *  C++ Object zurück.
         */
        VALUE getScriptObject( void* ptr ) const;
        /** Gibt das zugehörige C++ Object zu einem eingetragenen
         *   RubyObject zurück.
         */
        void* getPtr( VALUE val ) const;

        void insertPointerValuePair( void* ptr, VALUE& val, bool shouldOwn );
        void removePointerValuePair( void* ptr, VALUE& val, bool rbOverwrite );

        void own( void* ptr );
        void disown( void* ptr );

        void removePointer( void* ptr );
        void removeValue( VALUE val );

        virtual void pointerDeleted( void* ptr );
    private:
        typedef std::map<void*,VALUE> PointerValueMap;
        typedef std::pair<void*,VALUE> PointerValuePair;
        typedef std::map<VALUE,void*> ValuePointerMap;
        typedef std::pair<VALUE,void*> ValuePointerPair;
        typedef std::map<VALUE,unsigned int> ValueCountMap;
        typedef std::pair<VALUE,unsigned int> ValueCountPair;

        PointerValueMap m_CToRubyMap;
        ValuePointerMap m_RubyToCMap;
        ValueCountMap m_RubyRefCountMap;

        VALUE mRubyArray;
    };
}

#endif
