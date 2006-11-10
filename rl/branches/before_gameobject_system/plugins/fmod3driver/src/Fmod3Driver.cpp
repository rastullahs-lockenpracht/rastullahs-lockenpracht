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
#include "Fmod3Driver.h"

extern "C" {
    #include <fmod_errors.h>
}

#include "ConfigFile.h"
#include "Fmod3SoundSample.h"
#include "Fmod3SoundStream.h"
#include "Fmod3Listener.h"
#include "Logger.h"
#include "SoundResource.h"


using namespace Ogre;

Ogre::String rl::Fmod3Driver::NAME = "FMOD3";

namespace rl
{

/** 
 * Konstruktor
 * @author JoSch
 * @date 12-23-2005
 */
Fmod3Driver::Fmod3Driver(Ogre::ResourceManager* soundResourceManager):
    SoundDriver(soundResourceManager)
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
		// TODO Anhand der Konfiguration
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
#else
        FSOUND_SetOutput(FSOUND_OUTPUT_ESD);
#endif
        FSOUND_SetMixer(FSOUND_MIXER_QUALITY_AUTODETECT);
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
void Fmod3Driver::initialize()
{
	if (FSOUND_GetVersion() != FMOD_VERSION)
    {
		Throw(RuntimeException, 
			String("Error : You are using the wrong DLL version!  You should be using FMOD ")
			+ StringConverter::toString(FMOD_VERSION));
    }

        // File Callbacks fuer FMOD setzen
    FSOUND_File_SetCallbacks(
        Fmod3Driver::open,
        Fmod3Driver::close,
        Fmod3Driver::read,
        Fmod3Driver::seek,
        Fmod3Driver::tell); 

	// Daten sammeln
	collectData();

    // fmod initialisieren und Fehler zuruecksetzen.
	// TODO Initialisieren wie in Config steht.
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
#else
    FSOUND_SetOutput(FSOUND_OUTPUT_ESD);
#endif

	FSOUND_SetMixer(FSOUND_MIXER_QUALITY_AUTODETECT);
 
	switch (FSOUND_GetOutput())
    {
        case FSOUND_OUTPUT_NOSOUND:
			LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Output: NoSound");
            break;
        case FSOUND_OUTPUT_WINMM:
            LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Output: Windows Multimedia Waveout");
            break;
        case FSOUND_OUTPUT_DSOUND:
            LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Output: Direct Sound");
            break;
        case FSOUND_OUTPUT_ASIO:
            LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Output: ASIO");
            break;
        case FSOUND_OUTPUT_OSS:
            LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Output: Open Sound System");
            break;
        case FSOUND_OUTPUT_ESD:
            LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Output: Enlightment Sound Daemon");
            break;
        case FSOUND_OUTPUT_ALSA:
            LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Output: Alsa");
            break;
		default:
            LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Output: Unknown");
            break;
    }

    unsigned int caps = 0;
    FSOUND_GetDriverCaps(FSOUND_GetDriver(), &caps);
    
    LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Driver capabilities");
	const char* driverName = FSOUND_GetDriverName(FSOUND_GetDriver());
    if (driverName != NULL)
		LOG_MESSAGE(Logger::MULTIMEDIA, driverName);
	else
		LOG_MESSAGE(Logger::MULTIMEDIA, "No driver");
    if (!caps)
        LOG_MESSAGE(Logger::MULTIMEDIA, "- This driver will support software mode only.\n  It does not properly support 3D sound hardware.");
    if (caps & FSOUND_CAPS_HARDWARE)
        LOG_MESSAGE(Logger::MULTIMEDIA, "- Driver supports hardware 3D sound!");
    if (caps & FSOUND_CAPS_EAX2)
        LOG_MESSAGE(Logger::MULTIMEDIA, "- Driver supports EAX 2 reverb!");
    if (caps & FSOUND_CAPS_EAX3)
        LOG_MESSAGE(Logger::MULTIMEDIA, "- Driver supports EAX 3 reverb!");

 	if (!FSOUND_Init(44100, 32, 0))
    {
        Throw(RuntimeException, "FMOD not initialized");
    }
	LOG_MESSAGE(Logger::MULTIMEDIA, 
		String("fmod initialisiert, Fehler: ")
		+ FMOD_ErrorString(FSOUND_GetError()));
   
    FSOUND_3D_SetRolloffFactor(1.0);
    FSOUND_SetSFXMasterVolume(255);

    // Wir initialisieren den Listener
    // Position of the listener.
    float v[3] = {0, 0, 0};
    FSOUND_3D_Listener_SetAttributes(v, v, 1, 0, 0, 1, 0, 0); 
    LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Driver: Listener set");
    
    // Verschiedene Daten ausgeben.
    printData();    

}

/** 
 * Beeende den Treiber und gib alle Resourcen frei.
 * @author JoSch
 * @date 12-23-2005
 */
void Fmod3Driver::shutdown()
{
    FSOUND_Close();
}

/** 
 * Gibt den Anzeigename des Treiber zurück.
 * @return Anzeigename
 * @author JoSch
 * @date 12-23-2005
 */
Ogre::String Fmod3Driver::getName() const
{
    return NAME;
}

/**
 * @param handle Das Handle der zu schliessenden Datei
 * @author JoSch
 * @date 08-22-2005
 */
void F_CALLBACKAPI Fmod3Driver::close(void *handle)
{
    if (handle != 0)
    {
    	SoundResourcePtr *sound = static_cast<SoundResourcePtr*>(handle);
    	if (!sound->isNull())
    	{
    		DataStreamPtr ds = (*sound)->getDataStream();
        	if (!ds.isNull())
        	{
				LOG_MESSAGE(Logger::MULTIMEDIA, "Stream closed");
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
void* F_CALLBACKAPI Fmod3Driver::open(const char *name)
{
	SoundResourcePtr *res = new SoundResourcePtr(sSoundResourceManager->getByName(name));
    (*res)->load();
    LOG_MESSAGE(Logger::MULTIMEDIA, 
		"Opened stream " + String((*res)->getOrigin()));
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
int F_CALLBACKAPI Fmod3Driver::read(void *buffer, int size, void *handle)
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
int F_CALLBACKAPI Fmod3Driver::seek(void *handle, int pos, signed char mode)
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
int F_CALLBACKAPI Fmod3Driver::tell(void *handle)
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
    LOG_DEBUG(Logger::MULTIMEDIA, "Updaten von Fmod3");
 	FSOUND_Update();
 }
 
 /**
  * Einen Sound-Stream mit Resource erzeugen
  * @return Der erzeugte Stream
  * @author JoSch
  * @date 03-06-2006
  */
Sound *Fmod3Driver::createStream(const SoundResourcePtr &res)
{
 	return new Fmod3SoundStream(this, res);
}

/**
 * Einen Sound-Sample mit Resource erzeugen
 * @return Das erzeugte Sample
 * @author JoSch
 * @date 03-06-2006
 */
Sound *Fmod3Driver::createSample(const SoundResourcePtr &res)
{
 	return new Fmod3SoundSample(this, res);
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
void Fmod3Driver::printData() const
{
	int numDrivers = FSOUND_GetNumDrivers();
	int activeDriver = FSOUND_GetDriver();
	for(int driver = 0; driver < numDrivers; driver++)
	{
        LOG_MESSAGE(Logger::MULTIMEDIA, 
			String("FMOD Driver #")
			+ StringConverter::toString(driver) + ": " +
			+ FSOUND_GetDriverName(driver)
			+ (driver == activeDriver ? "(active)" : ""));
	}

    switch (FSOUND_GetMixer())
    {
        case FSOUND_MIXER_BLENDMODE:
			LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Mixer: FSOUND_MIXER_BLENDMODE");
            break;
        case FSOUND_MIXER_MMXP5:
            LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Mixer: FSOUND_MIXER_MMXP5");
            break;
        case FSOUND_MIXER_MMXP6:
            LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Mixer: FSOUND_MIXER_MMXP6");
            break;
        case FSOUND_MIXER_QUALITY_FPU:
            LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Mixer: FSOUND_MIXER_QUALITY_FPU");
            break;
        case FSOUND_MIXER_QUALITY_MMXP5:
            LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Mixer: FSOUND_MIXER_QUALITY_MMXP5");
            break;
        case FSOUND_MIXER_QUALITY_MMXP6:
            LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Mixer: FSOUND_MIXER_QUALITY_MMXP6");
            break;
        default:
            LOG_MESSAGE(Logger::MULTIMEDIA, "FMOD Mixer: Unknown");
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
    LOG_MESSAGE(Logger::MULTIMEDIA, line);
}

void Fmod3Driver::setMasterVolume(const Ogre::Real& vol)
{
	SoundDriver::setMasterVolume(vol);
	FSOUND_SetSFXMasterVolume(int(vol * 255.0f));
}

/*
 * Die Konfiguration in Datei schreiben.
 * @author JoSch
 * @date 05-07-2006
 * @param conf Die Konfigurationdatei zum Schreiben.
 */
void Fmod3Driver::saveConf(ConfigFile &conf) const
{
	SoundDriver::saveConf(conf);
	conf.setValue(mRolloffFactor, "3DRolloffFactor", "Fmod3");
}

/*
 * Die Konfiguration lesen
 * @author JoSch
 * @date 05-07-2006
 * @param conf Die Konfigurationdatei, aus der gelesen werden soll
 */
void Fmod3Driver::loadConf(ConfigFile &conf)
{
	SoundDriver::loadConf(conf);
	mRolloffFactor = conf.getValue(Ogre::Real(1.0), "3DRolloffFactor", "Fmod3");
}

/*
 * Die gesammelten Daten zurückgeben
 * @return Die gesammelten Daten
 * @author JoSch
 * @date 05-31-2006
 */
const DriverMap &Fmod3Driver::getDriverData() const
{
	return mDriverData;
}

/*
 * Die Treiberdaten für einen Output sammeln
 * @param output Die Fmod-Konstante, für die wir die Treiberdaten sammeln.
 * @author JoSch
 * @date 05-31-2006
 */
const StringList Fmod3Driver::getDriversForOutput(int output) const
{
	StringList retVal;
	FSOUND_SetOutput(output);
	int numDrivers = FSOUND_GetNumDrivers();
	for(int driver = 0; driver < numDrivers; driver++)
	{
		String drivername = String(FSOUND_GetDriverName(driver));
		retVal.push_back(drivername);
	}
	return retVal;
}

/*
 * Die Treiberdaten sammeln
 * @author JoSch
 * @date 05-31-2006
 */

void Fmod3Driver::collectData()
{
	mDriverData.clear();
	mDriverData["NOSOUND"] = getDriversForOutput(FSOUND_OUTPUT_NOSOUND);
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	mDriverData["WINMM"] = getDriversForOutput(FSOUND_OUTPUT_WINMM);
	mDriverData["DSOUND"] = getDriversForOutput(FSOUND_OUTPUT_DSOUND);
	mDriverData["A3D"] = getDriversForOutput(FSOUND_OUTPUT_A3D);
#else
	mDriverData["ESD"] = getDriversForOutput(FSOUND_OUTPUT_ESD);
	mDriverData["ALSA"] = getDriversForOutput(FSOUND_OUTPUT_ALSA);
	mDriverData["OSS"] = getDriversForOutput(FSOUND_OUTPUT_OSS);
#endif
}

void Fmod3Driver::setRolloffFactor(const Ogre::Real& factor)
{
	mRolloffFactor = factor;
	FSOUND_3D_SetRolloffFactor(factor);
}

const Ogre::Real Fmod3Driver::getRolloffFactor()
{
	return mRolloffFactor;
}

void Fmod3Driver::setActiveOutput(const rl::CeGuiString &outputName)
{
}

void Fmod3Driver::checkErrors()
{
    int err = FSOUND_GetError();
    if (err != FMOD_ERR_NONE)
    {
        Throw(RuntimeException, "Fmod Error:" + StringConverter::toString(err));
    }
}

}
