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

class VideoEventListener;
	
class  Video {
public:
    // Default-Konstruktor
    Video(rl::CeGuiString textureName, rl::CeGuiString movieName);

    /// Getter fuer Breite
    unsigned int getWidth() const;
    /// Getter fuer Hoehe
    unsigned int getHeight() const;
    /// Das Video auffrischen
    void update();
    /// Video anhalten
    void stop();
    /// Video unterbrechen oder wieder starten
    void pause(bool bPause = true);
    /// Video abspielen
    void play();
    /// Video an bestimmter Stelle weiterspielen
    // void seek(float percentage);
    /// Erzeuge Textur
    CEGUI::Texture* getTexture() const;
    /// Texturname zurueckgeben.
    const rl::CeGuiString getTextureName() const;
    /// Die Videolaenge zurueckgeben
    float getMaxTime() const;

    /// Gibt den Zustand zurÃ¼ck.
    bool isPlaying() const;
    /// Fuegt einen EventListener hinzu, der zukuenftig bei Events benachrichtigt wird 
    void addEventListener(rl::VideoEventListener*  listener);
    /// Entfernt einen Listener
    void removeEventListener(rl::VideoEventListener* listener);
    /// Entfernt alle Listener
    void removeEventListeners();
    /// Gibt zurueck ob sich Listener angemeldet haben
    bool hasListeners() const;
};

%template(SoundEventCaster) rl::EventCaster<rl::SoundEvent>;
class SoundChannel : public rl::EventCaster<rl::SoundEvent>
{
public:
	SoundChannel(Sound *sound, const Ogre::String &name);
	~SoundChannel();
    
    /// Name zurückgeben
    const Ogre::String& getName() const;
    /// Kamera informieren
    void _notifyCurrentCamera(Ogre::Camera *cam);
    /// Unsere Bounding-Box
    const Ogre::AxisAlignedBox& getBoundingBox(void) const;
    /// Bound-Radius
    Ogre::Real getBoundingRadius() const;
    /// Rendern
    void _updateRenderQueue(Ogre::RenderQueue *queue);


    /// Gibt die eingestellte Position der Soundquelle zurueck
    const Ogre::Vector3 getPosition() const;
    /// Setzt die Position der Soundquelle.
    void setPosition(const Ogre::Vector3& direction) = 0;
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
    /// Zurueck auf Anfang.
//        void rewind() ;
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
