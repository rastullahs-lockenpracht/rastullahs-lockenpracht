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

#ifndef __SoundMovable_H__
#define __SoundMovable_H__

#include "SoundPrerequisites.h"
#include "SoundResource.h"

namespace Ogre {
    class MovableObject;
}

namespace rl {

    class _RlSoundExport SoundMovable : public Ogre::MovableObject
    {
        public:
            /// Konstruktor
            SoundMovable(const Ogre::String& name);
            /// Destruktor
            virtual ~SoundMovable();
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
            /// SoundResource zurueckgeben.
            SoundResourcePtr getSound() const;
        
        private:
            /// Der Name des SoundMovable
            Ogre::String mName;
            /// Der eigentliche Sound, auf den wir verweisen.
            SoundResourcePtr mSound;
    }; 
}

#endif
