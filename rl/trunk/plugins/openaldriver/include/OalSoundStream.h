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
#ifndef __OalSoundStream_H__
#define __OalSoundStream_H__

#include "OpenALDriverPrerequisites.h"
#include "SoundResource.h"
#include "Sound.h"

namespace Ogre {
    class MovableObject;
    class Vector3;
}

namespace rl {

	class OalDriver;

   /** Diese Klasse dient der Interaktion mit Ogre3D
    * @author Josch
    * @date 07-03-2005
    * @version 1.0
    */
    class _RlOpenALDriverExport OalSoundStream : public Sound
    {
    public:
        /// Konstruktor
        OalSoundStream(OalDriver* driver, const SoundResourcePtr &soundres);
        /// Destruktor
        virtual ~OalSoundStream();

        /// Laedt den Sound.
        virtual void load() throw (RuntimeException);
        /// Entlaedt den Sound.
        virtual void unload() throw (RuntimeException);        

        // Wir erzeugen einen Channel für SoundChannel
        virtual SoundChannel *createChannel() throw (RuntimeException);

	protected:
        // Sind wir gueltig?
        virtual bool isValid() const throw (RuntimeException);
   
    private:
        /// Loop?
        bool mLooping;
		OalDriver* mDriver;
    }; 

    class _RlOpenALDriverExport OalSoundStreamPtr :
        public Ogre::SharedPtr<OalSoundStream>
    {
    public:
        OalSoundStreamPtr() : Ogre::SharedPtr<OalSoundStream>() {}
        explicit OalSoundStreamPtr(OalSoundStream* rep) : Ogre::SharedPtr<OalSoundStream>(rep) {}
        OalSoundStreamPtr(const OalSoundStreamPtr& res) : Ogre::SharedPtr<OalSoundStream>(res) {}
    protected:
        void destroy();
    };

}
#endif
