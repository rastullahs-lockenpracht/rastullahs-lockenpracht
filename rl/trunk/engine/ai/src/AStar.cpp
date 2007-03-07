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
#include "AStar.h"
#include "AStarWayPointNode.h"
#include "AStarHeuristic.h"
#include "AStarCosts.h"
#include "AStarNodePool.h"
#include "WayPointGraph.h"
#include "WayPointNode.h"
#include "CoreSubsystem.h"
#include "World.h"
#include "LineSetPrimitive.h"
#include <algorithm>

using namespace std;

namespace rl {

AStarStatistics::AStarStatistics()
: mNodesSearched(0),
  mNodesAdded(0),
  mNodesRemoved(0),
  mNodesVisited(0),
  mNodesLeft(0),
  mPathLength(0),
  mPathCost(0)
{
}

void AStarStatistics::reset()
{
  mNodesSearched = 0;
  mNodesAdded = 0;
  mNodesRemoved = 0;
  mNodesVisited = 0;
  mNodesLeft = 0;
  mPathLength = 0;
  mPathCost = 0;
}

AStar::AStar( const AStarCosts* Costs, const WayPointGraph* WPGraph )
    : mCosts(Costs),
	mWPGraph(WPGraph),
	mStartPos(0,0,0),
	mEndPos(0,0,0),
	mANStart(NULL),
	mANEnd(NULL),
    mDebugAstar(false)
{
}

AStar::AStar( const AStarCosts* Costs, const WayPointGraph* WPGraph,
			const Ogre::Vector3& StartPos, const Ogre::Vector3& EndPos )
	: mCosts(Costs),
	mWPGraph(WPGraph),
	mStartPos(StartPos),
	mEndPos(EndPos),
	mANStart(NULL),
	mANEnd(NULL),
    mDebugAstar(false)
{
}

AStar::~AStar()
{
	reset();
}

void AStar::reset()
{
	//mAStarNodePool.clearall();
	mANStart = NULL;
	mANEnd = NULL;

	AStarSet::iterator it;
	for (it = mOpen.begin(); it != mOpen.end(); it++)
	{
		delete (*it);
	}
	for (it = mClosed.begin(); it != mClosed.end(); it++)
	{
		delete (*it);
	}
    mOpen.clear();
    mClosed.clear();
}

void AStar::initialise()
{
	// drop everything already allocated
	// perhaps it's wise to first have a look where we're going to head ?
	// and decide it then depending on the destionation ?
	reset();

	// first find 'real' start and end positions by searching for the corresponding waypoints
	mANStart = new AStarWayPointNode( mWPGraph->getNearestWayPoint(mStartPos) );
	mANEnd = new AStarWayPointNode( mWPGraph->getNearestWayPoint(mEndPos) );
}

void AStar::searchFromTo(AStarPath& resultPath, const Ogre::Vector3& StartPos,
						 const Ogre::Vector3& EndPos )
{
	mStartPos = StartPos;
	mEndPos = EndPos;
	search(resultPath);
}

/*
The A* algorithm, stripped of all the code, is fairly simple. 
There are two sets, OPEN and CLOSED. The OPEN set contains those nodes that are candidates for examining.
Initially, the OPEN set contains just one element: the starting position.
The CLOSED set contains those nodes that have already been examined.
Initially, the CLOSED set is empty. Graphically, the OPEN set is the "frontier"
and the CLOSED set is the "interior" of the visited areas. Each node also keeps
a pointer to its parent node so that we can determine how it was found.

There is a main loop that repeatedly pulls out the best node n in OPEN
(the node with the lowest f value) and examines it. If n is the goal, then we're done.
Otherwise, node n is removed from OPEN and added to CLOSED. 
Then, its neighbors n' are examined. A neighbor that is in CLOSED has already
been seen, so we don't need to look at it (*). A neighbor that is in OPEN is
scheduled to be looked at, so we don't need to look at it now (*). Otherwise,
we add it to OPEN, with its parent set to n. The path cost to n', g(n'), will
be set to g(n) + movementcost(n, n').

(*) I'm skipping a small detail here. You do need to check to see if the
node's g value can be lowered, and if so, you re-open it.
*/

void AStar::search(AStarPath& resultPath)
{
	initialise();
	mOpen.push_back(mANStart);
	mANStart->setH(mCosts->calcHeuristic(mWPGraph,mANStart,mANEnd));
    mANStart->setG(0);

	AStarWayPointNode* Node;

	// While there are still unvisited nodes
    while( !mOpen.empty() )
	{
        Node = mOpen.front();
		pop_heap(mOpen.begin(), mOpen.end(), AStarWayPointNode::SortMethod);
		mOpen.pop_back();

		mClosed.push_back(Node);

		// check if goal reached
        if ( Node->EqualPosition(mANEnd) ) {
			// remember new 'end'
            delete mANEnd;
            mANEnd = Node;
            // create result
            for (; Node; Node = Node->getParent())
			{
				resultPath.push_back(Node->getWP()->getPosition());
			}
            createPrimitive(resultPath);
			// terminate search
			break;
		}

		// now look through the neighbours
		WayPointNode::WayPointWeightNodeList WPList = Node->getWP()->getNeighbours();
		WayPointNode::WayPointWeightNodeList::iterator it;

		for (it = WPList.begin(); it != WPList.end(); it++)
		{
			AStarWayPointNode* ASubNode = new AStarWayPointNode((*it).second);
			float cost = mCosts->calcCost(mWPGraph,ASubNode,Node);

			ASubNode->setG( Node->getG() + cost );
			ASubNode->setH( mCosts->calcHeuristic(mWPGraph,ASubNode,mANEnd) );

			AStarSet::iterator ASit;
			// now check if the node is already in Open
			
			if ( (ASit = searchSet(mOpen, ASubNode)) != mOpen.end() ) 
			{
				AStarWayPointNode* ASfound = (*ASit);
				// check if current g is better
				if (ASfound->getG() > ASubNode->getG()) {
					ASfound->setG(ASubNode->getG());
					ASfound->setH(ASubNode->getH());
					ASfound->setParent(Node);
					// dark magic ... for resorting the heap from the top
					// down to the changed element
					push_heap( mOpen.begin(), ASit+1, AStarWayPointNode::SortMethod );
				}
				// anyway the newly created node isn't needed
				delete ASubNode;
			
			}  // now check if the node is already in Closed
			else if ( (ASit = searchSet(mClosed, ASubNode)) != mClosed.end()) 
			{
				AStarWayPointNode* ASfound = (*ASit);
				// (check if current g is better, then reopen)
				if (ASfound->getG() > ASubNode->getG()) {
					ASfound->setG(ASubNode->getG());
					ASfound->setH(ASubNode->getH());
					ASfound->setParent(Node);
					// remove the node from the closed list
					mClosed.erase(ASit);

					// dark magic ... for resorting the heap from the top
					// down to the changed element
					push_heap( mOpen.begin(), mOpen.end(), AStarWayPointNode::SortMethod );
				}
				// anyway the newly created node isn't needed
				delete ASubNode;
			} 
			else 
			{
				// neither in Open nor Closed, so add it to Open
				mOpen.push_back( ASubNode );
                push_heap( mOpen.begin(), mOpen.end(), AStarWayPointNode::SortMethod );
				ASubNode->setParent(Node);
			}
		}
	}

}

AStar::AStarSet::iterator AStar::searchSet(AStarSet& Set, AStarWayPointNode* Node)
{
	AStarSet::iterator it;

    /* try binary search - but keep in mind this compares the F and not G&H values 
     * so this is not going to work at the moment with the current sortMethod ...
     */
    
    //AStarWayPointNode* help = NULL;
    //bool found = std::binary_search(Set.begin(), Set.end(), Node, AStarWayPointNode::SortMethod);
    //while (it != Set.end())
    //{
    //    if ( (*it)->Equal(Node) ) // if equal, then Node was 'found'
    //        break;
        // otherwise continue binary search
        //it = std::binary_search(it, Set.end(), Node, AStarWayPointNode::SortMethod);
    //}
    
    
	for (it = Set.begin(); it != Set.end(); it++) 
	{
        if ( (*it)->EqualPosition(Node) )
			return it;
	}

	return it;
}

void AStar::createPrimitive(const AStarPath& searchPath)
{
    // if there is no line primitive just skip this here
    if (!mPrimitive)
        return;

    AStarPath::const_iterator it = searchPath.begin();
    AStarPath::const_iterator pit;
	LineSetPrimitive* lineSet = static_cast<LineSetPrimitive*>(mPrimitive);

	lineSet->clear();
    if (it == searchPath.end())
        return;

    Ogre::Vector3 From;
    Ogre::Vector3 To;
    for (it++ ; it != searchPath.end(); it++) 
    {
        pit = it - 1;
        From = (*pit) + Ogre::Vector3(0,0.5,0);
        To = (*it) + Ogre::Vector3(0,0.5,0);

        lineSet->addLine(From, To, Ogre::ColourValue::Green);
    }
}

DebugVisualisableFlag AStar::getFlag() const
{
	return DVF_WAYPOINT;
}

void AStar::updatePrimitive()
{
	if (mSceneNode->getParent() == NULL)
    {
		//mActor = ActorManager::createEmptyActor("__Astar_Debugger__");
		CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->
			getRootSceneNode()->addChild(mSceneNode);
    }
    // no update here because updates are only necessary when a search
    // has finished
}

void AStar::doCreatePrimitive()
{
	mPrimitive = new LineSetPrimitive();
}

};
