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

#include "PhysicsManager.h"

using namespace Ogre;

namespace rl
{
    Selector::Selector(unsigned long mask) : mSelectionMask(mask)
    {
    }

    Selector::~Selector()
    {
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
            if (go != NULL && (go->getQueryFlags() & mSelectionMask))
            {
                mSelection.push_back(go);
            }
        }
    }

    //------------------------------------------------------------------------

    RaySelector::RaySelector(unsigned long mask)
        : Selector(mask), mQuery(mask)
    {
    }

    void RaySelector::setRay(const Ogre::Vector3& start, const Ogre::Vector3& end)
    {
        mQuery.setRay(start, end);
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
        lineSet->addLine(mQuery.getRayStart(), mQuery.getRayEnd(), ColourValue::Red);
    }

    const ActorVector& RaySelector::doExecuteQuery()
    {
        return mQuery.execute();
    }

    void RaySelector::doCreatePrimitive()
    {
        mPrimitive = new LineSetPrimitive();
    }

    //////////////////////////////////////////////////////////////////////////

    HalfSphereSelector::HalfSphereSelector(Ogre::SceneManager* smgr,
        unsigned long mask)
        : Selector(mask),
          mQuery(smgr, mask),
          mCheckVisibility(false),
          mReferenceGo(NULL)
    {
    }

    void HalfSphereSelector::updateSelection()
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
            if (go != NULL && (go->getQueryFlags() & mSelectionMask))
            {
                // Is this GO seen when we need it to be seen?
                if (mCheckVisibility && mReferenceGo &&
                    !SelectionHelper::checkLineOfSight(mReferenceGo, go))
                {
                    // Nope, check next.
                    continue;
                }
                // Ok, LoS either not needed or given.
                mSelection.push_back(go);
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
        mReferenceGo = reference;
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
