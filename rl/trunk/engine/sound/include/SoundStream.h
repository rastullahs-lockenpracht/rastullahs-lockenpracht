/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Perl Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Perl Artistic License for more details.
*
*  You should have received a copy of the Perl Artistic License
*  along with this program; if not you can get it here
*  http://www.perldoc.com/perl5.6/Artistic.html.
*/

#ifndef __SoundStream_H__
#define __SoundStream_H__

#include "SoundPrerequisites.h"
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
    class _RlSoundExport SoundStream : public Sound
    {
    private:
        FSOUND_STREAM *mStream;
        
        // FMOD Stream Callback
        static signed char streamCallback(
            FSOUND_STREAM *stream,
            void *buf, int len,
            void *userdata);

    public:
        /// Konstruktor
        SoundStream(const Ogre::String& name);
        /// Konstruktor
        SoundStream(const SoundResourcePtr &soundres);
        /// Destruktor
        virtual ~SoundStream();

        // Stream zurueckgeben
        FSOUND_STREAM *getStream() const;

        // Wir erzeugen einen Channel für SoundChannel
        virtual int createChannel() throw (RuntimeException);
    
protected:
        // Sind wir gueltig?
        virtual bool isValid() const throw (RuntimeException);
        // Sample setzen
        void setStream(FSOUND_STREAM *stream);
        
        /// Laedt den Sound.
        virtual void load() throw (RuntimeException);
        /// Entlaedt den Sound.
        virtual void unload() throw (RuntimeException);        
        
    }; 

    class _RlSoundExport SoundStreamPtr :
        public Ogre::SharedPtr<SoundStream>
    {
    public:
        SoundStreamPtr() : Ogre::SharedPtr<SoundStream>() {}
        explicit SoundStreamPtr(SoundStream* rep) : Ogre::SharedPtr<SoundStream>(rep) {}
        SoundStreamPtr(const SoundStreamPtr& res) : Ogre::SharedPtr<SoundStream>(res) {}
    protected:
        void destroy();
    };

}
#endif
