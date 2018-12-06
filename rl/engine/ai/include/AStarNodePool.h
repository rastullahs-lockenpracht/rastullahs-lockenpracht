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
#ifndef __RlAI_AStarNodeManager_H__
#define __RlAI_AStarNodeManager_H__

#include "AiPrerequisites.h"
#include "AStarWayPointNode.h"

namespace rl {

	/** A class for the memory management of Nodes for the A Star.
	 * Internally it keeps a list of nodes and just remembers which
	 * are used externally and which are not. So alloc/dealloc doesn't
	 * happen that often when throwing away nodes.
	 */
	class _RlAiExport AStarNodePool
	{
		//! typedefinition for a list of AStar nodes
		typedef std::vector<AStarWayPointNode *> AStarNodeList;

		/** default constructor
		 */
		AStarNodePool();
		/** explicit destructor
		 */
		~AStarNodePool();

		/** fetches a AStarWayPointNode for use
		 * @param WP WayPointNode to save in the AStarWayPointNode
		 */
		AStarWayPointNode* createAStarWayPointNode( const WayPointNode* WP );

		/** releases a AStarWayPointNode from use
		 * @param Node the node to release
		 */
		void release( AStarWayPointNode* Node );

		/** releases all AStarWayPointNodes from use
		 */
		void releaseAll();

		/** removes a single AStarWayPointNode (delete)
		 */
		void remove( AStarWayPointNode* Node );

		/** removes all AStarWayPointNodes (delete)
		 */
		void removeAll( );

		/** retrieves the AStarWayPointNode at the specified Index.
		 * Retrieval is conducted on the list of nodes in use. (mUsed)
		 * @param Index to retrieve AStarWayPointNode at.
		 */
		AStarWayPointNode* getAt(unsigned int Index);

		/** retrieves the number of AStarWayPointNodes available for retrieval.
		 * Is necessary to avoid throwing of an error
		 * @returns unsigned int giving the number of elements stored in mUsed.
		 */
		unsigned int getSize();

	protected:
		/** helper function to remove all contents of a AStarNodeList
		 */
		void removeListContents(AStarNodeList& List);

		//! list of nodes in use
		AStarNodeList mUsed;
		//! list of nodes not yet in use
		AStarNodeList mFree;

	};

};

#endif
