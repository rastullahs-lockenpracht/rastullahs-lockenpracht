/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h" //precompiled header

#include "ConfigFile.h"
#include "SoundDriver.h"

using namespace Ogre;

namespace rl
{

    SoundDriver::SoundDriver(ResourceManager* soundResourceManager)
        : mSoundResourceManager(soundResourceManager)
        , mStreamSet()
        , mSampleSet()
        , mDefaultMusicVolume(40)
        , mDefaultSoundVolume(100)
        , mMasterVolume(100)
    {
    }

    SoundDriver::~SoundDriver()
    {
        SoundSet sounds;
        sounds.insert(mSampleSet.begin(), mSampleSet.end());
        sounds.insert(mSampleSet.begin(), mSampleSet.end());
        for (SoundSet::iterator it = sounds.begin(); it != sounds.end(); ++it)
        {
            if ((*it)->isPlaying())
            {
                (*it)->stop();
            }
            delete *it;
        }
    }

    /// Die Standardlautstaerke fuer Musik einstellen
    void SoundDriver::setDefaultMusicVolume(const Ogre::Real& vol)
    {
        mDefaultMusicVolume = vol;
        SoundSet::iterator it;
        for (it = mStreamSet.begin(); it != mStreamSet.end(); it++)
        {
            (*it)->setVolume(vol);
        }
    }

    /// Die Standardlautstaerke fuer Musik zurueckgeben.
    const Ogre::Real SoundDriver::getDefaultMusicVolume() const
    {
        return mDefaultMusicVolume;
    }

    /// Die Standardlautstaerke fuer Musik einstellen
    void SoundDriver::setDefaultSoundVolume(const Ogre::Real& vol)
    {
        mDefaultSoundVolume = vol;
        SoundSet::iterator it;
        for (it = mSampleSet.begin(); it != mSampleSet.end(); it++)
        {
            (*it)->setVolume(vol);
        }
    }

    /// Die Standardlautstaerke fuer Musik zurueckgeben.
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

    Ogre::NameValuePairList SoundDriver::getSettings() const
    {
        Ogre::NameValuePairList SoundSettings;

        // Append the settings to the list
        SoundSettings.insert(make_pair("MasterVolume", Ogre::StringConverter::toString(mMasterVolume)));
        SoundSettings.insert(make_pair("DefaultMusicVolume", Ogre::StringConverter::toString(mDefaultMusicVolume)));
        SoundSettings.insert(make_pair("DefaultSoundVolume", Ogre::StringConverter::toString(mDefaultSoundVolume)));

        return SoundSettings;
    }

    SoundDriverConfigComponent* SoundDriver::getConfigComponent()
    {
        return mConfigComponent;
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

    Sound* SoundDriver::createSound(const String& name, SoundType type)
    {
        SoundResourcePtr ptr = std::dynamic_pointer_cast<SoundResource>(
            mSoundResourceManager->load(name, ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME));
        if (!ptr)
        {
            Throw(IllegalArgumentException, "Sound " + name + "nicht gefunden");
        }
        return createSound(ptr, type);
    }

    Sound* SoundDriver::createSound(SoundResourcePtr res, SoundType type)
    {
        Sound* sound = createSoundImpl(res, type);
        if (sound != NULL)
        {
            if (type == ST_SAMPLE)
            {
                mSampleSet.insert(sound);
            }
            else
            {
                mStreamSet.insert(sound);
            }
        }
        return sound;
    }

    SoundStitching* SoundDriver::createSoundStitching(unsigned int label, const CeGuiString& name)
    {
        return createSoundStitchingImpl(label, name);
    }

    void SoundDriver::destroySound(Sound* sound)
    {
        if (sound->isPlaying())
        {
            sound->stop();
        }
        mStreamSet.erase(sound);
        mSampleSet.erase(sound);
        delete sound;
    }
}
