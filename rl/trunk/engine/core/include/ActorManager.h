/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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
		const World* const getWorld() const;

		Actor* getActorAt(Ogre::Real x, Ogre::Real y) const;

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
