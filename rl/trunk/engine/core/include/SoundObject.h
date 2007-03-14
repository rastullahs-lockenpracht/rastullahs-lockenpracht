/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __SoundObject_H__
#define __SoundObject_H__

#include "CorePrerequisites.h"
#include "ActorControlledObject.h"
#include "SoundEvents.h"
#include "EventListener.h"

namespace rl {
/// @TODO SharedPtr

    class Sound;
    
    
    /** Klasse für alle Soundobjekte, die man an einen Aktor
     *  heften kann. Zwischen Actor und ActorControlledObject
     *  besteht eine 1:1-Beziehung.
     */
    class _RlCoreExport SoundObject : public ActorControlledObject
    {
    public:
        SoundObject(Sound *sound, const Ogre::String &name);
        virtual ~SoundObject();
        
        /**
         * Interne Methode. Wird vom Aktor aufgerufen, wenn sich dessen
         * Status geändert hat. (Position, Orientierung, etc)
         * Die Standardimplementierung macht nichts, kann aber
         * von abgeleiteten Klassen überschrieben werden.
         */
        virtual void _update();
        
        /// Wie ActorControlledObject::getMovableObject()
        /// Nur schon gebrauchsfertig gecastet.
        Sound* getSound() const;
        
        
        virtual Ogre::String getObjectType();

		bool is3d() const;
		void set3d( bool is3d );

        /// Gibt zurück ob der Sound geloopt werden soll 
        bool isLooping() const;
        /// Setzt ob der Sound geloopt werden soll 
        void setLooping( bool looping );
        /// Pausiert den Sound.
        void pause(bool pausing);
        /// Ist der Sound pausiert?
        bool isPaused();
        /// Stoppt den Sound.
        virtual void stop();
		/// Setzt die Lautstaerke (1.0 = volle Lautstärke)
		void setVolume(float volume = 1.0);
        // Sets the priority of this sound
        void setPriority(const int priority);
        // Gets the priority of this sound
        const int getPriority() const;

		float getLength() const;

		/// Lädt den Ton
		virtual void load();
        /// Entlädt den Sound.
        virtual void unload();
        
        void play();

        /// Starte den Sound
        void start();
        /// Pausieren den Sound
        void pause();

		virtual bool isMeshObject();
    };
}
#endif
