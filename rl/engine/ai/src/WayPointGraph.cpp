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
#include "stdinc.h" //precompiled header

#include "WayPointGraph.h"

#include <algorithm>

#include "ConfigurationManager.h"
#include "ContentModule.h"
#include "CoreSubsystem.h"
#include "Exception.h"
#include "LineSetPrimitive.h"
#include "WayPointNode.h"
#include "World.h"

using namespace std;
using namespace Ogre;

namespace rl
{

    WayPointGraph::WayPointGraph()
        : mRoot(NULL)
        , mChanged(false)
    {
    }

    WayPointGraph::~WayPointGraph()
    {
        WayPointNodeList::iterator it;

        for (it = mNodeList.begin(); it != mNodeList.end(); it++)
        {
            delete *it;
        }
        // the scenenode seems
        // already deleted when this destructor gets called.

        // Remove debug scene node from root scene note, if debugview was used.
        if (mSceneNode != NULL && mSceneNode->getParent() != NULL)
        {
            CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->getRootSceneNode()->removeChild(
                mSceneNode);
        }
    }

    WayPointNode* WayPointGraph::addWayPoint(const Vector3& position, const WayPointNode::WayPointNodeType type)
    {
        WayPointNodeList::iterator it;

        for (it = mNodeList.begin(); it != mNodeList.end(); it++)
        {
            // equal to some waypoint already added ...
            if ((*it)->getPosition() == position)
                return NULL;
        }

        return rawAddWayPoint(position, type);
    }

    WayPointNode* WayPointGraph::rawAddWayPoint(const Vector3& position, const WayPointNode::WayPointNodeType type)
    {
        WayPointNode* newWayPoint = new WayPointNode(position, type);
        mNodeList.push_back(newWayPoint);
        mChanged = true;

        return newWayPoint;
    }

    void WayPointGraph::addConnection(WayPointNode* wp1, WayPointNode* wp2)
    {
        wp1->addNeighbour(wp2);
        wp2->addNeighbour(wp1);
        mChanged = true;
    }

    void WayPointGraph::addDirectedConnection(WayPointNode* wp1, const WayPointNode* wp2)
    {
        wp1->addNeighbour(wp2);
        mChanged = true;
    }

    void WayPointGraph::load(const Ogre::String& filename, const Ogre::String& resourceGroup)
    {
        Ogre::String group = resourceGroup;
        if (group.empty())
        {
            group = CoreSubsystem::getSingleton().getActiveAdventureModule()->getId();
        }

        tinyxml2::XMLDocument* doc = loadDocument(filename, group);
        if (doc)
        {
            tinyxml2::XMLElement* rootElem = doc->RootElement();

            tinyxml2::XMLElement* nodesElem = getChildNamed(rootElem, "waypointnodes");
            std::map<int, WayPointNode*> lookupTable;
            for (tinyxml2::XMLNode* curNode = nodesElem->FirstChild(); curNode; curNode = curNode->NextSibling())
            {
                tinyxml2::XMLElement* curElem = curNode->ToElement();

                if (curElem && hasNodeName(curElem, "node"))
                {
                    Vector3 pos = getValueAsVector3(curElem);
                    CeGuiString typeS = getAttributeValueAsString(curElem, "type");
                    int id = getAttributeValueAsInteger(curElem, "id");

                    WayPointNode::WayPointNodeType type = WayPointNode::WP_UNKNOWN;
                    if (typeS == "int")
                    {
                        type = WayPointNode::WP_INTERIOR;
                    }
                    else if (typeS == "ext")
                    {
                        type = WayPointNode::WP_EXTERIOR;
                    }

                    WayPointNode* node = addWayPoint(pos, type);
                    lookupTable[id] = node;
                }
            }

            tinyxml2::XMLElement* edgesElem = getChildNamed(rootElem, "waypointedges");
            for (tinyxml2::XMLNode* curNode = edgesElem->FirstChild(); curNode; curNode = curNode->NextSibling())
            {
                tinyxml2::XMLElement* curElem = curNode->ToElement();

                if (curElem && hasNodeName(curNode, "edge"))
                {
                    tinyxml2::XMLElement* curElem = static_cast<tinyxml2::XMLElement*>(curNode);
                    int source = getAttributeValueAsInteger(curElem, "source");
                    int destination = getAttributeValueAsInteger(curElem, "destination");
                    addDirectedConnection(lookupTable[source], lookupTable[destination]);
                }
            }
        }
    }

    const WayPointNode* WayPointGraph::getNearestWayPoint(const Vector3& position) const
    {
        WayPointNodeList::const_iterator it;
        WayPointNode* nearestWayPoint = NULL;
        Vector3 nearestVec;
        Ogre::Real nearestDistance;

        // if list is empty simply return no waypoint
        if (mNodeList.begin() == mNodeList.end())
            return NULL;

        // first waypoint is the nearest at the beginning
        it = mNodeList.begin();
        nearestVec = position - (*it)->getPosition();
        nearestDistance = nearestVec.length();
        nearestWayPoint = (*it);

        // search the full list for points that are nearer
        for (it = mNodeList.begin(); it != mNodeList.end(); it++)
        {
            // calculate distance
            nearestVec = position - (*it)->getPosition();

            // test if distance is smaller than the smallest seen until now
            if (nearestDistance > nearestVec.length())
            {
                nearestDistance = nearestVec.length();
                nearestWayPoint = (*it);
            }
        }

        return nearestWayPoint;
    }

    const WayPointGraph::WayPointNodeList& WayPointGraph::getWayPointList() const
    {
        return mNodeList;
    }

    const WayPointNode* WayPointGraph::getWayPointAt(unsigned int index) const
    {
        if (index >= mNodeList.size())
            Throw(OutOfRangeException, "no data at specified index");

        return mNodeList[index];
    }

    DebugVisualisableFlag WayPointGraph::getFlag() const
    {
        return DVF_WAYPOINT;
    }

    void WayPointGraph::updatePrimitive()
    {
        if (mSceneNode->getParent() == NULL)
        {
            CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->getRootSceneNode()->addChild(mSceneNode);
            // mCharacterActor->_getSceneNode()->addChild(mSceneNode);
        }

        // avoid building graph again and again
        if (!mChanged)
            return;

        LineSetPrimitive* lineSet = static_cast<LineSetPrimitive*>(mPrimitive);

        lineSet->clear();

        // list storing added lines (for avoiding drawing bidirectional lines twice)
        std::multimap<const WayPointNode*, const WayPointNode*> edgeList;
        std::multimap<const WayPointNode*, const WayPointNode*>::iterator edgeListIt;

        WayPointNodeList::const_iterator it;
        Vector3 wp1Vec;
        Vector3 wp2Vec;

        for (it = mNodeList.begin(); it != mNodeList.end(); it++)
        {
            wp1Vec = (*it)->getPosition();
            // draw the waypoint itself
            lineSet->addLine(wp1Vec, wp1Vec + Vector3(0, 1, 0), Ogre::ColourValue::Red);

            const WayPointNode::WayPointWeightNodeList subnodes = (*it)->getNeighbours();
            WayPointNode::WayPointWeightNodeList::const_iterator nit;

            for (nit = subnodes.begin(); nit != subnodes.end(); nit++)
            {
                /*bool found = false;
                while ( (edgeListIt = edgeList.find( (*nit).second )) != edgeList.end() )
                {
                    if ( (*edgeListIt).second == (*it) )
                        found = true;
                }
                if (found)	// already 'drawn'
                    continue;
                    */

                lineSet->addLine(wp1Vec, (*nit).second->getPosition(), Ogre::ColourValue::Blue);
                edgeList.insert(pair<const WayPointNode*, const WayPointNode*>((*nit).second, (*it)));
            }
        }
        edgeList.clear();

        mChanged = false;
    }

    void WayPointGraph::doCreatePrimitive()
    {
        mPrimitive = new LineSetPrimitive();
    }
};
