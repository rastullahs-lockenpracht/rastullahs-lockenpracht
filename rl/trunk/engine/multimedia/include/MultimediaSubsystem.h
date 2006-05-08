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

#ifndef __MULTIMEDIASUBSYSTEM_H__
#define __MULTIMEDIASUBSYSTEM_H__

#include "MultimediaPrerequisites.h"
#include <list>

namespace rl {

class Video;
typedef std::list<Video*> VideoList;
class SoundDriver;
typedef std::list<SoundDriver*> DriverList;
class ListenerMovable;

/** Diese Klasse dient der Initialisierung und Steuerung des
 * Sound-Subsystems.
 * @author Josch
 * @date 06-29-2005
 * @version 1.0
 * @version 2.0
 */
class _RlMultimediaExport MultimediaSubsystem : public Ogre::Singleton<MultimediaSubsystem>
{

public:
	/// Der Standardkonstruktor
	MultimediaSubsystem();
	/// Der Destruktor
    ~MultimediaSubsystem();

	/// Gibt Singleton-Objekt zurueck.
    static MultimediaSubsystem& getSingleton(void);
    /// Gibt Singleton-Zeiger zurueck.
    static MultimediaSubsystem* getSingletonPtr(void);
    /// Eine Log-Nachricht ausgeben.
   	static void log(const Ogre::LogMessageLevel level, const Ogre::String& msg, const Ogre::String& ident = "");
    /// fmod weiterlaufen lassen.
    void update(Ogre::Real elapsedTime);

    /// Die bisherige Zeit zurückgeben.
    Ogre::Real getElapsedTime() const;
    /// Die bisherige Zeit setzen
    void setElapsedTime(Ogre::Real elapsedTime);
    
    /// Neues Video hinzufügen.
    void addVideo(Video *video);
    /// Videoliste leeren.
    void clearVideos();
    /// Video entfernen
    void removeVideo(Video *video);
    
    /// Liste der Soundtreiber.
    const DriverList& getSoundDriverList() const;
    /// Den aktiven Treiber zurückgeben.
    SoundDriver* getActiveDriver() const;
    /// Den aktiven Treiber setzen.
    void setActiveDriver(SoundDriver *driver);
    
    /// Den aktiven Listener zurückgeben.
    ListenerMovable* getActiveListener() const;
    /// Den aktiven Listener setzen.
    void setActiveListener(ListenerMovable *listener);

	/// Hole die Soundkonfiguration
	void loadConf(const Ogre::String &filename);
	/// Schreibe die Soundkonfiguration
	void saveConf(const Ogre::String &filename) const;

private:
    /// Zeit mitschreiben
    Ogre::Real mElapsedTime;
    /// Die Liste der Videos, die upgedatet werden müssen.
    VideoList mVideoList;
    /// die Liste der bekannten und funktionierenden Treiber
    DriverList mDriverList;  
    /// Der aktuell bentutzte Soundtreiber
    SoundDriver *mActiveDriver;
    /// Der aktive Soundlistener
    ListenerMovable *mActiveListener;
};

}

#endif
