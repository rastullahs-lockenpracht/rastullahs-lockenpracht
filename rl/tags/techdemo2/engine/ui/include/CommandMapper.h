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

#ifndef __CommandMapper_H__
#define __CommandMapper_H__

#include "UiPrerequisites.h"

#include <OgreInput.h>
#include <map>

namespace rl {

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
		MOVE_JUMP = 1<<8,
		MOVE_RUN_LOCK = 1<<9
	};

	enum MapType
	{
		CMDMAP_KEYMAP_IN_COMBAT,
		CMDMAP_KEYMAP_OFF_COMBAT,
		CMDMAP_KEYMAP_MOVEMENT,
		CMDMAP_MOUSEMAP_IN_COMBAT,
		CMDMAP_MOUSEMAP_OFF_COMBAT
	};

	const int CMDMAP_NO_MAPPING = -9999999;

	class _RlUiExport CommandMapper
	{
	public:

		CommandMapper();
		~CommandMapper();

		void loadCommandMap(const Ogre::String& mapfile);
		void saveCommandMap(const Ogre::String& mapfile) const;

		void setMapping(
			MapType map, 
			int code, 
			const CeGuiString& actionName);

		int getMapping(
			MapType map, 
			const CeGuiString& actionName);

		std::map<CeGuiString, MovementState> getMovements();

		static int encodeKey(int scancode, int syskeys);
		static void decodeKey(int combinedKeyCode, int* scancode, int* syskeys);

		const MovementState getMovement(int keycode) const;

		// Finde die der Taste oder dem Mausbutton zugeordneten Aktion in der 
		// angegebenen CommandMap
		const CeGuiString& getAction(int keyCodeOrMouseButton, MapType mapType);

	private:

		// KeyCode -> Name
		typedef std::map<int, CeGuiString > KeyAndMouseCommandMap;
		typedef std::map<int, MovementState> MovementCommandMap;

		MovementCommandMap mMovementCommands;
		KeyAndMouseCommandMap mKeyCommandsInCombat;
		KeyAndMouseCommandMap mKeyCommandsOffCombat;
		KeyAndMouseCommandMap mMouseCommandsInCombat;
		KeyAndMouseCommandMap mMouseCommandsOffCombat;

		std::map<CeGuiString, MovementState> mMovement;

		KeyAndMouseCommandMap* getCommandMap(MapType mapType);
		int getKeyCode(const Ogre::String& keyDescription);
		int getMouseButtonCode(int buttonNum);
		int getMouseButtonCode(const Ogre::String& buttonDescription);
		MovementState getMovement(const Ogre::String& movementDescription);
	};

}


#endif

