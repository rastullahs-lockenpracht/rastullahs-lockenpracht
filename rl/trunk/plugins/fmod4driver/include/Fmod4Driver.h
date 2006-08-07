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
#ifndef _FMODDRIVER_H_
#define _FMODDRIVER_H_

#include "Fmod4DriverPrerequisites.h"

#include <fmod.hpp>

#include "SoundDriver.h"

namespace rl
{

    class Fmod4Sound;
    class Fmod4SoundChannel;
    class SoundChannel;
    
typedef map<Ogre::String, StringList> DriverMap;

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
    virtual void initialize();
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
    /// Einen SoundChannel erzeugen
    virtual SoundChannel *createChannel(Sound *channel, const Ogre::String &name);
    /// Einen Soundlistener erzeugen
    virtual ListenerMovable *createListener(const Ogre::String &name);

    virtual void setMasterVolume(const Ogre::Real& vol);

    /// Die Einstellungen in Datei schreiben
    virtual void saveConf(rl::ConfigFile &conf) const;
    /// Die Einstellungen laden
    virtual void loadConf(rl::ConfigFile &conf);

    /// Setzt den Faktor f, mit der die Lautstärke nach der Formel 1/(f*Entfernung) abnimmt
	virtual void setRolloffFactor(const Ogre::Real&);
	virtual const Ogre::Real getRolloffFactor();

	/// Datensammlung zurückgeben
	const DriverMap& getDriverData() const;

    FMOD::System* _getFmodSystem();

    void checkErrors(const FMOD_RESULT& errorcode) const;

protected:
    /// Informationen über den Treiber ausgeben
    virtual void printData() const;
    void printDriverInfo(int driver) const;

private:
	// Wir merken uns die Konfiguration von Fmod4
	DriverMap mDriverData;
    FMOD::System* mFmod4System;

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
