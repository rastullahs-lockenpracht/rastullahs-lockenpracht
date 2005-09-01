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

#ifndef __SoundSample_H__
#define __SoundSample_H__

#include "SoundPrerequisites.h"
#include "SoundResource.h"
#include "Sound.h"

extern "C" {
    #include <fmod.h>
    #include <fmod_errors.h>
}

namespace Ogre {
    class MovableObject;
    class Vector3;
}

namespace rl {

   /** Diese Klasse dient der Interaktion mit Ogre3D
    * @author Josch
    * @date 07-03-2005
    * @version 1.0
    */
    class _RlSoundExport SoundSample : public Sound
    {
    private:
        FSOUND_SAMPLE *mSample;

    public:
        /// Konstruktor
        SoundSample(const Ogre::String& name);
        /// Konstruktor
        SoundSample(const SoundResourcePtr &soundres);
        /// Destruktor
        virtual ~SoundSample();
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

    class _RlSoundExport SoundSamplePtr :
        public Ogre::SharedPtr<SoundSample>
    {
    public:
        SoundSamplePtr() : Ogre::SharedPtr<SoundSample>() {}
        explicit SoundSamplePtr(SoundSample* rep) : Ogre::SharedPtr<SoundSample>(rep) {}
        SoundSamplePtr(const SoundSamplePtr& res) : Ogre::SharedPtr<SoundSample>(res) {}
    protected:
        void destroy();
    };

}
#endif
