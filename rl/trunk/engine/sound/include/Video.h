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

#ifndef VIDEO_H_
#define VIDEO_H_

#include "SoundPrerequisites.h"

#include <TheoraMovieClip.h>
#include <TheoraVideoController.h>
#include "EventSource.h"
#include "EventCaster.h"
#include "EventListener.h"

namespace CEGUI
{
    class Texture;
    class OgreCEGUIRenderer;
}

namespace Ogre
{
    class TheoraVideoController;
}

namespace rl
{
    class VideoEvent;

typedef EventListener<VideoEvent> VideoEventListener;

/** 
 * Verbindet das VideoPlugin mit RL
 * @author JoSch
 * @date 08-27-2005
 * @version 1.0
 */
class _RlSoundExport Video : protected Ogre::TheoraMovieMessage,
                             public EventSource
{
private:
    Ogre::TheoraVideoController *mVideoControl;
    Ogre::TheoraMovieClip* mClip;

    CEGUI::OgreCEGUIRenderer* mGUIRenderer;
    CEGUI::Texture *mTexture;
    CEGUI::String mMovieName;
    CEGUI::String mTextureName;
    float mMaxTime;

public:
    // Default-Konstruktor
    Video(CEGUI::String textureName, CEGUI::String movieName);
    // Destruktor
    virtual ~Video();

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
    void createCETexture();
    /// Zerstoere Textur
    void destroyCETexture();
    /// Textur zur�ckgeben;
    CEGUI::Texture* getTexture() const;
    /// Texturname zur�ckgeben.
    const CEGUI::String getTextureName() const;
    /// Die Videol�nge zur�ckgeben
    float getMaxTime() const;

    /// F�gt einen ObjectStateChangeListener hinzu, der zuk�nftig bei Events benachrichtigt wird 
    void addEventListener(VideoEventListener*  listener);
    /// Entfernt einen Listener
    void removeEventListener(VideoEventListener* listener);
    /// Entfernt alle Listener
    void removeEventListeners();
    /// Gibt zur�ck ob sich Listener angemeldet haben
    bool hasListeners() const;
    
protected:
    /// Botschaften aus dem VideoPlugin
    virtual int messageEvent(PLUGIN_theora_message message);
    /// Setzt die Laenge des Films
    virtual void discoveredMovieTime(float discoveredTime);
    /// Anzeige des Frames
    virtual void displayedFrame( float vTime, 
                                 float aTime,
                                 unsigned int frameNumber,
                                 unsigned int framesDropped);
private:
    /// Der Ereignisverteiler
    EventCaster<VideoEvent> mEventCaster;
};

}

#endif /*VIDEO_H_*/
