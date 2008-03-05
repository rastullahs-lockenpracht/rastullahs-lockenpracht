/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#ifndef __Fmod4Listener_H__
#define __Fmod4Listener_H__

#include "Fmod4DriverPrerequisites.h"
#include "ListenerMovable.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    #include <fmod.hpp>
#else
    #include <fmodex/fmod.hpp>
#endif

namespace rl {
    
   /** Diese Klasse dient der Interkation mit Ogre3d und Fmod4
    * und kapselt den Hoerer.
    * @author Josch
    * @date 06-29-2005
    * @version 1.0
    * @version 2.0
    */
    class _RlFmod4DriverExport Fmod4Listener : public ListenerMovable
    {
    public:
        /// Konstruktor
        Fmod4Listener(const Ogre::String& name, FMOD::System* fmod4system, int listenerId = 0);
        /// Destruktor
        virtual ~Fmod4Listener();
        /// Moveable-Typ
        virtual const Ogre::String& getMovableType() const;
    
        /// Setzt die Position der Soundquelle.
        virtual void setPosition(const Ogre::Vector3& direction);
        /// Setzt die Richtung der Soundquelle.
        virtual void setOrientation(const Ogre::Quaternion &orientation);
        /// Setzt die Geschwindigkeit der Soundquelle.
        virtual void setVelocity(const Ogre::Vector3&);
   
    protected:
         /// Shared class-level name for Movable type
        static Ogre::String msMovableType;

    private:
        FMOD::System* mFmod4System;
        int mListenerId;
    };

}
#endif
