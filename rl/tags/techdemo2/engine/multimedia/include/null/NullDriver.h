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
#ifndef NULLDRIVER_H_
#define NULLDRIVER_H_

#include "MultimediaPrerequisites.h"
#include "SoundDriver.h"

namespace rl
{

/** Diese Klasse ist der Nulltreiber, der immer
 * vorhanden ist und einfach nichts ausgibt.
 */
class _RlMultimediaExport NullDriver : public rl::SoundDriver
{
public:
	/// Der Treibername
	static Ogre::String NAME;
    /// Der Konstruktor
	NullDriver(Ogre::ResourceManager* soundResourceManager);
    /// Der Destruktor
	~NullDriver();
    /// Ist der Treiber angeschaltet?
    bool isDriverAvailable();
    /// Initialisiere den Treiber.
    void init();
    /// Deinitialisiere den Treiber.
    void deInit();
    /// Der Name des Treibers
	Ogre::String getName() const;
    /// Update-Aufgaben erledigen
    void update();
    /// Einen Sound-Stream mit Resource erzeugen
    Sound *createStream(const SoundResourcePtr &res);
    /// Einen Sound-Sample mit Resource erzeugen
    Sound *createSample(const SoundResourcePtr &res);
    /// Einen SoundChannel erzeugen
    SoundChannel *createChannel(Sound *channel, const Ogre::String &name);
    /// Einen Soundlistener erzeugen
    ListenerMovable *createListener(const Ogre::String &name);

    /// Den  Konfigurationsdialog für Treiber aufrufen
    void doConfig();
    /// Die Einstellungen in Datei schreiben
    void saveConf(rl::ConfigFile &conf) const;
    /// Die Einstellungen laden
    void loadConf(rl::ConfigFile &conf);

	/// Setzt den Faktor f, mit der die Lautstärke nach der Formel 1/(f*Entfernung) abnimmt
	void setRolloffFactor(const Ogre::Real&);
	const Ogre::Real getRolloffFactor();
    
protected:
    /// Informationen über den Treiber ausgeben
    void printData() const;
};

}

#endif /*NULLDRIVER_H_*/
