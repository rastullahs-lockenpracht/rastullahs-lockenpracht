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

#ifndef __RL_SELECTOR_H__
#define __RL_SELECTOR_H__

#include "RulesPrerequisites.h"
#include "SceneQuery.h"
#include "GameObject.h"
#include "Creature.h"
#include "LineSetPrimitive.h"
#include "DebugVisualisable.h"

namespace rl {

    class _RlRulesExport SelectionFilter
    {
    public:
        virtual ~SelectionFilter() {}
        /// Return true, if the GameObject passes the filter, false else.
        virtual bool pass(GameObject*) const = 0;
    };

    class _RlRulesExport CreatureSelectionFilter : public SelectionFilter
    {
    public:
        CreatureSelectionFilter();

        void setAlignmentMask(unsigned int);

        /// Return true, if the GameObject is a Creature with properties as set in the filter.
        virtual bool pass(GameObject*) const;
    private:
        /// Only have alignment here. Can be mademore generic, if needed later on.
        unsigned int mAlignment;
    };

    class _RlRulesExport InSceneSelectionFilter : public SelectionFilter
    {
    public:
        InSceneSelectionFilter();

        /// Return true, if the GameObject is in scene with properties as set in the filter.
        virtual bool pass(GameObject*) const;
    };

    /// Superclass for all Selectors in RL.
    /// Selectors provide a way to query for GameObjects in a game.
    /// Either in the scene, or inventory, or anywhere else.
    class _RlRulesExport Selector : public DebugVisualisable
    {
    public:
        typedef std::vector<GameObject*> GameObjectVector;

        Selector(unsigned long mask = 0xffffffff);
        virtual ~Selector();
        virtual void updateSelection();

        void setSelectionMask(unsigned long mask);
        unsigned long getSelectionMask() const;

        /// Set an additional filter to narrow down selection. Set NULL, to unset filtering.
        void setFilter(SelectionFilter*);
        SelectionFilter* getFilter() const;

        GameObject* getFirstSelectedObject() const;
        const GameObjectVector& getAllSelectedObjects() const;
		unsigned int getSelectionCount() const;

    protected:
        GameObjectVector mSelection;
        unsigned long mSelectionMask;
        SelectionFilter* mFilter;

        virtual const ActorVector& doExecuteQuery() = 0;
        /// Apply the filter if set, or just return true, if no selection filter is set.
        virtual bool filter(GameObject*);
    };


    /// A Selector that selects all unoccluded GameObject the ray hits.
    class _RlRulesExport RaySelector : public Selector
    {
    public:
        RaySelector(unsigned long mask = 0xffffffff, bool useOgreQuery = false);
		~RaySelector();

        void setRay(const Ogre::Vector3& start, const Ogre::Vector3& end);

        // Overrides from DebugVisualisable

        virtual DebugVisualisableFlag getFlag() const;
        virtual void updatePrimitive();

    protected:
        RaySceneQuery* mQuery;

        virtual const ActorVector& doExecuteQuery();
        virtual void doCreatePrimitive();
    };

    /// Selects this GameObject within a half sphere pointing to -Z,
    /// that has the smallest angle difference to the -Z-Axis.
    class _RlRulesExport HalfSphereSelector : public Selector
    {
    public:
        HalfSphereSelector(Ogre::SceneManager* smgr, unsigned long mask = 0xffffffff);
		HalfSphereSelector(unsigned long mask = 0xffffffff);

        virtual void updateSelection();

        void setRadius(Ogre::Real radius);

        void setPosition(const Ogre::Vector3& pos);

        void setOrientation(const Ogre::Quaternion& ori);

        /// Instead of using transform set with setPosition/Orientation, use the
        /// GameObject's transform. Set to NULL to disable tracking.
        void track(GameObject* go);

        /// If check is true, visibility is checked from the POV of the GameObject reference
        /// This is currently done by casting a ray from the GameObject to the candidate GOs
        void setCheckVisibility(bool check, GameObject* reference = NULL);

        // Overrides from DebugVisualisable

        virtual DebugVisualisableFlag getFlag() const;
        virtual void updatePrimitive();

    protected:
        HalfSphereSceneQuery mQuery;
        bool mCheckVisibility;
        GameObject* mLoSReferenceGo;
        GameObject* mTrackedGo;

        virtual const ActorVector& doExecuteQuery();

        virtual void doCreatePrimitive();
    };
}
#endif

