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

#ifndef __ListenerMovable_H__
#define __ListenerMovable_H__

#include "SoundPrerequisites.h"
#include <OgreMovableObject.h>
#include <OgreVector3.h>


namespace rl {
    
   /** Diese Klasse dient der Interkation mit Ogre3d und
    * kapselt den Hörer.
    * @author Josch
    * @date 06-29-2005
    * @version 1.0
    * @version 2.0
    */
    class _RlSoundExport ListenerMovable : public Ogre::MovableObject
    {
    private:
        /// Der aktive Listener
        static ListenerMovable *gActiveListener;
        /// Der Name des ListenerMovable
        Ogre::String mName;
        
    protected:
         /// Shared class-level name for Movable type
        static Ogre::String msMovableType;
        /// Alignment-Box: gebraucht von MovableObject.
        static Ogre::AxisAlignedBox msAABox;
        /// Position
        Ogre::Vector3 mPosition;
        /// Geschwindigkeit
        Ogre::Vector3 mVelocity;
        /// Orientierung
        Ogre::Vector3 mAt, mUp;
        /// Hauptlautstärke
        int mGain;

    public:
        /// Konstruktor
        ListenerMovable(const Ogre::String& name);
        /// Destruktor
        virtual ~ListenerMovable();
        /// Name zurückgeben
        virtual const Ogre::String& getName() const;
        /// Moveable-Typ
        virtual const Ogre::String& getMovableType() const;
        /// Kamera informieren
        virtual void _notifyCurrentCamera(Ogre::Camera *cam);
        /// Unsere Bounding-Box
        virtual const Ogre::AxisAlignedBox& getBoundingBox(void) const;
        /// Bound-Radius
        virtual Ogre::Real getBoundingRadius() const;
        /// Rendern
        virtual void _updateRenderQueue(Ogre::RenderQueue *queue);
    
        /// Gibt die Hauptlautstaerke zurueck
        const int getGain() const throw (RuntimeException);
        /// Setzt die Hauptlautstaerke.
        void setGain(const int direction) throw (RuntimeException);
        /// Gibt die eingestellte Position der Soundquelle zurueck
        const Ogre::Vector3 getPosition() const throw (RuntimeException);
        /// Setzt die Position der Soundquelle.
        void setPosition(const Ogre::Vector3& direction) throw (RuntimeException);
        /// Gibt die Richtung der Soundquelle zurueck.
        const Ogre::Vector3 getOrientationAt() const throw (RuntimeException);
        /// Gibt die Richtung der Soundquelle zurueck.
        const Ogre::Vector3 getOrientationUp() const throw (RuntimeException);
        /// Gibt die Geschwindigkeit der Soundquelle zurueck.
        const Ogre::Vector3 getVelocity() const throw (RuntimeException);
        /// Setzt die Richtung der Soundquelle.
        void setOrientation(const Ogre::Vector3 &at, const Ogre::Vector3 &up) throw (RuntimeException);
        /// Setzt die Geschwindigkeit der Soundquelle.
        void setVelocity(const Ogre::Vector3&) throw (RuntimeException);
        /// Ueberpruefen, ob der aktive Listener.
        bool isActive() const;
        /// Den Listener als aktuellen setzen.
        void setActive() throw (RuntimeException);
        /// Den Listener als aktuellen setzen.
        static ListenerMovable* getActiveListener();
    };

    class _RlSoundExport ListenerMovablePtr :
        public Ogre::SharedPtr<ListenerMovable>
    {
    public:
        ListenerMovablePtr() : Ogre::SharedPtr<ListenerMovable>() {}
        explicit ListenerMovablePtr(ListenerMovable* rep) : Ogre::SharedPtr<ListenerMovable>(rep) {}
        ListenerMovablePtr(const ListenerMovablePtr& res) : Ogre::SharedPtr<ListenerMovable>(res) {}
    protected:
		void destroy() { /** @todo: implement*/ }
    };

}
#endif
