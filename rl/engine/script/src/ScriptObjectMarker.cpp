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
#include "stdinc.h" //precompiled header

#include "ScriptObjectMarker.h"

extern VALUE RL_RubyInstanceFor(void* ptr);
extern void RL_RubyRemoveTracking(void* ptr);

#include "Exception.h"

template <> rl::ScriptObjectMarker* Ogre::Singleton<rl::ScriptObjectMarker>::msSingleton = nullptr;

namespace rl
{

    ScriptObjectMarker::ScriptObjectMarker()
        : m_RubyRefCountMap()
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
        ScriptWrapper::getSingleton().setScriptWrapperInstance(this);
    }

    ScriptObjectMarker::~ScriptObjectMarker()
    {
        // @todo array leeren
        rb_gc_unregister_address(&mRubyArray);
        m_RubyRefCountMap.clear();

        // Beim ActorManager abmelden
        ScriptWrapper::getSingleton().setScriptWrapperInstance(0);
    }

    void ScriptObjectMarker::owned(void* ptr)
    {
        VALUE val = RL_RubyInstanceFor(ptr);

        // Hat kein Skript Equivalent, muss nicht verwaltet werden
        if (val == Qnil)
            return;

        ValueCountMap::iterator iter = m_RubyRefCountMap.find(val);
        unsigned int refCount = 1;

        // Es gab schon einen RefCount, dann diesen hochzaehlen
        if (iter != m_RubyRefCountMap.end())
            refCount = iter->second + 1;

        // In Liste einfuegen
        m_RubyRefCountMap.insert(ValueCountPair(val, refCount));

        // refCount wurde 1, beim GC anmelden
        if (refCount == 1)
        {
            rb_ary_push(mRubyArray, val);
            rb_gc_register_address(&val);
        }
    }

    void ScriptObjectMarker::disowned(void* ptr)
    {
        VALUE val = RL_RubyInstanceFor(ptr);

        // Hat kein Skript Equivalent, muss nicht verwaltet werden
        if (val == Qnil)
            return;

        ValueCountMap::iterator iter = m_RubyRefCountMap.find(val);
        // Gibt es ueberhaupt noch einen RefCount?
        if (iter == m_RubyRefCountMap.end())
            return;

        // Alter RefCount
        unsigned int refCount = iter->second;

        // Noch nicht 0
        if (refCount > 0)
        {
            // Herunterzaehlen
            refCount = refCount--;
            // Neu einspeichern
            m_RubyRefCountMap.insert(ValueCountPair(val, refCount));
        }
        // Null geworden
        if (refCount <= 1)
        {
            // Austragen aus RefCountliste
            m_RubyRefCountMap.erase(val);
            // Aus dem GC-Array loeschen
            rb_ary_delete(mRubyArray, val);
        }
    }

    void ScriptObjectMarker::deleted(void* ptr)
    {
        try
        {
            VALUE val = RL_RubyInstanceFor(ptr);
            // Hat kein Skript Equivalent, muss nicht verwaltet werden
            if (val == Qnil)
                return;

            // Austragen aus RefCountliste
            m_RubyRefCountMap.erase(val);
            // Aus dem GC-Array loeschen
            rb_ary_delete(mRubyArray, val);

            // Aus dem RubyArray loeschen
            rb_ary_delete(mRubyArray, val);

            RL_RubyRemoveTracking(ptr);

            RData* test = RDATA(val);
            /// @todo altes klass loeschen? Wie denn nur, ist nen VALUE
            const_cast<VALUE&>(test->basic.klass) = rb_cNilClass;
            // Nicht loeschen, ist der ptr
            test->data = nullptr;
        }
        catch (IllegalArgumentException&)
        {
        }
    }
}
