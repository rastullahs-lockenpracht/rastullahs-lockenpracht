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
    SoundSet::iterator it;
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
    SoundSet::iterator it;
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
void SoundDriver::remove(Sound *sound)
{
    mMusicSet.erase(sound);
    mSoundSet.erase(sound);
}

    Ogre::NameValuePairList SoundDriver::getSettings() const
    {
        Ogre::NameValuePairList SoundSettings;

        // Append the settings to the list
        SoundSettings.insert(make_pair("MasterVolume", Ogre::StringConverter::toString(mMasterVolume)));
        SoundSettings.insert(make_pair("DefaultMusicVolume", Ogre::StringConverter::toString(mDefaultMusicVolume)));
        SoundSettings.insert(make_pair("DefaultSoundVolume", Ogre::StringConverter::toString(mDefaultSoundVolume)));

        return SoundSettings;
    }

    void SoundDriver::applySettings(const Ogre::NameValuePairList& settings)
    {
        Ogre::NameValuePairList::const_iterator it;

        // Set the Master volume
        if ((it = settings.find("MasterVolume")) == settings.end())
        {
            // Set a sane default value
            setMasterVolume(Real(1.0));
        }
        else
        {
            setMasterVolume(Ogre::StringConverter::parseReal(it->second));
        }

        // Set the Default Music Volume
        if ((it = settings.find("DefaultMusicVolume")) == settings.end())
        {
            // Set a sane default value
            setDefaultMusicVolume(Real(0.3));
        }
        else
        {
            setDefaultMusicVolume(Ogre::StringConverter::parseReal(it->second));
        }

        // Set the Default Sound Volume
        if ((it = settings.find("DefaultSoundVolume")) == settings.end())
        {
            // Set a sane default value
            setDefaultSoundVolume(Real(1.0));
        }
        else
        {
            setDefaultSoundVolume(Ogre::StringConverter::parseReal(it->second));
        }
    }
}
