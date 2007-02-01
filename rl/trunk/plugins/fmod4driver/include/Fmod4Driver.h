/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#ifndef _FMODDRIVER_H_
#define _FMODDRIVER_H_

#include "Fmod4DriverPrerequisites.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    #include <fmod.hpp>
    #include <fmod_errors.h>
#else
    #include <fmodex/fmod.hpp>
    #include <fmodex/fmod_errors.h>
#endif

#include "SoundDriver.h"

namespace rl
{

typedef map<Ogre::String, StringList> DriverMap;

#define CHECK_FMOD4_ERRORS(fmod4_errorcode)\
{\
    if (fmod4_errorcode != FMOD_OK)\
    {\
        Throw(rl::RuntimeException,\
            "FMOD error #"\
            + StringConverter::toString(fmod4_errorcode)\
            + " "\
            + FMOD_ErrorString(fmod4_errorcode));\
    }\
}


/** Diese Klasse ist der Treiber, der OpenAL zur
 * Ausgabe benutzt.
 */
class _RlFmod4DriverExport Fmod4Driver : public rl::SoundDriver
{
public:
	/// Der Treibername
    static Ogre::String NAME;
    /// Der Konstruktor
	Fmod4Driver(Ogre::ResourceManager* soundResourceManager);
    /// Der Destruktor
	virtual ~Fmod4Driver();
    /// Ist der Treiber angeschaltet?
    virtual bool isDriverAvailable();
    /// Initialisiere den Treiber.
    virtual bool initialize();
    /// Deinitialisiere den Treiber.
    virtual void shutdown();
    /// Der Name des Treibers
    virtual Ogre::String getName() const;
    /// Update-Aufgaben erledigen
    virtual void update();
    /// Einen Sound-Stream mit Resource erzeugen
    virtual Sound *createStream(const SoundResourcePtr &res);
    /// Einen Sound-Sample mit Resource erzeugen
    virtual Sound *createSample(const SoundResourcePtr &res);
    /// Einen Soundlistener erzeugen
    virtual ListenerMovable *createListener(const Ogre::String &name);

    virtual void setMasterVolume(const Ogre::Real& vol);

    /// Die Einstellungen in Datei schreiben
    virtual void saveConf(rl::ConfigFile &conf) const;
    /// Die Einstellungen laden
    virtual void loadConf(rl::ConfigFile &conf);

    /// Setzt den Faktor f, mit der die Lautst�rke nach der Formel 1/(f*Entfernung) abnimmt
	virtual void setRolloffFactor(const Ogre::Real&);
	virtual const Ogre::Real getRolloffFactor();

	/// Datensammlung zurueckgeben
	const DriverMap& getDriverData() const;

    FMOD::System* _getFmodSystem();

private:
    /// Informationen �ber den Treiber ausgeben
    virtual void printData() const;
    void printDriverInfo(int driver) const;

    // Wir merken uns die Konfiguration von Fmod4
	DriverMap mDriverData;
    FMOD::System* mFmod4System;
    FMOD::ChannelGroup* mMasterChannelGroup;

    static FMOD_RESULT F_CALLBACK open(
        const char *  name, 
        int  unicode, 
        unsigned int *  filesize, 
        void **  handle, 
        void **  userdata
    );

    static FMOD_RESULT F_CALLBACK close(
        void *  handle, 
        void *  userdata
    );

    static FMOD_RESULT F_CALLBACK read(
        void *  handle, 
        void *  buffer, 
        unsigned int  sizebytes, 
        unsigned int *  bytesread, 
        void *  userdata
    );

    static FMOD_RESULT F_CALLBACK seek(
        void *  handle, 
        unsigned int  pos, 
        void *  userdata
    );

};

}

#endif /*FMODDRIVER_H_*/
