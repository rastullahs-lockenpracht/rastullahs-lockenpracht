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
#include "Exception.h"
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

	if (FSOUND_GetVersion() < FMOD_VERSION)
    {
		Throw(RuntimeException, 
			String("Error : You are using the wrong DLL version!  You should be using FMOD ")
			+ StringConverter::toString(FMOD_VERSION));
    }
	
	initializeDriver();

	// File Callbacks fuer FMOD setzen
    FSOUND_File_SetCallbacks(
        (FSOUND_OPENCALLBACK)SoundSubsystem::open,
        (FSOUND_CLOSECALLBACK)SoundSubsystem::close,
        (FSOUND_READCALLBACK)SoundSubsystem::read,
        (FSOUND_SEEKCALLBACK)SoundSubsystem::seek,
        (FSOUND_TELLCALLBACK)SoundSubsystem::tell);  
    
	if (!FSOUND_Init(44100, 32, 0))
    {
        Throw(RuntimeException, "FMOD not initialized");
    }
	Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, 
		String("fmod initialisiert, Fehler: ")
		+ FMOD_ErrorString(FSOUND_GetError()));
	printData();
    
    FSOUND_SetSFXMasterVolume(255);

    // Wir initialisieren den Listener.
    // Position of the listener.
    float v[3] = {0, 0, 0};
    FSOUND_3D_Listener_SetAttributes(v, v, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  0.0f);
    Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "Listener set");
    
    //Singletons erzeugen 
    new SoundManager();
    
    // SoundUpdates anschmeissen.	
    new SoundUpdateTask();
}


void SoundSubsystem::initializeDriver()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
#else
    FSOUND_SetOutput(FSOUND_OUTPUT_ALSA);
#endif


	FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);


	switch (FSOUND_GetOutput())
    {
        case FSOUND_OUTPUT_NOSOUND:
			Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Output: NoSound");
            break;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        case FSOUND_OUTPUT_WINMM:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Output: Windows Multimedia Waveout");
            break;
        case FSOUND_OUTPUT_DSOUND:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Output: Direct Sound");
            break;
        case FSOUND_OUTPUT_ASIO:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Output: ASIO");
            break;
		default:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Output: Unknown");
            break;
#else
        case FSOUND_OUTPUT_OSS:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Output: Open Sound System");
            break;
        case FSOUND_OUTPUT_ESD:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Output: Enlightment Sound Daemon");
            break;
        case FSOUND_OUTPUT_ALSA:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Output: Alsa");
            break;
		default:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Output: Unknown");
            break;
#endif
    }

    unsigned int caps = 0;
    FSOUND_GetDriverCaps(FSOUND_GetDriver(), &caps);
    
    Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Driver capabilities");
	const char* driverName = FSOUND_GetDriverName(FSOUND_GetDriver());
    if (driverName != NULL)
		Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, driverName);
	else
		Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "No driver");
    if (!caps)
        Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "- This driver will support software mode only.\n  It does not properly support 3D sound hardware.");
    if (caps & FSOUND_CAPS_HARDWARE)
        Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "- Driver supports hardware 3D sound!");
    if (caps & FSOUND_CAPS_EAX2)
        Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "- Driver supports EAX 2 reverb!");
    if (caps & FSOUND_CAPS_EAX3)
        Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "- Driver supports EAX 3 reverb!");
}

/**
 * Ausdruck einiger Werte des Soundsystems.
 * @author JoSch
 * @date 01-20-2006
 */
void SoundSubsystem::printData()
{
	int numDrivers = FSOUND_GetNumDrivers();
	int activeDriver = FSOUND_GetDriver();
	for(int driver = 0; driver < numDrivers; driver++)
	{
		Logger::getSingleton().log(
			Logger::SOUND, 
			Ogre::LML_TRIVIAL, 
			String("FMOD Driver #")
			+ StringConverter::toString(driver) + ": " +
			+ FSOUND_GetDriverName(driver)
			+ (driver == activeDriver ? "(active)" : ""));
	}

    switch (FSOUND_GetMixer())
    {
        case FSOUND_MIXER_BLENDMODE:
			Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Mixer: FSOUND_MIXER_BLENDMODE");
            break;
        case FSOUND_MIXER_MMXP5:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Mixer: FSOUND_MIXER_MMXP5");
            break;
        case FSOUND_MIXER_MMXP6:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Mixer: FSOUND_MIXER_MMXP6");
            break;
        case FSOUND_MIXER_QUALITY_FPU:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Mixer: FSOUND_MIXER_QUALITY_FPU");
            break;
        case FSOUND_MIXER_QUALITY_MMXP5:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Mixer: FSOUND_MIXER_QUALITY_MMXP5");
            break;
        case FSOUND_MIXER_QUALITY_MMXP6:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Mixer: FSOUND_MIXER_QUALITY_MMXP6");
            break;
        default:
            Logger::getSingleton().log(Logger::SOUND, Ogre::LML_TRIVIAL, "FMOD Mixer: Unknown");
            break;
    };
 
    int num2d, num3d, total;
    FSOUND_GetNumHWChannels(&num2d, &num3d, &total);
    String line = 
		"HW channels 3D: " 
		+ StringConverter::toString(num3d) 
		+ ", 2D: " 
		+ StringConverter::toString(num2d)
		+ ", Total: " 
		+ StringConverter::toString(total);
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
