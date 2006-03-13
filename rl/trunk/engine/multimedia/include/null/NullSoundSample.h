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

#ifndef __NullSoundSample_H__
#define __NullSoundSample_H__

#include "MultimediaPrerequisites.h"
#include "SoundResource.h"
#include "Sound.h"

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
    class _RlMultimediaExport NullSoundSample : public Sound
    {
    private:

    public:
        /// Konstruktor
        NullSoundSample(const Ogre::String& name);
        /// Konstruktor
        NullSoundSample(const SoundResourcePtr &soundres);
        /// Destruktor
        virtual ~NullSoundSample();
        /// Moveable-Typ
        virtual const Ogre::String& getMovableType() const;

        /// Laedt den Sound.
        virtual void load() throw (RuntimeException);
        /// Entlaedt den Sound.
        virtual void unload() throw (RuntimeException);
 
        // Wir erzeugen einen Channel für SoundChannel
        virtual SoundChannel* createChannel() throw (RuntimeException);
    
protected:
        /// Shared class-level name for Movable type
        static Ogre::String msMovableType;
        // Sind wir gueltig?
        virtual bool isValid() const throw (RuntimeException);
    }; 

    class _RlMultimediaExport NullSoundSamplePtr :
        public Ogre::SharedPtr<NullSoundSample>
    {
    public:
        NullSoundSamplePtr() : Ogre::SharedPtr<NullSoundSample>() {}
        explicit NullSoundSamplePtr(NullSoundSample* rep) : Ogre::SharedPtr<NullSoundSample>(rep) {}
        NullSoundSamplePtr(const NullSoundSamplePtr& res) : Ogre::SharedPtr<NullSoundSample>(res) {}
    protected:
        void destroy();
    };

}
#endif
