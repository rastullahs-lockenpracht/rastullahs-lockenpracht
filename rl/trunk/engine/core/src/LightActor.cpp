#include "LightActor.h"

namespace rl {

const String LightActor::TYPENAME="LightActor";

LightActor::LightActor( const String& name, Light* pLight) : Actor(name)
{
	mLight = pLight;
	mSceneNode->attachLight(mLight);
	mLight->setUserObject(this);
}

LightActor::LightActor( const String& name, SceneNode* pParentSceneNode, Light* pLight) 
	: Actor(name,pParentSceneNode)
{
	mLight = pLight;
    mSceneNode->attachLight(mLight);
	mLight->setUserObject(this);
}

LightActor::~LightActor()
{
	mSceneNode->detachObject(mLight->getName()); 
    mSceneNode->getCreator()->removeLight(mLight);
}

void LightActor::setAttenuation( Real range, Real constant, Real linear, Real quadric )
{
    mLight->setAttenuation( range, constant, linear, quadric );
}

void LightActor::setDiffuseColour( Real red, Real green, Real blue )
{
    mLight->setDiffuseColour(red,green,blue);
}

void LightActor::setDirection( Real x, Real y, Real z )
{
    mLight->setDirection(x,y,z);
}

void LightActor::setSpecularColour( Real red, Real green, Real blue )
{
    mLight->setSpecularColour(red,green,blue);
}

void LightActor::setSpotlightRange( Real innerangle, Real outerangle, Real falloff )
{
    mLight->setSpotlightRange(innerangle, outerangle, falloff );
}

void LightActor::setVisible( bool visible )
{
    mLight->setVisible( visible );
}


const String& LightActor::getTypeName(){
	return TYPENAME;
}

}
