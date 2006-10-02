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
#ifndef _OALSOUND_H_
#define _OALSOUND_H_

#include "OpenALDriverPrerequisites.h"
#include "Sound.h"
#include "Exception.h"

namespace rl
{
    const int NO_CHANNEL = -1234;
    
	class OpenALDriver;

class _RlOpenALDriverExport OpenALSound  : public Sound
{
public:
	OpenALSound(OpenALDriver* driver, const SoundResourcePtr& res);
	virtual ~OpenALSound();
    
    /// Moveable-Typ
    virtual const Ogre::String& getMovableType() const;

	float getLength() const;

    /// Gibt die eingestellte Position der Soundquelle zurueck
    virtual const Ogre::Vector3 getPosition() const;
    /// Setzt die Position der Soundquelle.
    virtual void setPosition(const Ogre::Vector3& direction);
    /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck
    virtual const float getVolume() const; 
    /// Setzt die relative Lautstaerke der Soundquelle.
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
    /// Zurueck auf Anfang.
//        virtual void rewind();
    /// Laeuft der Sound noch
    virtual const bool isPlaying() const;
    
    // Sind wir gueltig
    bool isValid() const throw (RuntimeException);

private:
    /// Fmod-Channel 
    signed int mChannel;
    /// Ob der Sound spielt
    bool mIsPlaying;
    OpenALDriver* mDriver;

    static Ogre::String msMovableType;
    
};

};

#endif //_OALSOUNDCHANNEL_H_
