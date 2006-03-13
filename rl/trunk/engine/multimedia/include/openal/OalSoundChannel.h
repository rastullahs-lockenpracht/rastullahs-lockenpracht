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
#ifdef WITH_OAL
#ifndef _OALSOUNDCHANNEL_H_
#define _OALSOUNDCHANNEL_H_

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

class _RlMultimediaExport OalSoundChannel  : public SoundChannel
{
private:
    /// Shared class-level name for Movable type
    static Ogre::String msMovableType;
    /// Fmod-Channel 
    signed int mChannel;
    /// Position, Richtung, Geschwindigkeit
    Ogre::Vector3 mPosition, mDirection, mVelocity;
    /// Ob der Sound spielt
    bool mIsPlaying;
    /// Lautstärke
    int mGain;
    
public:
	OalSoundChannel(Sound *sound, const Ogre::String &name);
	virtual ~OalSoundChannel();
    
    /// Moveable-Typ
    virtual const Ogre::String& getMovableType() const;
    /// Den Soundkanal zurueckgeben.
    const signed int getChannel() const;
    /// Den Soundkanal setzen.
    void setChannel(signed int channel);  

    /// Gibt die eingestellte Position der Soundquelle zurueck
    virtual const Ogre::Vector3 getPosition() const throw (RuntimeException);
    /// Setzt die Position der Soundquelle.
    virtual void setPosition(const Ogre::Vector3& direction) throw (RuntimeException);
    /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck
    virtual const int getGain() const throw (RuntimeException); 
    /// Setzt die relative Lautstaerke der Soundquelle.
    virtual void setGain(const int gain) throw (RuntimeException);
    /// Gibt die Richtung der Soundquelle zurueck.
    virtual const Ogre::Vector3 getDirection() const throw (RuntimeException);
    /// Gibt die Geschwindigkeit der Soundquelle zurueck.
    virtual const Ogre::Vector3 getVelocity() const throw (RuntimeException);
    /// Setzt die Richtung der Soundquelle.
    virtual void setDirection(const Ogre::Vector3&) throw (RuntimeException);
    /// Setzt die Geschwindigkeit der Soundquelle.
    virtual void setVelocity(const Ogre::Vector3&) throw (RuntimeException);

    /// Spielt den Sound ab.
    virtual void play() throw (RuntimeException);
    /// Pausiert den Sound.
    virtual void pause(bool pausing) throw (RuntimeException);
    /// Ist der Sound pausiert?
    virtual bool isPaused() throw (RuntimeException);
    /// Stoppt den Sound.
    virtual void stop() throw (RuntimeException);
    /// Zurueck auf Anfang.
//        virtual void rewind() throw (RuntimeException);
    /// Laeuft der Sound noch
    virtual const bool isPlaying() const;
    
    // Sind wir gueltig
    bool isValid() const throw (RuntimeException);
};

};

#endif //_OALSOUNDCHANNEL_H_
#endif // WITH_OAL
