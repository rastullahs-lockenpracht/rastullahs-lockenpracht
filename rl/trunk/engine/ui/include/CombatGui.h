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

#ifndef __CombatGui_H__
#define __CombatGui_H__

#include "UiPrerequisites.h"

#include "CameraObject.h"
#include "Combat.h"
#include "CombatWindow.h"

namespace rl {
	
	/// GUI for user control of combat.
	/// Handles all graphical combat visualisation.
	class _RlUiExport CombatGui
	{
	public:
		CombatGui(Combat*, CameraObject* mCamera);
		virtual ~CombatGui();

		void update();

		void show();
		void hide();

	private:
		CombatWindow* mCombatWindow;
		Combat* mCombat;
		CameraObject* mCamera;
        /// Little Helper-MO for visualisation for everything that is difficult with cegui.
        Ogre::ManualObject* mHud;

        Ogre::Rectangle getScreenRectFromWorldAabb(const Ogre::AxisAlignedBox& aabb) const;
	};
}

#endif
