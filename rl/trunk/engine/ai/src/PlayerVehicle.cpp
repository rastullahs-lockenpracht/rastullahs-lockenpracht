/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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
#include "PlayerVehicle.h"
#include "Actor.h"
#include "MeshObject.h"
#include "PhysicalThing.h"

using namespace rl;

PlayerVehicle::PlayerVehicle(Actor* character)
	: SteeringVehicle(NULL, character)
{
}

PlayerVehicle::~PlayerVehicle(void)
{
}

void PlayerVehicle::update(const float currentTime, const float elapsedTime)
{
	PhysicalThing* pt = mActor->getPhysicalThing();
	OgreNewt::Body* body = NULL;
	if(pt) pt->_getBody();
	if(body)
	{
		Vector3 position;
		Quaternion orientation;
		body->getPositionOrientation(position, orientation);
		setPosition(Vec3(position.x, position.y, position.z));

	//  Get the velocity vector
		mCurrentVelocity = body->getVelocity();
	//  enforce speed limit
	//  newVelocity = newVelocity.truncateLength (maxSpeed ());
	//  update speed
		setSpeed(mCurrentVelocity.length());
		Vec3 newVelocity(mCurrentVelocity.x, mCurrentVelocity.y, mCurrentVelocity.z);

	//  regenerate local space (by default: align vehicle's forward axis with
	//  new velocity, but this behavior may be overridden by derived classes.)
		if (speed() > 0) regenerateOrthonormalBasisUF (newVelocity / speed());

	//  prevent adding a counter force against gravity
		if (mCurrentVelocity.y < 0.0f) mCurrentVelocity.y = 0.0f;
	}
	mCurrentForce = Ogre::Vector3::ZERO;
}