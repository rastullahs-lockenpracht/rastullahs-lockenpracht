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

#include "LightActor.h"

namespace rl {

const String LightActor::TYPENAME="LightActor";

LightActor::LightActor( const String& name, Light* pLight) : Actor(name)
{
	mLight = pLight;
	mSceneNode->attachObject(mLight);
	mLight->setUserObject(this);
}

LightActor::LightActor( const String& name, SceneNode* pParentSceneNode, Light* pLight) 
	: Actor(name,pParentSceneNode)
{
	mLight = pLight;
    mSceneNode->attachObject(mLight);
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
    mLight->setSpotlightRange(Degree(innerangle), Degree(outerangle), falloff );
}

void LightActor::setVisible( bool visible )
{
    mLight->setVisible( visible );
}


const String& LightActor::getTypeName(){
	return TYPENAME;
}

}
