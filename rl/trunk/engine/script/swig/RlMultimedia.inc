/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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

namespace rl {

class SoundChannel 
{
private:
	SoundChannel(Sound *sound, const Ogre::String &name);
public:
	
	virtual ~SoundChannel();
    
    /// Name zurückgeben
    const Ogre::String& getName() const;

    /// Gibt die eingestellte Position der Soundquelle zurueck
    const Ogre::Vector3 getPosition() const;
    /// Setzt die Position der Soundquelle.
    void setPosition(const Ogre::Vector3& direction);
    /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck
    const Ogre::Real getVolume() const; 
    /// Setzt die relative Lautstaerke der Soundquelle.
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
	void setRolloffStartDistance(const Ogre::Real dist);
	const Ogre::Real getRolloffStartDistance() const;
	/// Setzt die Entfernung, ab der ein 3D-Sound nicht mehr leiser wird
	void setRolloffEndDistance(const Ogre::Real dist);
	const Ogre::Real getRolloffEndDistance() const;

    /// Spielt den Sound ab.
    void play() ;
    /// Pausiert den Sound.
    void pause(bool pausing) ;
    /// Ist der Sound pausiert?
    bool isPaused() ;
    /// Stoppt den Sound.
    void stop() ;
    /// Laeuft der Sound noch
    const bool isPlaying() const ;

    /// Gibt zurück ob der Sound geloopt werden soll 
    bool isLooping() const;
    /// Setzt ob der Sound geloopt werden soll 
    void setLooping( bool looping );
    
    // Sind wir gueltig
    bool isValid() const;
};


}
