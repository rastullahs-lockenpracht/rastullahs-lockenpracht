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

class  Video {

class VideoEventListener;

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

    /// Gibt den Zustand zurück.
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

}
