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

#ifndef __Sound_H__
#define __Sound_H__

#include "SoundPrerequisites.h"
#include "SoundResource.h"
#include "SoundChannel.h"
#include "Exception.h"


namespace rl {

   /** Diese Klasse dient der Interaktion fmod
    * @author Josch
    * @date 06-29-2005
    * @version 1.0
    * @version 2.0
    */
    class _RlSoundExport Sound
    {
    private:
        /// Der Name des Sounds
        Ogre::String mName;
        /// Der Soundresource, auf den wir verweisen.
        SoundResourcePtr mSoundResource;

        // Damit die Timings alle 0.5 sek. abgeschickt werden.
        int mTicks;
        /// Die komplette Zeit des Stücks in Sekunden.
        double mTime;
        // Ob dieser Sound 3D sein soll.
        bool mIs3d;
        // Ob dieser Sound loopen soll.
        bool mIsLooping;
        
    public:
        /// Konstruktor
        Sound(const Ogre::String& name);
        /// Konstruktor
        Sound(const SoundResourcePtr &soundres);
        /// Destruktor
        virtual ~Sound();
        /// Name zurückgeben
        const Ogre::String& getName() const;
    
        
        /// Laedt den Sound.
        virtual void load() throw (RuntimeException) = 0;
        /// Entlaedt den Sound.
        virtual void unload() throw (RuntimeException) = 0;

        // SoundResource zurückgeben.
        SoundResourcePtr getSoundResource() const;
        // Wollen wir 3D?
        bool is3d() const;
        // Setzen des 3D-Flags.
        void set3d(bool is3d);
        // Sind wir gueltig
        virtual bool isValid() const throw (RuntimeException) = 0;
        
        // Wir erzeugen einen Channel für SoundChannel
        virtual SoundChannel* createChannel() throw (RuntimeException) = 0;
       
        // Sollen der Sound wiederholt werden?
        bool isLooping() const;
        // Setzen des Loop-Flags.
        void setLooping(bool looping);
    }; 

    class _RlSoundExport SoundPtr :
        public Ogre::SharedPtr<Sound>
    {
    public:
        SoundPtr() : Ogre::SharedPtr<Sound>() {}
        explicit SoundPtr(Sound* rep) : Ogre::SharedPtr<Sound>(rep) {}
        SoundPtr(const SoundPtr& res) : Ogre::SharedPtr<Sound>(res) {}
    protected:
        void destroy();
    };

}
#endif
