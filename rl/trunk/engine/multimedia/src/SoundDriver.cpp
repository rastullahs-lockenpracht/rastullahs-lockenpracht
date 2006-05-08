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
#include "SoundDriver.h"
#include "ConfigFile.h"
#include "OgreStringConverter.h"

using namespace Ogre;

namespace rl
{

SoundDriver::SoundDriver():
    mDefaultMusicVolume(40),
    mDefaultSoundVolume(100),
	mMasterVolume(100)
{
}

SoundDriver::~SoundDriver()
{
}

/// Die Standardlautstärke für Musik einstellen
void SoundDriver::setDefaultMusicVolume(unsigned int vol)
{
    mDefaultMusicVolume = vol;
    SoundChannelSet::iterator it;
    for(it = mMusicSet.begin(); it != mMusicSet.end(); it++)
    {
        (*it)->setVolume(vol);
    }
}

/// Die Standardlautstärke für Musik zurückgeben.
unsigned int SoundDriver::getDefaultMusicVolume() const
{
    return mDefaultMusicVolume;
}

/// Die Standardlautstärke für Musik einstellen
void SoundDriver::setDefaultSoundVolume(unsigned int vol)
{
    mDefaultSoundVolume = vol;
    SoundChannelSet::iterator it;
    for(it = mSoundSet.begin(); it != mSoundSet.end(); it++)
    {
        (*it)->setVolume(vol);
    }
}

/// Die Standardlautstärke für Musik zurückgeben.
unsigned int SoundDriver::getDefaultSoundVolume() const
{
    return mDefaultSoundVolume;
}

void SoundDriver::setMasterVolume(unsigned int vol)
{
    mMasterVolume = vol;
}

unsigned int SoundDriver::getMasterVolume() const
{
    return mMasterVolume;
}

/// Aus einer Liste entfernen.
void SoundDriver::removeFromLists(SoundChannel *channel)
{
    mMusicSet.erase(channel);
    mSoundSet.erase(channel);
}

/*
 * Die Konfiguration in Datei schreiben.
 * @author JoSch
 * @date 05-07-2006
 * @param conf Die Konfigurationdatei zum Schreiben.
 */
void SoundDriver::saveConf(ConfigFile &conf) const
{
	conf.setSetting("MasterVolume", StringConverter::toString(getMasterVolume()), "General");
	conf.setSetting("DefaultMusicVolume", StringConverter::toString(getDefaultMusicVolume()), "General");
	conf.setSetting("DefaultSoundVolume", StringConverter::toString(getDefaultSoundVolume()), "General");
}

/*
 * Die Konfiguration lesen
 * @author JoSch
 * @date 05-07-2006
 * @param conf Die Konfigurationdatei, aus der gelesen werden soll
 */
void SoundDriver::loadConf(ConfigFile &conf)
{
	setMasterVolume(StringConverter::parseInt(conf.getSetting("MasterVolume", "General")));
	setDefaultMusicVolume(StringConverter::parseInt(conf.getSetting("DefaultMusicVolume", "General")));
	setDefaultSoundVolume(StringConverter::parseInt(conf.getSetting("DefaultSoundVolume", "General")));
}

}
