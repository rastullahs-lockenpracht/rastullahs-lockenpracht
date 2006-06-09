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
#ifndef __OalSoundSample_H__
#define __OalSoundSample_H__

#include "OpenALDriverPrerequisites.h"
#include "SoundResource.h"
#include "Sound.h"

namespace rl {

	class OalDriver;

   /** Diese Klasse dient der Interaktion mit Ogre3D
    * @author Josch
    * @date 07-03-2005
    * @version 1.0
    */
    class _RlOpenALDriverExport OalSoundSample : public Sound
    {
    public:
        /// Konstruktor
        OalSoundSample(OalDriver* driver, const SoundResourcePtr &soundres);
        /// Destruktor
        virtual ~OalSoundSample();
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
    
	private:
		OalDriver* mDriver;
    }; 

    class _RlOpenALDriverExport OalSoundSamplePtr :
        public Ogre::SharedPtr<OalSoundSample>
    {
    public:
        OalSoundSamplePtr() : Ogre::SharedPtr<OalSoundSample>() {}
        explicit OalSoundSamplePtr(OalSoundSample* rep) : Ogre::SharedPtr<OalSoundSample>(rep) {}
        OalSoundSamplePtr(const OalSoundSamplePtr& res) : Ogre::SharedPtr<OalSoundSample>(res) {}
    protected:
        void destroy();
    };

}
#endif
