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
#ifndef FMODDRIVER_H_
#define FMODDRIVER_H_

#include "MultimediaPrerequisites.h"
#include "SoundDriver.h"

namespace rl
{

typedef map<Ogre::String, StringList> DriverMap;

/** Diese Klasse ist der Treiber, der OpenAL zur
 * Ausgabe benutzt.
 */
class _RlMultimediaExport Fmod3Driver : public rl::SoundDriver
{
private:
    // FMOD-Callbacks
    static void close(void *handle);
    static void *open(const char *name);
    static int read(void *buffer, int size, void *handle);
    static int seek(void *handle, int pos, signed char mode);
    static int tell(void *handle);

	// Wir merken uns die Konfiguration von Fmod3
	DriverMap mDriverData;

	// Die Daten für einen Output holen
	const StringList getDriversForOutput(int output) const;
	/// Informationen über den Treiber sammeln
	virtual void collectData();

public:
	/// Der Treibername
	static CeGuiString NAME;
    /// Der Konstruktor
	Fmod3Driver(Ogre::ResourceManager* soundResourceManager);
    /// Der Destruktor
	virtual ~Fmod3Driver();
    /// Ist der Treiber angeschaltet?
    virtual bool isDriverAvailable();
    /// Initialisiere den Treiber.
    virtual void init();
    /// Deinitialisiere den Treiber.
    virtual void deInit();
    /// Der Name des Treibers
    virtual CeGuiString getName() const;
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

	void setMasterVolume(unsigned int vol);

    /// Den  Konfigurationsdialog für Treiber aufrufen
    virtual void doConfig();
    /// Die Einstellungen in Datei schreiben
    virtual void saveConf(rl::ConfigFile &conf) const;
    /// Die Einstellungen laden
    virtual void loadConf(rl::ConfigFile &conf);

	/// Datensammlung zurückgeben
	const DriverMap& getDriverData() const;

protected:
    /// Informationen über den Treiber ausgeben
    virtual void printData() const;
};

}

#endif /*FMODDRIVER_H_*/
#endif // WITH_FMOD
