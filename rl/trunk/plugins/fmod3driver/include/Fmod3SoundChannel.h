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
#ifndef _FMODSOUNDCHANNEL_H_
#define _FMODSOUNDCHANNEL_H_

#include "Fmod3DriverPrerequisites.h"
#include "SoundChannel.h"
#include "Exception.h"

namespace rl
{
    const int NO_CHANNEL = -1234;
    
    class Sound;
	class SoundDriver;

	class _RlFmod3DriverExport Fmod3SoundChannel  : public SoundChannel
	{
	private:
		/// Shared class-level name for Movable type
		static Ogre::String msMovableType;
		/// Fmod-Channel 
		signed int mChannel;
	    
	public:
		Fmod3SoundChannel(SoundDriver* driver, Sound *sound, const Ogre::String &name);
		 ~Fmod3SoundChannel();
	    
		/// Moveable-Typ
		 const Ogre::String& getMovableType() const;
		/// Den Soundkanal zurueckgeben.
		const signed int getChannel() const;
		/// Den Soundkanal setzen.
		void setChannel(signed int channel);  

		/// Gibt die eingestellte Position der Soundquelle zurueck
		 const Ogre::Vector3 getPosition() const;
		/// Setzt die Position der Soundquelle.
		 void setPosition(const Ogre::Vector3& direction);
		/// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck (0..100)
		 const Ogre::Real getVolume() const; 
		/// Setzt die relative Lautstaerke der Soundquelle (0.100)
		 void setVolume(const Ogre::Real gain);
		/// Gibt die Richtung der Soundquelle zurueck.
		 const Ogre::Quaternion getDirection() const;
		/// Gibt die Geschwindigkeit der Soundquelle zurueck.
		 const Ogre::Vector3 getVelocity() const;
		/// Setzt die Richtung der Soundquelle.
		 void setDirection(const Ogre::Quaternion&);
		/// Setzt die Geschwindigkeit der Soundquelle.
		 void setVelocity(const Ogre::Vector3&);
		/// Setzt die Entfernung, ab der ein 3D-Sound leiser wird
		 void setRolloffStartDistance(const Ogre::Real&);
		 const Ogre::Real getRolloffStartDistance();
		/// Setzt die Entfernung, ab der ein 3D-Sound nicht mehr leiser wird
		 void setRolloffEndDistance(const Ogre::Real&);
		 const Ogre::Real getRolloffEndDistance();

		/// Spielt den Sound ab.
		 void play();
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
	    
		// Sind wir gueltig
		bool isValid() const;
	};

};

#endif //_FMODSOUNDCHANNEL_H_
