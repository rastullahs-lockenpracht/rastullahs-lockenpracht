/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#include "CommandMapper.h"
#include "UiSubsystem.h"
#include "Person.h"
#include "Exception.h"

using namespace Ogre;
using namespace std;

template <> rl::CommandMapper* Singleton<rl::CommandMapper>::ms_Singleton = 0;

namespace rl {

	CommandMapper::CommandMapper()
	    : mMovementCommands(),
		  mKeyCommandsInBattle(),
		  mKeyCommandsOffBattle(),
		  mMouseCommands(),
		  mActiveMovement(MOVE_NONE)		 
	{
		mMovementCommands.insert(make_pair(KC_A, MOVE_LEFT));
		mMovementCommands.insert(make_pair(KC_D, MOVE_RIGHT));
		mMovementCommands.insert(make_pair(KC_W, MOVE_FORWARD));
		mMovementCommands.insert(make_pair(KC_S, MOVE_BACKWARD));
		mMovementCommands.insert(make_pair(KC_Q, TURN_LEFT));
		mMovementCommands.insert(make_pair(KC_E, TURN_RIGHT));
		mMovementCommands.insert(make_pair(KC_LSHIFT, MOVE_RUN));

		ActionEntry ae;
		ae.actionClass = "ShowActionMenuAction";
		ae.actionName = "Aktionen";
		mKeyCommandsInBattle.insert(make_pair(KC_F3, ae));
		mKeyCommandsOffBattle.insert(make_pair(KC_F3, ae));

		ae.actionClass = "QuitGameAction";
		ae.actionName = "Ende";
		mKeyCommandsOffBattle.insert(make_pair(KC_ESCAPE, ae));

		ae.actionClass = "ToggleConsoleAction";
		ae.actionName = "Konsole";
		mKeyCommandsOffBattle.insert(make_pair(KC_TAB, ae));

		ae.actionClass = "ToggleDebugWindowAction";
		ae.actionName = "DebugWindow";
		mKeyCommandsOffBattle.insert(make_pair(KC_F2, ae));
	}

	CommandMapper::~CommandMapper()
	{
	}

	CommandMapper& CommandMapper::getSingleton()
	{
		return Singleton<CommandMapper>::getSingleton();
	}

	CommandMapper* CommandMapper::getSingletonPtr()
	{
		return Singleton<CommandMapper>::getSingletonPtr();
	}

	bool CommandMapper::injectMouseClicked(int mouseButtonMask)
	{
		return false;
	}

	bool CommandMapper::injectKeyClicked(int keycode)
	{
		KeyAndMouseCommandMap* commandMap;
		if (UiSubsystem::getSingleton().isInBattleMode())
			commandMap = &mKeyCommandsInBattle;
		else
			commandMap = &mKeyCommandsOffBattle;

		KeyAndMouseCommandMap::const_iterator command = commandMap->find(keycode);

		if (command != commandMap->end())
		{
			Person* chara = UiSubsystem::getSingleton().getActiveCharacter();
			chara->doAction((*command).second.actionClass, (*command).second.actionName, chara, chara);
			return true;
		}
		return false;
	}

	bool CommandMapper::injectKeyDown(int keycode)
	{
		MovementCommandMap::const_iterator mvcmd = mMovementCommands.find(keycode);

		if (mvcmd != mMovementCommands.end())
		{
			mActiveMovement |= (*mvcmd).second;
			return true;
		}
		return false;
	}

	bool CommandMapper::injectKeyUp(int keycode)
	{
		MovementCommandMap::const_iterator mvcmd = mMovementCommands.find(keycode);

		if (mvcmd != mMovementCommands.end())
		{
			mActiveMovement &= ~(*mvcmd).second;
			return true;
		}
		return false;
	}

	int CommandMapper::getActiveMovement()
	{
		return mActiveMovement;
	}

	bool CommandMapper::isMovementActive(MovementState movmt)
	{
		return (mActiveMovement & movmt) == movmt;
	}


	void CommandMapper::setMapping(
			MapType map, 
			int code, 
			const CeGuiString& actionClass, 
			const CeGuiString& actionName)
	{
		ActionEntry ae;
		ae.actionClass = actionClass;
		ae.actionName = actionName;

		switch (map)
		{
		case CMDMAP_KEYMAP_IN_BATTLE:
			mKeyCommandsInBattle.insert(make_pair(code, ae));
			break;
		case CMDMAP_KEYMAP_OFF_BATTLE:
			mKeyCommandsOffBattle.insert(make_pair(code, ae));
			break;
		case CMDMAP_MOUSEMAP:
			mKeyCommandsInBattle.insert(make_pair(code, ae));
			break;
		}
	}

	int CommandMapper::getMapping(
			MapType map, 
			const CeGuiString& actionClass, 
			const CeGuiString& actionName)
	{
		if (map == CMDMAP_KEYMAP_MOVEMENT)
		{
			return 0;
		}

		KeyAndMouseCommandMap* commandMap;

		if (map == CMDMAP_KEYMAP_IN_BATTLE)
			commandMap = &mKeyCommandsInBattle;
		else if (map == CMDMAP_KEYMAP_OFF_BATTLE)
            commandMap = &mKeyCommandsOffBattle;
		else if (map == CMDMAP_MOUSEMAP)
			commandMap = &mMouseCommands;
		else
			Throw(RuntimeException, "Unknown command map");

		for (KeyAndMouseCommandMap::iterator command = commandMap->begin();
				command != commandMap->end(); command++)
		{
			ActionEntry ae = (*command).second;
			if (ae.actionClass.compare(actionClass) &&
				ae.actionName.compare(actionName))
				return (*command).first;
		}

		return CMDMAP_NO_MAPPING;
	}

    
}

