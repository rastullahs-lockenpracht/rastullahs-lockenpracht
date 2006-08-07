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
#ifndef _FMOD4SOUND_H_
#define _FMOD4SOUND_H_

#include "Fmod4DriverPrerequisites.h"

#include "Sound.h"
#include <fmod.hpp>

namespace rl {

    class Fmod4Driver;
    class SoundChannel; 

    class _RlFmod4DriverExport Fmod4Sound : public Sound
    {
    public:
        enum SoundType 
        {
            SAMPLE,
            STREAM
        };

        /// Konstruktor
        Fmod4Sound(Fmod4Driver* driver, const SoundResourcePtr &soundres, SoundType type = SAMPLE);

        /// Laedt den Sound.
        virtual void load() throw (RuntimeException);
        /// Entlaedt den Sound.
        virtual void unload() throw (RuntimeException);
         // Wir geben zurück, wie lange der Sound ist.
	    float getLength() const;

        FMOD::Sound* _getFmodSound() const;

		// Sind wir gueltig
		bool isValid() const;

        private:
            SoundType mType;
            FMOD::Sound *mSound;
            Fmod4Driver* mDriver;
    };

    class _RlFmod4DriverExport Fmod4SoundPtr :
        public Ogre::SharedPtr<Fmod4Sound>
    {
    public:
        Fmod4SoundPtr() : Ogre::SharedPtr<Fmod4Sound>() {}
        explicit Fmod4SoundPtr(Fmod4Sound* rep) : Ogre::SharedPtr<Fmod4Sound>(rep) {}
        Fmod4SoundPtr(const Fmod4SoundPtr& res) : Ogre::SharedPtr<Fmod4Sound>(res) {}
    protected:
        void destroy();
    };

}

#endif /* FMOD3SOUND_H_ */
