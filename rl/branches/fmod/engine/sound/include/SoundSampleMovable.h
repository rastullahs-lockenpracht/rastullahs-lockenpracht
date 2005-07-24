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

#ifndef __SoundSampleMovable_H__
#define __SoundSampleMovable_H__

#include "SoundPrerequisites.h"
#include "SoundResource.h"
#include "EventSource.h"
#include "EventListener.h"
#include "EventCaster.h"
#include "SoundEvents.h"
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
    class _RlSoundExport SoundSampleMovable : public SoundMovable
    {
    private:
        FSOUND_SAMPLE *mSample;

    public:
        /// Konstruktor
        SoundSampleMovable(const Ogre::String& name);
        /// Konstruktor
        SoundSampleMovable(const SoundResourcePtr &soundres);
        /// Destruktor
        virtual ~SoundSampleMovable();
        /// Moveable-Typ
        virtual const Ogre::String& getMovableType() const;

        /// Laedt den Sound.
        virtual void load() throw (RuntimeException);
        /// Entlaedt den Sound.
        virtual void unload() throw (RuntimeException);
        // Sample zurueckgeben
        FSOUND_SAMPLE *getSample() const;
    
protected:
        /// Shared class-level name for Movable type
        static Ogre::String msMovableType;
        // Sind wir gueltig?
        virtual bool isValid() const throw (RuntimeException);
        // Sample setzen
        void setSample(FSOUND_SAMPLE *sample);
    }; 

    class _RlSoundExport SoundSampleMovablePtr :
        public Ogre::SharedPtr<SoundSampleMovable>
    {
    public:
        SoundSampleMovablePtr() : Ogre::SharedPtr<SoundSampleMovable>() {}
        explicit SoundSampleMovablePtr(SoundSampleMovable* rep) : Ogre::SharedPtr<SoundSampleMovable>(rep) {}
        SoundSampleMovablePtr(const SoundSampleMovablePtr& res) : Ogre::SharedPtr<SoundSampleMovable>(res) {}
    protected:
        void destroy();
    };

}
#endif
