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

#ifndef __ActorControlledObject_H__
#define __ActorControlledObject_H__

#include "CorePrerequisites.h"
#include <OgreMovableObject.h>

namespace rl {

    class Actor;

    /** Abstrakte Basisklasse für alle Objekte, die man zur
     * Visualisierung, Höhrbarmachung an einen Aktor heften
     * kann. Zwischen Actor und ActorControlledObject eine
     * 1:1-Beziehung.
     * 
     * Derzeit werden hier im Wesentlichen die OgreMovables
     * gekapselt, aber auch für Soundquellen ist diese Klasse
     * geeignet. Eigene Klasse, die gekapselt werden sollen
     * müssen von Ogre::MovableObject erben.
     */
    class _RlCoreExport ActorControlledObject
    {
    public:
        ActorControlledObject();
        virtual ~ActorControlledObject();
        
        /** Diese Methode wird intern benutzt, damit
         * der Aktor sich dieses Objekt zu eigen machen
         * kann.
         * @warning Wenn diese Methode überschrieben wird immer
         * auch ActorControlledObject::_setActor() aufrufen.
         */
        virtual void _setActor(Actor* actor);
        virtual Actor* getActor();
        
        virtual void _attachSceneNode(Ogre::SceneNode* node);
        virtual void _detachSceneNode(Ogre::SceneNode* node);
        
        /** Liefert das gekapselte Ogre::MovableObject. */
        Ogre::MovableObject* getMovableObject();
        
        /** Liefert die Typenbezeichnung der konkreten Klasse.
         * Sollte dem Typnamen entsprechen.
         */
        virtual Ogre::String getObjectType() = 0;
    
    protected:
        Ogre::MovableObject* mMovableObject;
    };
}
#endif
