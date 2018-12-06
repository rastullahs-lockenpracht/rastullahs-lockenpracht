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

#ifndef __RL_DEBUG_VISUALISABLE_H__
#define __RL_DEBUG_VISUALISABLE_H__

#include "CorePrerequisites.h"

namespace rl
{

    class PrimitiveObject;

    /// Flags to decern visualised elements. This enum can be used
    /// To only display the debug graphics for classes of visualisables.
    typedef enum
    {
        DVF_AREA = 1, ///< Flag used for all kinds of game area types
        DVF_BOT = 2, ///< Flag used for bot(-thinking) visualisation
        DVF_CONTROL = 4, ///< Flag for visualisation of user control
        DVF_WAYPOINT = 8, ///< Flag for visualisation of way points
    } DebugVisualisableFlag;

    /// Abstract base class for classes that provide visual debug helpers.
    /// This class encapsules a PrimitiveObject, that can help with debugging,
    /// by providing visible cues.
    /// @note This class' constructor automatically registers itself with the
    ///       DebugVisualsManager.
    class _RlCoreExport DebugVisualisable
    {
    public:
        DebugVisualisable();
        virtual ~DebugVisualisable();

        /// Shows this Visualisable in the scene.
        void show();
        /// Hides this Visualisable.
        void hide();

        /// Returns true, if this is shown in the scene.
        bool isShown() const;

        /// Provides the flag of the kind of visualisation done in the class
        /// This flag is used by the DebugVisualsManager to show/hide them by
        /// their flags.
        /// Has to be overridden by child classes.
        virtual DebugVisualisableFlag getFlag() const = 0;

        /// Called to update the debug primitive. Redrawing is done in here.
        virtual void updatePrimitive() = 0;

    protected:
        /// mSceneNode is created lazily, but before first call of updatePrimitive.
        Ogre::SceneNode* mSceneNode;
        PrimitiveObject* mPrimitive;
        bool mIsShown;

        /// Does the actual creation of the debug primitive.
        /// Since the actual primitive class is very specific to the data to be visualised
        /// this has to be delegated to the subclass
        virtual void doCreatePrimitive() = 0;
    };
}
#endif
