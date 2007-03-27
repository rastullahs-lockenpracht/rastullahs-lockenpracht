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
	class AxisAlignedBox;
	class Vector2;
	class Vector3;
};

#include "CorePrerequisites.h"
#include "Exception.h"

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
class TQuadTreeBasicNode
{
public:
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
protected:
	bool		mSubDivided;	//!< true if node has subnodes
	TData		mData;			//!< data of this node
};

/** A basic quadtree with pointer to it's children nodes.
 * This quadree template is for (non) uniform quadtrees where additional
 * nodes can be allocated and inserted on demand.
 */
template <class TData, class TNode>
class TQuadTreeBasicPointerNode : public TQuadTreeBasicNode< TData >
{
protected:
	TNode*		mNodes[4];	//!< four pointers to subnodes

public:
	/** default constructor.
	 * Initializes the subquad pointers to NULL
	 */
	TQuadTreeBasicPointerNode();

	/** Retrieve the specified subnode.
	 * Accounting the nature of (non) uniform quadtrees the returned pointer might
	 * be NULL.
	 * @param location whether to fetch the upper/lower left/right quad.
	 * @returns the subquadtree node
	 */
	TNode*	getNode(enum rl::TQuadTreeBasicNode<TData>::NodeLocation location);

	// function for recursively deleting the quad tree
	void remove(); //Node (TQuadTreeBasicPointerNode<TData, TNode> *node);
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
class TQuadTreeOgreNode : public TQuadTreeBasicPointerNode< TData, TNode >
{
protected:
	Ogre::Vector2	mVertexTL;	//!< top left vertex of this node
	Ogre::Vector2	mVertexBR;	//!< bottom right vertex of this node
	Ogre::Real		mWidth;		//!< length of this quad

public:
	/** Default constructor.
	 * initializes a completely empty quad, that even doesn't represent a part
	 * a space.
	 */
	TQuadTreeOgreNode();

	/** Constructor to use for correct setup of quad.
	 * Initialises the quad with all necessary data to function properly.
	 */
	TQuadTreeOgreNode(const Ogre::Vector2& tlc, const Ogre::Vector2& brc, float mWidth=0.0f);

	/** Constructor to use for correct setup of quad.
	 * Initialises the quad with all necessary data to function properly.
	 */
	//TQuadTreeOgreNode(Ogre::Vector2& tlc, Ogre::Vector2& blc, Ogre Vector2& lrc);

	/** Fetches the width of the quad.
	 * @returns the width of the quad.
	 */
	inline Ogre::Real getWidth() { return mWidth; }

	/** Retrieve the specified corner vertex.
	 * Since only the upper left and lower right corner of the quad are stored,
	 * the two others have to be constructed.
	 * @param location specifies which vertex to fetch (upper/lower-left/right).
	 * @returns the wanted vertex.
	 */
	inline Ogre::Vector2 getVertex(enum rl::TQuadTreeBasicNode<TData>::NodeLocation location)
	{
		switch (location)
		{
		case TOP_LEFT:
			return mVertexTL;
		case BOTTOM_LEFT:
			return Ogre::Vector2(mVertexTL.x, mVertexBR.y);
		case BOTTOM_RIGHT:
			return mVertexBR;
		case TOP_RIGHT:
			return Ogre::Vector2(mVertexBR.x, mVertexTL.y);
		default:
			Throw(IllegalArgumentException, "unknown NodeLocation type.");
		}

		return Ogre::Vector2(0.0f,0.0f);
	}

    inline Ogre::Vector2 getTopLeft() { return mVertexTL; }
    inline Ogre::Vector2 getBottomRight() { return mVertexBR; }

	/** tests if the given u value is inside the quad.
	 * Since this is a 2D test, the name refers to u coordinate
	 * (instead of x for the 3D case).
	 * @param u coordinate to test whether it is inside the quad or not.
	 */
	inline bool isUInside(Ogre::Real u) 
	{ 
		return (mVertexTL.x < u && u < mVertexBR.x);
	}
	/** tests if the given v value is inside the quad.
	 * Since this is a 2D test, the name refers to v coordinate
	 * (instead of z for the 3D case).
	 * @param v coordinate to test whether it is inside the quad or not.
	 */
	inline bool isVInside(Ogre::Real v) 
	{
		return (mVertexTL.y < v && v < mVertexBR.y);
	}

	void remove(); //TQuadTree<TData, TNode> *node);
}; /* TQuadTree */

template <class TData, class TNode>
TQuadTreeOgreNode<TData, TNode>::TQuadTreeOgreNode()
: mWidth(0.0f),
  mVertexTL(0,0),
  mVertexBR(0,0)
{
	//memset((char*) mVertices, 0, sizeof(mVertices));
}

template <class TData, class TNode>
TQuadTreeOgreNode<TData, TNode>::TQuadTreeOgreNode(
    const Ogre::Vector2& tlc, const Ogre::Vector2& brc, float mWidth)
: mWidth(mWidth),
  mVertexTL(tlc),
  mVertexBR(brc)
{
	/*
	mVertices[TOP_LEFT] = tlc;
	mVertices[BOTTOM_LEFT].x = tlc.x;
	mVertices[BOTTOM_LEFT].z = brc.z;
	mVertices[BOTTOM_RIGHT] = brc;
	mVertices[TOP_RIGHT].x = brc.x;
	mVertices[TOP_RIGHT].z = tlc.z;
	*/
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
class TLooseQuadTree //: public TQuadTreeOgreNode< std::vector< TData >, TNode >
{
public:
	//! default constructor
	TLooseQuadTree();

	TLooseQuadTree(int maxData, int maxDepth, float looseness, 
		const Ogre::Vector2& tlc, const Ogre::Vector2& brc, float mWidth=0.0f);

	//TLooseQuadTree(const TLooseQuadTree<TData, TNode>& Node);

	/** Retrieves the loosness factor.
	 */
	float getLooseness() { return (mLooseness / mWidth * 2.0f); }
	/** Retrieves the maximum depth of the quadtree
	 */
	int getMaxDepth() { return mMaxDepth; }
	/** Retrieves the maximum number of data elements for a quad.
	 */
	int getMaxData() { return mMaxData; }

    TLooseQuadTree<TData, TNode>* find(const Ogre::Vector3& position);
	
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
	void setLoosenessFactor(float loosenessfactor) { mLoosenessFactor = loosenessfactor; }
	
	void setMaxDepth(int maxDepth) { mMaxDepth = maxDepth; }
	
	void setMaxData(int maxData) { mMaxData = maxData; }

    inline void setExtents(const Ogre::Vector2& tlc, const Ogre::Vector2& brc)
    {
        if (!mRoot)
            mRoot = new TNode(mMaxData, mMaxDepth, mLoosenessFactor * (brc.x-tlc.x) / 2.0f,
            tlc, brc, brc.x-tlc.x);
    }

	/** adds the specified data.
	 * Takes into account that the specified data might not be completely
	 * inside the quadtree resulting into a top node 'split'.
	 * @param data the data to insert.
	 */
	void add(TData data);


    void removeAll();

protected:

    /* adds nodes to the quadtree at it's top.
     * This extends the quadtree above it's root node, so that the specified
     * point is covered by the node.
     * @param u x-coordinate of point in world space
     * @param v z-coordinate of point in world space
     */
    void extend(const Ogre::Real& u, const Ogre::Real& v);

    /* creates a new root node and adds the tree as one of it's subparts.
     * The nodelocation gives the orientation of the new rootnode relative to
     * the existing root node. So e.g TOP_LEFT means that the new root node
     * will extend into the TOP_LEFT direction (!= place the old root node
     * TOP_LEFT).
     * @param location new location direction
     */
    void createNewRootNode(enum rl::TQuadTreeBasicNode<TData>::NodeLocation location);

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
	float mLoosenessFactor;

    /** The root node of the quad tree.
     */
    TNode* mRoot;
    //TLooseQuadTreeNode< std::vector< TData >,
    
};

/** A basic loose quadtree.
 * A loose quadtree consists of loose quads, which have an extended
 * 'loose' area around them. So the quads do not only store the
 * contents of the quad, but of the extende area too. This can be
 * helpfull in eliminating additional traversals of the tree, when
 * objects are near the border of the quad (or reaching into neighbouring
 * quads).
 */
template <class TData, class TNode>
class TLooseQuadTreeNode : public TQuadTreeOgreNode< std::vector< TData >, TNode >
{
public:
    TLooseQuadTreeNode(int maxData, int maxDepth, float looseness, 
		const Ogre::Vector2& tlc, const Ogre::Vector2& brc, float mWidth=0.0f);

    TLooseQuadTreeNode(const TLooseQuadTreeNode<TData, TNode>& Node);

	/** Retrieves the loosness factor.
	 */
	float getLooseness() { return (mLooseness / mWidth * 2.0f); }
	/** Retrieves the maximum depth of the quadtree
	 */
	int getMaxDepth() { return mMaxDepth; }
	/** Retrieves the maximum number of data elements for a quad.
	 */
	int getMaxData() { return mMaxData; }

   	/** tests if the given u value is inside the quad.
	 * Since this is a 2D test, the name refers to u coordinate
	 * (instead of x for the 3D case).
	 * @param u coordinate to test whether it is inside the quad or not.
	 */
	inline bool isUInsideLoose(Ogre::Real u)
	{ 
		return (mVertexTL.x - mLooseness < u && u < mVertexBR.x + mLooseness);
		//return (mVertices[TOP_LEFT].x - mLooseness < u &&
		//	u < mVertices[BOTTOM_RIGHT].x + mLooseness);
	}
	/** tests if the given v value is inside the quad.
	 * Since this is a 2D test, the name refers to v coordinate
	 * (instead of z for the 3D case).
	 * @param v coordinate to test whether it is inside the quad or not.
	 */
	inline bool isVInsideLoose(Ogre::Real v) 
	{
		return (mVertexTL.y - mLooseness < v && v < mVertexBR.y + mLooseness);
		//return (mVertices[TOP_LEFT].y - mLooseness < v &&
		//	v < mVertices[BOTTOM_RIGHT].y + mLooseness);
	}

   	/** inserts the specified data.
	 * The supplied axisaligned box specifies the extents of the given data.
	 * According to that information the data is inserted at one or more
	 * leaf nodes.
	 */
	void insert(TData data);

	/** removes a node.
	 * When the node has got subnodes they are also removed
	 */
	void remove();

	/** searches the quadtree for a leaf that covers the specified position.
	 * The search is done as if this is a normal quadtree (without any
	 * loose boundaries applied 
	 * @param position in world space coordinates
	 */
	TLooseQuadTreeNode* find(const Ogre::Vector3& position);

    friend TLooseQuadTree< TData, TNode >;

protected:
   	/** splits the node into subnodes.
	 * Forces a split of the nodes data, creating 4 subnodes containing the
	 * data of the original node. The original node persists as the parent
	 * node of the new nodes.
	 */
	void split();

    void SetVertex(enum rl::TQuadTreeBasicNode<TData>::NodeLocation location, Ogre::Vector2 vertex);

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
};

template <class TData, class TNode>
TLooseQuadTreeNode<TData, TNode>::TLooseQuadTreeNode(int maxData, int maxDepth,
											 float looseness, const Ogre::Vector2& tlc,
											 const Ogre::Vector2& brc, float mWidth)
: TQuadTreeOgreNode< std::vector< TData >, TNode >(tlc, brc, mWidth),
  mMaxData(maxData),
  mMaxDepth(maxDepth),
  mLooseness(looseness)
{
}

template <class TData, class TNode>
TLooseQuadTreeNode<TData, TNode>::TLooseQuadTreeNode(const TLooseQuadTreeNode<TData, TNode>& Node)
: TQuadTreeOgreNode< std::vector< TData >, TNode >(Node.mVertexTL, Node.mVertexBR, Node.mWidth),
  mMaxData(Node.mMaxData),
  mMaxDepth(Node.mMaxDepth),
  mLooseness(Node.mLooseness)
{
}

template <class TData, class TNode>
void TLooseQuadTreeNode<TData,TNode>::insert(TData data)
{
	if (mSubDivided)
	{
		// try to add the data to any of the subnodes
		for (int i=0; i<4; i++)
		{
			if (mNodes[i])
				mNodes[i]->insert(data);
		}
	}
	else
	{
		Ogre::AxisAlignedBox aabb = TNode::getAABB(data);
		// check whether any of the 4 edges of the axisalignedbox are in the loose quad
		Ogre::Vector3 flb = aabb.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM);
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
void TLooseQuadTreeNode<TData,TNode>::split()
{
	Ogre::Real HalfWidth = mWidth/2.0f;
	Ogre::Vector2 center (getVertex(TOP_LEFT) + Ogre::Vector2(HalfWidth, -HalfWidth));
	// create 4 subnodes
	mNodes[TOP_LEFT] = 
		new TNode(mMaxData,mMaxDepth-1,mLooseness/2.0f,
		          mVertexTL, center, HalfWidth);
	mNodes[BOTTOM_LEFT] = 
		new TNode(mMaxData,mMaxDepth-1,mLooseness/2.0f,
		          Ogre::Vector2(mVertexTL.x, center.y),
				  Ogre::Vector2(center.x, mVertexBR.y), HalfWidth);
	mNodes[BOTTOM_RIGHT] =
		new TNode(mMaxData,mMaxDepth-1,mLooseness/2.0f, 
				  center, mVertexBR, HalfWidth);
	mNodes[TOP_RIGHT] =
		new TNode(mMaxData,mMaxDepth-1,mLooseness/2.0f,
				  Ogre::Vector2(center.x, mVertexTL.y),
				  Ogre::Vector2(mVertexBR.x, center.y), HalfWidth);

	// distribute the data accordingly between the subnodes
	Ogre::AxisAlignedBox aab;
	for (std::vector<TData>::iterator it = mData.begin();
		it != mData.end(); it++)
	{
		insert((*it));
	}
	// simply erase the data here ...
	mData.clear();
	mData.resize(1);

	// remember that it's subdivided
	mSubDivided = true;
}

template <class TData, class TNode>
void TLooseQuadTreeNode<TData,TNode>::remove()
{
	if (mSubDivided)
	{
		// try to remove the data of any of the subnodes
		for (int i=0; i<4; i++)
		{
			if (mNodes[i]) 
			{
				mNodes[i]->remove();
				delete mNodes[i];
				mNodes[i] = NULL;
			}
		}
	}
	else 
		mSubDivided = false;
}

template <class TData, class TNode>
TLooseQuadTreeNode<TData, TNode>* TLooseQuadTreeNode<TData,TNode>::find(const Ogre::Vector3& position)
{
	TLooseQuadTree* result = NULL;
	if (mSubdivided)
	{
		// try to search for the data in any of the subnodes
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
		if ( isUInside(position.x) && isVInside(position.z) )
		{
			// if position is in the loose quad, return it
			result = *this;
		}
	}
	return result;
}

template <class TData, class TNode>
void TLooseQuadTreeNode<TData, TNode>::SetVertex(
	enum rl::TQuadTreeBasicNode<TData>::NodeLocation location, Ogre::Vector2 vertex)
{
	switch (location)
	{
	case TOP_LEFT:
		mVertexTL = vertex;
		break;
	case BOTTOM_RIGHT:
		mVertexBR = vertex;
		break;
	case BOTTOM_LEFT:
	case TOP_RIGHT:
		Throw (IllegalArgumentException, "Cannot set lowerleft/upperright corner");
		break;
	default:
		Throw (IllegalArgumentException, "unknown NodeLocation type.");
	}
}



template <class TData, class TNode>
TLooseQuadTree<TData, TNode>::TLooseQuadTree()
: mMaxData(0),
  mMaxDepth(0),
  mLoosenessFactor(0),
  mRoot(NULL)
{
}

template <class TData, class TNode>
TLooseQuadTree<TData, TNode>::TLooseQuadTree(int maxData, int maxDepth,
											 float loosenessfactor, const Ogre::Vector2& tlc,
											 const Ogre::Vector2& brc, float mWidth)
: mMaxData(maxData),
  mMaxDepth(maxDepth),
  mLoosenessFactor(loosenessfactor)
{
    mRoot = new TNode(maxData,maxDepth,looseness, tlc, brc, mWidth);
}



template <class TData, class TNode>
void TLooseQuadTree<TData,TNode>::add(TData data)
{
    Ogre::AxisAlignedBox aabb = TNode::getAABB(data);
    Ogre::Vector3 flb = aabb.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM);
    Ogre::Vector3 nrb = aabb.getCorner(Ogre::AxisAlignedBox::NEAR_RIGHT_BOTTOM);

    if (!mRoot)
        mRoot = new TNode(mMaxData, mMaxDepth, mLoosenessFactor * (nrb.x-flb.x) / 2.0f,
        Ogre::Vector2(flb.x,flb.z), Ogre::Vector2(nrb.x,nrb.z), nrb.x-flb.x);

	// check if the bounding box is completely inside
    if ( ! (mRoot->isVInside(flb.x) && mRoot->isUInside(flb.z) && 
            mRoot->isVInside(nrb.x) && mRoot->isUInside(nrb.z)) )
    {
        // when not, then split root node accordingly
        extend(flb.x, flb.z);
        extend(nrb.x, nrb.z);
    }
    // after test insert normally
    mRoot->insert(data);
}

template <class TData, class TNode>
void TLooseQuadTree<TData,TNode>::createNewRootNode(
    enum rl::TQuadTreeBasicNode<TData>::NodeLocation location)
{
    typedef rl::TQuadTreeBasicNode<TData> BNode;
    TNode* oldroot = mRoot;
    int oldMaxDepth = mMaxDepth;
    float oldLooseness = oldroot->getLooseness();
    float oldWidth = oldroot->getWidth();

    float newLooseness = oldLooseness * 2.0f;
    float newWidth = oldWidth * 2.0f;
    mMaxDepth++;
    Ogre::Vector2 vertexTL(oldroot->mVertexTL);
    Ogre::Vector2 vertexBR(oldroot->mVertexBR);
    
    Ogre::Vector2 newVertexTL;
    Ogre::Vector2 newVertexBR;
	switch (location)
	{
	case BNode::TOP_LEFT:
        // first create new root (top) node
        newVertexTL = vertexTL + Ogre::Vector2(-oldWidth,oldWidth);
        mRoot = new TNode(mMaxData, mMaxDepth, newLooseness,
            newVertexTL, vertexBR, newWidth);

        mRoot->mNodes[BNode::TOP_LEFT] = 
            new TNode(mMaxData, oldMaxDepth, oldLooseness,
                      vertexTL + Ogre::Vector2(-oldWidth,oldWidth), 
                      vertexTL, oldWidth);
        mRoot->mNodes[BNode::BOTTOM_LEFT] =
            new TNode(mMaxData, oldMaxDepth, oldLooseness,
                      vertexTL + Ogre::Vector2(-oldWidth,0.0f), 
                      vertexBR + Ogre::Vector2(-oldWidth,0.0f), oldWidth);
        mRoot->mNodes[BNode::BOTTOM_RIGHT] = oldroot;
        mRoot->mNodes[BNode::TOP_RIGHT] =
            new TNode(mMaxData, mMaxDepth, oldLooseness,
            vertexTL + Ogre::Vector2(0.0f,oldWidth), 
            vertexBR + Ogre::Vector2(0.0f,oldWidth), oldWidth);
		break;
    case BNode::BOTTOM_LEFT:
        newVertexTL = vertexTL + Ogre::Vector2(-oldWidth,0.0f);
        newVertexBR = vertexBR + Ogre::Vector2(0.0f,-oldWidth);
        mRoot = new TNode(mMaxData, mMaxDepth, newLooseness,
            newVertexTL, newVertexBR, newWidth);

        mRoot->mNodes[BNode::TOP_LEFT] = new TNode(mMaxData, oldMaxDepth, oldLooseness,
            vertexTL + Ogre::Vector2(-oldWidth,0.0f), 
            vertexBR + Ogre::Vector2(-oldWidth,0.0f), oldWidth);
        mRoot->mNodes[BNode::BOTTOM_LEFT] = new TNode(mMaxData, oldMaxDepth, oldLooseness,
            vertexTL + Ogre::Vector2(-oldWidth,-oldWidth), 
            vertexBR + Ogre::Vector2(-oldWidth,-oldWidth), oldWidth);
        mRoot->mNodes[BNode::BOTTOM_RIGHT] = new TNode(mMaxData, oldMaxDepth, oldLooseness,
            vertexTL + Ogre::Vector2(0.0f,-oldWidth), 
            vertexBR + Ogre::Vector2(0.0f,-oldWidth), oldWidth);
        mRoot->mNodes[BNode::TOP_RIGHT] = oldroot;
        break;
    case BNode::BOTTOM_RIGHT:
        newVertexBR = vertexBR + Ogre::Vector2(oldWidth,-oldWidth);
        mRoot = new TNode(mMaxData, mMaxDepth, newLooseness,
            vertexTL, newVertexBR, newWidth);

		mRoot->mNodes[BNode::TOP_LEFT] = oldroot;
        mRoot->mNodes[BNode::BOTTOM_LEFT] = new TNode(mMaxData, oldMaxDepth, oldLooseness,
            vertexTL + Ogre::Vector2(0.0f,-oldWidth), 
            vertexBR + Ogre::Vector2(0.0f,-oldWidth), oldWidth);
        mRoot->mNodes[BNode::BOTTOM_RIGHT] = new TNode(mMaxData, oldMaxDepth, oldLooseness,
            vertexBR,
            vertexBR + Ogre::Vector2(oldWidth,-oldWidth), oldWidth);
        mRoot->mNodes[rl::TQuadTreeBasicNode<TData>::TOP_RIGHT] = new TNode(mMaxData, oldMaxDepth, oldLooseness,
            vertexTL + Ogre::Vector2(oldWidth,0.0f), 
            vertexBR + Ogre::Vector2(oldWidth,0.0f), oldWidth);
        
		break;
	case BNode::TOP_RIGHT:
        newVertexTL = vertexTL + Ogre::Vector2(0.0f,oldWidth);
        newVertexBR = vertexBR + Ogre::Vector2(oldWidth,0.0f);
        mRoot = new TNode(mMaxData, mMaxDepth, newLooseness,
            newVertexTL, newVertexBR, newWidth);

        mRoot->mNodes[BNode::TOP_LEFT] = new TNode(mMaxData, oldMaxDepth, oldLooseness,
            vertexTL + Ogre::Vector2(0.0f,oldWidth),
            vertexBR + Ogre::Vector2(0.0f,oldWidth), oldWidth);
        mRoot->mNodes[BNode::BOTTOM_LEFT] = oldroot;
        mRoot->mNodes[BNode::BOTTOM_RIGHT] = new TNode(mMaxData, oldMaxDepth, oldLooseness,
            vertexTL + Ogre::Vector2(oldWidth,0.0f),
            vertexBR + Ogre::Vector2(oldWidth,0.0f), oldWidth);
        mRoot->mNodes[BNode::TOP_RIGHT] = new TNode(mMaxData, oldMaxDepth, oldLooseness,
            vertexTL + Ogre::Vector2(oldWidth,oldWidth),
            vertexBR + Ogre::Vector2(oldWidth,oldWidth), oldWidth);
		break;
	default:
		Throw (IllegalArgumentException, "unknown NodeLocation type.");
    }
}

template <class TData, class TNode>
void TLooseQuadTree<TData,TNode>::extend(const Ogre::Real& u, const Ogre::Real& v)
{
    // grow the root node according to the position of the point
    if ( u < mRoot->mVertexTL.x ) 
    {
        if ( v < mRoot->mVertexTL.y )
        {
            createNewRootNode(rl::TQuadTreeBasicNode<TData>::BOTTOM_LEFT);
            extend(u,v);
        }
        else if ( v > mRoot->mVertexBR.y )
        {
            createNewRootNode(rl::TQuadTreeBasicNode<TData>::TOP_LEFT);
            extend(u,v);
        }
        else
        {
            createNewRootNode(rl::TQuadTreeBasicNode<TData>::BOTTOM_LEFT);
            extend(u,v);
        }
    }
    else if ( u > mRoot->mVertexBR.x )
    {
        if ( v < mRoot->mVertexBR.y )
        {
            createNewRootNode(rl::TQuadTreeBasicNode<TData>::BOTTOM_RIGHT);
            extend(u,v);
        }
        else if ( v > mRoot->mVertexTL.y )
        {
            createNewRootNode(rl::TQuadTreeBasicNode<TData>::TOP_RIGHT);
            extend(u,v);
        }  
        else
        {
            createNewRootNode(rl::TQuadTreeBasicNode<TData>::BOTTOM_RIGHT);
            extend(u,v);
        }
    }
    else //  u is inside
    {
        if ( v < mRoot->mVertexBR.y )
        {
            createNewRootNode(rl::TQuadTreeBasicNode<TData>::BOTTOM_RIGHT);
            extend(u,v);
        }
        else if ( v > mRoot->mVertexTL.y )
        {
            createNewRootNode(rl::TQuadTreeBasicNode<TData>::TOP_RIGHT);
            extend(u,v);
        }
    }
}

template <class TData, class TNode>
void TLooseQuadTree<TData,TNode>::removeAll()
{
    if (mRoot)
        delete mRoot;
    mRoot = NULL;
}

template <class TData, class TNode>
TLooseQuadTree<TData, TNode>* TLooseQuadTree<TData,TNode>::find(const Ogre::Vector3& position)
{
    return mRoot->find(position);
}

}; /* world */

#endif /* __QUADTREE_H__ */
