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
#ifndef __RlAI_AStarWayPointNode_H__
#define __RlAI_AStarWayPointNode_H__

#include "AiPrerequisites.h"

namespace rl
{
	
	class WayPointNode;

	/** This class used as a node for creating the a* search graph.
	 * It is tightly linked with the real map representation of waypoints which
	 * is done with WayPointNode - hence its name is AStarWayPointNode to express
	 * that tight link.
	 */
	class _RlAiExport AStarWayPointNode
	{
	public:

		/** Constructor for an AStar Node
		 * AStarWayPointNode is not derived from WayPointNode, because doubling
		 * information that present at some other place is not necessary and 
		 * just makes it difficult to the if it's accurate at the moment.
		 * Therefore only a pointer to the waypoint, this Astar node is linked
		 * to, is stored.
		 * @param WP is a WayPointNode object storing, position and neigbours.
		 */
		AStarWayPointNode(const WayPointNode* WP);

		/** explicit virtual destructor
		 */
		virtual ~AStarWayPointNode();

		/** 2 nodes are compared by comparing their f value.
		 * The f value of a node is the sum of g and h value. (really needed?)
		 */
		inline bool operator < ( const AStarWayPointNode& wp2 ) //, const AStarWayPointNode& wp2 )
		{
			return (mG+mH) < (wp2.mG+wp2.mH);
		    //return (wp1->mG+wp1->mH) < (wp2->mG+wp2->mH);
		}
		
        /** 2 nodes compared for equilibrium by comparing their values.
		 * This is needed, because operator overloading for pointer types
         * does not work.
         * @param wp pointer to an AStarWayPointNode
		 */
		inline bool EqualPosition ( const AStarWayPointNode* wp )
		{
			return (mWP == wp->mWP); //&& (mG == wp->mG) && (mH == wp->mH);
		}

		/** 2 nodes compared for equilibrium by comparing their values.
		 * (really needed?) 
		 */
		static inline bool EqualPosition ( const AStarWayPointNode* wp1 , const AStarWayPointNode* wp2 )
		{
			return (wp1->mWP == wp2->mWP);
		}

		/** 2 nodes compared by comparing their f value.
		 * The f value of a node is the sum of g and h value.
		 * This function is used when sorting a stl::vector.
         * It can be fine tune extended so that binary_search
         * will work, by addind functionality so that when wp1->F == wp2->F
         * a finer sort mechanism kicks in.
         * sorting for G is not enough then.
         * a sort for H is not needed then, because H depends on G and
         * will just be reverse proportional to G. Therefore we just need
         * to sort either for G or H .
		 */
		static bool SortMethod(const AStarWayPointNode* wp1, const AStarWayPointNode* wp2)
		{
            //float wp1F = wp1->mG+wp1->mH;
            //float wp2F = wp2->mG+wp2->mH;
            //if (wp1F == wp2F)
            //    return wp1->mG > wp2->mG;

            //return wp1F > wp2F;
			return (wp1->mG+wp1->mH) > (wp2->mG+wp2->mH);
		}

		/** fetches the G value (cost til now)
		 * @returns the cost value up to this waypoint
		 */
		inline float getG() const {return mG;}
		/** fetches the H value (cost heuristic from now to goal)
		 * @returns the cost value from this waypoint to the goal
		 */
		inline float getH() const {return mH;}
		/** fetches the Parent of this node
		 * @returns the parent of this node in the search path
		 */
		inline AStarWayPointNode* getParent() const {return mParent;}
		/** returns the attached Waypoint node
		 * Is necessary to generate the connection to the graph of nodepoints
		 */
		inline const WayPointNode* getWP() const {return mWP;}

		/** fetches the G value (cost til now)
		 * @returns the cost value up to this waypoint
		 */
		inline void setG(float G) {mG=G;}
		/** fetches the H value (cost heuristic from now to goal)
		 * @returns the cost value from this waypoint to the goal
		 */
		inline void setH(float H) {mH=H;}
		/** fetches the Parent of this node
		 * @returns the parent of this node in the search path
		 */
		inline void setParent(AStarWayPointNode* Parent) {mParent=Parent;}

	protected:
		//! the positional WayPoint
		const WayPointNode* mWP;
		//! the real costs up to this node
		float mG;
		//! the estimated costs to the goal
		float mH;
		//! the parent of this node in the search
		AStarWayPointNode* mParent;
	};

}; // namespace

#endif
