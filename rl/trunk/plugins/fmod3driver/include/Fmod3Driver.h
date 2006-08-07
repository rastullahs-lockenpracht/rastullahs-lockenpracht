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
#ifndef _FMOD3DRIVER_H_
#define _FMOD3DRIVER_H_

#include "Fmod3DriverPrerequisites.h"
#include "SoundDriver.h"

extern "C" {
    #include <fmod.h>
}


namespace rl
{

typedef map<Ogre::String, StringList> DriverMap;

/** Diese Klasse ist der Treiber, der OpenAL zur
 * Ausgabe benutzt.
 */
class _RlFmod3DriverExport Fmod3Driver : public rl::SoundDriver
{
public:
	/// Der Treibername
    static Ogre::String NAME;
    /// Der Konstruktor
	Fmod3Driver(Ogre::ResourceManager* soundResourceManager);
    /// Der Destruktor
	virtual ~Fmod3Driver();
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

	/// Setzt den Faktor f, mit der die Lautstärke nach der Formel 1/(f*Entfernung) abnimmt
	virtual void setRolloffFactor(const Ogre::Real&);
	virtual const Ogre::Real getRolloffFactor();

    /// Die Einstellungen in Datei schreiben
    virtual void saveConf(rl::ConfigFile &conf) const;
    /// Die Einstellungen laden
    virtual void loadConf(rl::ConfigFile &conf);

	/// Datensammlung zurückgeben
	const DriverMap& getDriverData() const;

    void setActiveOutput(const CeGuiString& outputName);

protected:
    /// Informationen über den Treiber ausgeben
    void printData() const;

private:
    // FMOD-Callbacks
    static void* F_CALLBACKAPI open(const char *name);
    static void F_CALLBACKAPI close(void *handle);
    static int F_CALLBACKAPI read(void *buffer, int size, void *handle);
    static int F_CALLBACKAPI seek(void *handle, int pos, signed char mode);
    static int F_CALLBACKAPI tell(void *handle);

	// Wir merken uns die Konfiguration von Fmod3
	DriverMap mDriverData;

	Ogre::Real mRolloffFactor;

	// Die Daten für einen Output holen
	const StringList getDriversForOutput(int output) const;
	/// Informationen über den Treiber sammeln
	void collectData();
};

}

#endif /*FMODDRIVER_H_*/
