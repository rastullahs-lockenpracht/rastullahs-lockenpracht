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

#include "ParticleSystemActor.h"

namespace rl {

const String ParticleSystemActor::TYPENAME="ParticleSystemActor";

ParticleSystemActor::ParticleSystemActor( const String& name, ParticleSystem* pParticleSystem) 
    : Actor(name)
{
	mParticleSystem = pParticleSystem;
	mSceneNode->attachObject(mParticleSystem);
	mParticleSystem->setUserObject(this);
}

ParticleSystemActor::ParticleSystemActor( const String& name, SceneNode* pParentSceneNode, ParticleSystem* pParticleSystem) 
	: Actor(name,pParentSceneNode)
{
	mParticleSystem = pParticleSystem;
    mSceneNode->attachObject(mParticleSystem);
	mParticleSystem->setUserObject(this);
}

ParticleSystemActor::~ParticleSystemActor()
{
    mSceneNode->detachObject(mParticleSystem->getName()); 
    ParticleSystemManager::getSingleton().destroySystem(mParticleSystem);
}

void ParticleSystemActor::setScale( Real x, Real y, Real z )
{
    mSceneNode->setScale(x,y,z);
}

const String& ParticleSystemActor::getTypeName(){
	return TYPENAME;
}

}
