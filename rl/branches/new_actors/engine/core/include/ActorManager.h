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
#include "PhysicsManager.h"

namespace rl {

class World;
class Actor;

typedef std::map<String,Actor*> ActorPtrMap;
typedef std::pair<String,Actor*> ActorPtrPair;

class _RlCoreExport ActorManager : protected Singleton<ActorManager>
{
    public:
        ActorManager( );
        virtual ~ActorManager( );

        Actor* getActor(const String& name);

        //Actor* createLightActor(const String& name, int type);
		Actor* createMeshActor(const String& name,const String& meshname,
		    int geomType, Ogre::Real density);
        //Actor* createParticleSystemActor(const String& name,
        //    const String& partname);
            
        void destroyActor(Actor* actor);
        void destroyAllActors();

        void setWorld(World* world);
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
