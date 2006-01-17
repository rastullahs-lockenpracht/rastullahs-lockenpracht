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

#ifndef _SOUNDCHANNEL_H_
#define _SOUNDCHANNEL_H_

#include "SoundPrerequisites.h"
#include "Exception.h"

namespace Ogre {
    class MovableObject;
    class Vector3;
}

namespace rl
{
    const int NO_CHANNEL = -1234;
    
    class Sound;

class _RlSoundExport SoundChannel  : public Ogre::MovableObject
{
private:
    rl::Sound *mSound;

    /// Shared class-level name for Movable type
    static Ogre::String msMovableType;
    static Ogre::AxisAlignedBox msAABox;
    /// Der Name des Soundchannels
    Ogre::String mName;
    /// Fmod-Channel 
    signed int mChannel;
    
public:
	SoundChannel(Sound *sound, const Ogre::String &name);
	virtual ~SoundChannel();
    
    /// Name zurückgeben
    const Ogre::String& getName() const;
    /// Den Soundkanal zurueckgeben.
    const signed int getChannel() const;
    /// Den Soundkanal setzen.
    void setChannel(signed int channel);  
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
    virtual void play() throw (RuntimeException);
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

    /// Gibt zurück ob der Sound geloopt werden soll 
    bool isLooping() const;
    /// Setzt ob der Sound geloopt werden soll 
    void setLooping( bool looping );
    /// Setzt die Lautstaerke
	void setVolume(float volume);
	bool is3d() const;
	void set3d( bool is3d );
    
    // Sind wir gueltig
    bool isValid() const throw (RuntimeException);
};

};

#endif //_SOUNDCHANNEL_H_
