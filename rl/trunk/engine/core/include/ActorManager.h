#ifndef __ActorManager_H__
#define __ActorManager_H__

#include <map>

#include "CorePrerequisites.h"

namespace rl {

class World;
class Actor;
class CameraActor;
class LightActor;
class ParticleSystemActor;
class GameActor;
class AnimatedActor;
class MeshActor;

typedef std::map<String,Actor*> ActorPtrMap;
typedef std::pair<String,Actor*> ActorPtrPair;

class _RlCoreExport ActorManager : protected Singleton<ActorManager>
{
    public:
        ActorManager( );
        virtual ~ActorManager( );

        Actor* getActor(const String& name);
		void deleteActor(const String& name);
        void deleteAllActors();

		CameraActor* createCameraActor(const String& name);
        LightActor* createLightActor(const String& name);
		MeshActor* createMeshActor(const String& name,const String& meshname);
		GameActor* createGameActor(const String& name,const String& meshname);
        ParticleSystemActor* createParticleSystemActor(const String& name,const String& partname);

        void setWorld( World* pWorld );

        /** Returns the Singleton */
	    static ActorManager & getSingleton(void);
	    static ActorManager * getSingletonPtr(void);

    private:
        String nextUniqueName(const String& basename);
        ActorPtrMap mActors;

        World* mWorld;
};

}
#endif
