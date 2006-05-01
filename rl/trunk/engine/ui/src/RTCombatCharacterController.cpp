/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include "RTCombatCharacterController.h"

#include "CommandMapper.h"
#include "Creature.h"
#include "GameObject.h"
#include "InputManager.h"
#include "UiSubsystem.h"

namespace rl {

	RTCombatCharacterController::RTCombatCharacterController(
		Actor* camera, Creature* character)
		: MovementCharacterController(camera, character),
		RTCombatController()
	{
		setGroup(1);
	}

	RTCombatCharacterController::~RTCombatCharacterController()
	{
	}

	CharacterController::ControllerType RTCombatCharacterController::getType() const
	{
		return CTRL_RTCOMBAT;
	}

	void RTCombatCharacterController::notifyActionEnabled(bool enabled)
	{
	}

	void RTCombatCharacterController::notifyReactionEnabled(bool enabled)
	{
	}

	void RTCombatCharacterController::notifyCombatEnd()
	{
		UiSubsystem::getSingleton().setCharacterController(CharacterController::CTRL_MOVEMENT);
	}

	void RTCombatCharacterController::run(Ogre::Real timeElapsed)
	{
		MovementCharacterController::run(timeElapsed);
	}

	bool RTCombatCharacterController::injectMouseClicked(int mouseButtonMask)
	{
		return startAction(
			mCommandMapper->getAction(mouseButtonMask, CMDMAP_MOUSEMAP_IN_COMBAT), 
			getCurrentCreature());
	}

	bool RTCombatCharacterController::injectMouseDown(int mouseButtonMask)
	{
		return false;
	}

	bool RTCombatCharacterController::injectMouseUp(int mouseButtonMask)
	{
		return startAction(
			mCommandMapper->getAction(mouseButtonMask, CMDMAP_MOUSEMAP_IN_COMBAT), 
			getCurrentCreature());		
	}

	bool RTCombatCharacterController::injectKeyClicked(int keycode)
	{
		return startAction(
			mCommandMapper->getAction(keycode, CMDMAP_KEYMAP_IN_COMBAT), 
			getCurrentCreature());
	}

	bool RTCombatCharacterController::injectKeyDown(int keycode)
	{
		return MovementCharacterController::injectKeyDown(keycode);
	}

	bool RTCombatCharacterController::injectKeyUp(int keycode)
	{
		return MovementCharacterController::injectKeyUp(keycode);
	}

	void RTCombatCharacterController::attack()
	{
		GameObject* obj = InputManager::getSingleton().getPickedObject();
		if (obj != NULL)
		{
			mLastAttackTarget = dynamic_cast<Creature*>(obj);
		}

		RTCombatController::attack(mLastAttackTarget);
	}

	void RTCombatCharacterController::block()
	{
		GameObject* obj = InputManager::getSingleton().getPickedObject();
		if (obj != NULL)
		{
			mLastPareeTarget = dynamic_cast<Creature*>(obj);
		}

		RTCombatController::block(mLastPareeTarget);
	}
}
