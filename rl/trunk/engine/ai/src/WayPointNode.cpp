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
#include "WayPointNode.h"

using namespace std;

namespace rl {

WayPointNode::WayPointNode(const Ogre::Vector3& pos, const WayPointNodeType type)
: mPosition(pos),
mType(type)
{
}

WayPointNode::~WayPointNode()
{
	mNeighbours.clear();
}

void WayPointNode::addNeighbour(const WayPointNode* neighbour) 
{
	WayPointWeightNodeList::iterator it;

	// test if neighbour not already added
	for (it = mNeighbours.begin(); it != mNeighbours.end(); it++) {
		// already added
		if ( (*it).second == neighbour ) 
			return;
		
	}

	// calculate cost to neighbour
	Ogre::Vector3 distanceVector = getPosition() - neighbour->getPosition();
	Ogre::Real distance = distanceVector.length();

	// add neighbour
	mNeighbours.push_back(WayPointWeightEdge(distance, neighbour));
}

const WayPointNode::WayPointWeightNodeList& WayPointNode::getNeighbours() const
{
	return mNeighbours;
}

Ogre::Vector3 WayPointNode::getPosition() const
{
	return mPosition;
}

WayPointNode::WayPointNodeType WayPointNode::getType() const
{
	return mType;
}

};
