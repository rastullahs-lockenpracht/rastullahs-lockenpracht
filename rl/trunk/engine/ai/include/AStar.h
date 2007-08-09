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
        /** default constructor
         */
        AStarStatistics();

        /** resets all variables to zero
         */
        void reset();

        int mNodesSearched;
        int mNodesAdded;
        int mNodesRemoved;
        int mNodesVisited;
        int mNodesLeft;
        int mPathLength;
        int mPathCost;
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
         * This is a convenience constructor so that there is no need to do
         * a search right from the start, but can be done sometime later.
		 * @param Costs encapsulates cost and heuristic for distance calculations.
		 * @param WPGraph contains the map of WayPoints to examine for a path.
         */
        AStar( const AStarCosts* Costs, const WayPointGraph* WPGraph );

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
         * The A* utilises AStarCost and searches for a path by building
         * a A* node graph to the goal. Afterwards the resultpath is
         * extracted by traversing the graph from the goal to the start.
         * This actually creates a _reverse_ path to the goal.
         * @param resultPath contains the researched path in reverse order.
		 */
		void search(AStarPath& resultPath);

		/** starts the search for the given start and end positions
         * @param resultPath contains the researched path in reverse order.
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

        /** creates the primitive from the searchpath
         */
        virtual void createPrimitive(const AStarPath& searchPath);

		// End of Overrides from DebugVisualisable

		/** sets up everyting needed for the search.
		 */
		void initialise();

		/** search for a AStarWayPointNode in the specified set
		 * @param Set to search through
		 * @param Node to search for
		 */
		AStarSet::iterator searchSet(AStarSet& Set, AStarWayPointNode* Node);

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

        /** debug variable - prevents regenerating the lineprimitive.
         * When nothing has changed this is set to false and prevents
         * the regeneration
         */
        //bool mChanged;

        //! to be able to turn debug on/off per AStar object
        bool mDebugAstar;

	};
	
}; // namespace

#endif
