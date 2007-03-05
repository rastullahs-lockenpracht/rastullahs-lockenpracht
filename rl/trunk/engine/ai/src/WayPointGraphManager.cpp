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
#include "WayPointGraph.h"
#include "WayPointGraphManager.h"

using namespace std;
using namespace Ogre;

template<> rl::WayPointGraphManager* Singleton<rl::WayPointGraphManager>::ms_Singleton = 0;

namespace rl {

WayPointGraphManager& WayPointGraphManager::getSingleton(void)
{
	return Singleton<WayPointGraphManager>::getSingleton();
}

WayPointGraphManager* WayPointGraphManager::getSingletonPtr(void)
{
	return Singleton<WayPointGraphManager>::getSingletonPtr();
}

WayPointGraphManager::WayPointGraphManager()
{
}
WayPointGraphManager::~WayPointGraphManager()
{
	removeAllWayPointGraphs();
}

WayPointGraph* WayPointGraphManager::createWayPointGraph()
{
	WayPointGraph* Graph = new WayPointGraph();
	mGraphs.push_back(Graph);
	return Graph;
}

void WayPointGraphManager::removeAllWayPointGraphs()
{
	for(WayPointGraphList::iterator it = mGraphs.begin(); it != mGraphs.end(); it++)
	{
		delete (*it);
	}
	mGraphs.clear();
}

};