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
#include <vector>
#include "EventSource.h"
#include "EventListener.h"
#include "EventCaster.h"
#include "SoundEvents.h"

namespace Ogre {
    class MovableObject;
    class Vector3;
}

namespace rl {
    typedef vector<unsigned int> UintVector;
    const int NO_CHANNEL = -1234;

   /** Diese Klasse dient der Interaktion mit Ogre3D
    * @author Josch
    * @date 06-29-2005
    * @version 1.0
    * @version 2.0
    */
    class _RlSoundExport SoundMovable : public Ogre::MovableObject,
         public virtual EventListener<SoundEvent>,
         public virtual EventSource, 
         public virtual EventCaster<SoundEvent>
    {
    private:
        /// Fmod-Channel 
        int mChannel;
        /// Der Name des SoundMovable
        Ogre::String mName;
        /// Der Soundresource, auf den wir verweisen.
        SoundResourcePtr mSoundResource;

        // Damit die Timings alle 0.5 sek. abgeschickt werden.
        int mTicks;
        /// Die komplette Zeit des Stücks in Sekunden.
        double mTime;
        
    public:
        /// Konstruktor
        SoundMovable(const Ogre::String& name);
        /// Konstruktor
        SoundMovable(const SoundResourcePtr &soundres);
        /// Destruktor
        virtual ~SoundMovable();
        /// Name zurückgeben
        virtual const Ogre::String& getName() const;
        /// Den Soundkanal zurueckgeben.
        virtual const int getChannel() const;
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
    
        /// Gibt die eingestellte Position der Soundquelle zurueck
        const Ogre::Vector3 getPosition() const throw (RuntimeException);
        /// Setzt die Position der Soundquelle.
        void setPosition(const Ogre::Vector3& direction) throw (RuntimeException);
        /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck
        const int getGain() const throw (RuntimeException); 
        /// Setzt die relative Lautstaerke der Soundquelle.
        void setGain(const int gain) throw (RuntimeException);
        /// Gibt die Richtung der Soundquelle zurueck.
        const Ogre::Vector3 getDirection() const throw (RuntimeException);
        /// Gibt die Geschwindigkeit der Soundquelle zurueck.
        const Ogre::Vector3 getVelocity() const throw (RuntimeException);
        /// Setzt die Richtung der Soundquelle.
        void setDirection(const Ogre::Vector3&) throw (RuntimeException);
        /// Setzt die Geschwindigkeit der Soundquelle.
        void setVelocity(const Ogre::Vector3&) throw (RuntimeException);
        
        /// Spielt den Sound ab.
        virtual void play() throw (RuntimeException) = 0;
        /// Pausiert den Sound.
        virtual void pause(bool pausing) throw (RuntimeException);
        /// Ist der Sound pausiert?
        virtual bool isPaused() throw (RuntimeException);
        /// Stoppt den Sound.
        virtual void stop() throw (RuntimeException);
        /// Zurueck auf Anfang.
//        virtual void rewind() throw (RuntimeException);
        /// Ist laeuft AL noch
        const bool isPlaying() const;


protected:
        /// Wir haben ein Ereignis erhalten.
        virtual bool eventRaised(SoundEvent *anEvent) const;

        /// Shared class-level name for Movable type
        static Ogre::String msMovableType;
        static Ogre::AxisAlignedBox msAABox;
        // Sind wir gueltig
        virtual bool isValid() const throw (RuntimeException) = 0;
        


    }; 

    class _RlSoundExport SoundMovablePtr :
        public Ogre::SharedPtr<SoundMovable>
    {
    public:
        SoundMovablePtr() : Ogre::SharedPtr<SoundMovable>() {}
        explicit SoundMovablePtr(SoundMovable* rep) : Ogre::SharedPtr<SoundMovable>(rep) {}
        SoundMovablePtr(const SoundMovablePtr& res) : Ogre::SharedPtr<SoundMovable>(res) {}
    protected:
        void destroy();
    };

}
#endif
