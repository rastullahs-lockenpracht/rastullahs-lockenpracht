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
#ifdef WITH_FMOD
#include "FmodDriver.h"
#include "MultimediaSubsystem.h"
#include "SoundResource.h"
#include "SoundManager.h"

extern "C" {
    #include <fmod.h>
    #include <fmod_errors.h>
}

using namespace Ogre;


namespace rl
{

/** 
 * Konstruktor
 * @author JoSch
 * @date 12-23-2005
 */
FmodDriver::FmodDriver()
{
}

/** 
 * Destruktor
 * @author JoSch
 * @date 12-23-2005
 */
FmodDriver::~FmodDriver()
{
}

/** 
 * Zeigt an, ob der Treiber funktionsfähig ist.
 * @return Immer TRUE.
 * @author JoSch
 * @date 12-23-2005
 */
bool FmodDriver::isDriverAvailable()
{
    try {
        FSOUND_SetOutput(FSOUND_OUTPUT_ALSA);
        FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);
        bool success = FSOUND_Init(44100, 32, 0);
        if (success)
        {
            FSOUND_Close();
        }
        return success; 
    } catch (...)
    {
        return false;
    }
    return false;
}

/** 
 * Starte den Nulltreiber und mache alle Initialisierung.
 * @author JoSch
 * @date 12-23-2005
 */
void FmodDriver::init()
{
    // fmod initialisieren und Fehler zuruecksetzen.
 
    FSOUND_SetMaxHardwareChannels(16);
    FSOUND_SetMinHardwareChannels(8);
    FSOUND_SetOutput(FSOUND_OUTPUT_ALSA);
    FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);
    // File Callbacks fuer FMOD setzen
    FSOUND_File_SetCallbacks(
        (FSOUND_OPENCALLBACK)FmodDriver::open,
        (FSOUND_CLOSECALLBACK)FmodDriver::close,
        (FSOUND_READCALLBACK)FmodDriver::read,
        (FSOUND_SEEKCALLBACK)FmodDriver::seek,
        (FSOUND_TELLCALLBACK)FmodDriver::tell); 

    bool success = FSOUND_Init(44100, 32, 0);
    MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Driver: fmod initialisiert");
    
    FSOUND_3D_SetRolloffFactor(0.5);
    FSOUND_SetSFXMasterVolume(255);

    // Wir initialisieren den Listener
    // Position of the listener.
    float v[3] = {0, 0, 0};
    FSOUND_3D_Listener_SetAttributes(v, v, 1, 0, 0, 1, 0, 0); 
    MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Driver: Listener set");    
}

/** 
 * Beeende den Nulltreiber und gebe alle Resourcen frei.
 * @author JoSch
 * @date 12-23-2005
 */
void FmodDriver::deInit()
{
    FSOUND_Close();
}

/** 
 * Gibt den Anzeigename des Treiber zurück.
 * @return Anzeigename
 * @author JoSch
 * @date 12-23-2005
 */
CeGuiString FmodDriver::getName() const
{
    return "FMOD";
}

/**
 * @param handle Das Handle der zu schliessenden Datei
 * @author JoSch
 * @date 08-22-2005
 */
void FmodDriver::close(void *handle)
{
    if (handle != 0)
    {
        DataStreamPtr ds = *reinterpret_cast<DataStreamPtr*>(handle);
        if (!ds.isNull())
        {
            MultimediaSubsystem::log(LML_TRIVIAL, "Stream closed");
            ds->close();
        }
    }
}

/**
 * @param name. Der der Name der zu oeffnenden Datei
 * @author JoSch
 * @date 08-22-2005
 */
void *FmodDriver::open(const char *name)
{
    SoundResource res(*SoundManager::getSingleton().getByName(name));
    res.load();
    DataStreamPtr *dsp = new DataStreamPtr(res.getDataStream());
    MultimediaSubsystem::log(LML_TRIVIAL,
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
int FmodDriver::read(void *buffer, int size, void *handle)
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
int FmodDriver::seek(void *handle, int pos, signed char mode)
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
int FmodDriver::tell(void *handle)
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
#endif // WITH_FMOD
