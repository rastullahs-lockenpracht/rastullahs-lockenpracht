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

#ifndef __MeshActor_H__
#define __MeshActor_H__

#include "CorePrerequisites.h"
#include "Actor.h"

#include "RlAnimation.h"

namespace rl {

class _RlCoreExport MeshActor : public virtual Actor
{
public:
	MeshActor(const String& name, Entity* pEntity);
	MeshActor(const String& name, SceneNode* pParentSceneNode, Entity* pEntity);

	virtual ~MeshActor();
	virtual const String& getTypeName();

	Entity* getEntity(void) const;
    virtual void setSceneNode(SceneNode* node);

    void setScale( Real x, Real y, Real z );

    Ogre::Vector3 getExtent();
    
    /// Relativ zum lokalen Koordinatensystem
    Ogre::Vector3 getCenter();
    Ogre::Real getRadius();
    Ogre::Real getHeight();

    RlAnimation* startAnimation(const String&, Real speed=1.0, unsigned int timesToPlay=0);
	RlAnimation* getAnimation(const String&) const;
	void stopAnimation(const String&);

    void setCastShadows (bool enabled);
    bool getCastShadows (void) const;

    void attachActorToBone(Actor* actor, String name);
    void attachActorToBone(Actor* actor,  unsigned short id);
    void detachActorFromBone(Actor* actor);
protected:
	static const String TYPENAME;
	Entity* mEntity;
};

}
#endif

