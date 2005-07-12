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

#ifndef __SoundStreamMovable_H__
#define __SoundStreamMovable_H__

#include "SoundPrerequisites.h"
#include "SoundMovable.h"

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
    class _RlSoundExport SoundStreamMovable : public SoundMovable
    {
    private:
        FSOUND_STREAM *mStream;

    public:
        /// Konstruktor
        SoundStreamMovable(const Ogre::String& name);
        /// Konstruktor
        SoundStreamMovable(const SoundResourcePtr &soundres);
        /// Destruktor
        virtual ~SoundStreamMovable();
        /// Moveable-Typ
        virtual const Ogre::String& getMovableType() const;

        /// Laedt den Sound.
        virtual void load() throw (RuntimeException);
        /// Spielt den Sound ab.
        virtual void play() throw (RuntimeException);
    
protected:
        /// Shared class-level name for Movable type
        static Ogre::String msMovableType;
        // Sind wir gueltig?
        virtual bool isValid() const throw (RuntimeException);
        // Sample zurueckgeben
        FSOUND_STREAM *getStream() const;
        // Sample setzen
        void setStream(FSOUND_STREAM *stream);
        
        
    }; 

    class _RlSoundExport SoundStreamMovablePtr :
        public Ogre::SharedPtr<SoundStreamMovable>
    {
    public:
        SoundStreamMovablePtr() : Ogre::SharedPtr<SoundStreamMovable>() {}
        explicit SoundStreamMovablePtr(SoundStreamMovable* rep) : Ogre::SharedPtr<SoundStreamMovable>(rep) {}
        SoundStreamMovablePtr(const SoundStreamMovablePtr& res) : Ogre::SharedPtr<SoundStreamMovable>(res) {}
    protected:
        void destroy();
    };

}
#endif
