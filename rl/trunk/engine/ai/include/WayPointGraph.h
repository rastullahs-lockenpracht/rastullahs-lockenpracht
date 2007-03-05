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
#ifndef __RlAI_WayPointGraph_H__
#define __RlAI_WayPointGraph_H__

#include "AiPrerequisites.h"
#include "WayPointNode.h"
#include "DebugVisualisable.h"
#include "OgreVector3.h"


namespace rl
{

	/** Stores a waypointgraph and is capable of loading and saving it.
	 */
	//template <class NodeType>
	class _RlAiExport WayPointGraph 
		: public DebugVisualisable
	{
	public:
		//! defines a list of waypoint nodes
		typedef std::vector< WayPointNode* > WayPointNodeList;

		/* default constructor
		 */
		WayPointGraph();
		/* explicit virtual destructor.
		 * deletes all stored WayPointNodes
		 */
		virtual ~WayPointGraph();

		/** adds a new waypoint at the given position to the graph.
		 * Prohibits adding waypoints more than once by ignoring add 
		 * requests for them and simply returning NULL for them.
		 * @param pos Position of the Waypoint
		 * @param type terrain type of Waypoint \see WayPointNodeType
		 * @returns WayPointNode that was newly added (if it wasn't added, returns NULL)
		 */
		WayPointNode* addWayPoint(const Ogre::Vector3& position, const WayPointNode::WayPointNodeType type);


		/** adds a connection from one Waypoint to an other.
		 * The added connection is bidirectional.
		 * @param wp1 Waypoint edge first waypoint
		 * @param wp2 Waypoint edge second waypoint
		 */
		void addConnection(WayPointNode* wp1, WayPointNode* wp2);

		/** adds a directed connection from one WayPoint to an other.
		 * The added connection is only one direction.
		 * @param wp1 Waypoint edge origin
		 * @param wp2 Waypoint edge destination
		 */
		void addDirectedConnection(WayPointNode* wp1, const WayPointNode* wp2);


		/** loads the Waypoints from a file.
		 * @param filename of the file to load.
		 */
		void load (const Ogre::String& filename);
		/** saves the Waypoints to a file.
		 * Structure of the file is as follows:
		 * Header\n
		 * numberOfWaypoints\n
		 * waypoint1.x waypoint1.y waypoint1.z waypoint2.x ...\n
		 * 4 1 2 3 4\n
		 * ...
		 * \n
		 * \n
		 * 
		 * As you can see the waypoint coordinates are written consecutivily on one line
		 * and the connections of one waypoint to the others are written per waypoint on one line.
		 * The first number gives the number of connections and the rest are the indices of the
		 * waypoints the connections have to be made to.
		 * If the line is empty that means that the waypoint has got not other waypoints to connect to
		 * (very unlikely)
		 * the file is terminated either with end of file or two newlines.
		 *
		 * @param filename of the file to save to.
		 */
		void save (const Ogre::String& filename) const;

		/** retrieves the nearest WayPoint for the given position
		 * @param position in world space whose nearest waypoint has to be found.
		 * @returns WayPointNode pointer to a waypoint object
		 */
		const WayPointNode* getNearestWayPoint(const Ogre::Vector3& position) const;

		/** retrieves the WayPoint at the specified index from the list
		 * @param index of the waypoint to fetch.
		 */
		const WayPointNode* getWayPointAt(unsigned int index) const;

		/** fetches all stored WayPoints
		 * @returns a const iterator for the list of the stored waypoints
		 */
		const WayPointNodeList& getWayPointList() const;

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
	
		/** adds a new waypoint at the given position to the graph.
		 * Doesn't test if there is already a Waypoint for the specified position
		 * @param pos Position of the Waypoint
		 * @param type terrain type of Waypoint \see WayPointNodeType
		 */
		WayPointNode* rawAddWayPoint(const Ogre::Vector3& position, const WayPointNode::WayPointNodeType type);

		/** root node of the waypoint graph.
		 * Note that there is no up or down in this graph, because it is a 
		 * cyclic, directed graph.
		 */
		WayPointNode* mRoot;

		//! to ease handling of the WayPointTree we also store a list of nodes
		WayPointNodeList mNodeList;
	};

	/*
	class WayPointGraphDebugger
	{
		WayPointGraphDebugger();
		~WayPointGraphDebugger();

		void init( Ogre::SceneManager* smgr );
		void deInit();
		void showLines( WayPointGraph* graph );
		void hideLines();
	};
	*/
};

#endif