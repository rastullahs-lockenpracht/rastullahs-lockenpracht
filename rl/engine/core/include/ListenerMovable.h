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

#ifndef __ListenerMovable_H__
#define __ListenerMovable_H__

#include "CorePrerequisites.h"

namespace rl
{

    /** Diese Klasse dient der Interaktion mit Ogre3d und
     * kapselt den Hoerer.
     * @author Josch
     * @date 06-29-2005
     * @version 1.0
     * @version 2.0
     */
    class _RlCoreExport ListenerMovable : public Ogre::MovableObject
    {
    public:
        /// Konstruktor
        ListenerMovable(const Ogre::String& name);
        /// Destruktor
        virtual ~ListenerMovable() = 0;
        /// Name zurueckgeben
        virtual const Ogre::String& getName() const;
        /// Moveable-Typ
        virtual const Ogre::String& getMovableType() const;
        /// Kamera informieren
        virtual void _notifyCurrentCamera(Ogre::Camera* cam);
        /// Unsere Bounding-Box
        virtual const Ogre::AxisAlignedBox& getBoundingBox(void) const;
        /// Bound-Radius
        virtual Ogre::Real getBoundingRadius() const;
        /// Rendern
        virtual void _updateRenderQueue(Ogre::RenderQueue* queue);

        /// Gibt die Hauptlautstaerke zurueck
        virtual const int getGain() const;
        /// Setzt die Hauptlautstaerke.
        virtual void setGain(const int direction);
        /// Gibt die eingestellte Position der Soundquelle zurueck
        virtual const Ogre::Vector3 getPosition() const;
        /// Setzt die Position der Soundquelle.
        virtual void setPosition(const Ogre::Vector3& direction);
        /// Gibt die Richtung der Soundquelle zurueck.
        virtual const Ogre::Quaternion getOrientation() const;
        /// Gibt die Geschwindigkeit der Soundquelle zurueck.
        virtual const Ogre::Vector3 getVelocity() const;
        /// Setzt die Richtung der Soundquelle.
        virtual void setOrientation(const Ogre::Quaternion& orientation);
        /// Setzt die Geschwindigkeit der Soundquelle.
        virtual void setVelocity(const Ogre::Vector3&);

        /// Ist dies der aktuelle Listener
        bool isActive() const;
        void setActive(bool active);

        /// From MovableObject
        virtual void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false);

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
        Ogre::Quaternion mOrientation;
        /// Hauptlautstaerke
        int mGain;

    private:
        /// Der Name des ListenerMovable
        Ogre::String mName;
        bool mActive;
    };
}
#endif
