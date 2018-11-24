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

#ifndef __NullSoundSample_H__
#define __NullSoundSample_H__

#include "CorePrerequisites.h"
#include "Sound.h"
#include "SoundResource.h"

namespace rl
{

    /** Diese Klasse dient der Interaktion mit Ogre3D
     * @author Josch
     * @date 07-03-2005
     * @version 1.0
     */
    class _RlCoreExport NullSound : public Sound
    {
    public:
        /// Konstruktor
        NullSound(const SoundResourcePtr& soundres, SoundDriver* creator);
        /// Destruktor
        virtual ~NullSound();

        /// Laedt den Sound.
        virtual void load();
        /// Entlaedt den Sound.
        virtual void unload();

        // Wir geben zurueck, wie lange der Sound ist.
        virtual float getLength() const;

        /// Moveable-Typ
        virtual const Ogre::String& getMovableType() const;

        /// Gibt die eingestellte Position der Soundquelle zurueck
        virtual const Ogre::Vector3 getPosition() const;
        /// Setzt die Position der Soundquelle.
        virtual void setPosition(const Ogre::Vector3& direction);
        /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck
        virtual const Ogre::Real getVolume() const;
        /// Setzt die relative Lautstaerke der Soundquelle.
        void setVolume(const Ogre::Real gain);
        /// Gibt die Richtung der Soundquelle zurueck.
        const Ogre::Quaternion getDirection() const;
        /// Gibt die Geschwindigkeit der Soundquelle zurueck.
        const Ogre::Vector3 getVelocity() const;
        /// Setzt die Richtung der Soundquelle.
        void setDirection(const Ogre::Quaternion&);
        /// Setzt die Geschwindigkeit der Soundquelle.
        void setVelocity(const Ogre::Vector3&);

        /// Spielt den Sound ab.
        void play(bool destroyWhenDone = false);
        /// Pausiert den Sound.
        void pause(bool pausing);
        /// Ist der Sound pausiert?
        bool isPaused();
        /// Stoppt den Sound.
        void stop();
        /// Zurueck auf Anfang.
        //         void rewind() throw (RuntimeException);
        /// Laeuft der Sound noch
        const bool isPlaying() const;

        // Sets the priority of this sound
        virtual void setPriority(const int priority);
        // Gets the priority of this sound
        virtual const int getPriority() const;

    protected:
        // Sind wir gueltig?
        virtual bool isValid() const;

    private:
        // Shared class-level name for Movable type
        static Ogre::String msMovableType;
    };

    using NullSoundPtr = Ogre::SharedPtr<NullSound>;
}
#endif
