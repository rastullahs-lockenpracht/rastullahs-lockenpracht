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
#ifndef _FMOD4SOUNDCHANNEL_H_
#define _FMOD4SOUNDCHANNEL_H_

#include "Fmod4DriverPrerequisites.h"
#include "SoundChannel.h"
#include "Exception.h"
#include "Fmod4Sound.h"
#include "Fmod4Driver.h"

namespace rl
{
	class _RlFmod4DriverExport Fmod4SoundChannel  : public SoundChannel
	{
	public:
		Fmod4SoundChannel(Fmod4Driver* driver, Sound *sound, const Ogre::String &name);
		virtual ~Fmod4SoundChannel();

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

		/// Spielt den Sound ab.
		virtual void play();
		/// Pausiert den Sound.
		virtual void pause(bool pausing);
		/// Ist der Sound pausiert?
		virtual bool isPaused();
		/// Stoppt den Sound.
		virtual void stop();
		/// Laeuft der Sound noch
		virtual const bool isPlaying() const;
	    
		// Sind wir gueltig
		bool isValid() const;

        virtual Fmod4Driver* getDriver() const;
        virtual Fmod4Sound* getSound() const;


    private:
		/// Shared class-level name for Movable type
		static Ogre::String msMovableType;
        FMOD::Channel* mChannel;
	    
	};

};

#endif //_FMODSOUNDCHANNEL_H_
