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
#ifndef __RlAI_WayPointGraphManager_H__
#define __RlAI_WayPointGraphManager_H__

#include "AiPrerequisites.h"

namespace rl
{
	class WayPointGraph;

	/** Manager for WayPointGraphs.
	 * Organizes the management of WayPointGraphs.
	 * The really sexy thing here is that enhancing the whole
	 * waypoint finding can be done right in this class by extending it
	 * with a grid like algorithm storage for waypointgraphs. That would
	 * most likely reduce searching for a waypoint drastically - _if_ there
	 * are many waypoints. At the moment this not something to consider.
	 */
	class _RlAiExport WayPointGraphManager 
		: public Ogre::Singleton<WayPointGraphManager>
	{

	public:
		/** Constructor
		 */
		WayPointGraphManager();
		/** explicit virtual destructor
		 */
		virtual ~WayPointGraphManager();

		/** Creates an WayPointGraph.
		 * WayPointGraphManager is responsible for memory management of
		 * the WayPointGraph
		 */
		WayPointGraph* createWayPointGraph();

		/** Removes all registered WayPointGraphs and deletes their objects.
		 * Clears all internal lists and deallocates the memory of the 
		 * stored objects.
		 */
		void removeAllWayPointGraphs();

	protected:
		//! defines a std::list of Graphs
		typedef std::vector<WayPointGraph*> WayPointGraphList;

		//! list of stored graphs
		WayPointGraphList mGraphs;
	};
};

#endif
