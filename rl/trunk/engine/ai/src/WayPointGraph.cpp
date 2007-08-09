/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */
#include "stdinc.h" //precompiled header

#include "WayPointGraph.h"
#include "WayPointNode.h"
#include <algorithm>
#include "OgreVector3.h"
#include "CoreSubsystem.h"
#include "World.h"
#include "Exception.h"
#include "LineSetPrimitive.h"
#include "ConfigurationManager.h"

using namespace std;

namespace rl {

WayPointGraph::WayPointGraph()
: mRoot(NULL),
  mChanged(false)
{
}

WayPointGraph::~WayPointGraph()
{
	WayPointNodeList::iterator it;

	for (it = mNodeList.begin(); it != mNodeList.end(); it++)
	{
		delete *it;
	}
	// the scenenode seems
	// already deleted when this destructor gets called.

	// Remove debug scene node from root scene note, if debugview was used.
    if (mSceneNode != NULL && mSceneNode->getParent() != NULL)
    {
		CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->
			getRootSceneNode()->removeChild(mSceneNode);
    }
}

WayPointNode* WayPointGraph::addWayPoint(const Ogre::Vector3& position, const WayPointNode::WayPointNodeType type)
{
	WayPointNodeList::iterator it;

	for (it = mNodeList.begin(); it != mNodeList.end(); it++)
	{
		// equal to some waypoint already added ...
		if ( (*it)->getPosition() == position )
			return NULL;
	}

	return rawAddWayPoint(position, type);

}

WayPointNode* WayPointGraph::rawAddWayPoint(const Ogre::Vector3& position, const WayPointNode::WayPointNodeType type)
{
	WayPointNode* newWayPoint = new WayPointNode(position, type);
	mNodeList.push_back(newWayPoint);
	mChanged = true;

	return newWayPoint;
}

void WayPointGraph::addConnection(WayPointNode* wp1, WayPointNode* wp2)
{
	wp1->addNeighbour(wp2);
	wp2->addNeighbour(wp1);
	mChanged = true;
}

void WayPointGraph::addDirectedConnection(WayPointNode* wp1, const WayPointNode* wp2)
{
	wp1->addNeighbour(wp2);
	mChanged = true;
}

void WayPointGraph::load (const Ogre::String& filename)
{
	// fetch modules directory
	Ogre::String modulespath(
		ConfigurationManager::getSingleton().getModulesRootDirectory());
	Ogre::String fullfilename(modulespath + "/" + filename);

	// check if the file exists
	if (! boost::filesystem::exists(boost::filesystem::path(fullfilename)) )
		Throw(FileNotFoundException, fullfilename);

	std::ifstream input(fullfilename.c_str(), ios::binary);

	if (input.fail())
		Throw(Error, filename+": couldn't open");
	if (input.eof())
		Throw(Error, filename+": premature end of file");

	std::string MagicHeader;
	input >> MagicHeader;
	if (MagicHeader != "RLWayPointGraph")
		Throw(WrongFormatException, filename);

	if (input.eof())
		Throw(Error, filename+": premature end of file");

	char line[20];
	input.getline(line, sizeof(line));

	WayPointNodeList::size_type numberOfNodes;
	input.read((char *) &numberOfNodes, sizeof(numberOfNodes));

	//read the rest of the line (lineend)
	input.getline(line, sizeof(line));

	// read in the points
	Ogre::Vector3 Position;
	WayPointNode::WayPointNodeType type;
	unsigned int count = 0;
	while (!input.eof() && count < numberOfNodes)
	{
		input.read((char *) &Position, sizeof(Position));
		if (input.eof())
			Throw(Error, filename+": fileend while reading waypoints");
		input.read((char *) &type, sizeof(type));
		if (input.eof())
			Throw(Error, filename+": fileend while reading waypoints");
		rawAddWayPoint(Position, type);
		count++;
	}
	if (count != numberOfNodes)
		Throw(Error, filename+": didn't find as many WayPoints as expected.");

	// should be needed to read newline at end of waypoint line, but actually it's
	// not necessary (enabling it breaks the loading of the connections)
	//input.getline(line, sizeof(line));

	WayPointNode* FromNode;
	WayPointNode* ToNode;
	WayPointNode::WayPointWeightNodeList::size_type nrOfConnections;
	unsigned int index;

	// read in the connections
	count=0;
	while (!input.eof() && count < numberOfNodes)
	{
		input.read((char *) &nrOfConnections, sizeof(nrOfConnections));
		if (input.eof())
			Throw(Error, filename+": fileend while reading connections");

		for (unsigned int concount = 0; concount < nrOfConnections; concount++) {
			input.read((char *) &index, sizeof(index));
			FromNode = mNodeList[count];
			ToNode = mNodeList[index];
			FromNode->addNeighbour(ToNode);
		}
		count++;
		input.getline(line, sizeof(line));
	}
	if (count != numberOfNodes)
		Throw(Error, filename+": couldn't read expeced number of connections");

	input.close();
	mChanged = true;
}

void WayPointGraph::save (const Ogre::String& filename) const
{
	unsigned int count;
	WayPointNodeList::const_iterator it;
	std::map<const WayPointNode*, unsigned int> IndexList;

	// fetch modules directory
	Ogre::String modulespath(
		ConfigurationManager::getSingleton().getModulesRootDirectory());
	Ogre::String fullfilename(modulespath + "/" + filename);

	std::ofstream output(fullfilename.c_str(), std::ios::binary);
	// opening file for write failed
	if (output.fail())
		Throw (Error,filename + "couldn't open for writing");

	// write file magic header
	output << "RLWayPointGraph" << endl;
	// write number of waypoints
	WayPointNodeList::size_type numberOfNodes = mNodeList.size();
	output.write((char *) &numberOfNodes, sizeof(numberOfNodes));
	output << endl;

	// save the full list of points
	// and construct an index map for the nodes
	count = 0;
	Ogre::Vector3 Position;
	WayPointNode::WayPointNodeType type;
	for (it = mNodeList.begin(); it != mNodeList.end(); it++)
	{
		// write waypoint to file
		Position = (*it)->getPosition();
		type = (*it)->getType();
		output.write((char *) &Position, sizeof(Position));
		output.write((char *) &type, sizeof(type));
		// add waypoint pointer to the list with it's index
		IndexList.insert(pair<WayPointNode*, unsigned int>((*it),count));
		count++;
	}
	// additionally terminate list with newline
	cout << endl;

	// save the index-index pairs for the connections between the nodes
	count = 0;
	for (it = mNodeList.begin(); it != mNodeList.end(); it++)
	{
		const WayPointNode::WayPointWeightNodeList subnodes = (*it)->getNeighbours();
		WayPointNode::WayPointWeightNodeList::size_type subnodessize = subnodes.size();
		WayPointNode::WayPointWeightNodeList::const_iterator nit;
		std::map<const WayPointNode*, unsigned int>::iterator itIndex;

		output.write((char *) &(subnodessize), sizeof(subnodessize));
		// iteratte through the neighbours
		for (nit = subnodes.begin(); nit != subnodes.end(); nit++)
		{
			itIndex = IndexList.find( (*nit).second );
			output.write((char *) &((*itIndex).second), sizeof((*itIndex).second));
		}
		// terminate connection list for one node
		output << endl;

		count++;
	}
	output.close();
}

const WayPointNode* WayPointGraph::getNearestWayPoint(const Ogre::Vector3& position) const
{
	WayPointNodeList::const_iterator it;
	WayPointNode* nearestWayPoint = NULL;
	Ogre::Vector3 nearestVec;
	Ogre::Real nearestDistance;

	// if list is empty simply return no waypoint
	if (mNodeList.begin() == mNodeList.end())
		return NULL;

	// first waypoint is the nearest at the beginning
	it = mNodeList.begin();
	nearestVec = position - (*it)->getPosition();
	nearestDistance = nearestVec.length();
	nearestWayPoint = (*it);

	// search the full list for points that are nearer
	for (it = mNodeList.begin(); it != mNodeList.end(); it++)
	{
		// calculate distance
		nearestVec = position - (*it)->getPosition();

		// test if distance is smaller than the smallest seen until now
		if ( nearestDistance > nearestVec.length() )
		{
			nearestDistance = nearestVec.length();
			nearestWayPoint = (*it);
		}
	}

	return nearestWayPoint;
}

const WayPointGraph::WayPointNodeList& WayPointGraph::getWayPointList() const
{
	return mNodeList;
}

const WayPointNode* WayPointGraph::getWayPointAt(unsigned int index) const
{
	if (index >= mNodeList.size())
		Throw(OutOfRangeException,"no data at specified index");

	return mNodeList[index];
}


DebugVisualisableFlag WayPointGraph::getFlag() const
{
	return DVF_WAYPOINT;
}

void WayPointGraph::updatePrimitive()
{
	if (mSceneNode->getParent() == NULL)
    {
		CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->
			getRootSceneNode()->addChild(mSceneNode);
		//mCharacterActor->_getSceneNode()->addChild(mSceneNode);
    }

	// avoid building graph again and again
	if (! mChanged)
		return;

	LineSetPrimitive* lineSet = static_cast<LineSetPrimitive*>(mPrimitive);

	lineSet->clear();

	// list storing added lines (for avoiding drawing bidirectional lines twice)
	std::multimap<const WayPointNode*,const WayPointNode*> edgeList;
	std::multimap<const WayPointNode*,const WayPointNode*>::iterator edgeListIt;

	WayPointNodeList::const_iterator it;
	Ogre::Vector3 wp1Vec;
	Ogre::Vector3 wp2Vec;

	for (it = mNodeList.begin(); it != mNodeList.end(); it++)
	{
		wp1Vec = (*it)->getPosition();
		// draw the waypoint itself
		lineSet->addLine(wp1Vec, wp1Vec + Ogre::Vector3(0,1,0), Ogre::ColourValue::Red);

		const WayPointNode::WayPointWeightNodeList subnodes = (*it)->getNeighbours();
		WayPointNode::WayPointWeightNodeList::const_iterator nit;

		for (nit = subnodes.begin(); nit != subnodes.end(); nit++)
		{
			/*bool found = false;
			while ( (edgeListIt = edgeList.find( (*nit).second )) != edgeList.end() )
			{
				if ( (*edgeListIt).second == (*it) )
					found = true;
			}
			if (found)	// already 'drawn'
				continue;
				*/

			lineSet->addLine(wp1Vec, (*nit).second->getPosition(), Ogre::ColourValue::Blue);
			edgeList.insert(pair<const WayPointNode*,const WayPointNode*>((*nit).second, (*it)) );
		}
	}
	edgeList.clear();

	mChanged = false;
}

void WayPointGraph::doCreatePrimitive()
{
	mPrimitive = new LineSetPrimitive();
}

};
