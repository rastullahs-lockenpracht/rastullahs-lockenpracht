/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include "CommandMapper.h"
#include "UiSubsystem.h"
#include "Person.h"
#include "Exception.h"
#include "Action.h"
#include "ActionManager.h"
#include <OgreMouseEvent.h>

using namespace Ogre;
using namespace std;

template <> rl::CommandMapper* Singleton<rl::CommandMapper>::ms_Singleton = 0;

namespace rl {

	CommandMapper::CommandMapper()
	    : mMovementCommands(),
		  mKeyCommandsInBattle(),
		  mKeyCommandsOffBattle(),
		  mMouseCommandsInBattle(),
		  mMouseCommandsOffBattle(),
		  mActiveMovement(MOVE_NONE)		 
	{
		mMovementCommands.insert(make_pair(KC_A, MOVE_LEFT));
		mMovementCommands.insert(make_pair(KC_D, MOVE_RIGHT));
		mMovementCommands.insert(make_pair(KC_W, MOVE_FORWARD));
		mMovementCommands.insert(make_pair(KC_S, MOVE_BACKWARD));
		mMovementCommands.insert(make_pair(KC_Q, TURN_LEFT));
		mMovementCommands.insert(make_pair(KC_E, TURN_RIGHT));
		mMovementCommands.insert(make_pair(KC_LSHIFT, MOVE_RUN));
        mMovementCommands.insert(make_pair(KC_LCONTROL, MOVE_SNEAK));
		mMovementCommands.insert(make_pair(KC_SPACE, MOVE_JUMP));

		setMapping(CMDMAP_MOUSEMAP_OFF_BATTLE, MouseEvent::BUTTON0_MASK, "showactions");
		setMapping(CMDMAP_KEYMAP_OFF_BATTLE, KC_ESCAPE, "quitgame");
		setMapping(CMDMAP_KEYMAP_OFF_BATTLE, KC_TAB, "toggleconsole");
		setMapping(CMDMAP_KEYMAP_OFF_BATTLE, KC_F2, "toggledebugwindow");
		setMapping(CMDMAP_KEYMAP_OFF_BATTLE, KC_F3, "togglegamelogwindow");
		setMapping(CMDMAP_KEYMAP_OFF_BATTLE, KC_F8, "toggledialogwindow");
		setMapping(CMDMAP_KEYMAP_OFF_BATTLE, KC_F, "toggleviewmode");
		setMapping(CMDMAP_KEYMAP_OFF_BATTLE, KC_NUMPAD0, "resetcamera");
		setMapping(CMDMAP_KEYMAP_OFF_BATTLE, KC_P, "makescreenshot");
		setMapping(CMDMAP_KEYMAP_OFF_BATTLE, KC_L, "toggleodedebug");
		setMapping(CMDMAP_KEYMAP_OFF_BATTLE, KC_R, "usecurrentobjectdefaultaction");
		setMapping(CMDMAP_KEYMAP_OFF_BATTLE, KC_U, "showobjectactions");
		setMapping(CMDMAP_KEYMAP_OFF_BATTLE, KC_C, "showcharactersheet");
		setMapping(CMDMAP_KEYMAP_OFF_BATTLE, KC_T, "showtargetwindow");
		setMapping(CMDMAP_KEYMAP_OFF_BATTLE, KC_O, "togglecharacterstatewindow");
		setMapping(CMDMAP_KEYMAP_OFF_BATTLE, KC_F10, "toggleingameglobalmenu");

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

	bool CommandMapper::startAction(int keyCodeOrMouseButton, MapType mapType)
	{
		KeyAndMouseCommandMap* commandMap = getCommandMap(mapType);

		KeyAndMouseCommandMap::const_iterator command = commandMap->find(keyCodeOrMouseButton);

		if (command != commandMap->end())
		{
			Person* chara = UiSubsystem::getSingleton().getActiveCharacter();

			try
			{
				Action* action = ActionManager::getSingleton().getInGameGlobalAction((*command).second);
				if (action != NULL)
					action->doAction(NULL, NULL, NULL); //TODO: Eigene Klasse für globale Aktionen? doAction hat keine Parameter(?)
				else
					chara->doAction((*command).second, chara, chara);
				return true;
			}
			catch( ScriptInvocationFailedException& sife )
			{
				UiSubsystem::getSingleton().log(Ogre::LML_CRITICAL, sife.toString() );
			}
		}

		return false;
	}

	bool CommandMapper::injectMouseClicked(int mouseButtonMask)
	{
		if (UiSubsystem::getSingleton().isInBattleMode())
			return startAction(mouseButtonMask, CMDMAP_MOUSEMAP_IN_BATTLE);
		else
			return startAction(mouseButtonMask, CMDMAP_MOUSEMAP_OFF_BATTLE);		
	}

	bool CommandMapper::injectKeyClicked(int keycode)
	{
		if (UiSubsystem::getSingleton().isInBattleMode())
			return startAction(keycode, CMDMAP_KEYMAP_IN_BATTLE);
		else
			return startAction(keycode, CMDMAP_KEYMAP_OFF_BATTLE);		
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
			MapType mapType, 
			int code, 
			const CeGuiString& actionName)
	{
		getCommandMap(mapType)->insert(make_pair(code, actionName));
	}

	CommandMapper::KeyAndMouseCommandMap* CommandMapper::getCommandMap(MapType mapType)
	{
		KeyAndMouseCommandMap* commandMap;

		if (mapType == CMDMAP_KEYMAP_OFF_BATTLE)
			commandMap = &mKeyCommandsOffBattle;
		else if (mapType == CMDMAP_KEYMAP_IN_BATTLE)
			commandMap = &mKeyCommandsInBattle;
		else if (mapType == CMDMAP_MOUSEMAP_OFF_BATTLE)
			commandMap = &mMouseCommandsOffBattle;
		else if (mapType == CMDMAP_MOUSEMAP_IN_BATTLE)
			commandMap = &mMouseCommandsInBattle;
		else
			Throw(RuntimeException, "Unknown command map");

		return commandMap;
	}

	int CommandMapper::getMapping(
			MapType mapType, 
			const CeGuiString& actionName)
	{
		if (mapType == CMDMAP_KEYMAP_MOVEMENT)
		{
			return 0;
		}

		KeyAndMouseCommandMap* commandMap = getCommandMap(mapType);
	
		for (KeyAndMouseCommandMap::iterator command = commandMap->begin();
				command != commandMap->end(); command++)
		{
			CeGuiString name = (*command).second;
			if (name.compare(actionName) == 0)
				return (*command).first;
		}

		return CMDMAP_NO_MAPPING;
	}

    
}

