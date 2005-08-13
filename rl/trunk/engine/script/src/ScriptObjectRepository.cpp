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

#include "ScriptObjectRepository.h"

#include "Exception.h"

template<> rl::ScriptObjectRepository* Ogre::Singleton<rl::ScriptObjectRepository>::ms_Singleton = 0;

namespace rl {

    ScriptObjectRepository& ScriptObjectRepository::getSingleton()
    {
        return Ogre::Singleton<ScriptObjectRepository>::getSingleton();
    }

    ScriptObjectRepository* ScriptObjectRepository::getSingletonPtr()
    {
        return Ogre::Singleton<ScriptObjectRepository>::getSingletonPtr();
    }

    ScriptObjectRepository::ScriptObjectRepository() :
        m_CToRubyMap(),
        m_RubyToCMap(),
        m_RubyRefCountMap()
    {
        // Einen Ruby Array erzeugen
        mRubyArray = rb_ary_new();
        // Als globale Variable anmelden
        rb_global_variable(&mRubyArray);
        // Beim GC registrieren
        rb_gc_register_address(&mRubyArray);
        // Diesen Array einer globalen Variable zuordnen
        rb_define_variable("$UsedRubyInstances", &mRubyArray);

        // Beim ActorManager registrieren
        DeletionPropagator::getSingleton().setDeletionListener( this );
    }

    ScriptObjectRepository::~ScriptObjectRepository()
    {
        DeletionPropagator::getSingleton().setDeletionListener( 0 );
        rb_gc_unregister_address(&mRubyArray);
        m_CToRubyMap.clear();
        m_RubyToCMap.clear();
    }

    VALUE ScriptObjectRepository::getScriptObject( void* ptr ) const
    {
        PointerValueMap::const_iterator iter = m_CToRubyMap.find( ptr );

        if( iter != m_CToRubyMap.end() )
            return iter->second;
        else
            return Qnil;
    }

    void* ScriptObjectRepository::getPtr( VALUE val ) const
    {
        ValuePointerMap::const_iterator iter = m_RubyToCMap.find( val );

        if( iter != m_RubyToCMap.end() )
            return iter->second;
        else
            return NULL;
    }

    void ScriptObjectRepository::insertPointerValuePair( void* ptr, VALUE& val )
    {
        if( m_CToRubyMap.find( ptr ) != m_CToRubyMap.end() )
            Throw( InvalidArgumentException, "Dieser Zeiger existiert schon im ScriptObjectRepository" );
        if( m_RubyToCMap.find( val ) != m_RubyToCMap.end() )
            Throw( InvalidArgumentException, "Diese Ruby VALUE existiert schon im ScriptObjectRepository" );
        if( val == Qnil )
            // Nil muss nicht ins ScriptObjectRepository eingefügt werden
            return;
        if( ptr == NULL )
            // Null muss nicht ins ScriptObjectRepository eingefügt werden
            return;

        m_CToRubyMap.insert( PointerValuePair( ptr, val ) );
        m_RubyToCMap.insert( ValuePointerPair( val, ptr ) );

        rb_ary_push( mRubyArray, val );
    }

    void ScriptObjectRepository::incRefCount( VALUE& val )
    {
        ValueCountMap::iterator iter = m_RubyRefCountMap.find( val );
        unsigned int refCount = 1;
        if( iter != m_RubyRefCountMap.end() )         
            refCount = iter->second + 1;

        m_RubyRefCountMap.insert( ValueCountPair( val, refCount ) ); 
        if( refCount == 1 )
        {
            rb_gc_register_address( &val );
        }
    }

    void ScriptObjectRepository::decRefCount( VALUE& val )
    {
        ValueCountMap::iterator iter = m_RubyRefCountMap.find( val );
        if( iter == m_RubyRefCountMap.end() )   
            Throw( InvalidArgumentException, "Für diese VALUE existiert kein RefCount im ScriptObjectRepository" );
        
        unsigned int refCount = iter->second;

        if( refCount > 0 )
            m_RubyRefCountMap.insert( ValueCountPair(val, refCount-1) ); 
        
        if( refCount <= 1 )
        {
            rb_ary_delete(mRubyArray, val );
            rb_gc_unregister_address( &val );
        }
    }

    void ScriptObjectRepository::removePointerValuePair( void* ptr, VALUE& val )
    {
        // Aus den Maps entfernen
        m_CToRubyMap.erase( ptr );
        m_RubyToCMap.erase( val );

        // Aus dem RubyArray löschen
        rb_ary_delete(mRubyArray, val );

        RData* test = RDATA( val );
        /// @todo altes klass löschen? Wie denn nur, ist nen VALUE
        test->basic.klass = rb_cNilClass;
        // Nicht löschen, ist der ptr
        test->data = NULL;
    }

    void ScriptObjectRepository::removePointer( void* ptr )
    {
        PointerValueMap::iterator iter = m_CToRubyMap.find( ptr );
        if( iter == m_CToRubyMap.end() )
            Throw( InvalidArgumentException, "Dieser Zeiger existiert nicht im ScriptObjectRepository" );

        VALUE val = iter->second;
        removePointerValuePair( ptr, val );
    }

    void ScriptObjectRepository::removeValue( VALUE val )
    {
        ValuePointerMap::iterator iter = m_RubyToCMap.find( val );
        if( iter == m_RubyToCMap.end() )
            Throw( InvalidArgumentException, "Diese Ruby VALUE existiert nicht im ScriptObjectRepository" );

        void* ptr = iter->second;
        removePointerValuePair( ptr, val );
    }

    void ScriptObjectRepository::pointerDeleted( void* ptr )
    {
        try
        {
            removePointer( ptr );
        }
        catch( InvalidArgumentException& iae ) {}
    }

}