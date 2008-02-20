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
#include "stdinc.h" //precompiled header

#include "CombatGui.h"

#include "Combatant.h"
#include "CoreSubsystem.h"
#include "CreatureController.h"
#include "CreatureControllerManager.h"
#include "WindowFactory.h"
#include "World.h"

using namespace Ogre;

namespace rl {

	CombatGui::CombatGui(Combat* combat, CameraObject* camera)
		: mCombatWindow(NULL),
		  mCombat(combat),
		  mCamera(camera)
    {
        mCombatWindow = WindowFactory::getSingleton().getCombatWindow();

        // Initialise HUD-MO. Put it into 2D mode and make sure it is always rendered.
        SceneManager* sceneMgr = CoreSubsystem::getSingleton().getWorld()->getSceneManager();
        mHud = sceneMgr->createManualObject("__COMBAT_HUD__");
        mHud->setUseIdentityProjection(true);
        mHud->setUseIdentityView(true);
        AxisAlignedBox infiniteAabb;
        infiniteAabb.setInfinite();
        mHud->setBoundingBox(infiniteAabb);
        mHud->setRenderQueueGroup(RENDER_QUEUE_OVERLAY);
        sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mHud);
	}

	CombatGui::~CombatGui()
	{
		hide();
        SceneManager* sceneMgr = CoreSubsystem::getSingleton().getWorld()->getSceneManager();
		sceneMgr->destroyManualObject(mHud);
	}

	void CombatGui::update()
	{
        // update HUD
        mHud->clear();

        mHud->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_STRIP);
        const Combat::CombatantSet& opponents = mCombat->getAllOpponents();
        for (Combat::CombatantSet::const_iterator it = opponents.begin(), end = opponents.end();
            it != end; ++it)
        {
            Ogre::Rectangle rec = getScreenRectFromWorldAabb(
                (*it)->getCreatureController()->getCreature()->getActor()
                    ->_getSceneNode()->_getWorldAABB());
            mHud->position(rec.left,  rec.top,    0.0f);
            mHud->position(rec.left,  rec.bottom, 0.0f);
            mHud->position(rec.right, rec.top,    0.0f);
            mHud->position(rec.right, rec.bottom, 0.0f);

            mHud->index(0);
            mHud->index(1);
            mHud->index(3);
            mHud->index(2);
            mHud->index(0);
        }
        mHud->end();
	}

	void CombatGui::show()
	{
        mCombatWindow->setVisible(true);
	}

	void CombatGui::hide()
	{
        mCombatWindow->setVisible(false);
	}

    Ogre::Rectangle CombatGui::getScreenRectFromWorldAabb(
        const AxisAlignedBox& aabb) const
    {
        // Initialise each to the value of the opposite side, so that min/max work smoothly.
        Real left = 1.0f, bottom = 1.0f, right = -1.0f, top = -1.0f;

        // Determine screen pos of all corners and widen the rect if needed
        const Vector3* corners = aabb.getAllCorners();
        for (size_t i = 0; i < 8; ++i)
        {
			Vector3 screenSpacePos = mCamera->getPointOnScreen(corners[i]);
            if (screenSpacePos.z > 0) continue; // Behind camera

            left   = std::min(left,   screenSpacePos.x);
            right  = std::max(right,  screenSpacePos.x);
            bottom = std::min(bottom, screenSpacePos.y);
            top    = std::max(top,    screenSpacePos.y);
        }

        Ogre::Rectangle rval = {left, top, right, bottom};
        return rval;
    }
}
