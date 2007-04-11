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

#include "Creature.h"
#include "Actor.h"

namespace rl {
    CombatCharacterController::CombatCharacterController(Actor* camera, Actor* character)
        : CharacterController(camera, character)
    {
    }

	CombatCharacterController::~CombatCharacterController()
    {
    }

    CharacterController::ControllerType CombatCharacterController::getType() const
    {
        return CTRL_COMBAT;
    }

	void CombatCharacterController::run(Ogre::Real elapsedTime)
    {
    }

	void CombatCharacterController::toggleViewMode()
    {
    }

    void CombatCharacterController::resetCamera()
    {
    }

    bool CombatCharacterController::injectKeyUp(int keycode)
    {
        return false;
    }
}