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

#include "Actor.h"

#include "CoreSubsystem.h"
#include "World.h"
#include "PhysicsManager.h"
#include "PhysicalThing.h"

namespace rl {

const String Actor::TYPENAME="Actor";

Actor::Actor(  const String& name )
{
	mName = name;
	SceneManager* sceneMgr = CoreSubsystem::getSingleton().getWorld()->getSceneManager();
	mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode(name);
    mPhysical = 0;
}

Actor::Actor(  const String& name, SceneNode* parentNode )
{
	mName = name;
	mSceneNode =  parentNode->createChildSceneNode(name);
    mPhysical = 0;
}

Actor::~Actor()
{
    if( mPhysical != 0 )
        PhysicsManager::getSingleton().removePhysicalThing( this );

	SceneManager* pSceneMgr = CoreSubsystem::getSingleton().getWorld()->getSceneManager();
	mSceneNode->getParent()->removeChild(mSceneNode);

	pSceneMgr->destroySceneNode( mName );
}

PhysicalThing* Actor::getPhysicalThing()
{
    return mPhysical;
}

void Actor::setPhysicalThing( PhysicalThing* thing )
{
    mPhysical = thing;
}

SceneNode* Actor::getSceneNode(void)
{
	return mSceneNode;
}

void Actor::setSceneNode(SceneNode* node)
{
	mSceneNode = node;
}

/** Returns the unique Name of this Actor */
const String& Actor::getName()
{
    return mName;
}


bool Actor::isSimpleOgreActor()
{
    return ( mPhysical != 0 );
}

void Actor::attachActorToSceneNode(Actor* actor)
{
    actor->getSceneNode()->getParent()->removeChild(actor->getName());
    mSceneNode->addChild( actor->getSceneNode() );
}

void Actor::detachActorFromSceneNode(Actor* actor)
{
    mSceneNode->removeChild(actor->getName());
    SceneManager* sceneMgr = CoreSubsystem::getSingleton().getWorld()->getSceneManager();
    sceneMgr->getRootSceneNode()->addChild(actor->getSceneNode());
}

const String& Actor::getTypeName(){
	return TYPENAME;
}


// MOVABLE


// Override these
void Actor::setOrientation(Real w, Real x, Real y, Real z)
{
	mSceneNode->setOrientation( w, x, y, z);

    // TODO: Eventuell nicht updaten wenn von Physik gedreht
    if( mPhysical )
        mPhysical->setOrientation(w,x,y,z);
}

void Actor::setPosition(Real x, Real y, Real z)
{
    mSceneNode->setPosition(x, y, z);
    
    // TODO: Eventuell nicht updaten wenn von Physik bewegt
    if( mPhysical )
        mPhysical->setPosition(x,y,z);
}



// These Only Use the above
void Actor::setOrientation(const Quaternion& orientation)
{
    setOrientation(orientation.w,orientation.x,orientation.y,orientation.z);
}


void Actor::setPosition(const Vector3& vec)
{
	setPosition(vec.x, vec.y, vec.z);
}


void Actor::translate(const Vector3& d)
{
	// Adjust position by rotation
	Vector3 newTrans = mSceneNode->getOrientation() * d;
	translateWorldSpace(newTrans);
}

void Actor::translate(Real x, Real y, Real z)
{
	translate(Vector3(x, y, z));
}

void Actor::translateWorldSpace(Real x, Real y, Real z)
{
	translateWorldSpace(Vector3(x, y, z));
}

void Actor::translateWorldSpace(const Vector3& d)
{
	setPosition(getPosition() + d);
}

void Actor::roll(Real angle)
{
	rotate(Vector3::UNIT_Z, angle);
}

void Actor::pitch(Real angle)
{
	rotate(Vector3::UNIT_X, angle);
}

void Actor::yaw(Real angle)
{
	rotate(Vector3::UNIT_Y, angle);
}

void Actor::rotate(const Vector3& axis, Real angle)
{
	Quaternion q;
	q.FromAngleAxis(Degree(angle),axis);
	rotate(q);
}

void Actor::rotate(Real x, Real y, Real z, Real angle)
{
	Vector3 v = Vector3(x,y,z);
	rotate(v, angle);
}

void Actor::rotate(const Quaternion& q)
{
	setOrientation(getOrientation() * q);
}

const Vector3& Actor::getPosition(void)
{
	return mSceneNode->getPosition();
}

const Quaternion& Actor::getOrientation(void)
{
	return mSceneNode->getOrientation();
}


}
