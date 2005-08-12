/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Perl Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Perl Artistic License for more details.
*
*  You should have received a copy of the Perl Artistic License
*  along with this program; if not you can get it here
*  http://www.perldoc.com/perl5.6/Artistic.html.
*/

#ifndef __SCRIPTOBJECTREPOSITORY_H__
#define __SCRIPTOBJECTREPOSITORY_H__

#include "ScriptPrerequisites.h"

#include <OgreSingleton.h>

#include <map>

#include <OgreNoMemoryMacros.h>
#include "FixRubyHeaders.h"
#include <ruby.h>
#include "FixRubyHeaders.h"
#include <OgreMemoryMacros.h>

#include "ActorManager.h"


namespace rl {
    class ScriptObjectRepository : protected Ogre::Singleton<ScriptObjectRepository>, protected ActorDeletionListener
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

        void insertPointerValuePair( void* ptr, VALUE& val );
        void removePointerValuePair( void* ptr, VALUE& val );

        void incRefCount( VALUE& val );
        void decRefCount( VALUE& val );

        void removePointer( void* ptr );
        void removeValue( VALUE val );

        virtual void actorDeleted( Actor* act );
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
