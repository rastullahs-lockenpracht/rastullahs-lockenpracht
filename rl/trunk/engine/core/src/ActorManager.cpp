#include "ActorManager.h"

#include "CoreSubsystem.h"

#include "MeshActor.h"
#include "GameActor.h"
#include "CameraActor.h"
#include "LightActor.h"
#include "ParticleSystemActor.h"
#include "World.h"


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
		
	void ActorManager::deleteActor(const String& name)
	{
		ActorPtrMap::iterator pActorIter = mActors.find(name);

		if( pActorIter != mActors.end() )
		{
			Actor* pActor = pActorIter->second;
			mActors.erase(pActorIter);
			delete pActor;
		}
	}

    void ActorManager::deleteAllActors()
	{       
        ActorPtrMap::iterator pActorIter;

        while( mActors.begin() != mActors.end() ) 
        {
            pActorIter = mActors.begin();
            Actor* pActor = pActorIter->second;
			mActors.erase(pActorIter);
			delete pActor;
        }
	}

	CameraActor* ActorManager::createCameraActor(const String& name)
	{
		const String&  uniquename = nextUniqueName(name);

        try
        {
		    Camera* pCamera = mWorld->getSceneManager()->createCamera(uniquename);
		    CameraActor* pCameraActor = new CameraActor(uniquename,pCamera);
            //pCamera->rotate( mWorld->getWorldAxis() );

		    mActors.insert( ActorPtrPair(uniquename,pCameraActor) ); 
		    return pCameraActor;
        }
        catch( Ogre::Exception )
        {
            CoreSubsystem::log("ActorManager - Die Camera für den Aktor '"
                + uniquename + "' konnte nicht erstellt werden.");
        }
    
        return 0;
	}

    LightActor* ActorManager::createLightActor(const String& name)
	{
		const String&  uniquename = nextUniqueName(name);

        try
        {
            Light* pLight = mWorld->getSceneManager()->createLight(uniquename);
		    LightActor* pLightActor = new LightActor(uniquename,pLight);
            //pLightActor->rotate( mWorld->getWorldAxis() );

		    mActors.insert( ActorPtrPair(uniquename,pLightActor) ); 
		    return pLightActor;
        }
        catch( Ogre::Exception )
        {
            CoreSubsystem::log("ActorManager - Das Licht für den Aktor '" 
                + uniquename + "' konnte nicht erstellt werden.");
        }
    
        return 0;
	}

	MeshActor* ActorManager::createMeshActor(const String& name,const String& meshname)
	{
		const String&  uniquename = nextUniqueName(name);

        try
        {
		    Entity* entity = mWorld->getSceneManager()->createEntity(uniquename, meshname);
		    MeshActor* actor = new MeshActor(uniquename,entity);
            //actor->rotate( mWorld->getWorldAxis() );

		    mActors.insert( ActorPtrPair(uniquename,actor) ); 
		    return actor;
        }
        catch( Ogre::Exception )
        {
            CoreSubsystem::log("ActorManager - Das Mesh '"
                + meshname + "' für den Aktor '"
                + uniquename + "' konnte nicht erstellt werden.");
        }
    
        return 0;
	}

	GameActor* ActorManager::createGameActor(const String& name,const String& meshname)
	{
		const String&  uniquename = nextUniqueName(name);

        try
        {
		    Entity* entity = mWorld->getSceneManager()->createEntity(uniquename, meshname);		
		    GameActor* pGameActor = new GameActor(uniquename,entity);
            //pGameActor->rotate( mWorld->getWorldAxis() );

		    mActors.insert( ActorPtrPair(uniquename,pGameActor) ); 
		    return pGameActor;
        }
        catch( Ogre::Exception )
        {
            CoreSubsystem::log("ActorManager - Das Mesh '"
                + meshname + "' für den Aktor '"+ 
                uniquename + "' konnte nicht erstellt werden.");
        }
    
        return 0;
	}

    ParticleSystemActor* ActorManager::createParticleSystemActor(const String& name,const String& partname)
    {
        const String&  uniquename = nextUniqueName(name);

        try
        {
            ParticleSystem* pParticleSystem = ParticleSystemManager::getSingleton().
            createSystem(uniquename, partname);    
        
            ParticleSystemActor* pParticleSystemActor = new ParticleSystemActor(uniquename,pParticleSystem);
            //pParticleSystemActor->rotate( mWorld->getWorldAxis() );

		    mActors.insert( ActorPtrPair(uniquename,pParticleSystemActor) ); 
		    return pParticleSystemActor;
        }
        catch( Ogre::Exception )
        {
            CoreSubsystem::log("ActorManager - Das Partikelsystem '"
                + partname + "' für den Aktor '"
                + uniquename + "' konnte nicht erstellt werden.");
        }

        return 0;
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

	Actor* ActorManager::getActorAt(Real x, Real y) const
	{
		 // Start a new ray query
		Ogre::Ray cameraRay = getWorld()->getActiveCamera()->
			getOgreCamera()->getCameraToViewportRay( x, y );
		const World* w = getWorld();
		Ogre::RaySceneQuery *raySceneQuery = w->
			getSceneManager()->createRayQuery(cameraRay);
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



