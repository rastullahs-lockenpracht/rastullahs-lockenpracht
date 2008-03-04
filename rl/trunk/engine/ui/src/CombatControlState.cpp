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
#include "stdinc.h"

#include "CombatControlState.h"

#include "Actor.h"
#include "CameraObject.h"
#include "Combat.h"
#include "CombatGui.h"
#include "CombatManager.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "CreatureController.h"
#include "CreatureControllerManager.h"
#include "InputManager.h"
#include "MeshObject.h"
#include "Person.h"
#include "PhysicalThing.h"
#include "Selector.h"
#include "World.h"

#include <OgreManualObject.h>
using namespace Ogre;

namespace rl {
    CombatControlState::CombatControlState(CommandMapper* cmdMapper,
        Actor* camera, Person* character, Combat* combat)
        : ControlState(cmdMapper, camera, character, CST_COMBAT),
          Combatant(combat, CreatureControllerManager::getSingleton().getCreatureController(character)),
          mCombatManager(CombatManager::getSingletonPtr()),
		  mCombatGui(NULL),
          mEnemySelector(CoreSubsystem::getSingleton().getWorld()->getSceneManager(),
            QUERYFLAG_CREATURE),
          mCamera(NULL),
		  mState(ROUND_EXECUTION)
    {
        CreatureSelectionFilter* filter = new CreatureSelectionFilter();
        filter->setAlignmentMask(Creature::ALIGNMENT_ENEMY);
        mEnemySelector.setFilter(filter);

        mCamera = static_cast<CameraObject*>(mCameraActor->getControlledObject());
		mCombatGui = new CombatGui(mCombat, mCamera);
    }

	CombatControlState::~CombatControlState()
    {
		delete mCombatGui;
        delete mEnemySelector.getFilter();
    }

    void CombatControlState::resume()
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

        mEnemySelector.updateSelection();
        const Selector::GameObjectVector& enemies = mEnemySelector.getAllSelectedObjects();
        if (!enemies.empty())
        {
            for (size_t i = 0; i < enemies.size(); ++i)
            {
                Combatant* opponent = mCombatManager->createCombatant(
                    static_cast<Creature*>(enemies[i]));
                mCombat->addOpponent(opponent);
            }
        }
        else
        {
            // Oops. Nothing to fight. Pop self.
            InputManager::getSingleton().popControlState();
            return;
        }

		mCombatGui->show();


        // We want to play too..
        mCombat->addAlly(this);

        mCombat->start();
    }

    void CombatControlState::pause()
    {
		mCombatGui->hide();

        mCameraActor->getPhysicalThing()->unfreeze();
        mCharacterActor->getPhysicalThing()->unfreeze();
        static_cast<MeshObject*>(mCharacterActor->getControlledObject())->stopAllAnimations();

        // reset current combat, in order to avoid a potential dangling pointer
        mCombat->stop();
        mCombat = NULL;
    }

	void CombatControlState::run(Ogre::Real elapsedTime)
    {
		mCombatGui->update();
    }

    Ogre::String CombatControlState::getCombatantTypeName() const
    {
        return "CombatControlState";
    }

    void CombatControlState::requestCombatantAction()
    {
        // Change state to allow user to choose actions for next round.
		mState = REQUEST_USER_INPUT;
		mCombatGui->enableUserInput(true);
    }

	void CombatControlState::executeAction(CombatAction* action)
	{
	}
}
