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

#include "CombatCharacterController.h"

#include "Actor.h"
#include "CameraObject.h"
#include "Combat.h"
#include "CombatManager.h"
#include "CombatWindow.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "InputManager.h"
#include "MeshObject.h"
#include "Person.h"
#include "PhysicalThing.h"
#include "Selector.h"
#include "WindowFactory.h"
#include "World.h"

#include <OgreManualObject.h>
using namespace Ogre;

namespace rl {
    CombatCharacterController::CombatCharacterController(CommandMapper* cmdMapper,
        Actor* camera, Person* character)
        : ControlState(cmdMapper, camera, character, CST_COMBAT),
          mCombatManager(CombatManager::getSingletonPtr()),
          mCombat(NULL),
          mCombatWindow(NULL),
          mEnemySelector(CoreSubsystem::getSingleton().getWorld()->getSceneManager(),
            QUERYFLAG_CREATURE),
          mCamera(NULL)
    {
        CreatureSelectionFilter* filter = new CreatureSelectionFilter();
        filter->setAlignmentMask(Creature::ALIGNMENT_ENEMY);
        mEnemySelector.setFilter(filter);

        mCombatWindow = WindowFactory::getSingleton().getCombatWindow();
        mCamera = static_cast<CameraObject*>(mCameraActor->getControlledObject());

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

	CombatCharacterController::~CombatCharacterController()
    {
        delete mEnemySelector.getFilter();
    }

    void CombatCharacterController::resume()
    {
        mCameraActor->getPhysicalThing()->freeze();
        mCharacterActor->getPhysicalThing()->freeze();

        ///\todo Richtig machen, nur temporär Ani hier setzen.
        static_cast<MeshObject*>(mCharacterActor->getControlledObject())
            ->startAnimation("kampf_schwerter_idle");

        // Set reference to character
        mEnemySelector.setCheckVisibility(true, mCharacter);
        mEnemySelector.track(mCharacter);
        mEnemySelector.setRadius(10.0);

        // Is there a combat running already?
        if (mCombatManager->getCurrentCombat() != NULL)
        {
            // Yes. Set this one as active.
            mCombat = mCombatManager->getCurrentCombat();
        }
        else
        {
            // No. Test, if we can start one.
            mEnemySelector.updateSelection();
            const Selector::GameObjectVector& enemies = mEnemySelector.getAllSelectedObjects();
            if (!enemies.empty())
            {
                // There are enemies in vicinity, so start a new combat and set it up properly.
                mCombat = mCombatManager->startCombat(mCharacter,
                    static_cast<Creature*>(enemies[0]));
                for (size_t i = 1; i < enemies.size(); ++i)
                {
                    mCombat->addOpponent(static_cast<Creature*>(enemies[i]));
                }
            }
            else
            {
                // Oops. Nothing to fight. Pop self.
                InputManager::getSingleton().popControlState();
                return;
            }
        }
        mCombatWindow->setVisible(true);
    }

    void CombatCharacterController::pause()
    {
        mCombatWindow->setVisible(false);

        mCameraActor->getPhysicalThing()->unfreeze();
        mCharacterActor->getPhysicalThing()->unfreeze();
        static_cast<MeshObject*>(mCharacterActor->getControlledObject())->stopAllAnimations();

        // reset current combat, in order to avoid a potential dangling pointer
        mCombat = NULL;
    }

	void CombatCharacterController::run(Ogre::Real elapsedTime)
    {
        // HUD aktualisieren.
        mHud->clear();
        
        mHud->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_STRIP);
        const Combat::CreatureSet& opponents = mCombat->getAllOpponents();
        for (Combat::CreatureSet::const_iterator it = opponents.begin(), end = opponents.end();
            it != end; ++it)
        {
            Ogre::Rectangle rec = getScreenRectFromWorldAABB(
                (*it)->getActor()->_getSceneNode()->_getWorldAABB());
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

    Ogre::Rectangle CombatCharacterController::getScreenRectFromWorldAABB(
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

        Ogre::Rectangle rval = {left,top, right, bottom};
        return rval;
    }
}
