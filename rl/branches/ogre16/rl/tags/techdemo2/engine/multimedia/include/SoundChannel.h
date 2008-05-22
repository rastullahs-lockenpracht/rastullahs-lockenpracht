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
#include <set>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include "SoundEvents.h"
#include "EventCaster.h"

namespace Ogre {
    class MovableObject;
}

namespace rl
{
    
    class Sound;
	class SoundDriver;

class _RlMultimediaExport SoundChannel  : public Ogre::MovableObject, 
    public EventCaster<SoundEvent>, public EventSource
{
public:
	SoundChannel(SoundDriver* driver, Sound *sound, const Ogre::String &name);
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
    virtual const Ogre::Vector3 getPosition() const = 0;
    /// Setzt die Position der Soundquelle.
    virtual void setPosition(const Ogre::Vector3& direction) = 0;
    /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck (0.0 ... 1.0)
	virtual const Ogre::Real getVolume() const = 0; 
    /// Setzt die relative Lautstaerke der Soundquelle (0.0 .. 1.0).
	virtual void setVolume(const Ogre::Real gain) = 0;
    /// Gibt die Richtung der Soundquelle zurueck.
    virtual const Ogre::Quaternion getDirection() const = 0;
    /// Gibt die Geschwindigkeit der Soundquelle zurueck.
    virtual const Ogre::Vector3 getVelocity() const = 0;
    /// Setzt die Richtung der Soundquelle.
    virtual void setDirection(const Ogre::Quaternion&) = 0;
    /// Setzt die Geschwindigkeit der Soundquelle.
    virtual void setVelocity(const Ogre::Vector3&) = 0;
	/// Setzt die Entfernung, ab der ein 3D-Sound leiser wird
	virtual void setRolloffStartDistance(const Ogre::Real&);
	virtual const Ogre::Real getRolloffStartDistance() const;
	/// Setzt die Entfernung, ab der ein 3D-Sound nicht mehr leiser wird
	virtual void setRolloffEndDistance(const Ogre::Real&);
	virtual const Ogre::Real getRolloffEndDistance() const;

    /// Spielt den Sound ab.
    virtual void play() = 0;
    /// Pausiert den Sound.
    virtual void pause(bool pausing) = 0;
    /// Ist der Sound pausiert?
    virtual bool isPaused() = 0;
    /// Stoppt den Sound.
    virtual void stop() = 0;
    /// Zurueck auf Anfang.
//        virtual void rewind() throw (RuntimeException) = 0;
    /// Laeuft der Sound noch
    virtual const bool isPlaying() const = 0;

	/// Laedt den Sound.
    virtual void load();
    /// Entlaedt den Sound.
    virtual void unload();

    /// Gibt zurück ob der Sound geloopt werden soll 
    bool isLooping() const;
    /// Setzt ob der Sound geloopt werden soll 
    void setLooping( bool looping );
    /// Ist es ein 3D-Stueck?
	bool is3d() const;
    /// Ob es ein 3D-Stueck sein soll.
	void set3d( bool is3d );
    
    // Sind wir gueltig
    virtual bool isValid() const;

    /// Hole den zugehoerigen Sound
    Sound *getSound() const;

	SoundDriver* getDriver() const;
    
protected:
    /// Die Lautstärke
	Ogre::Real mVolume;
    /// Die Position
    Ogre::Vector3 mPosition;
    /// Die Richtung
    Ogre::Quaternion mDirection;
    /// Die Geschwindigkeit
    Ogre::Vector3 mVelocity;

	Ogre::Real mRolloffStartDistance;
	Ogre::Real mRolloffEndDistance;

private:
    rl::Sound *mSound;
	SoundDriver* mDriver;

    /// Shared class-level name for Movable type
    static Ogre::AxisAlignedBox msAABox;
    /// Der Name des Soundchannels
    Ogre::String mName;
};

typedef std::set<SoundChannel*> SoundChannelSet;

};

#endif //_SOUNDCHANNEL_H_
