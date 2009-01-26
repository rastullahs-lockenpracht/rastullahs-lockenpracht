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
#ifndef _FMOD4SOUNDSTITCHING_H_
#define _FMOD4SOUNDSTITCHING_H_

#include "CorePrerequisites.h"

#include <fmod.hpp>

#include "SoundStitching.h"
#include "SoundDriver.h"

namespace rl {

    class Fmod4Driver;
    typedef std::vector<FMOD::Sound*> SubsoundVector;

    class Fmod4SoundStitching : public SoundStitching
    {
    public:
        /// Konstruktor
        Fmod4SoundStitching(unsigned int numSlots, const CeGuiString& name, SoundDriver* creator);

        /// Laedt den Sound.
        virtual void load() throw (RuntimeException);
        /// Entlaedt den Sound.
        virtual void unload() throw (RuntimeException);

		// Sind wir gueltig
		bool isValid() const throw (RuntimeException);

	    /// Moveable-Typ
		virtual const Ogre::String& getMovableType() const;
        /// Gibt die eingestellte Position der Soundquelle zurueck
		virtual const Ogre::Vector3 getPosition() const;
		/// Setzt die Position der Soundquelle.
		virtual void setPosition(const Ogre::Vector3& direction);
		/// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck (0..1)
		virtual const float getVolume() const; 
		/// Setzt die relative Lautstaerke der Soundquelle (0..1)
		virtual void setVolume(const float gain);
		/// Gibt die Richtung der Soundquelle zurueck.
		virtual const Ogre::Quaternion getDirection() const;
		/// Gibt die Geschwindigkeit der Soundquelle zurueck.
		virtual const Ogre::Vector3 getVelocity() const;
		/// Setzt die Richtung der Soundquelle.
		virtual void setDirection(const Ogre::Quaternion&);
		/// Setzt die Geschwindigkeit der Soundquelle.
		virtual void setVelocity(const Ogre::Vector3&);
        
        /// Gibt den FMOD Channel zurück.
        FMOD::Channel* getFmodChannel() const;
        /// Gibt den FMOD Sound zurück.
        FMOD::Sound* getFmodSound() const;

        virtual void play(bool destroyWhenDone=false);
		/// Pausiert den Sound.
		virtual void pause(bool pausing);
		/// Ist der Sound pausiert?
		virtual bool isPaused();
		/// Stoppt den Sound.
		virtual void stop();
		/// Laeuft der Sound noch
		virtual const bool isPlaying() const;
        
        // Sets the priority of this sound
        virtual void setPriority(const int priority);
        // Gets the priority of this sound
        virtual const int getPriority() const;
        
        
        /// Should the sound be autodestructed when playing is done.
        bool isAutoDestroying() const;

        /// Assign a sound recource to a slot.
        virtual void putSoundIntoSlot(unsigned int, CeGuiString label);
        /// Get the slot of the plaing sound.
        virtual unsigned int getPlayingSlot();

    private:
		/// Shared class-level name for Movable type
		static Ogre::String msMovableType;
        FMOD::Channel* mChannel;
        FMOD::Sound *mSound;
        Fmod4Driver* mDriver;
        bool mAutoDestroy;
        SubsoundVector mSoundSlots;
    };

    class Fmod4SoundStitchingPtr :
        public Ogre::SharedPtr<Fmod4SoundStitching>
    {
    public:
        Fmod4SoundStitchingPtr() : Ogre::SharedPtr<Fmod4SoundStitching>() {}
        explicit Fmod4SoundStitchingPtr(Fmod4SoundStitching* rep) : Ogre::SharedPtr<Fmod4SoundStitching>(rep) {}
        Fmod4SoundStitchingPtr(const Fmod4SoundStitchingPtr& res) : Ogre::SharedPtr<Fmod4SoundStitching>(res) {}
    protected:
        void destroy();
    };

}

#endif /* FMOD3SOUNDSTITCHING_H_ */
