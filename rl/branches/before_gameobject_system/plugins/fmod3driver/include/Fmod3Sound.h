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
#ifndef FMOD3SOUND_H_
#define FMOD3SOUND_H_

#include "Sound.h"

namespace rl {

class Fmod3Driver; 

class Fmod3Sound : public Sound
{
	public:
        /// Konstruktor
        Fmod3Sound(Fmod3Driver* driver, const SoundResourcePtr &soundres);
        virtual ~Fmod3Sound();

		/// Moveable-Typ
		virtual const Ogre::String& getMovableType() const;

		/// Gibt die eingestellte Position der Soundquelle zurueck
		virtual const Ogre::Vector3 getPosition() const;
		/// Setzt die Position der Soundquelle.
		virtual void setPosition(const Ogre::Vector3& direction);
		/// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck (0..100)
		virtual const Ogre::Real getVolume() const; 
		/// Setzt die relative Lautstaerke der Soundquelle (0.100)
		virtual void setVolume(const Ogre::Real gain);
		/// Gibt die Richtung der Soundquelle zurueck.
		virtual const Ogre::Quaternion getDirection() const;
		/// Gibt die Geschwindigkeit der Soundquelle zurueck.
		virtual const Ogre::Vector3 getVelocity() const;
		/// Setzt die Richtung der Soundquelle.
		virtual void setDirection(const Ogre::Quaternion&);
		/// Setzt die Geschwindigkeit der Soundquelle.
		virtual void setVelocity(const Ogre::Vector3&);
		/// Setzt die Entfernung, ab der ein 3D-Sound leiser wird
		virtual void setRolloffStartDistance(const Ogre::Real&);
		virtual const Ogre::Real getRolloffStartDistance();
		/// Setzt die Entfernung, ab der ein 3D-Sound nicht mehr leiser wird
		virtual void setRolloffEndDistance(const Ogre::Real&);
		virtual const Ogre::Real getRolloffEndDistance();

		/// Spielt den Sound ab.
		virtual void play();
		/// Pausiert den Sound.
		virtual void pause(bool pausing);
		/// Ist der Sound pausiert?
		virtual bool isPaused();
		/// Stoppt den Sound.
		virtual void stop();
		/// Zurueck auf Anfang.
	//         void rewind() throw (RuntimeException);
		/// Laeuft der Sound noch
		virtual const bool isPlaying() const;
	    
		// Sind wir gueltig
		virtual bool isValid() const throw (RuntimeException);

	protected:
        virtual int createChannel() = 0;

		static const int NO_CHANNEL = -999999;

        int mChannel;
        Fmod3Driver* mDriver;

    private:
		/// Shared class-level name for Movable type
		static Ogre::String msMovableType;

};

}

#endif /* FMOD3SOUND_H_ */
