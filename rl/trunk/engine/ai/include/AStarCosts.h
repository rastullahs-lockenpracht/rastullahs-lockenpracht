/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#ifndef __RlAI_AStarCosts_H__
#define __RlAI_AStarCosts_H__

#include "AiPrerequisites.h"
#include "AStarHeuristic.h"

namespace rl {

	class WayPointGraph;
	class AStarWayPointNode;

	/** Abstract cost class definining an interface for AStar to access.
	 * Contains the heuristic to calculate the distance cost to the goal and
	 * the calculation of the real costs from one node to an other.
	 */
	class _RlAiExport AStarCosts
	{
	public:
		/** default constructor
		 * @param Heuristic to use for calculating heuritic values
		 */
		AStarCosts(const AStarHeuristic* Heuristic, float TieBreakFactor = 1.0f);
		/** explicit virtual destructor
		 */
		virtual ~AStarCosts();

		/** calculates the (remaining) distance between wp1 and wp2.
		 * @param WPGraph contains a WayPointGraph that knows the Waypoints to the given nodes
		 * @param wp1 start AStarWayPoint
		 * @param wp2 goal AStarWayPoint
		 * @returns a float giving the result of the heuristic distance calculation
		 */
		virtual float calcHeuristic(const WayPointGraph* WPGraph, const AStarWayPointNode* wp1,
			const AStarWayPointNode* wp2) const;

		/** calculates the real cost between wp1 and wp2.
		 * @param WPGraph contains a WayPointGraph that knows the Waypoints to the given nodes
		 * @param wp1 start AStarWayPoint
		 * @param wp2 goal AStarWayPoint
		 * @returns a float giving the result of the real cost calculation
		 */
		virtual float calcCost(const WayPointGraph* WPGraph, const AStarWayPointNode* wp1,
			const AStarWayPointNode* wp2) const = 0;

	protected:
        //! stores the heuristic used to estimate the distance to the goal
		const AStarHeuristic* mHeuristic;
        //! a tie break factor that is multiplied with the heuritics result
        float mTieBreakFactor;
	};
	
	/* An implementation of the abstract interface of AStarCosts.
	 * currently utilises euklidean distance as a method for calculating
	 * the real costs.
	 * Of course there is a lot of tuning that can be done here.
	 */
	class _RlAiExport AStarCostsDefault
		: public AStarCosts
	{
    public:
		/** default constructor
		 * @param Heuristic to use for calculating heuritic values
		 */
		AStarCostsDefault(const AStarHeuristic* Heuristic, float TieBreakFactor = 1.0f);
		/** explicit virtual destructor
		 */
		virtual ~AStarCostsDefault();
		/** calculates the real cost between wp1 and wp2.
		 * The calculation in place is planely reporting the euclidean
		 * distance.
		 * @param WPGraph contains a WayPointGraph that knows the Waypoints to the given nodes
		 * @param wp1 start AStarWayPoint
		 * @param wp2 goal AStarWayPoint
		 * @returns a float giving the result of the real cost calculation
		 */
		virtual float calcCost(const WayPointGraph* WPGraph, const AStarWayPointNode* wp1,
			const AStarWayPointNode* wp2) const;

	protected:
		EuclideanDistance mEuclid;
	};

};  //namespace

#endif
