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
#include "MultimediaSubsystem.h"
#include "SoundManager.h"
#include "Logger.h"
#include "SoundResource.h"
#include "Video.h"
#include <stdio.h>
extern "C" {
    #include <fmod.h>
    #include <fmod_errors.h>
}
#include "SoundUpdateTask.h"

using namespace Ogre;

template<> rl::MultimediaSubsystem* Singleton<rl::MultimediaSubsystem>::ms_Singleton = 0;

namespace rl {

/**
 * @return Das Singleton-Objekt
 * @author JoSch
 * @date 05-26-2004
 */
MultimediaSubsystem& MultimediaSubsystem::getSingleton(void)
{
	return Singleton<MultimediaSubsystem>::getSingleton();
}

/**
 * @return Das Singleton-Zeiger
 * @author JoSch
 * @date 05-26-2004
 */
MultimediaSubsystem* MultimediaSubsystem::getSingletonPtr(void)
{
    return Singleton<MultimediaSubsystem>::getSingletonPtr();
}

/**
 * @author JoSch
 * @date 05-26-2004
 */
MultimediaSubsystem::MultimediaSubsystem()
{
    // fmod initialisieren und Fehler zuruecksetzen.
    /* TODO 
    FSOUND_SetMaxHardwareChannels(16);
    FSOUND_SetMinHardwareChannels(8);
    FSOUND_SetOutput(FSOUND_OUTPUT_ALSA);
    FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);
    // File Callbacks fuer FMOD setzen
    FSOUND_File_SetCallbacks(
        (FSOUND_OPENCALLBACK)MultimediaSubsystem::open,
        (FSOUND_CLOSECALLBACK)MultimediaSubsystem::close,
        (FSOUND_READCALLBACK)MultimediaSubsystem::read,
        (FSOUND_SEEKCALLBACK)MultimediaSubsystem::seek,
        (FSOUND_TELLCALLBACK)MultimediaSubsystem::tell); 

        FSOUND_Init(44100, 32, 0); // TODO Wenns schiefgeht.
	log(Ogre::LML_TRIVIAL, "fmod initialisiert");
    
    FSOUND_3D_SetRolloffFactor(0.5);
    FSOUND_SetSFXMasterVolume(255);

    // Wir initialisieren den Listener
    // Position of the listener.
    float v[3] = {0, 0, 0};
    FSOUND_3D_Listener_SetAttributes(v, v, 1, 0, 0, 1, 0, 0); 
    log(Ogre::LML_TRIVIAL, "Listener set"); */
    
    //Singletons erzeugen 
    new SoundManager();
    
    // SoundUpdates anschmeissen.	
    new SoundUpdateTask();
}

/**
 * @author JoSch
 * @date 05-26-2004
 */
MultimediaSubsystem::~MultimediaSubsystem()
{
    delete SoundUpdateTask::getSingletonPtr();
    delete SoundManager::getSingletonPtr();
    // TODO FSOUND_Close();
}

/**
 * @author Blakharaz
 * @date 10-14-2004
 */
void MultimediaSubsystem::log(Ogre::LogMessageLevel level, const Ogre::String& msg, const Ogre::String& ident )
{
    if (Logger::getSingletonPtr() != 0)
    {
        Logger::getSingleton().log(level, "Sound", msg, ident);
    }
}

/**
 * @author JoSch
 * @date 07-03-2005
 */
void MultimediaSubsystem::update(Real elapsedTime)
{
    setElapsedTime(elapsedTime);
    // TODO FSOUND_Update();
    for(VideoList::iterator i = mVideoList.begin(); // TODO: The STL way
        i != mVideoList.end(); i++)
    {
        (*i)->update();
    }
}

/**
 * @author JoSch
 * @date 07-24-2005
 */
Real MultimediaSubsystem::getElapsedTime() const
{
    return mElapsedTime;
}

/**
 * @author JoSch
 * @date 07-24-2005
 */
void MultimediaSubsystem::setElapsedTime(Real elapsedTime)
{
    mElapsedTime = elapsedTime;
}

/**
 * @param handle Das Handle der zu schliessenden Datei
 * @author JoSch
 * @date 08-22-2005
 */
void MultimediaSubsystem::close(void *handle)
{
    if (handle != 0)
    {
        DataStreamPtr ds = *reinterpret_cast<DataStreamPtr*>(handle);
        if (!ds.isNull())
        {
            MultimediaSubsystem::getSingleton().log(LML_TRIVIAL, "Stream closed");
            ds->close();
        }
    }
}

/**
 * @param name. Der der Name der zu oeffnenden Datei
 * @author JoSch
 * @date 08-22-2005
 */
void *MultimediaSubsystem::open(const char *name)
{
    SoundResource res(*SoundManager::getSingleton().getByName(name));
    res.load();
    DataStreamPtr *dsp = new DataStreamPtr(res.getDataStream());
    MultimediaSubsystem::getSingleton().log(LML_TRIVIAL,
        "Opened file " + String(name));
    return dsp;
}

/**
 * @param buffer. Zu fuellender Buffer
 * @param size. Die Groesse der Daten
 * @param handle. Handle der Datei
 * @return. Anzahl der Bytes, die ERFOLGREICH gelesen wurden.
 * @author JoSch
 * @date 08-22-2005
 */
int MultimediaSubsystem::read(void *buffer, int size, void *handle)
{
    if (handle != 0)
    {
        DataStreamPtr *ds = reinterpret_cast<DataStreamPtr*>(handle);
        if (!ds->isNull())
        {
            return (*ds)->read(buffer, size);
        }
    }
    return 0;
}

/**
 * @param handle. Das Handle der Datei.
 * @param pos. Gesuchte Dateiposition.
 * @param mode. Seekmode.
 * @return 0 bei Erfolg, Nicht-0 ansonsten.
 * @author JoSch
 * @date 08-22-2005
 */
int MultimediaSubsystem::seek(void *handle, int pos, signed char mode)
{
    if (handle != 0)
    {
        DataStreamPtr ds = *reinterpret_cast<DataStreamPtr*>(handle);
        if (!ds.isNull())
        {
            if (mode == SEEK_END)
            {
                pos += ds->size();
            }
            if (mode == SEEK_CUR)
            {
                pos += ds->tell();
            }
            ds->seek(pos);
            return 0;
        }
    }
    return -1;
}

/**
 * @return Aktuelle Position
 * @author JoSch
 * @date 08-22-2005
 */
int MultimediaSubsystem::tell(void *handle)
{
    if (handle != 0)
    {
        DataStreamPtr ds = *reinterpret_cast<DataStreamPtr*>(handle);
        if (!ds.isNull())
        {
            return ds->tell();
        }
    }
    return 0;
}

/**
 * @param video Das Video, das hinzugefügt werden soll.
 * @author JoSch
 * @date 09-06-2005
 */
void MultimediaSubsystem::addVideo(Video *video)
{
    mVideoList.push_back(video);
}

/**
 * @return Alle Videos in der Liste löschen
 * @author JoSch
 * @date 09-06-2005
 */
void MultimediaSubsystem::clearVideos()
{
    mVideoList.clear();
}

/**
 * @param Das Video, das gelöscht werden soll
 * @author JoSch
 * @date 09-06-2005
 */
void MultimediaSubsystem::removeVideo(Video *video)
{
    for(VideoList::iterator i = mVideoList.begin();
        i != mVideoList.end();)
    {
        if (*i == video)
        {
            i = mVideoList.erase(i);
        } else {
            i ++;
        }
    }
}

}
