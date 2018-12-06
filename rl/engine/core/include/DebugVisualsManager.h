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

#ifndef __RL_DEBUG_VISUALS_MANAGER_H__
#define __RL_DEBUG_VISUALS_MANAGER_H__

#include "CorePrerequisites.h"
#include "GameTask.h"

#include <set>

namespace rl {

    class DebugVisualisable;

    /// Manager class for the DebugVisualisables. Those are registered and unregistered here
    /// and visualisation and updating them is managed by this class.
    /// @see rl::DebugVisualisable
    class _RlCoreExport DebugVisualsManager
        : public GameTask,
          public Ogre::Singleton<DebugVisualsManager>
    {
    public:
        DebugVisualsManager();
        virtual ~DebugVisualsManager();

        /// Shows all visualisables, whose flags are set.
        /// @param flags Flags to compare to.
        void show(unsigned long flags);

        /// Hides all visualisables, whose flags are set.
        /// @param flags Flags to compare to.
        void hide(unsigned long flags);

        /// Shows all registered visualisables.
        void showAll();

        /// Hides all registered visualisables.
        void hideAll();

        /// Toggles visiblitity for all registered visualisables.
        void toggleAll();

        void registerDebugVisualisable(DebugVisualisable*);
        void unregisterDebugVisualisable(DebugVisualisable*);

        virtual void run(Ogre::Real elapsedTime);
        virtual const Ogre::String& getName() const;

    private:
        typedef std::set<DebugVisualisable*> VisualisableSet;
        VisualisableSet mVisualisables;
    };
}
#endif
