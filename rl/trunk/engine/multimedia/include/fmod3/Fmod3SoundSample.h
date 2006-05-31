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

#ifdef WITH_FMOD3
#ifndef __FmodSoundSample_H__
#define __FmodSoundSample_H__

#include "MultimediaPrerequisites.h"
#include "Fmod3Sound.h"

extern "C" {
    #include <fmod.h>
    #include <fmod_errors.h>
}

namespace Ogre {
    class MovableObject;
    class Vector3;
}

namespace rl {
	class SoundResource;

   /** Diese Klasse dient der Interaktion mit Ogre3D
    * @author Josch
    * @date 07-03-2005
    * @version 1.0
    */
    class _RlMultimediaExport Fmod3SoundSample : public Fmod3Sound
    {
    private:
        FSOUND_SAMPLE *mSample;

    public:
        /// Konstruktor
        Fmod3SoundSample(const SoundResourcePtr &soundres);
        /// Destruktor
        virtual ~Fmod3SoundSample();
        /// Moveable-Typ
        virtual const Ogre::String& getMovableType() const;

        /// Laedt den Sound.
        virtual void load() throw (RuntimeException);
        /// Entlaedt den Sound.
        virtual void unload() throw (RuntimeException);
        // Sample zurueckgeben
        FSOUND_SAMPLE *getSample() const;
        // Wir erzeugen einen Channel für SoundChannel
        virtual int createChannel() throw (RuntimeException);
 
protected:
        /// Shared class-level name for Movable type
        static Ogre::String msMovableType;
        // Sind wir gueltig?
        virtual bool isValid() const throw (RuntimeException);
        // Sample setzen
        void setSample(FSOUND_SAMPLE *sample);
    }; 

    class _RlMultimediaExport Fmod3SoundSamplePtr :
        public Ogre::SharedPtr<Fmod3SoundSample>
    {
    public:
        Fmod3SoundSamplePtr() : Ogre::SharedPtr<Fmod3SoundSample>() {}
        explicit Fmod3SoundSamplePtr(Fmod3SoundSample* rep) : Ogre::SharedPtr<Fmod3SoundSample>(rep) {}
        Fmod3SoundSamplePtr(const Fmod3SoundSamplePtr& res) : Ogre::SharedPtr<Fmod3SoundSample>(res) {}
    protected:
        void destroy();
    };

}
#endif
#endif // WITH_FMOD
