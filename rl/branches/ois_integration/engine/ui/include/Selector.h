/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __RL_SELECTOR_H__
#define __RL_SELECTOR_H__

#include "UiPrerequisites.h"
#include "SceneQuery.h"
#include "GameObject.h"
#include "LineSetPrimitive.h"
#include "DebugVisualisable.h"

namespace rl {

    /// Superclass for all Selectors in RL.
    /// Selectors provide a way to query for GameObjects in a game.
    /// Either in the scene, or inventory, or anywhere else.
    class _RlUiExport Selector : public DebugVisualisable
    {
    public:
        Selector(unsigned long mask = 0xffffffff);
        virtual ~Selector();
        virtual void updateSelection() = 0;

        void setSelectionMask(unsigned long mask);
        unsigned long getSelectionMask() const;

    protected:
        unsigned long mSelectionMask;
    };

    /// Superclass for Selectors that select a single GameObject.
    class _RlUiExport SingleSelector : public Selector
    {
    public:
        SingleSelector(unsigned long mask = 0xffffffff);

        virtual void updateSelection();

        GameObject* getSelectedObject() const;

    protected:
        GameObject* mSelectedObject;

        virtual const ActorVector& doExecuteQuery() = 0;
    };

    /// A Selector that selects the first unoccluded GameObject the ray hits.
    class _RlUiExport RaySingleSelector : public SingleSelector
    {
    public:
        RaySingleSelector(unsigned long mask = 0xffffffff);

        void setRay(const Ogre::Vector3& start, const Ogre::Vector3& end);

        // Overrides from DebugVisualisable

        virtual DebugVisualisableFlag getFlag() const;
        virtual void updatePrimitive();

    protected:
        RaySceneQuery mQuery;

        virtual const ActorVector& doExecuteQuery();
        virtual void doCreatePrimitive();
    };

    /// Selects this GameObject within a half sphere pointing to -Z,
    /// that has the smallest angle difference to the -Z-Axis.
    class _RlUiExport HalfSphereSingleSelector : public SingleSelector
    {
    public:
        HalfSphereSingleSelector(Ogre::SceneManager* smgr, unsigned long mask = 0xffffffff);

        virtual void updateSelection();

        void setRadius(Ogre::Real radius);

        void setPosition(const Ogre::Vector3& pos);

        void setOrientation(const Ogre::Quaternion& ori);

        /// If check is true, visibility is checked from the POV of the Actor reference
        /// This is currently done by casting a ray from the Actor to the candidate GOs
        void setCheckVisibility(bool check, Actor* reference = NULL);

        // Overrides from DebugVisualisable

        virtual DebugVisualisableFlag getFlag() const;
        virtual void updatePrimitive();

    protected:
        HalfSphereSceneQuery mQuery;
        bool mCheckVisibility;
        Actor* mReferenceActor;

        virtual const ActorVector& doExecuteQuery();

        virtual void doCreatePrimitive();
    };

}
#endif

