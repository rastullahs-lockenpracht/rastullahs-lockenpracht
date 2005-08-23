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

#include "ObjectStateChangeEventSource.h"
#include "ObjectStateChangeEvent.h"
#include "GameObject.h"

#include "ScriptObjectRepository.h"

namespace rl {

    ObjectStateChangeEventSource::ObjectStateChangeEventSource( GameObject* obj ) :
		mObjectStateChangeEventCaster(),
        mObject(obj)
    {       
    }

	ObjectStateChangeEventSource::ObjectStateChangeEventSource() :
	mObjectStateChangeEventCaster(),
		mObject(NULL)
	{       
	}

    ObjectStateChangeEventSource::~ObjectStateChangeEventSource() 
    {
        mObjectStateChangeEventCaster.removeEventListeners();
    }

    
	void ObjectStateChangeEventSource::fireObjectStateChangeEvent()
	{
		ObjectStateChangeEvent* evt = new ObjectStateChangeEvent(this);
		evt->setProvokingObject(mObject);
		mObjectStateChangeEventCaster.dispatchEvent(evt);
	}

    void ObjectStateChangeEventSource::addObjectStateChangeListener( ObjectStateChangeListener*  list )
    {
        if( !mObjectStateChangeEventCaster.containsListener( list ) )
        {        
            mObjectStateChangeEventCaster.addEventListener( list );
            ScriptObjectRepository::getSingleton().own( list );
        }
    }

    void ObjectStateChangeEventSource::removeObjectStateChangeListener( ObjectStateChangeListener* list )
    {
        if( mObjectStateChangeEventCaster.containsListener( list ) )
        { 
            mObjectStateChangeEventCaster.removeEventListener( list );
            ScriptObjectRepository::getSingleton().own( list );
        }
    }

    void ObjectStateChangeEventSource::removeObjectStateChangeListener(  )
    {
        EventCaster<ObjectStateChangeEvent>::EventSet arSet 
            = mObjectStateChangeEventCaster.getEventSet();
        EventCaster<ObjectStateChangeEvent>::EventSet::iterator iter 
            = arSet.begin();
        for (iter; iter != arSet.end(); ) 
        {
            EventListener<ObjectStateChangeEvent>* ev = *iter; 
            ObjectStateChangeListener* gal = dynamic_cast<ObjectStateChangeListener*>( ev );
            ScriptObjectRepository::getSingleton().disown( gal );
            iter++;
        }
        mObjectStateChangeEventCaster.removeEventListeners();
    }

    bool ObjectStateChangeEventSource::hasListeners( ) const
    {
        return mObjectStateChangeEventCaster.hasEventListeners();
    }
}

