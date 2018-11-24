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

#ifndef __QUADTREE_H__
#define __QUADTREE_H__

namespace Ogre
{
    class AxisAlignedBox;
};

#include "CorePrerequisites.h"
#include "Exception.h"

namespace rl
{

    /** Tells where the node within an array is located.
     * creates a counterclockwise location of nodes.
     */
    enum QuadTreeNodeLocation
    {
        TOP_LEFT = 0, //!< upper left quad
        BOTTOM_LEFT, //!< lower left quad
        BOTTOM_RIGHT, //!< lower right quad
        TOP_RIGHT //!< upper right quad
    };

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
    template <class TData, class TNode> class TQuadTreeBasicNode
    {
    public:
        TQuadTreeBasicNode();

        /** Returns the stored data.
         * @returns a const reference to the internally stored data
         */
        const TData& getData()
        {
            return mData;
        }

        /** Retrieve the specified subnode.
         * Accounting the nature of (non) uniform quadtrees the returned pointer might
         * be NULL.
         * @param location whether to fetch the upper/lower left/right quad.
         * @returns the subquadtree node
         */
        TNode* getNode(QuadTreeNodeLocation location);

        // function for recursively deleting the quad tree
        void remove(); // Node (TQuadTreeBasicPointerNode<TData, TNode> *node);

    protected:
        bool mSubDivided; //!< true if node has subnodes
        TData mData; //!< data of this node

        TNode* mNodes[4]; //!< four pointers to subnodes
    };

    template <class TData, class TNode> void TQuadTreeBasicNode<TData, TNode>::remove()
    {
        if (mSubDivided)
        {
            // try to remove the data of any of the subnodes
            for (int i = 0; i < 4; i++)
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

    template <class TData, class TNode> TQuadTreeBasicNode<TData, TNode>::TQuadTreeBasicNode()
    {
        for (int i = 0; i < 4; i++)
        {
            mNodes[i] = NULL;
        }
        mSubDivided = false;
    }

    /** A basic Quadtree containing vertex data
     */
    template <class TData, class TNode> class TQuadTreeOgreNode : public TQuadTreeBasicNode<TData, TNode>
    {
    protected:
        Ogre::Vector2 mVertexTL; //!< top left vertex of this node
        Ogre::Vector2 mVertexBR; //!< bottom right vertex of this node
        Ogre::Real mWidth; //!< length of this quad

    public:
        /** Default constructor.
         * initializes a completely empty quad, that even doesn't represent a part
         * a space.
         */
        TQuadTreeOgreNode();

        /** Constructor to use for correct setup of quad.
         * Initialises the quad with all necessary data to function properly.
         */
        TQuadTreeOgreNode(const Ogre::Vector2& tlc, const Ogre::Vector2& brc, float mWidth = 0.0f);

        /** Fetches the width of the quad.
         * @returns the width of the quad.
         */
        inline Ogre::Real getWidth()
        {
            return mWidth;
        }

        /** Retrieve the specified corner vertex.
         * Since only the upper left and lower right corner of the quad are stored,
         * the two others have to be constructed.
         * @param location specifies which vertex to fetch (upper/lower-left/right).
         * @returns the wanted vertex.
         */
        inline Ogre::Vector2 getVertex(QuadTreeNodeLocation location)
        {
            switch (location)
            {
            case QuadTreeNodeLocation::TOP_LEFT:
                return mVertexTL;
            case QuadTreeNodeLocation::BOTTOM_LEFT:
                return Ogre::Vector2(mVertexTL.x, mVertexBR.y);
            case QuadTreeNodeLocation::BOTTOM_RIGHT:
                return mVertexBR;
            case QuadTreeNodeLocation::TOP_RIGHT:
                return Ogre::Vector2(mVertexBR.x, mVertexTL.y);
            default:
                Throw(IllegalArgumentException, "unknown NodeLocation type.");
            }

            return Ogre::Vector2(0.0f, 0.0f);
        }

        /** retrieves the topleft vertex.
         * @todo is this really necessary?
         */
        inline Ogre::Vector2 getTopLeft()
        {
            return mVertexTL;
        }
        /** retrieves the bottom right vertex.
         * @todo is this really necessary?
         */
        inline Ogre::Vector2 getBottomRight()
        {
            return mVertexBR;
        }

        /** tests if the given u value is inside the quad.
         * Since this is a 2D test, the name refers to u coordinate
         * (instead of x for the 3D case).
         * @param u coordinate to test whether it is inside the quad or not.
         */
        inline bool isUInside(Ogre::Real u)
        {
            return (mVertexTL.x <= u && u <= mVertexBR.x);
        }
        /** tests if the given v value is inside the quad.
         * Since this is a 2D test, the name refers to v coordinate
         * (instead of z for the 3D case).
         * @param v coordinate to test whether it is inside the quad or not.
         */
        inline bool isVInside(Ogre::Real v)
        {
            return (mVertexTL.y <= v && v <= mVertexBR.y);
        }

        void remove(); // TQuadTree<TData, TNode> *node);
    }; /* TQuadTree */

    template <class TData, class TNode>
    TQuadTreeOgreNode<TData, TNode>::TQuadTreeOgreNode()
        : mWidth(0.0f)
        , mVertexTL(0, 0)
        , mVertexBR(0, 0)
    {
    }

    template <class TData, class TNode>
    TQuadTreeOgreNode<TData, TNode>::TQuadTreeOgreNode(const Ogre::Vector2& tlc, const Ogre::Vector2& brc, float mWidth)
        : mWidth(mWidth)
        , mVertexTL(tlc)
        , mVertexBR(brc)
    {
        // right hand side coordinate system (2D part) xz plane
        RlAssert((tlc < brc), "coordinates don't fit together");
    }

    /** A basic loose quadtree.
     * A loose quadtree consists of loose quads, which have an extended
     * 'loose' area around them. So the quads do not only store the
     * contents of the quad, but of the extende area too. This can be
     * helpfull in eliminating additional traversals of the tree, when
     * objects are near the border of the quad (or reaching into neighbouring
     * quads).
     */
    template <class TData, class TNode> class TLooseQuadTree //: public TQuadTreeOgreNode< std::vector< TData >, TNode >
    {
    public:
        //! default constructor
        TLooseQuadTree();

        /** Detailed constructor.
         * Gives the possibility to initialise all internal values to meaningfull
         * values
         * @param maxDepth gives the maximum number of levels inside the quadtree. \c setMaxDepth
         * @param maxData maximum number of data elements per node. \c setMaxData
         * @param looseness looseness factor in percent (e.g. 0.5 for 50%) \c setLooseness
         * @param tlc gives the top left corner (in world space) \c setExtents
         * @param tlc gives the bottom rigth corner (in world space) \c setExtents
         */
        TLooseQuadTree(int maxData, int maxDepth, float looseness, const Ogre::Vector2& tlc, const Ogre::Vector2& brc);

        // TLooseQuadTree(const TLooseQuadTree<TData, TNode>& Node);

        /** Searches the quadtree for the node containing position.
         * Returns the node containing the specified position or null if position
         * is outside the quadtree.
         * @return pointer to the node containing position or null if not inside.
         */
        TLooseQuadTree<TData, TNode>* find(const Ogre::Vector3& position);

        /** Retrieves the loosness factor.
         */
        float getLooseness()
        {
            return /*mLooseness / mWidth * 2.0f;*/ mLoosenessFactor;
        }
        /** Retrieves the maximum depth of the quadtree
         */
        int getMaxDepth()
        {
            return mMaxDepth;
        }
        /** Retrieves the maximum number of data elements for a quad.
         */
        int getMaxData()
        {
            return mMaxData;
        }

        /** Sets the looseness factor to a new value.
         * Changing this value is only possible, when the tree is empty.
         * mLooseness stores a precalculated looseness value.
         * The looseness is specified in percents of the width, so the loose quad
         * is the original quad + some additional area. e.g. 0.5 extends the original
         * length by 50% which means an additional area of 125%.
         * @param looseness looseness factor in percent (e.g. 0.5 for 50%)
         */
        void setLoosenessFactor(float loosenessfactor)
        {
            mLoosenessFactor = loosenessfactor;
        }

        /** Sets the maximum depth of the quadtree.
         * Changing this value is only possible, when the tree is empty.
         * This value can change (grow) when data is added to the quad tree
         * that is outside the quadtree.
         * Setting this value is only possible, when there is no data in the
         * quadtree.
         * @param maxDepth gives the maximum number of levels inside the quadtree.
         */
        void setMaxDepth(int maxDepth)
        {
            if (!mRoot)
                mMaxDepth = maxDepth;
            else
                LOG_DEBUG(Logger::CORE, "Setting maximum depth on non empty quadtree not possible");
        }

        /** Sets the maximum number of data elements per quad.
         * Changing this value is only possible, when the tree is empty.
         * Whenever this number is exceeded, the quad is split creating 4 new
         * subquads.
         * When maxData is reached at the maximum depth level of the quadtree,
         * no split will take place (because the depth limit prevents it),
         * \c setMaxDepth.
         * @param maxData maximum number of data elements per node.
         */
        void setMaxData(int maxData)
        {
            if (!mRoot)
                mMaxData = maxData;
            else
                LOG_DEBUG(Logger::CORE, "Setting maximum data on non empty quadtree not possible");
        }

        /** Sets the extents of the quadtree to the specified one.
         * Changing these value is only possible, when the tree is empty.
         * This creates a new quadtree with the specified extents, when the
         * tree was initially empty.
         * The coordinates have world space (right hand sided, y is facing down).
         * @param tlc gives the top left corner (in world space)
         * @param tlc gives the bottom rigth corner (in world space)
         */
        inline void setExtents(const Ogre::Vector2& tlc, const Ogre::Vector2& brc)
        {
            if (!mRoot)
                mRoot = new TNode(
                    mMaxData, mMaxDepth, mLoosenessFactor * (brc.x - tlc.x) / 2.0f, tlc, brc, brc.x - tlc.x);
            else
                LOG_DEBUG(Logger::CORE, "Settings extents on non empty quadtree not possible");
        }

        /** adds the specified data.
         * Takes into account that the specified data might not be completely
         * inside the quadtree resulting into a top node 'split'.
         * @param data the data to insert.
         */
        void add(TData data);

        /** deletes the complete quadtree.
         */
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
        void createNewRootNode(QuadTreeNodeLocation location);

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
         * Should be between 0.25 and 1.0 - less or more doesn't make sense at all.
         */
        float mLoosenessFactor;

        //! The root node of the quad tree.
        TNode* mRoot;
    };

    /** A basic loose quadtree node.
     * A loose quadtree consists of loose quads, which have an extended
     * 'loose' area around them. So the quads do not only store the
     * contents of the quad, but of the extended area too. This can be
     * helpfull in eliminating additional traversals of the tree, when searching
     * in case the objects are near the border of the quad (or reaching into
     * neighbouring quads).
     */
    template <class TData, class TNode> class TLooseQuadTreeNode : public TQuadTreeOgreNode<std::vector<TData>, TNode>
    {
    public:
        /** Detailed quad node constructor.
         * Gives the possibility to initialise all internal values to meaningfull
         * values
         * @param maxDepth gives the maximum number of levels inside the quadtree.
         * @param maxData maximum number of data elements per node.
         * @param looseness looseness factor in percent (e.g. 0.5 for 50%)
         * @param tlc gives the top left corner (in world space)
         * @param tlc gives the bottom rigth corner (in world space)
         * @param mWidth gives the width of the node (passing this for performance).
         */
        TLooseQuadTreeNode(int maxData, int maxDepth, float looseness, const Ogre::Vector2& tlc,
            const Ogre::Vector2& brc, float mWidth = 0.0f);

        /** copy constructor.
         * Should copy the data of the specified node into the new object.
         * Normally it is not necessary to do a deep copy here (wastes performance).
         * @param Node to copy.
         */
        TLooseQuadTreeNode(const TLooseQuadTreeNode<TData, TNode>& Node);

        /** Retrieves the loosness factor.
         */
        float getLooseness()
        {
            return /*mLooseness / mWidth * 2.0f*/ mLooseness;
        }
        /** Retrieves the maximum depth of the quadtree
         */
        int getMaxDepth()
        {
            return mMaxDepth;
        }
        /** Retrieves the maximum number of data elements for a quad.
         */
        int getMaxData()
        {
            return mMaxData;
        }

        /** tests if the given u value is inside the quad.
         * Since this is a 2D test, the name refers to u coordinate
         * (instead of x for the 3D case).
         * @param u coordinate to test whether it is inside the quad or not.
         */
        inline bool isUInsideLoose(Ogre::Real u)
        {
            return (this->mVertexTL.x - mLooseness <= u && u <= this->mVertexBR.x + mLooseness);
        }
        /** tests if the given v value is inside the quad.
         * Since this is a 2D test, the name refers to v coordinate
         * (instead of z for the 3D case).
         * @param v coordinate to test whether it is inside the quad or not.
         */
        inline bool isVInsideLoose(Ogre::Real v)
        {
            return (this->mVertexTL.y - mLooseness <= v && v <= this->mVertexBR.y + mLooseness);
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

        /** TLooseQuadTree is the management class for this node class.
         */
        friend class TLooseQuadTree<TData, TNode>;

    protected:
        /** splits the node into subnodes.
         * Forces a split of the nodes data, creating 4 subnodes containing the
         * data of the original node. The original node persists as the parent
         * node of the new nodes.
         */
        void split();

        /** sets the specified vertex to a new value
         * @param location gives the position of the vertex
         * @param vertex contains the new value
         */
        void setVertex(QuadTreeNodeLocation location, Ogre::Vector2 vertex);

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
         * Is added and substracted from the corner vertices.
         */
        float mLooseness;
    };

    template <class TData, class TNode>
    TLooseQuadTreeNode<TData, TNode>::TLooseQuadTreeNode(
        int maxData, int maxDepth, float looseness, const Ogre::Vector2& tlc, const Ogre::Vector2& brc, float mWidth)
        : TQuadTreeOgreNode<std::vector<TData>, TNode>(tlc, brc, mWidth)
        , mMaxData(maxData)
        , mMaxDepth(maxDepth)
        , mLooseness(looseness)
    {
    }

    template <class TData, class TNode>
    TLooseQuadTreeNode<TData, TNode>::TLooseQuadTreeNode(const TLooseQuadTreeNode<TData, TNode>& Node)
        : TQuadTreeOgreNode<std::vector<TData>, TNode>(Node.mVertexTL, Node.mVertexBR, Node.mWidth)
        , mMaxData(Node.mMaxData)
        , mMaxDepth(Node.mMaxDepth)
        , mLooseness(Node.mLooseness)
    {
    }

    template <class TData, class TNode> void TLooseQuadTreeNode<TData, TNode>::insert(TData data)
    {
        if (this->mSubDivided)
        {
            // try to add the data to any of the subnodes
            for (int i = 0; i < 4; i++)
            {
                if (this->mNodes[i] != NULL)
                {
                    this->mNodes[i]->insert(data);
                }
            }
        }
        else
        {
            Ogre::AxisAlignedBox aabb = TNode::getAABB(data);
            // check whether any of the 4 edges of the axisalignedbox are in the loose quad
            Ogre::Vector3 flb = aabb.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM);
            Ogre::Vector3 nrb = aabb.getCorner(Ogre::AxisAlignedBox::NEAR_RIGHT_BOTTOM);
            if ((isVInsideLoose(flb.x) && (isUInsideLoose(flb.z) || isUInsideLoose(nrb.z)))
                || (isVInsideLoose(nrb.x) && (isUInsideLoose(flb.z) || isUInsideLoose(nrb.z))))
            {
                // if at least one is in the loose quad, store the data
                this->mData.push_back(data);
            }
            // split if maximum number of elements is reached ...
            if (this->mData.size() == mMaxData)
            {
                split();
            }
        }
    }

    template <class TData, class TNode> void TLooseQuadTreeNode<TData, TNode>::split()
    {
        // first test if this node is allowed to be subdivided
        if (mMaxDepth == 0)
            return; // last level reached, prevent subdivision

        Ogre::Real halfWidth = this->mWidth / 2.0f;
        Ogre::Vector2 center(this->getVertex(QuadTreeNodeLocation::TOP_LEFT) + Ogre::Vector2(halfWidth, halfWidth));
        // create 4 subnodes
        this->mNodes[QuadTreeNodeLocation::TOP_LEFT]
            = new TNode(mMaxData, mMaxDepth - 1, mLooseness / 2.0f, this->mVertexTL, center, halfWidth);
        this->mNodes[QuadTreeNodeLocation::BOTTOM_LEFT] = new TNode(mMaxData, mMaxDepth - 1, mLooseness / 2.0f,
            Ogre::Vector2(this->mVertexTL.x, center.y), Ogre::Vector2(center.x, this->mVertexBR.y), halfWidth);
        this->mNodes[QuadTreeNodeLocation::BOTTOM_RIGHT]
            = new TNode(mMaxData, mMaxDepth - 1, mLooseness / 2.0f, center, this->mVertexBR, halfWidth);
        this->mNodes[QuadTreeNodeLocation::TOP_RIGHT] = new TNode(mMaxData, mMaxDepth - 1, mLooseness / 2.0f,
            Ogre::Vector2(center.x, this->mVertexTL.y), Ogre::Vector2(this->mVertexBR.x, center.y), halfWidth);

        // distribute the data accordingly between the subnodes
        Ogre::AxisAlignedBox aab;
        for (size_t i = 0, end = this->mData.size(); i < end; ++i)
        {
            insert(this->mData[i]);
        }
        // simply erase the data here ...
        this->mData.clear();
        this->mData.resize(1);

        // remember that it's subdivided
        this->mSubDivided = true;
    }

    template <class TData, class TNode> void TLooseQuadTreeNode<TData, TNode>::remove()
    {
        if (this->mSubDivided)
        {
            // try to remove the data of any of the subnodes
            for (int i = 0; i < 4; i++)
            {
                if (this->mNodes[i] != NULL)
                {
                    this->mNodes[i]->remove();
                    delete this->mNodes[i];
                    this->mNodes[i] = NULL;
                }
            }
        }
        else
        {
            this->mSubDivided = false;
        }
    }

    template <class TData, class TNode>
    TLooseQuadTreeNode<TData, TNode>* TLooseQuadTreeNode<TData, TNode>::find(const Ogre::Vector3& position)
    {
        TLooseQuadTree<TData, TNode>* result = NULL;
        if (this->mSubDivided)
        {
            // try to search for the data in any of the subnodes
            for (int i = 0; i < 4; i++)
            {
                if (this->mNodes[i] != NULL)
                {
                    result = this->mNodes[i].find(position);
                    if (result != NULL)
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            // check whether any of the 4 edges of the axisalignedbox are in the loose quad
            if (isUInsideLoose(position.x) && isVInsideLoose(position.z))
            {
                // if position is in the loose quad, return it
                result = *this;
            }
        }
        return result;
    }

    template <class TData, class TNode>
    void TLooseQuadTreeNode<TData, TNode>::setVertex(QuadTreeNodeLocation location, Ogre::Vector2 vertex)
    {
        switch (location)
        {
        case QuadTreeNodeLocation::TOP_LEFT:
            this->mVertexTL = vertex;
            break;
        case QuadTreeNodeLocation::BOTTOM_RIGHT:
            this->mVertexBR = vertex;
            break;
        case QuadTreeNodeLocation::BOTTOM_LEFT:
        case QuadTreeNodeLocation::TOP_RIGHT:
            Throw(IllegalArgumentException, "Cannot set lowerleft/upperright corner");
            break;
        default:
            Throw(IllegalArgumentException, "unknown NodeLocation type.");
        }
    }

    template <class TData, class TNode>
    TLooseQuadTree<TData, TNode>::TLooseQuadTree()
        : mMaxData(0)
        , mMaxDepth(0)
        , mLoosenessFactor(0)
        , mRoot(NULL)
    {
    }

    template <class TData, class TNode>
    TLooseQuadTree<TData, TNode>::TLooseQuadTree(
        int maxData, int maxDepth, float loosenessfactor, const Ogre::Vector2& tlc, const Ogre::Vector2& brc)
        : mMaxData(maxData)
        , mMaxDepth(maxDepth)
        , mLoosenessFactor(loosenessfactor)
    {
        RlAssert(brc.x - tlc.x == brc.y - tlc.y, "topleft and bottom right vertices specify non quad");
        mRoot = new TNode(maxData, maxDepth, loosenessfactor, tlc, brc, brc.x - tlc.x);
    }

    template <class TData, class TNode> void TLooseQuadTree<TData, TNode>::add(TData data)
    {
        Ogre::AxisAlignedBox aabb = TNode::getAABB(data);
        Ogre::Vector3 flb = aabb.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM);
        Ogre::Vector3 nrb = aabb.getCorner(Ogre::AxisAlignedBox::NEAR_RIGHT_BOTTOM);

        // essential, create a root node when none is present
        if (!mRoot)
            mRoot = new TNode(mMaxData, mMaxDepth, mLoosenessFactor * (nrb.x - flb.x) / 2.0f,
                Ogre::Vector2(flb.x, flb.z), Ogre::Vector2(nrb.x, nrb.z), nrb.x - flb.x);

        // check if the bounding box is completely inside
        if (!(mRoot->isVInside(flb.x) && mRoot->isUInside(flb.z) && mRoot->isVInside(nrb.x) && mRoot->isUInside(nrb.z)))
        {
            // when not, then split root node accordingly
            extend(flb.x, flb.z);
            extend(nrb.x, nrb.z);
        }
        // after test insert normally
        mRoot->insert(data);
    }

    template <class TData, class TNode>
    void TLooseQuadTree<TData, TNode>::createNewRootNode(QuadTreeNodeLocation location)
    {
        typedef rl::TQuadTreeBasicNode<TData, TNode> BNode;
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
        case QuadTreeNodeLocation::TOP_LEFT:
            // first create new root (top) node
            newVertexBR = vertexBR + Ogre::Vector2(oldWidth, oldWidth);
            mRoot = new TNode(mMaxData, mMaxDepth, newLooseness, vertexTL, newVertexBR, newWidth);

            mRoot->mNodes[QuadTreeNodeLocation::TOP_LEFT] = oldroot;
            mRoot->mNodes[QuadTreeNodeLocation::BOTTOM_LEFT] = new TNode(mMaxData, oldMaxDepth, oldLooseness,
                Ogre::Vector2(vertexTL.x, vertexBR.y), Ogre::Vector2(vertexBR.x, newVertexBR.y), oldWidth);
            mRoot->mNodes[QuadTreeNodeLocation::BOTTOM_RIGHT]
                = new TNode(mMaxData, oldMaxDepth, oldLooseness, vertexBR, newVertexBR, oldWidth);
            mRoot->mNodes[QuadTreeNodeLocation::TOP_RIGHT] = new TNode(mMaxData, oldMaxDepth, oldLooseness,
                Ogre::Vector2(vertexBR.x, vertexTL.y), Ogre::Vector2(newVertexBR.x, vertexBR.y), oldWidth);

            break;
        case QuadTreeNodeLocation::BOTTOM_LEFT:
            newVertexTL = vertexTL + Ogre::Vector2(0.0f, -oldWidth);
            newVertexBR = vertexBR + Ogre::Vector2(oldWidth, 0.0f);
            mRoot = new TNode(mMaxData, mMaxDepth, newLooseness, newVertexTL, newVertexBR, newWidth);

            mRoot->mNodes[QuadTreeNodeLocation::TOP_LEFT] = new TNode(
                mMaxData, oldMaxDepth, oldLooseness, newVertexTL, Ogre::Vector2(vertexBR.x, vertexTL.y), oldWidth);
            mRoot->mNodes[QuadTreeNodeLocation::BOTTOM_LEFT] = oldroot;
            mRoot->mNodes[QuadTreeNodeLocation::BOTTOM_RIGHT] = new TNode(
                mMaxData, oldMaxDepth, oldLooseness, Ogre::Vector2(vertexBR.x, vertexTL.y), newVertexBR, oldWidth);
            mRoot->mNodes[QuadTreeNodeLocation::TOP_RIGHT] = new TNode(mMaxData, oldMaxDepth, oldLooseness,
                Ogre::Vector2(vertexBR.x, newVertexTL.y), Ogre::Vector2(newVertexBR.x, vertexTL.y), oldWidth);
            break;
        case QuadTreeNodeLocation::BOTTOM_RIGHT:
            newVertexTL = vertexTL + Ogre::Vector2(-oldWidth, -oldWidth);
            mRoot = new TNode(mMaxData, mMaxDepth, newLooseness, newVertexTL, vertexBR, newWidth);

            mRoot->mNodes[QuadTreeNodeLocation::TOP_LEFT]
                = new TNode(mMaxData, oldMaxDepth, oldLooseness, newVertexTL, vertexTL, oldWidth);
            mRoot->mNodes[QuadTreeNodeLocation::BOTTOM_LEFT] = new TNode(mMaxData, oldMaxDepth, oldLooseness,
                Ogre::Vector2(newVertexTL.x, vertexTL.y), Ogre::Vector2(vertexTL.x, vertexBR.y), oldWidth);
            mRoot->mNodes[QuadTreeNodeLocation::BOTTOM_RIGHT] = oldroot;
            mRoot->mNodes[QuadTreeNodeLocation::TOP_RIGHT] = new TNode(mMaxData, mMaxDepth, oldLooseness,
                Ogre::Vector2(vertexTL.x, newVertexTL.y), Ogre::Vector2(vertexBR.x, vertexTL.y), oldWidth);
            break;
        case QuadTreeNodeLocation::TOP_RIGHT:
            newVertexTL = vertexTL + Ogre::Vector2(-oldWidth, 0.0f);
            newVertexBR = vertexBR + Ogre::Vector2(0.0f, oldWidth);
            mRoot = new TNode(mMaxData, mMaxDepth, newLooseness, newVertexTL, newVertexBR, newWidth);

            mRoot->mNodes[QuadTreeNodeLocation::TOP_LEFT] = new TNode(
                mMaxData, oldMaxDepth, oldLooseness, newVertexTL, Ogre::Vector2(vertexTL.x, vertexBR.y), oldWidth);
            mRoot->mNodes[QuadTreeNodeLocation::BOTTOM_LEFT] = new TNode(mMaxData, oldMaxDepth, oldLooseness,
                Ogre::Vector2(newVertexTL.x, vertexBR.y), Ogre::Vector2(vertexTL.x, newVertexBR.y), oldWidth);
            mRoot->mNodes[QuadTreeNodeLocation::BOTTOM_RIGHT] = new TNode(
                mMaxData, oldMaxDepth, oldLooseness, Ogre::Vector2(vertexTL.x, vertexBR.y), newVertexBR, oldWidth);
            mRoot->mNodes[QuadTreeNodeLocation::TOP_RIGHT] = oldroot;
            break;
        default:
            Throw(IllegalArgumentException, "unknown NodeLocation type.");
        }
    }

    template <class TData, class TNode>
    void TLooseQuadTree<TData, TNode>::extend(const Ogre::Real& u, const Ogre::Real& v)
    {
        // grow the root node according to the position of the point
        if (u < mRoot->mVertexTL.x)
        {
            if (v < mRoot->mVertexTL.y)
            {
                createNewRootNode(QuadTreeNodeLocation::BOTTOM_RIGHT);
                extend(u, v);
            }
            else if (v > mRoot->mVertexBR.y)
            {
                createNewRootNode(QuadTreeNodeLocation::TOP_RIGHT);
                extend(u, v);
            }
            else
            {
                createNewRootNode(QuadTreeNodeLocation::BOTTOM_RIGHT);
                extend(u, v);
            }
        }
        else if (u > mRoot->mVertexBR.x)
        {
            if (v < mRoot->mVertexTL.y)
            {
                createNewRootNode(QuadTreeNodeLocation::BOTTOM_LEFT);
                extend(u, v);
            }
            else if (v > mRoot->mVertexBR.y)
            {
                createNewRootNode(QuadTreeNodeLocation::TOP_LEFT);
                extend(u, v);
            }
            else
            {
                createNewRootNode(QuadTreeNodeLocation::BOTTOM_LEFT);
                extend(u, v);
            }
        }
        else //  u is inside
        {
            if (v < mRoot->mVertexTL.y)
            {
                createNewRootNode(QuadTreeNodeLocation::BOTTOM_RIGHT);
                extend(u, v);
            }
            else if (v > mRoot->mVertexBR.y)
            {
                createNewRootNode(QuadTreeNodeLocation::TOP_RIGHT);
                extend(u, v);
            }
        }
    }

    template <class TData, class TNode> void TLooseQuadTree<TData, TNode>::removeAll()
    {
        delete mRoot;
        mRoot = NULL;
    }

    template <class TData, class TNode>
    TLooseQuadTree<TData, TNode>* TLooseQuadTree<TData, TNode>::find(const Ogre::Vector3& position)
    {
        TLooseQuadTree<TData, TNode>* result = NULL;
        if (mRoot != NULL)
        {
            result = mRoot->find(position);
        }
        return result;
    }

}; /* world */

#endif /* __QUADTREE_H__ */
