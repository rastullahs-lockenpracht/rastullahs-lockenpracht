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
