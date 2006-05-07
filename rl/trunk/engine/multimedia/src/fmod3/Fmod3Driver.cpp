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
#ifdef WITH_FMOD3
#include "Fmod3Driver.h"
#include "MultimediaSubsystem.h"
#include "SoundResource.h"
#include "SoundManager.h"
#include "Fmod3SoundSample.h"
#include "Fmod3SoundStream.h"
#include "Fmod3SoundChannel.h"
#include "Fmod3Listener.h"

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
Fmod3Driver::Fmod3Driver():
    SoundDriver()
{
}

/** 
 * Destruktor
 * @author JoSch
 * @date 12-23-2005
 */
Fmod3Driver::~Fmod3Driver()
{
}

/** 
 * Zeigt an, ob der Treiber funktionsfähig ist.
 * @return Immer TRUE.
 * @author JoSch
 * @date 12-23-2005
 */
bool Fmod3Driver::isDriverAvailable()
{
    try {
        FSOUND_SetOutput(FSOUND_OUTPUT_ESD);
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
void Fmod3Driver::init()
{
	if (FSOUND_GetVersion() < FMOD_VERSION)
    {
		Throw(RuntimeException, 
			String("Error : You are using the wrong DLL version!  You should be using FMOD ")
			+ StringConverter::toString(FMOD_VERSION));
    }
    // fmod initialisieren und Fehler zuruecksetzen.
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
#else
    FSOUND_SetOutput(FSOUND_OUTPUT_ESD);
#endif


	FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);


	switch (FSOUND_GetOutput())
    {
        case FSOUND_OUTPUT_NOSOUND:
			MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Output: NoSound");
            break;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        case FSOUND_OUTPUT_WINMM:
            MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Output: Windows Multimedia Waveout");
            break;
        case FSOUND_OUTPUT_DSOUND:
            MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Output: Direct Sound");
            break;
        case FSOUND_OUTPUT_ASIO:
            MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Output: ASIO");
            break;
		default:
            MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Output: Unknown");
            break;
#else
        case FSOUND_OUTPUT_OSS:
            MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Output: Open Sound System");
            break;
        case FSOUND_OUTPUT_ESD:
            MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Output: Enlightment Sound Daemon");
            break;
        case FSOUND_OUTPUT_ALSA:
            MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Output: Alsa");
            break;
		default:
            MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Output: Unknown");
            break;
#endif
    }

    unsigned int caps = 0;
    FSOUND_GetDriverCaps(FSOUND_GetDriver(), &caps);
    
    MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Driver capabilities");
	const char* driverName = FSOUND_GetDriverName(FSOUND_GetDriver());
    if (driverName != NULL)
		MultimediaSubsystem::log(Ogre::LML_TRIVIAL, driverName);
	else
		MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "No driver");
    if (!caps)
        MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "- This driver will support software mode only.\n  It does not properly support 3D sound hardware.");
    if (caps & FSOUND_CAPS_HARDWARE)
        MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "- Driver supports hardware 3D sound!");
    if (caps & FSOUND_CAPS_EAX2)
        MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "- Driver supports EAX 2 reverb!");
    if (caps & FSOUND_CAPS_EAX3)
        MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "- Driver supports EAX 3 reverb!");

    // File Callbacks fuer FMOD setzen
    FSOUND_File_SetCallbacks(
        (FSOUND_OPENCALLBACK)Fmod3Driver::open,
        (FSOUND_CLOSECALLBACK)Fmod3Driver::close,
        (FSOUND_READCALLBACK)Fmod3Driver::read,
        (FSOUND_SEEKCALLBACK)Fmod3Driver::seek,
        (FSOUND_TELLCALLBACK)Fmod3Driver::tell); 

 	if (!FSOUND_Init(44100, 32, 0))
    {
        Throw(RuntimeException, "FMOD not initialized");
    }
	MultimediaSubsystem::log(Ogre::LML_TRIVIAL, 
		String("fmod initialisiert, Fehler: ")
		+ FMOD_ErrorString(FSOUND_GetError()));
   
    FSOUND_3D_SetRolloffFactor(1.0);
    FSOUND_SetSFXMasterVolume(255);

    // Wir initialisieren den Listener
    // Position of the listener.
    float v[3] = {0, 0, 0};
    FSOUND_3D_Listener_SetAttributes(v, v, 1, 0, 0, 1, 0, 0); 
    MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Driver: Listener set");
    
    // Verschiedene Daten ausgeben.
    printData();    

}

/** 
 * Beeende den Nulltreiber und gebe alle Resourcen frei.
 * @author JoSch
 * @date 12-23-2005
 */
void Fmod3Driver::deInit()
{
    FSOUND_Close();
}

/** 
 * Gibt den Anzeigename des Treiber zurück.
 * @return Anzeigename
 * @author JoSch
 * @date 12-23-2005
 */
CeGuiString Fmod3Driver::getName() const
{
    return "FMOD3";
}

/**
 * @param handle Das Handle der zu schliessenden Datei
 * @author JoSch
 * @date 08-22-2005
 */
void Fmod3Driver::close(void *handle)
{
    if (handle != 0)
    {
    	SoundResourcePtr *sound = static_cast<SoundResourcePtr*>(handle);
    	if (!sound->isNull())
    	{
    		DataStreamPtr ds = (*sound)->getDataStream();
        	if (!ds.isNull())
        	{
            	MultimediaSubsystem::log(LML_TRIVIAL, "Stream closed");
            	ds->close();
        	}
	        delete sound;
        }
    }
}

/**
 * @param name. Der der Name der zu oeffnenden Datei
 * @author JoSch
 * @date 08-22-2005
 */
void *Fmod3Driver::open(const char *name)
{
    SoundResourcePtr *res = new SoundResourcePtr(SoundManager::getSingleton().getByName(name));
    (*res)->load();
    MultimediaSubsystem::log(LML_TRIVIAL, "Opened stream " + String((*res)->getOrigin()));
    return res;
}

/**
 * @param buffer. Zu fuellender Buffer
 * @param size. Die Groesse der Daten
 * @param handle. Handle der Datei
 * @return. Anzahl der Bytes, die ERFOLGREICH gelesen wurden.
 * @author JoSch
 * @date 08-22-2005
 */
int Fmod3Driver::read(void *buffer, int size, void *handle)
{
    if (handle != 0)
    {
    	SoundResourcePtr *sound = static_cast<SoundResourcePtr*>(handle);
    	if (!sound->isNull())
    	{
    		DataStreamPtr ds = (*sound)->getDataStream();
        	if (!ds.isNull())
        	{
	            return ds->read(buffer, size);
        	}
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
int Fmod3Driver::seek(void *handle, int pos, signed char mode)
{
    if (handle != 0)
    {
    	SoundResourcePtr *sound = static_cast<SoundResourcePtr*>(handle);
    	if (!sound->isNull())
    	{
    		DataStreamPtr ds = (*sound)->getDataStream();
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
    }
    return -1;
}

/**
 * @return Aktuelle Position
 * @author JoSch
 * @date 08-22-2005
 */
int Fmod3Driver::tell(void *handle)
{
    if (handle != 0)
    {
    	SoundResourcePtr *sound = static_cast<SoundResourcePtr*>(handle);
    	if (!sound->isNull())
    	{
    		DataStreamPtr ds = (*sound)->getDataStream();
        	if (!ds.isNull())
        	{
	            return ds->tell();
        	}
        }
    }
    return 0;
}

/**
 * Update-Aufgaben erledigen
 * @author JoSch
 * @date 03-06-2006
 */
 void Fmod3Driver::update()
 {
    if (MultimediaSubsystem::getSingleton().getActiveDriver() != NULL)
    {
        MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "Updaten von Fmod3");
    }
 	FSOUND_Update();
 }
 
 /**
  * Einen Sound-Stream mit Name erzeugen
  * @return Der erzeugte Stream
  * @author JoSch
  * @date 03-06-2006
  */
 Sound *Fmod3Driver::createStream(const Ogre::String &name)
 {
 	Sound *sound = new Fmod3SoundStream(name);
 	return sound;
 }
 
 /**
  * Einen Sound-Stream mit Resource erzeugen
  * @return Der erzeugte Stream
  * @author JoSch
  * @date 03-06-2006
  */
Sound *Fmod3Driver::createStream(const SoundResourcePtr &res)
{
 	Sound *sound = new Fmod3SoundStream(res);
 	return sound;
}

/**
 * Einen Sound-Sample mit Name erzeugen
 * @return Das erzeugte Sample
 * @author JoSch
 * @date 03-06-2006
 */
Sound *Fmod3Driver::createSample(const Ogre::String &name)
{
 	Sound *sound = new Fmod3SoundSample(name);
 	return sound;	
}

/**
 * Einen Sound-Sample mit Resource erzeugen
 * @return Das erzeugte Sample
 * @author JoSch
 * @date 03-06-2006
 */
Sound *Fmod3Driver::createSample(const SoundResourcePtr &res)
{
 	Sound *sound = new Fmod3SoundSample(res);
 	return sound;
}

/**
 * Einen Sound-Channel erzeugen
 * @return Das erzeugte Sample
 * @param sound Der Sound, der kapselt wird.
 * @param name Der Name des Channels.
 * @author JoSch
 * @date 03-06-2006
 */
SoundChannel *Fmod3Driver::createChannel(Sound *sound, const Ogre::String &name)
{
 	SoundChannel *channel = new Fmod3SoundChannel(sound, name);
    if (sound->is3d())
    {
        channel->setVolume(mDefaultSoundVolume);
        mSoundSet.insert(channel);
    } else
    {
        channel->setVolume(mDefaultMusicVolume);
        mMusicSet.insert(channel);
    }
 	return channel;
}

/**
 * Einen Soundlistener erzeugen
 * @return Der erzeugte Listener
 * @param name Der Name des Channels.
 * @author JoSch
 * @date 04-04-2006
 */
ListenerMovable *Fmod3Driver::createListener(const Ogre::String &name)
{
    ListenerMovable *listener = new Fmod3Listener(name);
    return listener;
}

/**
 * Ausdruck einiger Werte des Soundsystems.
 * @author JoSch
 * @date 01-20-2006
 */
void Fmod3Driver::printData()
{
	int numDrivers = FSOUND_GetNumDrivers();
	int activeDriver = FSOUND_GetDriver();
	for(int driver = 0; driver < numDrivers; driver++)
	{
		MultimediaSubsystem::log(
			Ogre::LML_TRIVIAL, 
			String("FMOD Driver #")
			+ StringConverter::toString(driver) + ": " +
			+ FSOUND_GetDriverName(driver)
			+ (driver == activeDriver ? "(active)" : ""));
	}

    switch (FSOUND_GetMixer())
    {
        case FSOUND_MIXER_BLENDMODE:
			MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Mixer: FSOUND_MIXER_BLENDMODE");
            break;
        case FSOUND_MIXER_MMXP5:
            MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Mixer: FSOUND_MIXER_MMXP5");
            break;
        case FSOUND_MIXER_MMXP6:
            MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Mixer: FSOUND_MIXER_MMXP6");
            break;
        case FSOUND_MIXER_QUALITY_FPU:
            MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Mixer: FSOUND_MIXER_QUALITY_FPU");
            break;
        case FSOUND_MIXER_QUALITY_MMXP5:
            MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Mixer: FSOUND_MIXER_QUALITY_MMXP5");
            break;
        case FSOUND_MIXER_QUALITY_MMXP6:
            MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Mixer: FSOUND_MIXER_QUALITY_MMXP6");
            break;
        default:
            MultimediaSubsystem::log(Ogre::LML_TRIVIAL, "FMOD Mixer: Unknown");
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
    MultimediaSubsystem::log(Ogre::LML_TRIVIAL, line);
}

void Fmod3Driver::setMasterVolume(unsigned int vol)
{
	SoundDriver::setMasterVolume(vol);
	FSOUND_SetSFXMasterVolume(int(vol * 2.55));
}

/*
 * Die Konfiguration in Datei schreiben.
 * @author JoSch
 * @date 05-07-2006
 * @param conf Die Konfigurationdatei zum Schreiben.
 */
void Fmod3Driver::writeConf(ConfigFile &conf)
{
}

/*
 * Die Konfiguration lesen
 * @author JoSch
 * @date 05-07-2006
 * @param conf Die Konfigurationdatei, aus der gelesen werden soll
 */
void Fmod3Driver::loadConf(ConfigFile &conf)
{
}

/*
 * Den Konfigurationsdialog aufrufen
 * @author JoSch
 * @date 05-07-2006
 */
void Fmod3Driver::doConfig()
{
    // DO NOTHING FOR NOW
}


}
#endif // WITH_FMOD
