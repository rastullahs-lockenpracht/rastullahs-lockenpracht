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

#include "ActorManager.h"

#include "CoreSubsystem.h"

#include "Actor.h"
#include "World.h"
#include "MeshObject.h"
#include "CameraObject.h"
#include "LightObject.h"
#include "SoundObject.h"
#include "Sound.h"
#include "ListenerMovable.h"
#include "ListenerObject.h"
#include "PhysicalThing.h"
#include "ScriptWrapper.h"
#include "MultimediaSubsystem.h"
#include "SoundDriver.h"

template<> rl::ActorManager* Singleton<rl::ActorManager>::ms_Singleton = 0;

namespace rl {

    ActorManager& ActorManager::getSingleton(void)
	{
		return Singleton<ActorManager>::getSingleton();
	}

	ActorManager* ActorManager::getSingletonPtr(void)
	{
		return Singleton<ActorManager>::getSingletonPtr();
	}

    ActorManager::ActorManager() : 
        mActors()
    {
		static const int RADIUS = 20, LENGTH = 300;
        mWorld = CoreSubsystem::getSingleton().getWorld();
		m_ActorRenderingDistance = 10000; 
    }

    ActorManager::~ActorManager()
    {
    }

    void ActorManager::setWorld( World* world )
    {
        mWorld = world;
    }

	const World* const ActorManager::getWorld() const
	{
		return mWorld;
	}

	Ogre::Real ActorManager::getDefaultActorRenderingDistance() const
	{
		return m_ActorRenderingDistance;
	}

	void ActorManager::setDefaultActorRenderingDistance( Ogre::Real dist ) 
	{
		for (ActorPtrMap::iterator it = mActors.begin();
            it != mActors.end();) 
        {
            Actor* actor = it->second;

			// Nur die Actoren verändern, deren Distanz nicht geändert wurde
			if( actor->getRenderingDistance() == m_ActorRenderingDistance )
            {
				actor->setRenderingDistance( dist );
            } 

            ++it;
        }

		m_ActorRenderingDistance = dist;
	}

    Actor* ActorManager::getActor(const String& name)
	{
		ActorPtrMap::const_iterator pActorIter = mActors.find(name);

		if( pActorIter != mActors.end() )
			return pActorIter->second;

		return 0;
	}

	void ActorManager::destroyActor( Actor* actor )
	{
        if( actor == NULL )
            Throw( NullPointerException, "Der zu löschende Actor existiert nicht" );
        
        mActors.erase( actor->getName() );
        doDestroyActor( actor );
	}

    void ActorManager::doDestroyActor( Actor* actor )
    {
        ActorControlledObject* actObj = actor->getControlledObject();
        PhysicalThing* physThing = actor->getPhysicalThing();

        ScriptWrapper::getSingleton().deleted( actor );
        delete actor;

        if( actObj )
        {
            ScriptWrapper::getSingleton().deleted( actObj );
            delete actObj;
        }
        if( physThing )
            PhysicsManager::getSingleton().removeAndDestroyPhysicalThing( physThing );
    }
    
    void ActorManager::destroyAllActors()
	{
        for (ActorPtrMap::iterator it = mActors.begin();
            it != mActors.end();) 
        {
            Actor* actor = it->second;
            // Kameras spezieller betrachten...
            if( (!actor->getControlledObject()) || 
                (actor->getControlledObject()->getObjectType().compare( "CameraObject" ) != 0 ) )
            {
                mActors.erase( it++ );  
                doDestroyActor( actor );
            } else {
                ++it;
            }
        }
	}

    Actor* ActorManager::createLightActor(const String& name, rl::LightObject::LightTypes type )
	{
        const String&  uniquename = nextUniqueName(name);

        Actor* actor = 0;
        try
        {
            ///@todo Typen in Einklang bringen
            LightObject* lo = new LightObject(uniquename, type);

            actor = new Actor(uniquename, lo);
            mActors.insert(ActorPtrPair(uniquename,actor)); 
        }
        catch( Ogre::Exception& e)
        {
            Logger::getSingleton().log(Logger::CORE, Ogre::LML_CRITICAL, "ActorManager - Das Licht '"
                + uniquename + "' konnte nicht erstellt werden. Grund: "
                + e.getFullDescription());
        }

        return actor;
	}

    Actor* ActorManager::createSoundSampleActor(const String& name, const String& soundfile)
    {
        const String&  uniquename = nextUniqueName(name);

        Actor* actor = 0;
        try
        {
            Sound* sm = MultimediaSubsystem::getSingleton().getActiveDriver()->createSample(soundfile);
            SoundObject* so = new SoundObject(sm, soundfile);

            actor = new Actor(uniquename, 0 /* TODO so */);
            mActors.insert(ActorPtrPair(uniquename,actor));  
        }
        catch( Ogre::Exception& e)
        {
            Logger::getSingleton().log(Logger::CORE, Ogre::LML_CRITICAL, "ActorManager - Der Sound '"
                + uniquename + "' konnte nicht erstellt werden. Grund: "
                + e.getFullDescription());
        }

        return actor;
    }

    Actor* ActorManager::createSoundStreamActor(const String& name, const String& soundfile)
    {
        const String&  uniquename = nextUniqueName(name);

        Actor* actor = 0;
        try
        {
            Sound* sm = MultimediaSubsystem::getSingleton().getActiveDriver()->createStream(soundfile);
            SoundObject* so = new SoundObject(sm, soundfile);

            actor = new Actor(uniquename, 0 /* TODO so */);
            mActors.insert(ActorPtrPair(uniquename,actor)); 
        }
        catch( Ogre::Exception& e)
        {
			Logger::getSingleton().log(Logger::CORE, Ogre::LML_CRITICAL, "ActorManager - Der Sound '"
                + uniquename + "' konnte nicht erstellt werden. Grund: "
                + e.getFullDescription());
        }

        return actor;
    }

    Actor* ActorManager::createListenerActor(const String& name)
    {
        const String&  uniquename = nextUniqueName(name);

        Actor* actor = 0;
        try
        {
            ListenerMovable* lm = new ListenerMovable(name);
            ListenerObject* lo = new ListenerObject(lm);

            actor = new Actor(uniquename, lo);
            mActors.insert(ActorPtrPair(uniquename,actor)); 
        }
        catch( Ogre::Exception& e)
        {
            Logger::getSingleton().log(Logger::CORE, Ogre::LML_CRITICAL, "ActorManager - Der Listener '"
                + uniquename + "' konnte nicht erstellt werden. Grund: "
                + e.getFullDescription());
        }

        return actor;
    }

    Actor* ActorManager::createEmptyActor(const String& name)
    {
        const String&  uniquename = nextUniqueName(name);

        Actor* actor = new Actor(uniquename);
        mActors.insert(ActorPtrPair(uniquename,actor)); 

        return actor;
    }

    Actor* ActorManager::createCameraActor(const String& name)
    {
        const String&  uniquename = nextUniqueName(name);
        Actor* actor = 0;
        try
        {
            CameraObject* co = new CameraObject(uniquename);
            PhysicalThing* pt = PhysicsManager::getSingleton()
                .createPhysicalThing(PhysicsManager::GT_SPHERE, co, 
                    100.0f, PhysicsManager::OM_CENTERED, true);
			PhysicsManager::getSingleton().createPhysicsProxy(pt, NULL);
            actor = new Actor(uniquename, co, pt);

            mActors.insert(ActorPtrPair(uniquename,actor)); 
        }
        catch( Ogre::Exception& e)
        {
            Logger::getSingleton().log(Logger::CORE, Ogre::LML_CRITICAL, "ActorManager - Die Kamera '"
                + name + "' für den Aktor '"
                + uniquename + "' konnte nicht erstellt werden. Grund: "
                + e.getFullDescription());
        }

        return actor;
    }

	Actor* ActorManager::createMeshActor(const String& name,const String& meshname,
		PhysicsManager::GeometryTypes geomType, Ogre::Real mass, PhysicsManager::OffsetMode offsetMode )
	{
		const String&  uniquename = nextUniqueName(name);
		
		Actor* actor = 0;
        try
        {
            PhysicalThing* pt = 0;
		    MeshObject* mo = new MeshObject(uniquename, meshname);
            pt = PhysicsManager::getSingleton().createPhysicalThing( geomType, mo, mass, offsetMode );

		    actor = new Actor(uniquename, mo, pt);
		    mActors.insert(ActorPtrPair(uniquename,actor)); 
        }
        catch( Ogre::Exception& e)
        {
            Logger::getSingleton().log(Logger::CORE, Ogre::LML_CRITICAL, "ActorManager - Das Mesh '"
                + meshname + "' für den Aktor '"
                + uniquename + "' konnte nicht erstellt werden. Grund: "
                + e.getFullDescription());
        }
    
        return actor;
	}

    Actor* ActorManager::createParticleSystemActor(const String& name,const String& partname)
    {
        const String&  uniquename = nextUniqueName(name);

		Actor* actor = 0;
        try
        {
			ParticleSystemObject* po = new ParticleSystemObject(uniquename, partname);

			actor = new Actor(uniquename, po, 0);
			mActors.insert(ActorPtrPair(uniquename,actor)); 
        }
        catch( Ogre::Exception& e )
        {
            Logger::getSingleton().log(Logger::CORE, Ogre::LML_CRITICAL, "ActorManager - Das Partikelsystem '"
                + partname + "' für den Aktor '"
				+ uniquename + "' konnte nicht erstellt werden. Grund: "
				+ e.getFullDescription());
        }

        return actor;
    }

    String ActorManager::nextUniqueName(const String& basename)
	{
		String newname = basename;
	
		int iID = 0;

		while( this->getActor(newname) != 0 )
		{
			newname = basename + "_" + StringConverter::toString(++iID);
		}
		
		if( basename != newname )
            Logger::getSingleton().log(Logger::CORE, Ogre::LML_CRITICAL, "ActorManager - Warnung! '" + basename
                + "' war schon vergeben! Neuer Name ist '" + newname + "'.");
        
		return newname;
	}

	Actor* ActorManager::getActorAt(Real x, Real y, Real width, Real length, bool infinite)
	{      
        if (getWorld()->getActiveCamera() == NULL ||
			getWorld()->getActiveActor() == NULL)
			return NULL;

		std::vector<Actor*> selectableObjects = collectSelectableObjects( x, y, length );
		
		Actor* closestObject = NULL;
		Real closestDistance = 400;

		if (selectableObjects.size() == 0)
			return NULL;

		std::vector<Actor*>::iterator resultIterator;
		for ( resultIterator = selectableObjects.begin(); resultIterator != selectableObjects.end(); resultIterator++ ) 
		{
			Actor* movable = *resultIterator;
			closestObject = movable;
		}

		return closestObject;
	}


	std::vector<Actor*> ActorManager::collectSelectableObjects(Real x, Real y, Real length)
	{
		std::vector<Actor*> selectableObjects;
		// Start a new ray query
		Ogre::Ray cameraRay = getWorld()->getActiveCamera()->
			getCameraToViewportRay( x, y );

		//Startpunkt des Rays setzen
		Vector3 rayStart = cameraRay.getOrigin();

		//Ray "abfeuern"
		RaySceneQuery* query = getWorld()->getSceneManager()->createRayQuery(cameraRay);

		//Nach Abstand sortieren
		query->setSortByDistance(true);

		//Treffer auslesen
		RaySceneQueryResult result = query->execute();

		//Auswählbare Objekte aus der Liste löschen
		selectableObjects.clear();

		//Falls mehr als 0 Objekte getroffen wurde
		if (result.size() > 0)
		{
			//Iterator loslaufen lassen
			for (RaySceneQueryResult::iterator iter = result.begin(); iter != result.end(); iter++)
			{
				//Entsprechendes MovableObject lesen
				Ogre::MovableObject* mo = (*iter).movable;

				//Die Position des Getroffenen Objekts speichern
				Vector3 actorPos = mo->getParentNode()->getPosition();

				//Ein bisschen Mathematik um den Abstand zwischen dem Actor und dem Ray zu berechnen
				//(Abstand = Betrag der Differenz der Beiden Vektoren)
				//(Betrag = Wurzel aus der Summe der Quadrate der einzelnen Vektorkomponenten)
				Real actorDistance = sqrt(((actorPos.x-rayStart.x)*(actorPos.x-rayStart.x))
										  +((actorPos.y-rayStart.y)*(actorPos.y-rayStart.y))
										  +((actorPos.z-rayStart.z)*(actorPos.z-rayStart.z)));

				//Falls mo nicht vorhanden oder falls mo die aktuelle Kamera ist
				if (mo->getUserObject() == NULL || mo == getWorld()->getActiveCamera())
					continue; //Schleife wiederholen

				//Falls der Abstand des Actors zum Ray größer ist als die Länge des Rays
				if (actorDistance > length)
					continue; //Schleife wiederholen

				//Actor auslesen
				Actor* actor = reinterpret_cast<Actor*> (mo->getUserObject());

				//Falls der Actor der aktuelle Spielcharacter ist
				if (actor == getWorld()->getActiveActor())
					continue; //Schleife wiederholen

				//Actor zu den auswählbaren Objekten hinzufügen
				selectableObjects.push_back(actor);
			}
		}
		//Die auswählbaren Objekte zurückgeben
		return selectableObjects;
	}
}
