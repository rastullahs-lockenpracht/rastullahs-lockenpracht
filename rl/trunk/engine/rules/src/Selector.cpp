/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include "Selector.h"
#include "SelectionHelper.h"

#include "CoreSubsystem.h"
#include "PhysicsManager.h"
#include "World.h"

using namespace Ogre;

namespace rl
{
    CreatureSelectionFilter::CreatureSelectionFilter()
        : mAlignment(Creature::ALIGNMENT_ALLY |
                     Creature::ALIGNMENT_NEUTRAL |
                     Creature::ALIGNMENT_ENEMY)
    {
    }

    void CreatureSelectionFilter::setAlignmentMask(unsigned int mask)
    {
        mAlignment = mask;
    }

    bool CreatureSelectionFilter::pass(GameObject* go) const
    {
        Creature* creature = dynamic_cast<Creature*>(go);
        if (creature != NULL)
        {
            return (mAlignment & creature->getAlignment()) != 0;
        }
        return false;
    }

    InSceneSelectionFilter::InSceneSelectionFilter()
    {
    }

    bool InSceneSelectionFilter::pass(GameObject* go) const
    {
        if (go->getState() == GOS_IN_SCENE)
        {
            return true;
        }
        return false;
    }

    Selector::Selector(unsigned long mask) 
		: DebugVisualisable(), mSelection(), mSelectionMask(mask), mFilter(NULL)
    {
    }

    Selector::~Selector()
    {
    }

    void Selector::setFilter(SelectionFilter* filter)
    {
        mFilter = filter;
    }

    SelectionFilter* Selector::getFilter() const
    {
        return mFilter;
    }

    void Selector::setSelectionMask(unsigned long mask)
    {
        mSelectionMask = mask;
    }

    unsigned long Selector::getSelectionMask() const
    {
        return mSelectionMask;
    }

    GameObject* Selector::getFirstSelectedObject() const
    {
        return mSelection.empty() ? NULL : mSelection[0];
    }

    const Selector::GameObjectVector& Selector::getAllSelectedObjects() const
    {
        return mSelection;
    }

	unsigned int Selector::getSelectionCount() const
	{
		return mSelection.size();
	}

    void Selector::updateSelection()
    {
        // Remove old selection
        mSelection.clear();

        // Do the query, results are in proper order
        const ActorVector& actors = doExecuteQuery();

        // Set selection to first result with a GameObject attached.
        for (ActorVector::const_iterator it = actors.begin(), end = actors.end();
            it != end; ++it)
        {
            Actor* actor = *it;
            GameObject* go = static_cast<GameObject*>(actor->getGameObject());
            if (go != NULL && (go->getQueryFlags() & mSelectionMask) && filter(go))
            {
                mSelection.push_back(go);
            }
        }
    }

    bool Selector::filter(GameObject* go)
    {
        return mFilter == NULL || mFilter->pass(go);
    }

    //------------------------------------------------------------------------

    RaySelector::RaySelector(unsigned long mask, bool useOgreQuery)
        : Selector(mask)
    {
		if (useOgreQuery)
		{
			mQuery = new OgreRaySceneQuery(mask);
		}
		else
		{
			mQuery = new RaySceneQuery(mask);
		}
    }

    void RaySelector::setRay(const Ogre::Vector3& start, const Ogre::Vector3& end)
    {
        mQuery->setRay(start, end);
    }

    // Overrides from DebugVisualisable
    DebugVisualisableFlag RaySelector::getFlag() const
    {
        return DVF_CONTROL;
    }

    void RaySelector::updatePrimitive()
    {
        LineSetPrimitive* lineSet = static_cast<LineSetPrimitive*>(mPrimitive);
        lineSet->clear();
        lineSet->addLine(mQuery->getRayStart(), mQuery->getRayEnd(), ColourValue::Red);
    }

    const ActorVector& RaySelector::doExecuteQuery()
    {
        return mQuery->execute();
    }

    void RaySelector::doCreatePrimitive()
    {
        mPrimitive = new LineSetPrimitive();
    }

	RaySelector::~RaySelector()
	{
		delete mQuery;
	}

    //////////////////////////////////////////////////////////////////////////

    HalfSphereSelector::HalfSphereSelector(Ogre::SceneManager* smgr,
        unsigned long mask)
        : Selector(mask),
          mQuery(smgr, mask),
          mCheckVisibility(false),
          mLoSReferenceGo(NULL),
		  mTrackedGo(NULL)
    {
    }

    HalfSphereSelector::HalfSphereSelector(unsigned long mask)
        : Selector(mask),
		  mQuery(CoreSubsystem::getSingleton().getWorld()->getSceneManager(), mask),
          mCheckVisibility(false),
          mLoSReferenceGo(NULL),
		  mTrackedGo(NULL)
    {
    }

    void HalfSphereSelector::updateSelection()
    {
        // Remove old selection
        mSelection.clear();

        // Auto tracking enabled?
        if (mTrackedGo != NULL)
        {
            // If not in scene, then we're done
            if ((mTrackedGo->getState() & GOS_IN_SCENE) == 0)
            {
                return;
            }

            // Set query transform according to position and orientation of tracked GO.
            setPosition(mTrackedGo->getPosition());
            setOrientation(mTrackedGo->getOrientation());
        }

        // Do the query, results are in proper order
        const ActorVector& actors = doExecuteQuery();

        // Set selection to first result with a GameObject attached.
        for (ActorVector::const_iterator it = actors.begin(), end = actors.end();
            it != end; ++it)
        {
            Actor* actor = *it;
            GameObject* go = static_cast<GameObject*>(actor->getGameObject());
            if (go != NULL && (go->getQueryFlags() & mSelectionMask))
            {
                // Is this GO seen when we need it to be seen?
                if (mCheckVisibility && mLoSReferenceGo &&
                    !SelectionHelper::checkLineOfSight(mLoSReferenceGo, go))
                {
                    // Nope, check next.
                    continue;
                }
                else if (filter(go))
                {
                    // Ok, LoS either not needed or given.
                    mSelection.push_back(go);
                }
            }
        }
    }

    void HalfSphereSelector::updatePrimitive()
    {
        LineSetPrimitive* lineSet = static_cast<LineSetPrimitive*>(mPrimitive);
        lineSet->clear();

        Vector3 pos = mQuery.getPosition();
        Quaternion ori = mQuery.getOrientation();
        Real radius = mQuery.getRadius();

        lineSet->addLine(pos, pos + radius * (ori * Vector3::UNIT_X), ColourValue::Red);
        lineSet->addLine(pos, pos + radius * (ori * Vector3::UNIT_Y), ColourValue::Green);
        lineSet->addLine(pos, pos + radius * (ori * Vector3::NEGATIVE_UNIT_Z),
            ColourValue::Blue);
    }

    void HalfSphereSelector::setRadius(Ogre::Real radius)
    {
        mQuery.setRadius(radius);
    }

    void HalfSphereSelector::setPosition(const Ogre::Vector3& pos)
    {
        mQuery.setPosition(pos);
    }

    void HalfSphereSelector::setOrientation(const Ogre::Quaternion& ori)
    {
        mQuery.setOrientation(ori);
    }

    void HalfSphereSelector::setCheckVisibility(bool check, GameObject* reference)
    {
        mCheckVisibility = check;
        mLoSReferenceGo = reference;
    }

    void HalfSphereSelector::track(GameObject* go)
    {
        mTrackedGo = go;
    }

    DebugVisualisableFlag HalfSphereSelector::getFlag() const
    {
        return DVF_CONTROL;
    }

    const ActorVector& HalfSphereSelector::doExecuteQuery()
    {
        return mQuery.execute();
    }

    void HalfSphereSelector::doCreatePrimitive()
    {
        mPrimitive = new LineSetPrimitive();
    }
}
