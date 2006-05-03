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

#ifndef __ActorManager_H__
#define __ActorManager_H__

#include <map>
#include <vector>

#include "CorePrerequisites.h"
#include "PhysicsManager.h"
#include "LightObject.h"
#include "ParticleSystemObject.h"

namespace rl {

class World;
class Actor;

typedef std::map<const Ogre::String,Actor*> ActorPtrMap;
typedef std::pair<const Ogre::String,Actor*> ActorPtrPair;

class _RlCoreExport ActorManager : protected Ogre::Singleton<ActorManager>
{
    public:
        ActorManager( );
        virtual ~ActorManager( );

        Actor* getActor(const Ogre::String& name);

        Actor* createLightActor(const Ogre::String& name, rl::LightObject::LightTypes type );
        Actor* createSoundSampleActor(const Ogre::String& name, const Ogre::String& soundfile );
        Actor* createSoundStreamActor(const Ogre::String& name, const Ogre::String& soundfile );
        Actor* createListenerActor(const Ogre::String& name);
        Actor* createCameraActor(const Ogre::String& name);
        Actor* createEmptyActor(const Ogre::String& name);
   	    Actor* createMeshActor(const Ogre::String& name,const Ogre::String& meshname,
			PhysicsManager::GeometryTypes geomType = PhysicsManager::GT_NONE,
			Ogre::Real mass = 1.0);
        Actor* createParticleSystemActor(const Ogre::String& name, 
            const Ogre::String& partname);
        //void createDebugActors();
        void destroyActor(Actor* actor);
        
        ///@warning Actors with a camera attached wont be destroyed. 
		///@todo Check for active Viewport, and disable that first
        void destroyAllActors();

        void setWorld(World* world);
		const World* const getWorld() const;

		Actor* getActorAt(Ogre::Real x, Ogre::Real y, Ogre::Real width, Ogre::Real length, bool infinite = false);

		// View Distance
		Ogre::Real getDefaultActorRenderingDistance() const;
		void setDefaultActorRenderingDistance( Ogre::Real dist );

        /** Returns the Singleton */
	    static ActorManager & getSingleton(void);
	    static ActorManager * getSingletonPtr(void);        
    private:
		/**	Variante mit Strahl, ob der geringen Ausdehnung eines Strahls nicht zufriedenstellend.	 
				Geändert von ChaosBlender am 11.März 2006 (irgendwann nachts)
		    Änderung: Man muss sich nun in der Nähe eines Objekts befinden um eine Aktion auszuführen
		    Weitere Änderung: collectSelectableObjects hat jetzt auch die Länge des Rays als Argument, die in
		    InputManager.cpp übergeben wird. Sie sollte einen Wert zwischen 5-8 haben. (5 - nah dran, 8 - weiter weg)
		 */
		std::vector<Actor*> collectSelectableObjects( Ogre::Real x, Ogre::Real y, Ogre::Real length );
        void doDestroyActor( Actor* actor );
        Ogre::String nextUniqueName(const Ogre::String& basename);

        ActorPtrMap mActors;
		World* mWorld;

		Ogre::Real m_ActorRenderingDistance;
};

}
#endif
