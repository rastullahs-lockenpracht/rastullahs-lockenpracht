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
#include "stdinc.h" //precompiled header

#include "AStarNodePool.h"
#include "AStarWayPointNode.h"

namespace rl {

AStarNodePool::AStarNodePool()
{
}

AStarNodePool::~AStarNodePool()
{
	removeAll();
}

void AStarNodePool::removeAll()
{
	removeListContents(mUsed);
	removeListContents(mFree);
}

void AStarNodePool::releaseAll()
{
	AStarNodeList::iterator it;
	for (it = mUsed.begin(); it != mUsed.end(); it++)
	{
		mFree.push_back( (*it) );
	}
	mUsed.clear();
}

void AStarNodePool::release(AStarWayPointNode* Node)
{
	AStarNodeList::iterator it;
	for (it = mUsed.begin(); it != mUsed.end(); it++)
	{
		if ( (*it) == Node )
		{
			mUsed.erase(it);
			mFree.push_back(Node);
		}
	}
}

void AStarNodePool::remove( AStarWayPointNode* Node )
{
	AStarNodeList::iterator it;
	for (it = mUsed.begin(); it != mUsed.end(); it++)
	{
		if ( (*it) == Node )
		{
			mUsed.erase(it);
			delete Node;
		}
	}
}

AStarWayPointNode* AStarNodePool::createAStarWayPointNode(const WayPointNode* WP)
{
	AStarWayPointNode* Node;

	if ( mFree.empty())
		Node = new AStarWayPointNode(WP);
	else
	{
		Node = mFree.back();
		mFree.pop_back();
	}

	mUsed.push_back(Node);

	return Node;
}

void AStarNodePool::removeListContents(AStarNodeList& List)
{
	AStarNodeList::iterator it;
	for (it = List.begin(); it != List.end(); it++)
	{
		delete (*it);
	}
	List.clear();
}

AStarWayPointNode* AStarNodePool::getAt(unsigned int Index)
{
	return mUsed[Index];
}


unsigned int AStarNodePool::getSize()
{
	return mUsed.size();
}

};
