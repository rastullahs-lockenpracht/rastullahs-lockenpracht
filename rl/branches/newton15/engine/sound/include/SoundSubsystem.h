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

#ifndef __SOUNDSUBSYSTEM_H__
#define __SOUNDSUBSYSTEM_H__

#include "SoundPrerequisites.h"
#include <OgreLogManager.h>
#include <list>

namespace rl {

class Video;
typedef std::list<Video*> VideoList;

/** Diese Klasse dient der Initialisierung und Steuerung des
 * Sound-Subsystems.
 * @author Josch
 * @date 06-29-2005
 * @version 1.0
 * @version 2.0
 */
class _RlSoundExport SoundSubsystem : public Ogre::Singleton<SoundSubsystem>
{

public:
	/// Der Standardkonstruktor
	SoundSubsystem();
	/// Der Destruktor
    ~SoundSubsystem();

	/// Gibt Singleton-Objekt zurueck.
    static SoundSubsystem& getSingleton(void);
    /// Gibt Singleton-Zeiger zurueck.
    static SoundSubsystem* getSingletonPtr(void);
    /// fmod weiterlaufen lassen.
    void update(Ogre::Real elapsedTime);

    /// Die bisherige Zeit zur�ckgeben.
    Ogre::Real getElapsedTime() const;
    /// Die bisherige Zeit setzen
    void setElapsedTime(Ogre::Real elapsedTime);
    
    /// Neues Video hinzufügen.
    void addVideo(Video *video);
    /// Videoliste leeren.
    void clearVideos();
    /// Video entfernen
    void removeVideo(Video *video);
        

private:
    /// Zeit mitschreiben
    Ogre::Real mElapsedTime;
    /// Die Liste der Videos, die upgedatet werden müssen.
    VideoList mVideoList;

    // FMOD-Callbacks
    static void close(void *handle);
    static void *open(const char *name);
    static int read(void *buffer, int size, void *handle);
    static int seek(void *handle, int pos, signed char mode);
    static int tell(void *handle);
    
	void initializeDriver();
    void printData();
        
};

}

#endif
