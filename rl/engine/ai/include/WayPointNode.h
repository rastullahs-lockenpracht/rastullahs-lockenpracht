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
#ifndef __RlAI_WayPointNode_H__
#define __RlAI_WayPointNode_H__

#include "AiPrerequisites.h"

namespace rl
{

	/* class storing a single WayPoint.
	 * It contains the position of the waypoint and connections to its
	 * neighbours along with the real cost of travel.
	 */
	class _RlAiExport WayPointNode
	{
	public:
		//! defines the different types of terrain
		enum WayPointNodeType 
		{
			WP_UNKNOWN = 0,			//!< initial type - shouldn't be used
			WP_EXTERIOR = 1 << 0,	//!< when the node is 'outdoors'
			WP_INTERIOR = 1 << 1,	//!< when the node is inside (a building)
		};
		//! defines a weighted edge with a distance weigth
		typedef std::pair<Ogre::Real, const WayPointNode*> WayPointWeightEdge;
		//! defines a list of weighted 'edges' connecting to other nodes
		typedef std::list<WayPointWeightEdge> WayPointWeightNodeList;

		/** Creates a new waypoint at the given position.
		 * @param pos Position of the Waypoint
		 * @param type terrain type of Waypoint \see WayPointNodeType
		 */
		WayPointNode(const Ogre::Vector3& pos, const WayPointNodeType type);
		/** explicit virtual destructor
		 */
		virtual ~WayPointNode();

		/** adds other WayPointNodes as neighbours
		 * @param neighbour WayPointNode to add as a neighbour
		 */
		void addNeighbour(const WayPointNode* neighbour);

		/** fetches the list of neighbour nodes
		 * 
		 */
		const WayPointWeightNodeList& getNeighbours() const;

		/** retrieve the position of the WayPointNode
		 * @returns the position of the WayPoint in world space coordinates
		 */
		Ogre::Vector3 getPosition() const;

		/** retrieve the type of the WayPointNode
		 * @returns the type of the WayPoint \see WayPointNodeType
		 */
		WayPointNodeType getType() const;

	protected:
		/** calculates the distance to the neighbour in 3D world space.
		 * Is used internally when adding a node and calculates the euclidean
		 * distance. 
		 * At the moment terrain types etc. are ignored here.
		 * But this would be the place 
		 * @param neighbour WayPointNode to calculate the movement cost to.
		 */
		Ogre::Real calcCost(WayPointNode* neighbour);

		//! list of neighbour WayPointNodes
		WayPointWeightNodeList mNeighbours;
		//! 3D position of this WayPointNode in world coordinates
		Ogre::Vector3 mPosition;
		//! defines the type of the Waypoint \see WayPointNodeType.
		WayPointNodeType mType;
	};

};

#endif
