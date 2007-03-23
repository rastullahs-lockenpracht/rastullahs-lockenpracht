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
  
#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "CorePrerequisites.h"

namespace rl
{
	
/** A very basic quad tree template.
 * It just stores whether the current node is subdivided or not and
 * the data contained in the node.
 * Normally this would be used in a uniform flat quadtree. That means
 * that the quadtrees memory would be preallocated once and indexing
 * would be used to map the 2D structure into the 1D memory.
 * Non uniform quadtrees are also possible, but quite useless, because
 * they consume the preallocated memory anyway.
 * Anyway the node indexing should follow the defined style by the enum
 * NodeLocation.
 */
template <class TData>
struct TBasicQuadTree 
{
	/** Tells where the node within an array is located.
	 * creates a counterclockwise location of nodes.
	 */
	enum NodeLocation 
	{
		TOP_LEFT = 0,		//!< upper left quad
		BOTTOM_LEFT,		//!< lower left quad
		BOTTOM_RIGHT,		//!< lower right quad
		TOP_RIGHT			//!< upper right quad
	};

	bool		mSubDivided;	//!< true if node has subnodes
	TData		mData;			//!< data of this node
};

/** A basic quadtree with pointer to it's children nodes.
 * This quadree template is for (non) uniform quadtrees where additional
 * nodes can be allocated and inserted on demand.
 */
template <class TData, class TNode>
struct TBasicPointerQuadTree : public TBasicQuadTree< TData >
{
	TNode*		mNodes[4];	//!< four pointers to subnodes

	// function for recursively deleting the quad tree
	static void Delete (TBasicPointerQuadTree<TData, TNode> *node);
};

/** A basic Quadtree containing vertex data
 */
template <class TData, class TNode>
struct TQuadTree : public TBasicPointerQuadTree< TData, TNode >
{
	Ogre::Vector3	mVertices[4];	//!< vertices of this node
	float			mWidth;	//!< length of this quad


	static void Delete (TQuadTree<TData, TNode> *node);
}; /* TQuadTree */

/** A basic loose quadtree.
 * A loose quadtree consists of loose quads, which have an extended
 * 'loose' area around them. So the quads do not only store the
 * contents of the quad, but of the extende area too. This can be
 * helpfull in eliminating additional traversals of the tree, when
 * objects are near the border of the quad (or reaching into neighbouring
 * quads).
 */
template <class TData, class TNode>
struct TLooseQuadTree : public TQuadTree< std::vector< TData >, TNode >
{
	/** maximum number of elements in one leaf.
	 * Whenever this number is reached a subdivision takes place.
	 */
	int mMaxData;
	/** maximum level of depth.
	 * prevents further subdivision when a leafnode is at this subdivision
	 * level
	 */
	int mMaxDepth;
	/** The looseness factor
	 * Defines the size of the 'loose' area around the quad.
	 * Should be between 1.0 and 1.5 - less or more doesn't make sense at all.
	 */
	float mLooseness;


	void SetLooseness(float looseness);
	void SetMaxDepth(int maxDepth);
	void SetMaxData(int maxData);

	float GetLooseness();
	int GetMaxDepth();
	int GetMaxData();

	/** inserts the specified data.
	 * The supplied axisaligned box specifies the extents of the given data.
	 * According to that information the data is inserted at one or more
	 * leaf nodes.
	 */
	void insert(const Ogre::AxisAlignedBoundingBox& aabb, TData data);

	/** removes a node.
	 * When the node has got subnodes they are also removed
	 */
	void removeNode();

	/** searches the quadtree for a leaf that covers the specified position.
	 * The search is done as if this is a normal quadtree (without any
	 * loose boundaries applied 
	 * @param position in world space coordinates
	 */
	TLooseQuadTree* find(Ogre::Vector3 position);

}

// contains the code for the defined functions
#include "TQuadTree.hpp"

}; /* world */

#endif /* __QUADTREE_H__ */
