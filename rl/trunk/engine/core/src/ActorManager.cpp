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
        mWorld = CoreSubsystem::getSingleton().getWorld();
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

    void ActorManager::destroyAllActors()
	{
        for (ActorPtrMap::iterator it = mActors.begin();
            it != mActors.end();) 
        {
            Actor* actor = it->second;
            if (actor->getControlledObject()->getObjectType() != "CameraObject")
            {
                mActors.erase(it++);
                destroyActor(actor);
            } else {
                ++it;
            }
        }
	}

    Actor* ActorManager::createLightActor(const String& name, int type)
	{
        const String&  uniquename = nextUniqueName(name);

        Actor* actor = 0;
        try
        {
            ///@todo Typen in Einklang bringen
            LightObject* lo = new LightObject(uniquename, (Light::LightTypes)type);

            actor = new Actor(uniquename, lo);
            mActors.insert(ActorPtrPair(uniquename,actor)); 
        }
        catch( Ogre::Exception& e)
        {
            CoreSubsystem::log("ActorManager - Das Light '"
                + uniquename + "' konnte nicht erstellt werden. Grund: "
                + e.getFullDescription());
        }

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
            co->getCamera()->setFarClipDistance(10000);
            PhysicalThing* pt = PhysicsManager::getSingleton()
                .createPhysicalThing(PhysicsManager::PT_SPHERE,
                    Vector3(co->getCamera()->getNearClipDistance() * 1.2, 0, 0),
                    0.0f);
            actor = new Actor(uniquename, co, pt);

            mActors.insert(ActorPtrPair(uniquename,actor)); 
        }
        catch( Ogre::Exception& e)
        {
            CoreSubsystem::log("ActorManager - Die Camera '"
                + name + "' für den Aktor '"
                + uniquename + "' konnte nicht erstellt werden. Grund: "
                + e.getFullDescription());
        }

        return actor;
    }
    
	Actor* ActorManager::createMeshActor(const String& name,const String& meshname,
	    int geomType, Ogre::Real density)
	{
		const String&  uniquename = nextUniqueName(name);
		
		Actor* actor = 0;
        try
        {
		    MeshObject* mo = new MeshObject(uniquename, meshname);
		    PhysicalThing* pt = PhysicsManager::getSingleton()
		        .createPhysicalThing(geomType, mo->getSize(), density);

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

    //Actor* ActorManager::createParticleSystemActor(const String& name,const String& partname)
    //{
    //    const String&  uniquename = nextUniqueName(name);

    //    try
    //    {
    //        ParticleSystem* particleSystem =
    //            ParticleSystemManager::getSingleton().createSystem(uniquename, partname);    
    //    
    //        Actor* actor = new Actor(uniquename, particleSystem);

		  //  mActors.insert(ActorPtrPair(uniquename, actor) ); 
		  //  return actor;
    //    }
    //    catch( Ogre::Exception )
    //    {
    //        CoreSubsystem::log("ActorManager - Das Partikelsystem '"
    //            + partname + "' für den Aktor '"
    //            + uniquename + "' konnte nicht erstellt werden.");
    //    }

    //    return 0;
    //}

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

	Actor* ActorManager::getActorAt(Real x, Real y) const
	{
		if (getWorld()->getActiveCamera() == NULL ||
			getWorld()->getActiveActor() == NULL)
			return NULL;

		 // Start a new ray query
		Ogre::Ray cameraRay = getWorld()->getActiveCamera()->
			getCameraToViewportRay( x, y );
		Ogre::Ray selectRay(
			cameraRay.getOrigin()/*getWorld()->getActiveActor()->getPosition()*/, 
			cameraRay.getDirection());
		Ogre::RaySceneQuery *raySceneQuery = getWorld()->
			getSceneManager()->createRayQuery(selectRay);
		raySceneQuery->execute();
		Ogre::RaySceneQueryResult result = raySceneQuery->getLastResults();
		 
		Ogre::MovableObject *closestObject = NULL;
		Real closestDistance = LONG_MAX;

		std::list< Ogre::RaySceneQueryResultEntry >::iterator rayIterator;
		 
		for ( rayIterator = result.begin(); rayIterator != result.end(); rayIterator++ ) {
			if ((*rayIterator).movable->getUserObject() != NULL) 
			{
				if ((*rayIterator).distance < closestDistance) 
				{
					closestObject = (*rayIterator).movable;
					closestDistance = (*rayIterator).distance;
				}
			}
		}

		Actor* rval;
		// No object clicked
		if ( closestObject == NULL) {   
			rval = NULL;
		} else {
			rval = static_cast<Actor*>(closestObject->getUserObject());
		}

		raySceneQuery->clearResults();
		getWorld()->getSceneManager()->destroyQuery(raySceneQuery);

		return rval;
	}
}
