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
#include "Video.h"
#include <stdio.h>
extern "C" {
    #include <fmod.h>
    #include <fmod_errors.h>
}
#include "SoundUpdateTask.h"

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
//    FSOUND_SetOutput(FSOUND_OUTPUT_ALSA);
    FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);
    // File Callbacks fuer FMOD setzen
    FSOUND_File_SetCallbacks(
        (FSOUND_OPENCALLBACK)SoundSubsystem::open,
        (FSOUND_CLOSECALLBACK)SoundSubsystem::close,
        (FSOUND_READCALLBACK)SoundSubsystem::read,
        (FSOUND_SEEKCALLBACK)SoundSubsystem::seek,
        (FSOUND_TELLCALLBACK)SoundSubsystem::tell);  

    FSOUND_Init(44100, 32, 0); // TODO Wenns schiefgeht.
	Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "fmod initialisiert");
    printData();
    
    //FSOUND_3D_SetRolloffFactor(0.5);
    FSOUND_SetSFXMasterVolume(255);

    // Wir initialisieren den Listener
    // Position of the listener.
    float v[3] = {0, 0, 0};
    FSOUND_3D_Listener_SetAttributes(v, v, 1, 0, 0, 1, 0, 0);
    Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "Listener set");

    
    //Singletons erzeugen 
    new SoundManager();
    
    // SoundUpdates anschmeissen.	
    new SoundUpdateTask();
}


/**
 * Ausdruck einiger Werte des Soundsystems.
 * @author JoSch
 * @date 01-20-2006
 */
void SoundSubsystem::printData()
{
        switch (FSOUND_GetOutput())
    {
        case FSOUND_OUTPUT_NOSOUND:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "NoSound");
            break;
#if OGRE_PLATFORM == PLATFORM_WIN32
        case FSOUND_OUTPUT_WINMM:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "Windows Multimedia Waveout");
            break;
        case FSOUND_OUTPUT_DSOUND:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "Direct Sound");
            break;
        case FSOUND_OUTPUT_ASIO:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "ASIO");
            break;
#else
        case FSOUND_OUTPUT_OSS:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "Open Sound System");
            break;
        case FSOUND_OUTPUT_ESD:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "Enlightment Sound Daemon");
            break;
        case FSOUND_OUTPUT_ALSA:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "Alsa");
            break;
#endif
    }

    Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FSOUND Mixer");
    switch (FSOUND_GetMixer())
    {
        case FSOUND_MIXER_BLENDMODE:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FSOUND_MIXER_BLENDMODE");
            break;
        case FSOUND_MIXER_MMXP5:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FSOUND_MIXER_MMXP5");
            break;
        case FSOUND_MIXER_MMXP6:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FSOUND_MIXER_MMXP6");
            break;
        case FSOUND_MIXER_QUALITY_FPU:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FSOUND_MIXER_QUALITY_FPU");
            break;
        case FSOUND_MIXER_QUALITY_MMXP5:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FSOUND_MIXER_QUALITY_MMXP5");
            break;
        case FSOUND_MIXER_QUALITY_MMXP6:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FSOUND_MIXER_QUALITY_MMXP6");
            break;
    };
 
    unsigned int caps = 0;
    FSOUND_GetDriverCaps(FSOUND_GetDriver(), &caps);
    
    Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "Driver capabilities");
    Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, FSOUND_GetDriverName(FSOUND_GetDriver()));
    if (!caps)
        Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "- This driver will support software mode only.\n  It does not properly support 3D sound hardware.");
    if (caps & FSOUND_CAPS_HARDWARE)
        Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "- Driver supports hardware 3D sound!");
    if (caps & FSOUND_CAPS_EAX2)
        Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "- Driver supports EAX 2 reverb!");
    if (caps & FSOUND_CAPS_EAX3)
        Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "- Driver supports EAX 3 reverb!");
 
    int num2d, num3d;
    FSOUND_GetNumHWChannels(&num2d, &num3d, NULL);
    String line = "HW channels 3D: " + StringConverter::toString(num3d) + ", 2D: " + StringConverter::toString(num2d);
    Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, line);
}

/**
 * @author JoSch
 * @date 05-26-2004
 */
SoundSubsystem::~SoundSubsystem()
{
    if (SoundManager::getSingletonPtr())
    {
        delete SoundManager::getSingletonPtr();
    }
    FSOUND_Close();
}

/**
 * @author JoSch
 * @date 07-03-2005
 */
void SoundSubsystem::update(Real elapsedTime)
{
    setElapsedTime(elapsedTime);
    FSOUND_Update();
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
			Logger::getSingleton().log(Logger::SOUND, LML_TRIVIAL, "Stream closed");
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
	Logger::getSingleton().log(Logger::SOUND, LML_TRIVIAL,
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

/**
 * @param video Das Video, das hinzugefügt werden soll.
 * @author JoSch
 * @date 09-06-2005
 */
void SoundSubsystem::addVideo(Video *video)
{
    mVideoList.push_back(video);
}

/**
 * @return Alle Videos in der Liste löschen
 * @author JoSch
 * @date 09-06-2005
 */
void SoundSubsystem::clearVideos()
{
    mVideoList.clear();
}

/**
 * @param Das Video, das gelöscht werden soll
 * @author JoSch
 * @date 09-06-2005
 */
void SoundSubsystem::removeVideo(Video *video)
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
