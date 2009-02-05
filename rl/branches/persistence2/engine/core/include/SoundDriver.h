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
#ifndef SOUNDDRIVER_H_
#define SOUNDDRIVER_H_

#include "CorePrerequisites.h"
#include "Sound.h"
#include "SoundStitching.h"

namespace rl
{
    class ListenerMovable;
    class SoundDriverConfigComponent;

    typedef enum {ST_SAMPLE, ST_STREAM} SoundType;

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

        /**
         *  Create a Sound object.
         *  Sounds created with this functions have to be destroyed using
         *  SoundDriver#destroySound, unless destroyWhenDone is set to true.
         *  @param res the resource from which to create the sound.
         *  @param type whether to create the sound as sample or stream.
         */
        virtual Sound* createSound(SoundResourcePtr res, SoundType type=ST_SAMPLE);

        /// @overload
        virtual Sound* createSound(const Ogre::String& res, SoundType type=ST_SAMPLE);

        /// Create a sound stitching object
        virtual SoundStitching* createSoundStitching(unsigned int numSlots, const CeGuiString &name);

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
         * If this driver is a Ogre plugin, it returns TRUE.
         * @return bool TRUE if Ogre plugin, FALSE otherwise.
         */
        virtual bool isDriverPlugin() = 0;

        /**
         * Returns the current sound driver settings as a key-value list
         *
         * @return key-value list of sound options
         */
        virtual Ogre::NameValuePairList getSettings() const;

        /**
         * Retrieves a pointer to the ConfigComponent of this driver
         *
         * @return pointer to ConfigComponent
         */
        virtual SoundDriverConfigComponent* getConfigComponent();

        /**
         * Sets certain options for the sound driver, like volume settings for
         * sound effects and music.
         *
         * @param settings key-value list of settings
         */
        virtual void applySettings(const Ogre::NameValuePairList& settings);

        /**
         * Sets an eax preset, the preset is defined by a string
         * @retval true if successfull
        */
        virtual bool setEaxPreset(const Ogre::String& name) = 0;

    protected:
        virtual Sound* createSoundImpl(SoundResourcePtr res, SoundType type) = 0;
        virtual SoundStitching* createSoundStitchingImpl(unsigned int numSlots, const CeGuiString& name) = 0;

        Ogre::ResourceManager* mSoundResourceManager;

        SoundSet mStreamSet;
        SoundSet mSampleSet;

        Ogre::Real mDefaultMusicVolume;               ///!< Music volume
        Ogre::Real mDefaultSoundVolume;               ///!< Sound effect volume
        Ogre::Real mMasterVolume;                     ///!< Master volume
        SoundDriverConfigComponent* mConfigComponent; ///!< Configuration dialog for driver specific settings
    };
}

#endif /*SOUNDDRIVER_H_*/
