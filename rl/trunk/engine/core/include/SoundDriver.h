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

    /**
     * Diese Klasse dient zur Definition einer Schnittstelle
     * zu den m�glichen Soundbibliotheken. Sie geben Preis, ob
     * ein Treiber vorhanden ist oder nicht. Der Nulltreiber
     * existiert dabei immer.
     */
    class _RlCoreExport SoundDriver
    {
    public:
        /// Der Konstruktor
        SoundDriver(Ogre::ResourceManager* soundResourceManager);
        /// Der Destruktor
        virtual ~SoundDriver();
        /// Zeigt an, ob der Treiber funktionsf�hig ist.
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
        /// Einen Sound-Sample mit Resource erzeugen
        virtual Sound* createSample(const SoundResourcePtr &res);

        /// Destroy a sound created by this driver. Do not call the Sound destructor yourself!
        virtual void destroySound(Sound*);

        /// Einen Soundlistener erzeugen
        virtual ListenerMovable *createListener(const Ogre::String &name) = 0;


        /// Setzt den Faktor f, mit der die Lautst�rke nach der Formel 1/(f*Entfernung) abnimmt
        virtual void setRolloffFactor(const Ogre::Real& f) = 0;
        virtual const Ogre::Real getRolloffFactor() = 0;
        /// Die Standardlautst�rke f�r Musik einstellen
        void setDefaultMusicVolume(const Ogre::Real& vol);
        /// Die Standardlautst�rke f�r Musik zur�ckgeben.
        const Ogre::Real getDefaultMusicVolume() const;
        /// Die Standardlautst�rke f�r Musik einstellen
        void setDefaultSoundVolume(const Ogre::Real& vol);
        /// Die Standardlautst�rke f�r Musik zur�ckgeben.
        const Ogre::Real getDefaultSoundVolume() const;
        /// Die Lautst�rke aller Sounds
        virtual void setMasterVolume(const Ogre::Real& vol);
        /// Die Standardlautst�rke f�r Musik zur�ckgeben.
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
        static Ogre::ResourceManager* sSoundResourceManager;

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
