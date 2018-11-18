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

#ifndef __SoundStitchingObject_H__
#define __SoundStitchingObject_H__

#include "CorePrerequisites.h"
#include "ActorControlledObject.h"
#include "EventListener.h"
#include "SoundEvents.h"
#include "SoundResource.h"

namespace rl {

    class SoundStitching;

   /** Diese Klasse dient der Interaktion mit dem jeweiligen Soundsystem
    * ein Objekt stellt eine einzelne Sounddatei oder einen Stream dar
    * @author Josch
    * @date 06-29-2005
    * @version 1.0
    * @version 2.0
    */
    class _RlCoreExport SoundStitchingObject : public ActorControlledObject
    {
    public:
        /// Konstruktor
        SoundStitchingObject(SoundStitching *stitching, const Ogre::String &name);
        /// Destruktor
        virtual ~SoundStitchingObject();
          
        /**
         * Interne Methode. Wird vom Aktor aufgerufen, wenn sich dessen
         * Status gendert hat. (Position, Orientierung, etc)
         * Die Standardimplementierung macht nichts, kann aber
         * von abgeleiteten Klassen berschrieben werden.
         */
        virtual void _update();
        
        /// Wie ActorControlledObject::getMovableObject()
        /// Nur schon gebrauchsfertig gecastet.
        SoundStitching* getSoundStitching() const;
        
        
        virtual Ogre::String getObjectType() const;

        // Wollen wir 3D?
        bool is3d() const;
        // Setzen des 3D-Flags.
        void set3d(bool is3d);

        // Sollen der Sound wiederholt werden?
        bool isLooping() const;
        // Setzen des Loop-Flags.
        void setLooping(bool looping);
        float getLength() const;
        /// Pausiert den Sound.
        void pause();
        /// Pausiert den Sound.
        void pause(bool pausing);
        /// Ist der Sound pausiert?
        bool isPaused();
        /// Stoppt den Sound.
        virtual void stop();
        /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck (0.0 ... 1.0)
	    const Ogre::Real getVolume() const; 
        /// Setzt die relative Lautstaerke der Soundquelle (0.0 .. 1.0).
	    void setVolume(const Ogre::Real gain);
        // Sets the priority of this sound
        void setPriority(const int priority);
        // Gets the priority of this sound
        const int getPriority() const;
		/// Laedt den Ton
		void load();
        /// Entlaedt den Sound.
        void unload();
        /// Aktivieren
        virtual void setActive(bool active);

        /// Start sound sequence
        void start();
        /// Spielt den Sound ab.
        virtual void play(bool destroyWhenDone=false);
        /// Laeuft der Sound noch
        virtual const bool isPlaying() const = 0;
		virtual bool isMeshObject() const;

        /// Count of configured slots.
        unsigned int getNumSlots();
        /// Add a sound resource to the cache overwritting existing sound resource.
        virtual void addSoundToCache(CeGuiString label, SoundResourcePtr sound);
        /// Assign a sound recource to a slot.
        virtual void putSoundIntoSlot(unsigned int, CeGuiString label) = 0;
        /// Get the slot of the plaing sound.
        virtual unsigned int getPlayingSlot() = 0;
    }; 

}
#endif
