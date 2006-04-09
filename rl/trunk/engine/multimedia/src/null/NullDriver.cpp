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
#include "NullSoundChannel.h"
#include "NullListener.h"

namespace rl
{

/** 
 * Konstruktor
 * @author JoSch
 * @date 12-23-2005
 */
NullDriver::NullDriver()
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
void NullDriver::init()
{
}

/** 
 * Beeende den Nulltreiber und gebe alle Resourcen frei.
 * @author JoSch
 * @date 12-23-2005
 */
void NullDriver::deInit()
{
}

/** 
 * Gibt den Anzeigename des Treiber zurück.
 * @return Anzeigename
 * @author JoSch
 * @date 12-23-2005
 */
CeGuiString NullDriver::getName() const
{
    return "Nulltreiber";
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
  * Einen Sound-Stream mit Name erzeugen
  * @return Der erzeugte Stream
  * @param name Der Name des Stücks zur Erzeugung des Sounds
  * @author JoSch
  * @date 03-06-2006
  */
 Sound *NullDriver::createStream(const Ogre::String &name)
 {
 	Sound *sound = new NullSound(name);
 	return sound;
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
 * Einen Sound-Sample mit Name erzeugen
 * @return Das erzeugte Sample
 * @param name Der Name des Stücks zur Erzeugung des Sounds
 * @author JoSch
 * @date 03-06-2006
 */
Sound *NullDriver::createSample(const Ogre::String &name)
{
 	Sound *sound = new NullSound(name);
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
 * Einen Sound-Channel erzeugen
 * @return Das erzeugte Sample
 * @param sound Der Sound, der kapselt wird.
 * @param name Der Name des Channels.
 * @author JoSch
 * @date 03-06-2006
 */
SoundChannel *NullDriver::createChannel(Sound *sound, const Ogre::String &name)
{
 	SoundChannel *channel = new NullSoundChannel(sound, name);
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
ListenerMovable *NullDriver::createListener(const Ogre::String &name)
{
    ListenerMovable *listener = new NullListener(name);
    return listener;
}


/**
 * Ausdruck einiger Werte des Soundsystems.
 * @author JoSch
 * @date 01-20-2006
 */
void NullDriver::printData()
{
}

}
