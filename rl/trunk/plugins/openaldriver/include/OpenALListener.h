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
#ifndef __OpenALListener_H__
#define __OpenALListener_H__

#include "OpenALDriverPrerequisites.h"
#include "ListenerMovable.h"


namespace rl {
    
   /** Diese Klasse dient der Interkation mit Ogre3d und Fmod3
    * und kapselt den Hörer.
    * @author Josch
    * @date 06-29-2005
    * @version 1.0
    * @version 2.0
    */
    class _RlOpenALDriverExport OpenALListener : public ListenerMovable
    {
    private:
        /// Der Name des ListenerMovable
        Ogre::String mName;
        
    protected:
         /// Shared class-level name for Movable type
        static Ogre::String msMovableType;

    public:
        /// Konstruktor
        OpenALListener(const Ogre::String& name);
        /// Destruktor
        virtual ~OpenALListener();
        /// Moveable-Typ
        virtual const Ogre::String& getMovableType() const;
    
        /// Setzt die Hauptlautstaerke.
        virtual void setGain(const int direction);
        /// Setzt die Position der Soundquelle.
        virtual void setPosition(const Ogre::Vector3& direction);
        /// Setzt die Richtung der Soundquelle.
        virtual void setOrientation(const Ogre::Quaternion &orientation);
        /// Setzt die Geschwindigkeit der Soundquelle.
        virtual void setVelocity(const Ogre::Vector3&);
    };

}
#endif
