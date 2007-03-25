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

namespace Ogre {
	class AxisAlignedBoundingBox;
};

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
struct TQuadTreeBasicNode
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

	/** Returns the stored data.
	 * @returns a const reference to the internally stored data
	 */
	const TData& getData() { return mData; }

	bool		mSubDivided;	//!< true if node has subnodes
	TData		mData;			//!< data of this node
};

/** A basic quadtree with pointer to it's children nodes.
 * This quadree template is for (non) uniform quadtrees where additional
 * nodes can be allocated and inserted on demand.
 */
template <class TData, class TNode>
struct TQuadTreeBasicPointerNode : public TQuadTreeBasicNode< TData >
{
	TNode*		mNodes[4];	//!< four pointers to subnodes

	TNode*	getNode(enum rl::TQuadTreeBasicNode<TData>::NodeLocation location);

	TQuadTreeBasicPointerNode();

	// function for recursively deleting the quad tree
	void removeNode (TQuadTreeBasicPointerNode<TData, TNode> *node);
};

template <class TData, class TNode>
TQuadTreeBasicPointerNode<TData, TNode>::TQuadTreeBasicPointerNode()
{
	memset(mNodes, 0, sizeof(mNodes));
	mSubDivided = false;
}

/** A basic Quadtree containing vertex data
 */
template <class TData, class TNode>
struct TQuadTreeOgreNode : public TQuadTreeBasicPointerNode< TData, TNode >
{
	Ogre::Vector3	mVertices[4];	//!< vertices of this node
	float			mWidth;	//!< length of this quad

	TQuadTreeOgreNode();

	/** tests if the given u value is inside the quad.
	 * Since this is a 2D test, the name refers to u coordinate
	 * (instead of x for the 3D case).
	 * @param u coordinate to test whether it is inside the quad or not.
	 */
	inline bool isUInside(Ogre::Real u) 
	{ 
		return (mVertices[TOP_LEFT].x < u && u < mVertices[BOTTOM_RIGHT].x);
	}
	/** tests if the given v value is inside the quad.
	 * Since this is a 2D test, the name refers to v coordinate
	 * (instead of z for the 3D case).
	 * @param v coordinate to test whether it is inside the quad or not.
	 */
	inline bool isVInside(Ogre::Real v) 
	{
		return (mVertices[TOP_LEFT].z < v && v < mVertices[BOTTOM_RIGHT].z);
	}

	void remove(); //TQuadTree<TData, TNode> *node);
}; /* TQuadTree */

template <class TData, class TNode>
TQuadTreeOgreNode<TData, TNode>::TQuadTreeOgreNode()
: mWidth(0.0f)
{
	memset((char*) mVertices, 0, sizeof(mVertices));
}

/** A basic loose quadtree.
 * A loose quadtree consists of loose quads, which have an extended
 * 'loose' area around them. So the quads do not only store the
 * contents of the quad, but of the extende area too. This can be
 * helpfull in eliminating additional traversals of the tree, when
 * objects are near the border of the quad (or reaching into neighbouring
 * quads).
 */
template <class TData, class TNode>
struct TLooseQuadTree : public TQuadTreeOgreNode< std::vector< TData >, TNode >
{
	TLooseQuadTree();

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

	/** Retrieves the loosness factor.
	 */
	float GetLooseness() { return (mLooseness / mWidth * 2.0f); }
	/** Retrieves the maximum depth of the quadtree
	 */
	int GetMaxDepth() { return mMaxDepth; }
	/** Retrieves the maximum number of data elements for a quad.
	 */
	int GetMaxData() { return mMaxData; }
	
	/** Sets the looseness factor to a new value.
	 * mLooseness stores a precalculated looseness value.
	 * The looseness is specified in percents of the width, so the loose quad
	 * is the original quad + some additional area. e.g. 0.5 extends the original
	 * length by 50% which means an additional area of 125%. Since this is evenly
	 * distributed in all directions, the value that has to be added/substracted 
	 * to the quad's original vertices is 25% of width. Those 25% of width are
	 * stored in mLooseness.
	 * @param looseness looseness factor in percent (e.g. 0.5 for 50%)
	 */
	void SetLooseness(float looseness) { mLooseness = mWidth * looseness / 2.0f; }
	
	void SetMaxDepth(int maxDepth) { mMaxDepth = maxDepth; }
	
	void SetMaxData(int maxData) { mMaxData = maxData; }
	
	/** tests if the given u value is inside the quad.
	 * Since this is a 2D test, the name refers to u coordinate
	 * (instead of x for the 3D case).
	 * @param u coordinate to test whether it is inside the quad or not.
	 */
	inline bool isUInsideLoose(Ogre::Real u) 
	{ 
		return (mVertices[TOP_LEFT].x - mLooseness < u &&
			u < mVertices[BOTTOM_RIGHT].x + mLooseness);
	}
	/** tests if the given v value is inside the quad.
	 * Since this is a 2D test, the name refers to v coordinate
	 * (instead of z for the 3D case).
	 * @param v coordinate to test whether it is inside the quad or not.
	 */
	inline bool isVInsideLoose(Ogre::Real v) 
	{
		return (mVertices[TOP_LEFT].z - mLooseness < v &&
			v < mVertices[BOTTOM_RIGHT].z + mLooseness);
	}

	/** inserts the specified data.
	 * The supplied axisaligned box specifies the extents of the given data.
	 * According to that information the data is inserted at one or more
	 * leaf nodes.
	 */
	void insert(Ogre::AxisAlignedBox& aabb, TData data);

	/** removes a node.
	 * When the node has got subnodes they are also removed
	 */
	void remove();

	/** searches the quadtree for a leaf that covers the specified position.
	 * The search is done as if this is a normal quadtree (without any
	 * loose boundaries applied 
	 * @param position in world space coordinates
	 */
	TLooseQuadTree* find(const Ogre::Vector3& position);

	/** splits the node into subnodes.
	 * Forces a split of the nodes data, creating 4 subnodes containing the
	 * data of the original node. The original node persists as the parent
	 * node of the new nodes.
	 */
	void split();

};

template <class TData, class TNode>
TLooseQuadTree<TData, TNode>::TLooseQuadTree()
: mMaxData(0),
  mMaxDepth(0),
  mLooseness(0)
{
}

template <class TData, class TNode>
void TLooseQuadTree<TData,TNode>::insert(Ogre::AxisAlignedBox& aabb, TData data)
{
	if (mSubDivided)
	{
		// try to add the data to any of the subnodes
		for (int i=0; i<4; i++)
		{
			if (mNodes[i])
				mNodes[i]->insert(aabb, data);
		}
	}
	else
	{
		// check whether any of the 4 edges of the axisalignedbox are in the loose quad
		Ogre::Vector3 flb = aabb.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM);
		//aabb.getCorner(Ogre::AxisAlignedBox::FAR_RIGHT_BOTTOM)
		//aabb.getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM) 
		Ogre::Vector3 nrb = aabb.getCorner(Ogre::AxisAlignedBox::NEAR_RIGHT_BOTTOM);
		if ( (isVInsideLoose(flb.x) && (isUInsideLoose(flb.z) || isUInsideLoose(nrb.z))) ||
			 (isVInsideLoose(nrb.x) && (isUInsideLoose(flb.z) || isUInsideLoose(nrb.z))) )
		{
			// if at least one is in the loose quad, store the data
			mData.push_back(data);
		}
		// split if maximum number of elements is reached ...
		if (mData.size() == mMaxData) 
		{
			split();
		}
	}
}

template <class TData, class TNode>
void TLooseQuadTree<TData,TNode>::split()
{
	// create 4 subnodes
	for (int i=0; i<4; i++)
	{
		if (! mNodes[i])
			mNodes[i] = new TNode();
	}

	// distribute the data accordingly
	Ogre::AxisAlignedBox aab;
	for (std::vector<TData>::iterator it = mData.begin();
		it != mData.end(); it++)
	{
		aab = TNode::getAABB((*it));
		insert(aab,(*it));
	}
	// simply erase the data here ...
	mData.clear();
	mData.resize(1);

	// remember that it's subdivided
	mSubDivided = true;
}

template <class TData, class TNode>
void TLooseQuadTree<TData,TNode>::remove()
{
	if (mSubdivided)
	{
		// try to add the data to any of the subnodes
		for (int i=0; i<4; i++)
		{
			if (mNodes[i]) 
			{
				mNodes[i].removeNode();
				delete mNodes[i];
			}
		}
	}
}

template <class TData, class TNode>
TLooseQuadTree<TData, TNode>* TLooseQuadTree<TData,TNode>::find(const Ogre::Vector3& position)
{
	TLooseQuadTree* result = NULL;
	if (mSubdivided)
	{
		// try to add the data to any of the subnodes
		for (int i=0; i<4; i++)
		{
			if (mNodes[i]) 
			{
				result = mNodes[i].find(position);
				if (result)
					break;
			}
		}
	}
	else
	{
		// check whether any of the 4 edges of the axisalignedbox are in the loose quad
		Ogre::Vector3 flb = aabb.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM);
		//aabb.getCorner(Ogre::AxisAlignedBox::FAR_RIGHT_BOTTOM)
		//aabb.getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM) 
		Ogre::Vector3 nrb = aabb.getCorner(Ogre::AxisAlignedBox::NEAR_RIGHT_BOTTOM);
		if ( isUInside(position.x) && isVInside(position.z) )
		{
			// if position is in the loose quad, return it
			result = *this;
		}
	}
	return result;
}


}; /* world */

#endif /* __QUADTREE_H__ */
