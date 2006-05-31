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

#ifndef _NULLSOUNDCHANNEL_H_
#define _NULLSOUNDCHANNEL_H_

#include "MultimediaPrerequisites.h"
#include "SoundChannel.h"
#include "Exception.h"

namespace Ogre {
    class MovableObject;
    class Vector3;
}

namespace rl
{
    const int NO_CHANNEL = -1234;
    
    class Sound;
	class SoundDriver;

class _RlMultimediaExport NullSoundChannel  : public SoundChannel
{
private:
    /// Shared class-level name for Movable type
    static Ogre::String msMovableType;
    /// Fmod-Channel 
    signed int mChannel;
    /// Ob der Sound spielt
    bool mIsPlaying;
    
public:
	NullSoundChannel(SoundDriver* driver, Sound *sound, const Ogre::String &name);
	virtual ~NullSoundChannel();
    
    /// Moveable-Typ
    virtual const Ogre::String& getMovableType() const;
    /// Den Soundkanal zurueckgeben.
    const signed int getChannel() const;
    /// Den Soundkanal setzen.
    void setChannel(signed int channel);  

    /// Gibt die eingestellte Position der Soundquelle zurueck
    virtual const Ogre::Vector3 getPosition() const;
    /// Setzt die Position der Soundquelle.
    virtual void setPosition(const Ogre::Vector3& direction);
    /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck
    virtual const unsigned int getVolume() const; 
    /// Setzt die relative Lautstaerke der Soundquelle.
    virtual void setVolume(const unsigned int gain);
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
    /// Zurueck auf Anfang.
//        virtual void rewind() throw (RuntimeException);
    /// Laeuft der Sound noch
    virtual const bool isPlaying() const;
    
    // Sind wir gueltig
    bool isValid() const;
};

};

#endif //_NULLSOUNDCHANNEL_H_
