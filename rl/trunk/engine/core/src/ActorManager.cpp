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

#include "ActorManager.h"

#include "CoreSubsystem.h"

#include "Actor.h"
#include "World.h"
#include "MeshObject.h"
#include "CameraObject.h"
#include "LightObject.h"
#include "SoundObject.h"
#include "SoundMovable.h"
#include "ListenerMovable.h"
#include "ListenerObject.h"
#include "PhysicalThing.h"

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

    ActorManager::ActorManager() : mActors()
    {
		static const int RADIUS = 20, LENGTH = 300;
        mWorld = CoreSubsystem::getSingleton().getWorld();
		mActorOdeSpace = new OgreOde::SimpleSpace();
		mSelectionCapsule = new OgreOde::CapsuleGeometry(RADIUS, LENGTH, NULL);
    }

    ActorManager::~ActorManager()
    {
		delete mActorOdeSpace;
    }

    void ActorManager::setWorld( World* world )
    {
        mWorld = world;
    }

	const World* const ActorManager::getWorld() const
	{
		return mWorld;
	}

    Actor* ActorManager::getActor(const String& name)
	{
		ActorPtrMap::const_iterator pActorIter = mActors.find(name);

		if( pActorIter != mActors.end() )
			return pActorIter->second;

		return 0;
	}

	void ActorManager::destroyActor(Actor* actor)
	{
        ///@todo Loesch ihn!
	}

// Harle: Finger weg ;-)
// Effective STL, item 5: Erase of associative containers doesn't return anything.
// Item 9: How to loop through the container with erase.
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
                mActors.erase(it++);
                destroyActor(actor);
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
            CoreSubsystem::log("ActorManager - Das Licht '"
                + uniquename + "' konnte nicht erstellt werden. Grund: "
                + e.getFullDescription());
        }

        return actor;
	}

    Actor* ActorManager::createSoundActor(const String& name, const String& soundfile)
    {
        const String&  uniquename = nextUniqueName(name);

        Actor* actor = 0;
        try
        {
            SoundMovable* sm = new SoundMovable(soundfile);
            SoundObject* so = new SoundObject(sm);

            actor = new Actor(uniquename, so);
            mActors.insert(ActorPtrPair(uniquename,actor)); 
        }
        catch( Ogre::Exception& e)
        {
            CoreSubsystem::log("ActorManager - Der Sound '"
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
            CoreSubsystem::log("ActorManager - Der Listener '"
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
            co->getCamera()->setNearClipDistance(10);
            co->getCamera()->setFarClipDistance(1000000);
            PhysicalThing* pt = PhysicsManager::getSingleton()
                .createPhysicalThing(PhysicsManager::GT_SPHERE,
                    Vector3(co->getCamera()->getNearClipDistance() * 1.5, 0, 0),
                    0.0f, mActorOdeSpace, PhysicsManager::OM_CENTERED);
            actor = new Actor(uniquename, co, pt);

            mActors.insert(ActorPtrPair(uniquename,actor)); 
        }
        catch( Ogre::Exception& e)
        {
            CoreSubsystem::log("ActorManager - Die Kamera '"
                + name + "' für den Aktor '"
                + uniquename + "' konnte nicht erstellt werden. Grund: "
                + e.getFullDescription());
        }

        return actor;
    }

	Actor* ActorManager::createMeshActor(const String& name,const String& meshname,
		PhysicsManager::GeometryTypes geomType, Ogre::Real density)
	{
		const String&  uniquename = nextUniqueName(name);
		
		Actor* actor = 0;
        try
        {
		    MeshObject* mo = new MeshObject(uniquename, meshname);
		    PhysicalThing* pt = PhysicsManager::getSingleton()
		        .createPhysicalThing(geomType, mo->getSize(), density, mActorOdeSpace);

		    actor = new Actor(uniquename, mo, pt);
		    mActors.insert(ActorPtrPair(uniquename,actor)); 
        }
        catch( Ogre::Exception& e)
        {
            CoreSubsystem::log("ActorManager - Das Mesh '"
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
            CoreSubsystem::log("ActorManager - Das Partikelsystem '"
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
            CoreSubsystem::log("ActorManager - Warnung! '" + basename
                + "' war schon vergeben! Neuer Name ist '" + newname + "'.");
        
		return newname;
	}

	Actor* ActorManager::getActorAt(Real x, Real y, Real width, Real length, bool infinite)
	{      
//		return NULL;

        if (getWorld()->getActiveCamera() == NULL ||
			getWorld()->getActiveActor() == NULL)
			return NULL;

		collectSelectableObjects( x, y );
		
		Actor* closestObject = NULL;
		Real closestDistance = LONG_MAX;

		if (mSelectableObjects.size() == 0)
			return NULL;

		std::vector<Actor*>::iterator resultIterator;
		for ( resultIterator = mSelectableObjects.begin(); resultIterator != mSelectableObjects.end(); resultIterator++ ) 
		{
			Actor* movable = *resultIterator;
			std::cerr<<movable->getName()<<"\n";
			closestObject = movable;
		}
		std::cerr<<"\n";

		return closestObject;
	}


	// Kollisionsvariante, funktioniert aus irgendwelchen Gründen nicht
	//void ActorManager::collectSelectableObjects( Real x, Real y )
	//{
	//	// Start a new ray query
	//	Ogre::Ray cameraRay = getWorld()->getActiveCamera()->
	//		getCameraToViewportRay( x, y );

	//	mSelectionCapsule->setPosition(cameraRay.getOrigin());
	//	mSelectionCapsule->setOrientation(getWorld()->getActiveCamera()->getWorldOrientation());

	//	mSelectableObjects.clear();
	//	collideWithActors(mSelectionCapsule, this);
	//}

	// Variante mit Strahl, ob der geringen Ausdehnung eines Strahls nicht zufriedenstellend
	void ActorManager::collectSelectableObjects( Real x, Real y )
	{
		// Start a new ray query
		Ogre::Ray cameraRay = getWorld()->getActiveCamera()->
			getCameraToViewportRay( x, y );

		RaySceneQuery* query = getWorld()->getSceneManager()->createRayQuery(cameraRay);
		query->setSortByDistance(true);
		RaySceneQueryResult result = query->execute();
		mSelectableObjects.clear();
		if (result.size() > 0)
		{
			for (RaySceneQueryResult::iterator iter = result.begin(); iter != result.end(); iter++)
			{
				Ogre::MovableObject* mo = (*iter).movable;

				if (mo->getUserObject() == NULL || mo == getWorld()->getActiveCamera())
					continue;

				Actor* actor = reinterpret_cast<Actor*>(mo->getUserObject());
				if (actor == getWorld()->getActiveActor())
					continue;

				mSelectableObjects.push_back(actor);
			}
		}
	}

	bool ActorManager::collision(OgreOde::Contact* contact)
	{
		OgreOde::Geometry* geom1 = contact->getFirstGeometry();
		OgreOde::Geometry* geom2 = contact->getSecondGeometry();

		Actor* target = NULL;
		if (geom1 == mSelectionCapsule)
			target = reinterpret_cast<Actor*>(geom2->getUserData());
		else if (geom2 == mSelectionCapsule)
			target = reinterpret_cast<Actor*>(geom2->getUserData());
		if (target != NULL)
			CoreSubsystem::getSingleton().log("Gefunden: "+target->getName());
		if (target != NULL && target != getWorld()->getActiveActor())
			mSelectableObjects.push_back(target);

		return true;
	}

	void ActorManager::collideWithActors(OgreOde::Geometry* geometry, OgreOde::CollisionListener* listener)
	{
		if (listener != NULL)
		{
			PhysicsManager* physMan = PhysicsManager::getSingletonPtr();
			physMan->getWorld()->setCollisionListener(listener);
			mActorOdeSpace->collide(geometry, NULL);
			physMan->getWorld()->setCollisionListener(physMan);
		}
		else
			mActorOdeSpace->collide(geometry, NULL);
	}
}
