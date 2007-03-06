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
#ifndef __RlAI_AStar_H__
#define __RlAI_AStar_H__

#include "AiPrerequisites.h"
#include "AStarWayPointNode.h"
#include "DebugVisualisable.h"
#include "OgreVector3.h"

namespace rl
{

	class WayPointGraph;
	class AStarCosts;

	/* Just some short notes to the ideas of amit:
	 * we're using heaps here for the open/close sets
	 * once we get beyond 800 waypoints, we can have a look at HOT buckets ...
	 */

	/** A class to get some statistical output about the AStar search.
	 */
	class AStarStatistics
	{
	public:
	};

	/** Stores a waypointgraph and is capable of loading and saving it.
	 */
	class _RlAiExport AStar 
		: public DebugVisualisable	
	{
	public:
		//! defines the type of the open/closed sets
		typedef std::vector< AStarWayPointNode* > AStarSet;
		//! defines the resultpath as a list of points in world space
		typedef std::vector< Ogre::Vector3 > AStarPath;

		/** The constructor of AStar
		 * AStar needs several things to find a path: 
		 * a heuristic to calculate the cost for the still not travelled part of the path.
		 * some sort of map to travel in.
		 * A start and and end point.
		 * @param Costs encapsulates cost and heuristic for distance calculations.
		 * @param WPGraph contains the map of WayPoints to examine for a path.
		 * @param StartPos is the start point in world space.
		 * @param StartPos is the end point in world space.
		 */
		AStar( const AStarCosts* Costs, const WayPointGraph* WPGraph,
			const Ogre::Vector3& StartPos, const Ogre::Vector3& EndPos );
		/** explicit virtual destructor
		 */
		~AStar();

		/** starts the search.
		 */
		void search(AStarPath& resultPath);

		/** starts the search for the given start and end positions
		 * @param StartPos gives the startpoint for the search
		 * @param EndPos gives the endposition for the search
		 */
		void searchFromTo(AStarPath& resultPath, const Ogre::Vector3& StartPos,
			const Ogre::Vector3& EndPos );

		/* retrieve a list of WayPoints (=the solution)
		 * This generates a path from the solution created by search
		 */
		//const AStarPath& getResultPointList() const;

		/** Resets the AStar to do an other search.
		 * Before searching can be done again all temporary data has to be
		 * deallocated.
		 */
		void reset();

		// Overrides from DebugVisualisable
		/** Provides the flag of the kind of visualisation done in the class.
		 * \see DebugVisualisable::DebugVisualisableFlag for further information
		 */
		virtual DebugVisualisableFlag getFlag() const;
		/** Called to update the debug primitive. Redrawing is done in here.
		 */
        virtual void updatePrimitive();

	protected:
		// Overrides from DebugVisualisable
		/** actualy creates the debug primitive.
		 * \see DebugVisualisable for further information
		 */
        virtual void doCreatePrimitive();

		// End of Overrides from DebugVisualisable

		/** sets up everyting needed for the search.
		 */
		void initialise();

		/** search for a AStarWayPointNode in the specified set
		 * @param Set to search through
		 * @param Node to search for
		 */
		AStarSet::iterator AStar::searchSet(AStarSet& Set, AStarWayPointNode* Node);

		// static data
		//! heuristic function
		const AStarCosts* mCosts;
		//! map (which is a waypoint graph)
		const WayPointGraph* mWPGraph;
		//! start position
		Ogre::Vector3 mStartPos;
		//! end position (to reach)
		Ogre::Vector3 mEndPos;
		//! the nearest waypoint to the start position
		AStarWayPointNode* mANStart;
		//! the nearest waypoint ot the end position
		AStarWayPointNode* mANEnd;

		typedef std::greater<AStarWayPointNode* > AStarNodeComp;

		// dynamic data (changed while searching)
		//! contains the OPEN set
		AStarSet mOpen;
		//! contains the CLOSED set
		AStarSet mClosed;

	};
	
}; // namespace

#endif