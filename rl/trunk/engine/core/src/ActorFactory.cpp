#include "ActorFactory.h"

#include "CoreSubsystem.h"

#include "MeshActor.h"
#include "GameActor.h"
#include "CameraActor.h"
#include "LightActor.h"
#include "ParticleSystemActor.h"
#include "World.h"


template<> rl::ActorFactory* Singleton<rl::ActorFactory>::ms_Singleton = 0;

namespace rl {

    ActorFactory& ActorFactory::getSingleton(void)
	{
		return Singleton<ActorFactory>::getSingleton();
	}
	ActorFactory* ActorFactory::getSingletonPtr(void)
	{
		return Singleton<ActorFactory>::getSingletonPtr();
	}

    ActorFactory::ActorFactory( ) : mActors()
    {
        mWorld = CoreSubsystem::getSingleton().getWorld();
    }

    ActorFactory::~ActorFactory( )
    {
    }

    void ActorFactory::setWorld( World* pWorld )
    {
        mWorld = pWorld;
    }

    Actor* ActorFactory::getActor(const String& name)
	{
		ActorPtrMap::const_iterator pActorIter = mActors.find(name);

		if( pActorIter != mActors.end() )
			return pActorIter->second;

		return 0;
	}
		
	void ActorFactory::deleteActor(const String& name)
	{
		ActorPtrMap::iterator pActorIter = mActors.find(name);

		if( pActorIter != mActors.end() )
		{
			Actor* pActor = pActorIter->second;
			mActors.erase(pActorIter);
			delete pActor;
		}
	}

    void ActorFactory::deleteAllActors()
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

	CameraActor* ActorFactory::createCameraActor(const String& name)
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
            CoreSubsystem::log("ActorFactory - Die Camera für den Aktor '"
                + uniquename + "' konnte nicht erstellt werden.");
        }
    
        return 0;
	}

    LightActor* ActorFactory::createLightActor(const String& name)
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
            CoreSubsystem::log("ActorFactory - Das Licht für den Aktor '" 
                + uniquename + "' konnte nicht erstellt werden.");
        }
    
        return 0;
	}

	MeshActor* ActorFactory::createMeshActor(const String& name,const String& meshname)
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
            CoreSubsystem::log("ActorFactory - Das Mesh '"
                + meshname + "' für den Aktor '"
                + uniquename + "' konnte nicht erstellt werden.");
        }
    
        return 0;
	}

	GameActor* ActorFactory::createGameActor(const String& name,const String& meshname)
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
            CoreSubsystem::log("ActorFactory - Das Mesh '"
                + meshname + "' für den Aktor '"+ 
                uniquename + "' konnte nicht erstellt werden.");
        }
    
        return 0;
	}

    ParticleSystemActor* ActorFactory::createParticleSystemActor(const String& name,const String& partname)
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
            CoreSubsystem::log("ActorFactory - Das Partikelsystem '"
                + partname + "' für den Aktor '"
                + uniquename + "' konnte nicht erstellt werden.");
        }

        return 0;
    }

    String ActorFactory::nextUniqueName(const String& basename)
	{
		String newname = basename;
	
		int iID = 0;

		while( this->getActor(newname) != 0 )
		{
			newname = basename + "_" + StringConverter::toString(++iID);
		}
		
		if( basename != newname )
            CoreSubsystem::log("ActorFactory - Warnung! '" + basename
                + "' war schon vergeben! Neuer Name ist '" + newname + "'.");
        
		return newname;
	}


}



