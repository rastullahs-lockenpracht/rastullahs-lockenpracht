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
#ifndef SOUNDDRIVER_H_
#define SOUNDDRIVER_H_

#include "MultimediaPrerequisites.h"
#include <OgreResourceManager.h>
#include "SoundChannel.h"

namespace rl
{
    class ConfigFile;
    class ListenerMovable;
	class Sound;
    class SoundResourcePtr;

/**
 * Diese Klasse dient zur Definition einer Schnittstelle
 * zu den möglichen Soundbibliotheken. Sie geben Preis, ob
 * ein Treiber vorhanden ist oder nicht. Der Nulltreiber
 * existiert dabei immer.
 */
class _RlMultimediaExport SoundDriver
{
public:
    /// Der Konstruktor
	SoundDriver(Ogre::ResourceManager* soundResourceManager);
    /// Der Destruktor
	virtual ~SoundDriver();
    /// Zeigt an, ob der Treiber funktionsfähig ist.
    virtual bool isDriverAvailable() = 0;
    /// Initialisiere den Treiber
    virtual void init() = 0;
    /// Beende den Treiber
    virtual void deInit() = 0;
    /// Der Name des Treibers
    virtual CeGuiString getName() const = 0;
    /// Update-Aufgaben erledigen
    virtual void update() = 0;
    /// Einen Sound-Stream mit Resource erzeugen
    virtual Sound *createStream(const SoundResourcePtr &res) = 0;
    /// Einen Sound-Sample mit Resource erzeugen
    virtual Sound *createSample(const SoundResourcePtr &res) = 0;
    /// Einen SoundChannel erzeugen
    virtual SoundChannel *createChannel(Sound *channel, const Ogre::String &name) = 0;
    /// Einen Soundlistener erzeugen
    virtual ListenerMovable *createListener(const Ogre::String &name) = 0;
    

	/// Setzt den Faktor f, mit der die Lautstärke nach der Formel 1/(f*Entfernung) abnimmt
	virtual void setRolloffFactor(const Ogre::Real&) = 0;
	virtual const Ogre::Real getRolloffFactor() = 0;
    /// Die Standardlautstärke für Musik einstellen
    void setDefaultMusicVolume(unsigned int vol);
    /// Die Standardlautstärke für Musik zurückgeben.
    unsigned int getDefaultMusicVolume() const;
    /// Die Standardlautstärke für Musik einstellen
    void setDefaultSoundVolume(unsigned int vol);
    /// Die Standardlautstärke für Musik zurückgeben.
    unsigned int getDefaultSoundVolume() const;
    /// Die Lautstärke aller Sounds
    virtual void setMasterVolume(unsigned int vol);
    /// Die Standardlautstärke für Musik zurückgeben.
    unsigned int getMasterVolume() const;
    /// Einen Sound aus den Listen entfernen
    void removeFromLists(SoundChannel *channel);
    
    /// Den  Konfigurationsdialog für Treiber aufrufen
    virtual void doConfig() = 0;
    /// Die Einstellungen in Datei schreiben
    virtual void saveConf(rl::ConfigFile &conf) const;
    /// Die Einstellungen laden
    virtual void loadConf(rl::ConfigFile &conf);
    
    
protected:
    /// Informationen über den Treiber ausgeben
    virtual void printData() const = 0;
    /// Liste der Musikstücke
    SoundChannelSet mMusicSet;
    /// Liste der Sounds
    SoundChannelSet mSoundSet;
    /// Standardeinstellung für Musik
    unsigned int mDefaultMusicVolume;
    /// Standardeinstellung für Sounds
    unsigned int mDefaultSoundVolume;

	unsigned int mMasterVolume;

	static Ogre::ResourceManager* sSoundResourceManager;
    
};

}

#endif /*SOUNDDRIVER_H_*/
