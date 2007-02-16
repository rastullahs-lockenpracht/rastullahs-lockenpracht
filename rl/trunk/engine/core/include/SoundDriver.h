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
#ifndef SOUNDDRIVER_H_
#define SOUNDDRIVER_H_

#include "CorePrerequisites.h"
#include <OgreResourceManager.h>
#include "Sound.h"

namespace rl
{
    class ListenerMovable;

    /// Abstract super class of all API specific sound drivers.
    class _RlCoreExport SoundDriver
    {
    public:
        /// Der Konstruktor
        SoundDriver(Ogre::ResourceManager* soundResourceManager);
        /// Der Destruktor
        virtual ~SoundDriver();

        /// returns true, if the driver can work on the current system. False else.
        virtual bool isDriverAvailable() = 0;

        /** Initialize driver
         *  @return <code>true</code> if successful, <code>false</code> otherwise
         */
        virtual bool initialize() = 0;

        /// Beende den Treiber
        virtual void shutdown() = 0;
        /// Der Name des Treibers
        virtual Ogre::String getName() const = 0;
        /// Update-Aufgaben erledigen
        virtual void update() = 0;
        /// Einen Sound-Stream mit Resource erzeugen
        virtual Sound* createStream(const SoundResourcePtr &res);
        virtual Sound* createStream(const Ogre::String& name);
        /// Einen Sound-Sample mit Resource erzeugen
        virtual Sound* createSample(const SoundResourcePtr &res);
        virtual Sound* createSample(const Ogre::String& name);


        /// Destroy a sound created by this driver. Do not call the Sound destructor yourself!
        virtual void destroySound(Sound*);

        /// Einen Soundlistener erzeugen
        virtual ListenerMovable *createListener(const Ogre::String &name) = 0;


        /// Set attenuation of the volume with distance. f influences
        /// volume like this: relative volume = 1/(f*distance)
        virtual void setRolloffFactor(const Ogre::Real& f) = 0;
        virtual const Ogre::Real getRolloffFactor() = 0;

        void setDefaultMusicVolume(const Ogre::Real& vol);
        const Ogre::Real getDefaultMusicVolume() const;

        void setDefaultSoundVolume(const Ogre::Real& vol);
        const Ogre::Real getDefaultSoundVolume() const;

        virtual void setMasterVolume(const Ogre::Real& vol);
        const Ogre::Real getMasterVolume() const;

        /**
         * Returns the current sound driver settings as a key-value list
         *
         * @return key-value list of sound options
         */
        virtual Ogre::NameValuePairList getSettings() const;

        /**
         * Sets certain options for the sound driver, like volume settings for
         * sound effects and music.
         *
         * @param settings key-value list of settings
         */
        virtual void applySettings(const Ogre::NameValuePairList& settings);

    protected:
        Ogre::ResourceManager* mSoundResourceManager;

        SoundSet mStreamSet;
        SoundSet mSampleSet;

        Ogre::Real mDefaultMusicVolume;  ///!< Music volume
        Ogre::Real mDefaultSoundVolume;  ///!< Sound effect volume
        Ogre::Real mMasterVolume;        ///!< Master volume

        virtual Sound* createStreamImpl(const SoundResourcePtr &res) = 0;
        virtual Sound* createSampleImpl(const SoundResourcePtr &res) = 0;
    };
}

#endif /*SOUNDDRIVER_H_*/
