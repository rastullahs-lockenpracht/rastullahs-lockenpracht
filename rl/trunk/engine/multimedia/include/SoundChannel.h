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

#include "MultimediaPrerequisites.h"
#include "Exception.h"

namespace Ogre {
    class MovableObject;
    class Vector3;
}

namespace rl
{
    
    class Sound;

class _RlMultimediaExport SoundChannel  : public Ogre::MovableObject
{
private:
    rl::Sound *mSound;

    /// Shared class-level name for Movable type
    static Ogre::AxisAlignedBox msAABox;
    /// Der Name des Soundchannels
    Ogre::String mName;
    
public:
	SoundChannel(Sound *sound, const Ogre::String &name);
	virtual ~SoundChannel();
    
    /// Name zurückgeben
    const Ogre::String& getName() const;
    /// Kamera informieren
    virtual void _notifyCurrentCamera(Ogre::Camera *cam);
    /// Unsere Bounding-Box
    virtual const Ogre::AxisAlignedBox& getBoundingBox(void) const;
    /// Bound-Radius
    virtual Ogre::Real getBoundingRadius() const;
    /// Rendern
    virtual void _updateRenderQueue(Ogre::RenderQueue *queue);


    /// Gibt die eingestellte Position der Soundquelle zurueck
    virtual const Ogre::Vector3 getPosition() const throw (RuntimeException) = 0;
    /// Setzt die Position der Soundquelle.
    virtual void setPosition(const Ogre::Vector3& direction) throw (RuntimeException) = 0;
    /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck
    virtual const int getGain() const throw (RuntimeException) = 0; 
    /// Setzt die relative Lautstaerke der Soundquelle.
    virtual void setGain(const int gain) throw (RuntimeException) = 0;
    /// Gibt die Richtung der Soundquelle zurueck.
    virtual const Ogre::Vector3 getDirection() const throw (RuntimeException) = 0;
    /// Gibt die Geschwindigkeit der Soundquelle zurueck.
    virtual const Ogre::Vector3 getVelocity() const throw (RuntimeException) = 0;
    /// Setzt die Richtung der Soundquelle.
    virtual void setDirection(const Ogre::Vector3&) throw (RuntimeException) = 0;
    /// Setzt die Geschwindigkeit der Soundquelle.
    virtual void setVelocity(const Ogre::Vector3&) throw (RuntimeException) = 0;

    /// Spielt den Sound ab.
    virtual void play() throw (RuntimeException) = 0;
    /// Pausiert den Sound.
    virtual void pause(bool pausing) throw (RuntimeException) = 0;
    /// Ist der Sound pausiert?
    virtual bool isPaused() throw (RuntimeException) = 0;
    /// Stoppt den Sound.
    virtual void stop() throw (RuntimeException) = 0;
    /// Zurueck auf Anfang.
//        virtual void rewind() throw (RuntimeException) = 0;
    /// Laeuft der Sound noch
    virtual const bool isPlaying() const = 0;

	/// Laedt den Sound.
    virtual void load() throw (RuntimeException);

    /// Gibt zurück ob der Sound geloopt werden soll 
    bool isLooping() const;
    /// Setzt ob der Sound geloopt werden soll 
    void setLooping( bool looping );
    /// Ist es ein 3D-Stueck?
	bool is3d() const;
    /// Ob es ein 3D-Stueck sein soll.
	void set3d( bool is3d );
    
    // Sind wir gueltig
    virtual bool isValid() const throw (RuntimeException);

    /// Hole den zugehoerigen Sound
    Sound *getSound() const;

};

};

#endif //_SOUNDCHANNEL_H_
