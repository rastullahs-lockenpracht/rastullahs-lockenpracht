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
#include "stdinc.h"

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
 * Zeigt an, ob der Treiber funktionsf�hig ist.
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
bool NullDriver::initialize()
{
    return true;
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
 * Gibt den Anzeigename des Treiber zurueck.
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

Sound *NullDriver::createSoundImpl(SoundResourcePtr res, SoundType type)
{
     Sound* sound = new NullSound(res, this);
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
    ListenerMovable* listener = new NullListener(name);
    return listener;
}

void NullDriver::setRolloffFactor(const Ogre::Real&)
{
}

const Ogre::Real NullDriver::getRolloffFactor()
{
    return 1.0;
}

bool NullDriver::setEaxPreset(const Ogre::String& name)
{
    return true;
}

}
