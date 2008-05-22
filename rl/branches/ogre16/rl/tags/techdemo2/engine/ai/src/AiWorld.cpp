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
#include "AiWorld.h"

using namespace rl;

AiWorld::AiWorld(void) : mObstacles()
{
//  create an obstacle as bounding box of the walkarea for npcs
//  this should be accessable through scripting, the Obstacles should have names
//  for easier access
	BoxObstacle* o = new BoxObstacle(25,50,25);
	o->setSeenFrom(AbstractObstacle::inside);
	o->setPosition(Vec3(-40.0f,-10.0f, 0.0f));
	o->setForward(0,0,-1);
	addObstacle(o);

	o = new BoxObstacle(2,50,2);
	o->setSeenFrom(AbstractObstacle::outside);
	o->setPosition(Vec3(-31.5f,-10.0f, -3.5f));
	o->setForward(0,0,-1);
	addObstacle(o);

}

AiWorld::~AiWorld(void)
{
	removeAllObstacles();
}

void AiWorld::addObstacle(Obstacle* obstacle)
{
	mObstacles.push_back(obstacle);	
}

ObstacleGroup AiWorld::getSteeringObstacles()
{
	return mObstacles;
}

void AiWorld::removeAllObstacles()
{
	ObstacleIterator itr = mObstacles.begin();
	for(; itr != mObstacles.end(); ++itr)
	{
		delete (*itr);
	}
	mObstacles.clear();
}