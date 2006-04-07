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
    void seek(float percentage);
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
    /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck
    virtual const unsigned int getVolume() const = 0; 
    /// Setzt die relative Lautstaerke der Soundquelle.
    virtual void setVolume(const unsigned int gain) = 0;
    /// Gibt die Richtung der Soundquelle zurueck.
    virtual const Ogre::Quaternion getDirection() const = 0;
    /// Gibt die Geschwindigkeit der Soundquelle zurueck.
    virtual const Ogre::Vector3 getVelocity() const = 0;
    /// Setzt die Richtung der Soundquelle.
    virtual void setDirection(const Ogre::Quaternion&) = 0;
    /// Setzt die Geschwindigkeit der Soundquelle.
    virtual void setVelocity(const Ogre::Vector3&) = 0;

    /// Spielt den Sound ab.
    virtual void play() = 0;
    /// Pausiert den Sound.
    virtual void pause(bool pausing) = 0;
    /// Ist der Sound pausiert?
    virtual bool isPaused() = 0;
    /// Stoppt den Sound.
    virtual void stop() = 0;
    /// Zurueck auf Anfang.
//        virtual void rewind() = 0;
    /// Laeuft der Sound noch
    virtual const bool isPlaying() const = 0;

    /// Gibt zurück ob der Sound geloopt werden soll 
    bool isLooping() const;
    /// Setzt ob der Sound geloopt werden soll 
    void setLooping( bool looping );
    
    // Sind wir gueltig
    virtual bool isValid() const;
};


}
