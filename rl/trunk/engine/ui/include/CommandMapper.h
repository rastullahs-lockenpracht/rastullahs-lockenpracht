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

#ifndef __CommandMapper_H__
#define __CommandMapper_H__

#include "UiPrerequisites.h"

#include <OgreSingleton.h>
#include <OgreInput.h>
#include <map>

namespace rl {

	class CommandExecutor;

	enum MovementState
	{
		MOVE_NONE = 0,
		MOVE_LEFT = 1<<0,
		MOVE_RIGHT = 1<<1,
		MOVE_FORWARD = 1<<2,
		MOVE_BACKWARD = 1<<3,
		TURN_LEFT = 1<<4,
		TURN_RIGHT = 1<<5,
		MOVE_RUN = 1<<6,
		MOVE_SNEAK = 1<<7,
		MOVE_JUMP = 1<<8
	};

	enum MapType
	{
		CMDMAP_KEYMAP_IN_BATTLE,
		CMDMAP_KEYMAP_OFF_BATTLE,
		CMDMAP_KEYMAP_MOVEMENT,
		CMDMAP_MOUSEMAP
	};

	struct ActionEntry
	{
		//ActionEntry()	{}

		CeGuiString actionClass;
		CeGuiString actionName;
	};

	const int CMDMAP_NO_MAPPING = -9999999;

	class _RlUiExport CommandMapper : public Ogre::Singleton<CommandMapper>
	{
	public:

		CommandMapper();
		~CommandMapper();

		static CommandMapper& getSingleton();
		static CommandMapper* getSingletonPtr();

		void loadCommandMap(const char* mapfile);

		int getActiveMovement();
		bool isMovementActive(MovementState movmt);

		bool injectMouseClicked(int mouseButtonMask);
		bool injectKeyClicked(int keycode);		
		bool injectKeyDown(int keycode);
		bool injectKeyUp(int keycode);		

		void setMapping(
			MapType map, 
			int code, 
			const CeGuiString& actionClass, 
			const CeGuiString& actionName);

		int getMapping(
			MapType map, 
			const CeGuiString& actionClass, 
			const CeGuiString& actionName);

	private:
		// KeyCode -> (Rubyklasse, Name)
		typedef std::map<int, ActionEntry > KeyAndMouseCommandMap;
		typedef std::map<int, MovementState> MovementCommandMap;

		MovementCommandMap mMovementCommands;
		KeyAndMouseCommandMap mKeyCommandsInBattle;
		KeyAndMouseCommandMap mKeyCommandsOffBattle;
		KeyAndMouseCommandMap mMouseCommands;

		int mActiveMovement;		
	};

}


#endif

