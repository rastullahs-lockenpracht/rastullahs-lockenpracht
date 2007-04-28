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
#include "Creature.h"
#include "PhysicalThing.h"

namespace rl {
    CombatCharacterController::CombatCharacterController(CommandMapper* cmdMapper,
        Actor* camera, Person* character)
        : CharacterController(cmdMapper, camera, character),
          mCombatManager(CombatManager::getSingletonPtr()),
          mCombat(NULL)
    {
    }

	CombatCharacterController::~CombatCharacterController()
    {
    }

    void CombatCharacterController::resume()
    {
        mCameraActor->getPhysicalThing()->freeze();
        mCharacterActor->getPhysicalThing()->freeze();

        // Is there a combat running already?
        if (mCombatManager->getCurrentCombat() != NULL)
        {
            // Yes, set this one as active.
            mCombat = mCombatManager->getCurrentCombat();
        }
        else
        {
            // No, test, if we can start one.
        }
    }

    void CombatCharacterController::pause()
    {
        mCameraActor->getPhysicalThing()->unfreeze();
        mCharacterActor->getPhysicalThing()->unfreeze();

        // reset current combat, in order to avoid a potential dangling pointer
        mCombat = NULL;
    }

	void CombatCharacterController::run(Ogre::Real elapsedTime)
    {
    }

	bool CombatCharacterController::injectMouseDown(int mouseButtonMask)
    {
        return false;
    }

    bool CombatCharacterController::injectMouseUp(int mouseButtonMask)
    {
        return false;
    }

    bool CombatCharacterController::injectKeyDown(int keycode)
    {
        return false;
    }

    bool CombatCharacterController::injectKeyUp(int keycode)
    {
        return false;
    }
}
