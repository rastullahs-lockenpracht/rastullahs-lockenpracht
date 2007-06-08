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

namespace rl {
    CombatCharacterController::CombatCharacterController(CommandMapper* cmdMapper,
        Actor* camera, Person* character)
        : CharacterController(cmdMapper, camera, character, CST_COMBAT),
          mCombatManager(CombatManager::getSingletonPtr()),
          mCombat(NULL),
          mCombatWindow(NULL),
          mEnemySelector(CoreSubsystem::getSingleton().getWorld()->getSceneManager(),
            QUERYFLAG_CREATURE)
    {
        CreatureSelectionFilter* filter = new CreatureSelectionFilter();
        filter->setAlignmentMask(Creature::ALIGNMENT_ENEMY);
        mEnemySelector.setFilter(filter);

        mCombatWindow = WindowFactory::getSingleton().getCombatWindow();
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
    }
}
