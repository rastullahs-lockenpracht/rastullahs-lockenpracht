#ifndef __ActorFactory_H__
#define __ActorFactory_H__

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

class _RlCoreExport ActorFactory : protected Singleton<ActorFactory>
{
    public:
        ActorFactory( );
        virtual ~ActorFactory( );

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
	    static ActorFactory & getSingleton(void);
	    static ActorFactory * getSingletonPtr(void);

    private:
        String nextUniqueName(const String& basename);
        ActorPtrMap mActors;

        World* mWorld;
};

}
#endif
