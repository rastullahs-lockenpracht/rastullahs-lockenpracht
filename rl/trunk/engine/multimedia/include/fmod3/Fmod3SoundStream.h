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
#ifndef __FmodSoundStream_H__
#define __FmodSoundStream_H__

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
    class _RlMultimediaExport Fmod3SoundStream : public Fmod3Sound
    {
    private:
        FSOUND_STREAM *mStream;
        
        // FMOD End Stream Callback
        static signed char streamEndCallback(
            FSOUND_STREAM *stream,
            void *buf, int len,
            void *userdata);

        // FMOD Sync Stream Callback
        static signed char streamSyncCallback(
            FSOUND_STREAM *stream,
            void *buf, int len,
            void *userdata);

    public:
        /// Konstruktor
        Fmod3SoundStream(const Ogre::String& name);
        /// Konstruktor
        Fmod3SoundStream(const SoundResourcePtr &soundres);
        /// Destruktor
        virtual ~Fmod3SoundStream();

        // Stream zurueckgeben
        FSOUND_STREAM *getStream() const;
        /// Laedt den Sound.
        virtual void load() throw (RuntimeException);
        /// Entlaedt den Sound.
        virtual void unload() throw (RuntimeException);        
        // Wir erzeugen einen Channel für SoundChannel
        virtual int createChannel() throw (RuntimeException);

protected:
        // Sind wir gueltig?
        virtual bool isValid() const throw (RuntimeException);
        // Sample setzen
        void setStream(FSOUND_STREAM *stream);
        
    }; 

    class _RlMultimediaExport Fmod3SoundStreamPtr :
        public Ogre::SharedPtr<Fmod3SoundStream>
    {
    public:
        Fmod3SoundStreamPtr() : Ogre::SharedPtr<Fmod3SoundStream>() {}
        explicit Fmod3SoundStreamPtr(Fmod3SoundStream* rep) : Ogre::SharedPtr<Fmod3SoundStream>(rep) {}
        Fmod3SoundStreamPtr(const Fmod3SoundStreamPtr& res) : Ogre::SharedPtr<Fmod3SoundStream>(res) {}
    protected:
        void destroy();
    };

}
#endif
#endif // WITH_FMOD
