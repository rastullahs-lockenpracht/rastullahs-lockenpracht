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
#include "SoundSubsystem.h"
#include "SoundManager.h"
#include "Logger.h"
#include "SoundResource.h"
#include <stdio.h>

using namespace Ogre;

template<> rl::SoundSubsystem* Singleton<rl::SoundSubsystem>::ms_Singleton = 0;

namespace rl {

/**
 * @return Das Singleton-Objekt
 * @author JoSch
 * @date 05-26-2004
 */
SoundSubsystem& SoundSubsystem::getSingleton(void)
{
	return Singleton<SoundSubsystem>::getSingleton();
}

/**
 * @return Das Singleton-Zeiger
 * @author JoSch
 * @date 05-26-2004
 */
SoundSubsystem* SoundSubsystem::getSingletonPtr(void)
{
    return Singleton<SoundSubsystem>::getSingletonPtr();
}

/**
 * @author JoSch
 * @date 05-26-2004
 */
SoundSubsystem::SoundSubsystem()
{
    // fmod initialisieren und Fehler zuruecksetzen.
    FSOUND_SetMaxHardwareChannels(16);
    FSOUND_SetMinHardwareChannels(8);
    /// TODO: More choices
    //FSOUND_SetOutput(FSOUND_OUTPUT_ALSA);
    FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);
    // File Callbacks fuer FMOD setzen
    FSOUND_File_SetCallbacks(
        (FSOUND_OPENCALLBACK)SoundSubsystem::open,
        (FSOUND_CLOSECALLBACK)SoundSubsystem::close,
        (FSOUND_READCALLBACK)SoundSubsystem::read,
        (FSOUND_SEEKCALLBACK)SoundSubsystem::seek,
        (FSOUND_TELLCALLBACK)SoundSubsystem::tell); 

    FSOUND_Init(44100, 32, 0); // TODO Wenns schiefgeht.
	log(Ogre::LML_TRIVIAL, "fmod initialisiert");
    
    // Wir initialisieren den Listener
    // Position of the listener.
    float v[3] = {0, 0, 0};
    FSOUND_3D_Listener_SetAttributes(v, v, 1, 0, 0, 1, 0, 0);
    log(Ogre::LML_TRIVIAL, "Listener set");
    
    //Singletons erzeugen 
    new SoundManager();

}

/**
 * @author JoSch
 * @date 05-26-2004
 */
SoundSubsystem::~SoundSubsystem()
{
    delete SoundManager::getSingletonPtr();
    FSOUND_Close();
}

/**
 * @author Blakharaz
 * @date 10-14-2004
 */
void SoundSubsystem::log(Ogre::LogMessageLevel level, const Ogre::String& msg, const Ogre::String& ident )
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
void SoundSubsystem::update(Real elapsedTime)
{
    setElapsedTime(elapsedTime);
    FSOUND_Update();
}

/**
 * @author JoSch
 * @date 07-24-2005
 */
Real SoundSubsystem::getElapsedTime() const
{
    return mElapsedTime;
}

/**
 * @author JoSch
 * @date 07-24-2005
 */
void SoundSubsystem::setElapsedTime(Real elapsedTime)
{
    mElapsedTime = elapsedTime;
}

/**
 * @param handle Das Handle der zu schliessenden Datei
 * @author JoSch
 * @date 08-22-2005
 */
void SoundSubsystem::close(void *handle)
{
    if (handle != 0)
    {
        DataStreamPtr ds = *reinterpret_cast<DataStreamPtr*>(handle);
        if (!ds.isNull())
        {
            SoundSubsystem::getSingleton().log(LML_TRIVIAL, "Stream closed");
            ds->close();
        }
    }
}

/**
 * @param name. Der der Name der zu oeffnenden Datei
 * @author JoSch
 * @date 08-22-2005
 */
void *SoundSubsystem::open(const char *name)
{
    SoundResource res(*SoundManager::getSingleton().getByName(name));
    res.load();
    DataStreamPtr *dsp = new DataStreamPtr(res.getDataStream());
    SoundSubsystem::getSingleton().log(LML_TRIVIAL,
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
int SoundSubsystem::read(void *buffer, int size, void *handle)
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
int SoundSubsystem::seek(void *handle, int pos, signed char mode)
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
int SoundSubsystem::tell(void *handle)
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

}
