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

#ifndef __Sound_H__
#define __Sound_H__

#include "CorePrerequisites.h"
#include <set>

#include "EventCaster.h"
#include "Exception.h"
#include "SoundEvents.h"
#include "SoundResource.h"

namespace rl
{

    class SoundDriver;

    /** Diese Klasse dient der Interaktion mit dem jeweiligen Soundsystem
     * ein Objekt stellt eine einzelne Sounddatei oder einen Stream dar
     * @author Josch
     * @date 06-29-2005
     * @version 1.0
     * @version 2.0
     */
    class _RlCoreExport Sound : public Ogre::MovableObject, public EventCaster<SoundEvent>, public EventSource
    {
    public:
        // Einige Standardwerte fuer Soundprioritaeten.
        enum SOUND_PRIORITY
        {
            PRIO_SPEECH = 30,
            PRIO_MUSIC = 75,
            PRIO_SFX_ENVIRONMENT = 128,
            PRIO_SFX_OTHER = 175,
            PRIO_OTHER = 220
        };

        /// Konstruktor
        Sound(const SoundResourcePtr& soundres, SoundDriver* creator);
        /// Destruktor
        virtual ~Sound();

        /// Laedt den Sound.
        virtual void load() = 0;
        /// Entlaedt den Sound.
        virtual void unload() = 0;

        // SoundResource zurueckgeben.
        const SoundResourcePtr& getSoundResource() const;
        // Wollen wir 3D?
        bool is3d() const;
        // Setzen des 3D-Flags.
        void set3d(bool is3d);
        // Sind wir gueltig
        virtual bool isValid() const = 0;

        // Sollen der Sound wiederholt werden?
        bool isLooping() const;
        // Setzen des Loop-Flags.
        void setLooping(bool looping);

        virtual float getLength() const = 0;

        /// Unsere Bounding-Box
        virtual const Ogre::AxisAlignedBox& getBoundingBox(void) const;
        /// Bound-Radius
        virtual Ogre::Real getBoundingRadius() const;
        /// Rendern
        virtual void _updateRenderQueue(Ogre::RenderQueue* queue);

        /// Gibt die eingestellte Position der Soundquelle zurueck
        virtual const Ogre::Vector3 getPosition() const = 0;
        /// Setzt die Position der Soundquelle.
        virtual void setPosition(const Ogre::Vector3& direction) = 0;
        /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck (0.0 ... 1.0)
        virtual const Ogre::Real getVolume() const = 0;
        /// Setzt die relative Lautstaerke der Soundquelle (0.0 .. 1.0).
        virtual void setVolume(const Ogre::Real gain) = 0;
        /// Gibt die Richtung der Soundquelle zurueck.
        virtual const Ogre::Quaternion getDirection() const = 0;
        /// Gibt die Geschwindigkeit der Soundquelle zurueck.
        virtual const Ogre::Vector3 getVelocity() const = 0;
        /// Setzt die Richtung der Soundquelle.
        virtual void setDirection(const Ogre::Quaternion&) = 0;
        /// Setzt die Geschwindigkeit der Soundquelle.
        virtual void setVelocity(const Ogre::Vector3&) = 0;
        /// Setzt die Entfernung, ab der ein 3D-Sound leiser wird
        virtual void setRolloffStartDistance(const Ogre::Real&);
        virtual const Ogre::Real getRolloffStartDistance() const;
        /// Setzt die Entfernung, ab der ein 3D-Sound nicht mehr leiser wird
        virtual void setRolloffEndDistance(const Ogre::Real&);
        virtual const Ogre::Real getRolloffEndDistance() const;
        // Sets the priority of this sound
        virtual void setPriority(const int priority) = 0;
        // Gets the priority of this sound
        virtual const int getPriority() const = 0;

        /// Spielt den Sound ab.
        virtual void play(bool destroyWhenDone = false) = 0;
        /// Pausiert den Sound.
        virtual void pause(bool pausing) = 0;
        /// Ist der Sound pausiert?
        virtual bool isPaused() = 0;
        /// Stoppt den Sound.
        virtual void stop() = 0;
        /// Zurueck auf Anfang.
        //        virtual void rewind() throw (RuntimeException) = 0;
        /// Laeuft der Sound noch
        virtual const bool isPlaying() const = 0;

        /// From MovableObject
        virtual void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false);

    protected:
        SoundDriver* mCreator;

        /// Die Lautstaerke
        Ogre::Real mVolume;
        /// Die Position
        Ogre::Vector3 mPosition;
        /// Die Richtung
        Ogre::Quaternion mDirection;
        /// Die Geschwindigkeit
        Ogre::Vector3 mVelocity;
        /// The priority of this sound
        int mPriority;

        Ogre::Real mRolloffStartDistance;
        Ogre::Real mRolloffEndDistance;

    private:
        /// Shared class-level name for Movable type
        static Ogre::AxisAlignedBox msAABox;

        /// Der Soundresource, auf den wir verweisen.
        SoundResourcePtr mSoundResource;

        // Damit die Timings alle 0.5 sek. abgeschickt werden.
        int mTicks;
        // Ob dieser Sound 3D sein soll.
        bool mIs3d;
        // Ob dieser Sound loopen soll.
        bool mIsLooping;
    };

    typedef std::set<Sound*> SoundSet;
}
#endif
