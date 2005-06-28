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

#ifndef __ListenerObject_H__
#define __ListenerObject_H__

#include "CorePrerequisites.h"
#include "ActorControlledObject.h"

/// @TODO SharedPtr
namespace rl {

    class ListenerMovable;
    
    
    /** Klasse für alle Listenerobjekte, die man an einen Aktor
     *  heften kann. Zwischen Actor und ActorControlledObject
     *  besteht eine 1:1-Beziehung.
     */
    class _RlCoreExport ListenerObject : public ActorControlledObject
    {
    private:
        static ListenerObject* gActiveListener;
        
    public:
        ListenerObject(ListenerMovable *listener);
        virtual ~ListenerObject();
        
        /**
         * Interne Methode. Wird vom Aktor aufgerufen, wenn sich dessen
         * Status geändert hat. (Position, Orientierung, etc)
         * Die Standardimplementierung macht nichts, kann aber
         * von abgeleiteten Klassen überschrieben werden.
         */
        virtual void _update();
        
        /// Wie ActorControlledObject::getMovableObject()
        /// Nur schon gebrauchsfertig gecastet.
        ListenerMovable* getListener();
        
        virtual Ogre::String getObjectType();

		virtual bool isMeshObject();
        
    };
}
#endif
