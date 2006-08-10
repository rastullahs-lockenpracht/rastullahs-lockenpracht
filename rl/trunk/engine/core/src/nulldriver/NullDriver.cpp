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
#include "NullDriver.h"
#include "NullSound.h"
#include "NullListener.h"


namespace rl
{
Ogre::String NullDriver::NAME = "NullDriver";

/** 
 * Konstruktor
 * @author JoSch
 * @date 12-23-2005
 */
NullDriver::NullDriver(Ogre::ResourceManager* soundResourceManager)
 : SoundDriver(soundResourceManager)
{
}

/** 
 * Destruktor
 * @author JoSch
 * @date 12-23-2005
 */
NullDriver::~NullDriver()
{
}

/** 
 * Zeigt an, ob der Treiber funktionsfähig ist.
 * @return Immer TRUE.
 * @author JoSch
 * @date 12-23-2005
 */
bool NullDriver::isDriverAvailable()
{
    return true;
}

/** 
 * Starte den Nulltreiber und mache alle Initialisierung.
 * @author JoSch
 * @date 12-23-2005
 */
void NullDriver::initialize()
{
}

/** 
 * Beeende den Nulltreiber und gib alle Resourcen frei.
 * @author JoSch
 * @date 12-23-2005
 */
void NullDriver::shutdown()
{
}

/** 
 * Gibt den Anzeigename des Treiber zurück.
 * @return Anzeigename
 * @author JoSch
 * @date 12-23-2005
 */
Ogre::String NullDriver::getName() const
{
    return NAME;
}

/**
 * Update-Aufgaben erledigen
 * @author JoSch
 * @date 03-06-2006
 */
 void NullDriver::update()
 {
 	// Erstmal nichts zu tun.
 }
 
 /**
  * Einen Sound-Stream mit Resource erzeugen
  * @return Der erzeugte Stream
  * @param res Die Resource zur Erzeugung des Sounds
  * @author JoSch
  * @date 03-06-2006
  */
Sound *NullDriver::createStream(const SoundResourcePtr &res)
{
 	Sound *sound = new NullSound(res);
 	return sound;
}

/**
 * Einen Sound-Sample mit Resource erzeugen
 * @return Das erzeugte Sample
 * @param res Die Resource zur Erzeugung des Sounds
 * @author JoSch
 * @date 03-06-2006
 */
Sound *NullDriver::createSample(const SoundResourcePtr &res)
{
 	Sound *sound = new NullSound(res);
 	return sound;
}

/**
 * Einen Soundlistener erzeugen
 * @return Der erzeugte Listener
 * @param name Der Name des Channels.
 * @author JoSch
 * @date 04-04-2006
 */
ListenerMovable *NullDriver::createListener(const Ogre::String &name)
{
    ListenerMovable *listener = new NullListener(name);
    return listener;
}


/*
 * Die Konfiguration in Datei schreiben.
 * @author JoSch
 * @date 05-07-2006
 * @param conf Die Konfigurationdatei zum Schreiben.
 */
void NullDriver::saveConf(ConfigFile &conf) const
{
	SoundDriver::saveConf(conf);
    // DO NOTHING
}

/*
 * Die Konfiguration lesen
 * @author JoSch
 * @date 05-07-2006
 * @param conf Die Konfigurationdatei, aus der gelesen werden soll
 */
void NullDriver::loadConf(ConfigFile &conf)
{
	SoundDriver::loadConf(conf);
    // DO NOTHING
}

void NullDriver::setRolloffFactor(const Ogre::Real&)
{
}

const Ogre::Real NullDriver::getRolloffFactor()
{
	return 1.0;
}

}
