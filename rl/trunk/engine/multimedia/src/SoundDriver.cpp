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
#include <OgreStringConverter.h>

using namespace Ogre;

namespace rl
{

Ogre::ResourceManager* SoundDriver::sSoundResourceManager = NULL;

SoundDriver::SoundDriver(ResourceManager* soundResourceManager)
 :  mMusicSet(),
    mSoundSet(),
    mDefaultMusicVolume(40),
    mDefaultSoundVolume(100),
	mMasterVolume(100)	
{
	sSoundResourceManager = soundResourceManager;
}

SoundDriver::~SoundDriver()
{
}

/// Die Standardlautstärke für Musik einstellen
void SoundDriver::setDefaultMusicVolume(const Ogre::Real& vol)
{
    mDefaultMusicVolume = vol;
    SoundChannelSet::iterator it;
    for(it = mMusicSet.begin(); it != mMusicSet.end(); it++)
    {
        (*it)->setVolume(vol);
    }
}

/// Die Standardlautstärke für Musik zurückgeben.
const Ogre::Real SoundDriver::getDefaultMusicVolume() const
{
    return mDefaultMusicVolume;
}

/// Die Standardlautstärke für Musik einstellen
void SoundDriver::setDefaultSoundVolume(const Ogre::Real& vol)
{
    mDefaultSoundVolume = vol;
    SoundChannelSet::iterator it;
    for(it = mSoundSet.begin(); it != mSoundSet.end(); it++)
    {
        (*it)->setVolume(vol);
    }
}

/// Die Standardlautstärke für Musik zurückgeben.
const Ogre::Real SoundDriver::getDefaultSoundVolume() const
{
    return mDefaultSoundVolume;
}

void SoundDriver::setMasterVolume(const Ogre::Real& vol)
{
    mMasterVolume = vol;
}

const Ogre::Real SoundDriver::getMasterVolume() const
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
	conf.setValue(getMasterVolume(), "MasterVolume", "General");
	conf.setValue(getDefaultMusicVolume(), "DefaultMusicVolume", "General");
	conf.setValue(getDefaultSoundVolume(), "DefaultSoundVolume", "General");
}

/*
 * Die Konfiguration lesen
 * @author JoSch
 * @date 05-07-2006
 * @param conf Die Konfigurationdatei, aus der gelesen werden soll
 */
void SoundDriver::loadConf(ConfigFile &conf)
{
	setMasterVolume(conf.getValue(Real(1.0), "MasterVolume", "General"));
	setDefaultMusicVolume(conf.getValue(Real(0.30), "DefaultMusicVolume", "General"));
	setDefaultSoundVolume(conf.getValue(Real(1.0), "DefaultSoundVolume", "General"));
}

}
