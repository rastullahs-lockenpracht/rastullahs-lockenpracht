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
        ActorControlledObject(Ogre::MovableObject* mo);
        virtual ~ActorControlledObject();
        
        /** Diese Methode wird intern benutzt, damit
         * der Aktor sich dieses Objekt zu eigen machen
         * kann.
         * @warning Wenn diese Methode überschrieben wird immer
         * auch ActorControlledObject::_setActor() aufrufen.
         */
        virtual void _setActor(Actor* actor);
        virtual Actor* getActor() const;
        
        virtual void _attachSceneNode(Ogre::SceneNode* node);
        virtual void _detachSceneNode(Ogre::SceneNode* node);

        virtual bool isAttachedToNode(Ogre::SceneNode* node) const;
        virtual bool isAttached() const;

        /**
         * Interne Methode. Wird vom Aktor aufgerufen, wenn sich dessen
         * Status geändert hat. (Position, Orientierung, etc)
         * Die Standardimplementierung macht nichts, kann aber
         * von abgeleiteten Klassen überschrieben werden.
         */
        virtual void _update();
        
        /** Liefert das gekapselte Ogre::MovableObject. */
        Ogre::MovableObject* getMovableObject() const;
        
        /** Liefert die Typenbezeichnung der konkreten Klasse.
         * Sollte dem Typnamen entsprechen.
         */
        virtual Ogre::String getObjectType() const = 0;

        /** Ermöglicht ein Highlighten des ActorControlled */
        virtual void setHighlighted( bool highlight ) {};

		virtual bool isMeshObject() const;
    protected:
        Ogre::MovableObject* mMovableObject;
    };
}
#endif
