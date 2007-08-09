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

#ifndef __CombatCharacterController_H__
#define __CombatCharacterController_H__

#include "UiPrerequisites.h"

#include "ControlState.h"
#include "Combatant.h"
#include "Selector.h"

#include <OgreRectangle.h>

namespace rl {

	class CameraObject;
    class Combat;
    class CombatManager;
    class CombatWindow;

	class _RlUiExport CombatControlState : public ControlState, public Combatant
	{
	public:
		/**
		*  @throw NullPointerException if camera is NULL.
		*/
		CombatControlState(CommandMapper* cmdMapper, Actor* camera, Person* character);
		virtual ~CombatControlState();

        virtual void pause();
        virtual void resume();

		void run(Ogre::Real elapsedTime);

        // Combatant override
        virtual Ogre::String getCombatantTypeName() const;

    private:
        CombatManager* mCombatManager;
        // Current combat that is controlled.
        Combat* mCombat;
        CombatWindow* mCombatWindow;
        HalfSphereSelector mEnemySelector;

        CameraObject* mCamera;

        /// Little Helper-MO for visualisation for everything that is difficult with cegui.
        Ogre::ManualObject* mHud;

        Ogre::Rectangle getScreenRectFromWorldAABB(const Ogre::AxisAlignedBox& aabb) const;
	};
}
#endif
