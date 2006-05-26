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
#include "InputManager.h"
#include <OgreMouseEvent.h>
#include <OgreConfigFile.h>

using namespace Ogre;
using namespace std;

namespace rl {

	CommandMapper::CommandMapper()
	    : mMovementCommands(),
		  mKeyCommandsInCombat(),
		  mKeyCommandsOffCombat(),
		  mMouseCommandsInCombat(),
		  mMouseCommandsOffCombat()
	{
	}

	CommandMapper::~CommandMapper()
	{
	}

	const CeGuiString& CommandMapper::getAction(int keyCodeOrMouseButton, MapType mapType)
	{
		KeyAndMouseCommandMap* commandMap = getCommandMap(mapType);

		KeyAndMouseCommandMap::const_iterator command = commandMap->find(keyCodeOrMouseButton);

		if (command != commandMap->end())
		{
			return (*command).second;
		}

		static CeGuiString NO_ACTION = "";
		return NO_ACTION;
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

		if (mapType == CMDMAP_KEYMAP_OFF_COMBAT)
			commandMap = &mKeyCommandsOffCombat;
		else if (mapType == CMDMAP_KEYMAP_IN_COMBAT)
			commandMap = &mKeyCommandsInCombat;
		else if (mapType == CMDMAP_MOUSEMAP_OFF_COMBAT)
			commandMap = &mMouseCommandsOffCombat;
		else if (mapType == CMDMAP_MOUSEMAP_IN_COMBAT)
			commandMap = &mMouseCommandsInCombat;
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

	int CommandMapper::encodeKey(int scancode, int syskeys)
	{
        return scancode | syskeys << 16;
	}

	void CommandMapper::decodeKey(int combinedKeyCode, int* scancode, int* syskeys)
	{
		*syskeys = combinedKeyCode >> 16;
		*scancode = combinedKeyCode & ~(*syskeys << 16);
	}
  
	const MovementState CommandMapper::getMovement(int keycode) const
	{
		MovementCommandMap::const_iterator mvcmd = mMovementCommands.find(keycode);

		if (mvcmd == mMovementCommands.end())
		{
			Logger::getSingleton().log(Logger::UI, LML_TRIVIAL, 
				StringConverter::toString(keycode)+" pressed, no movement assigned");
			return MOVE_NONE;
		}
		else
		{
			return (*mvcmd).second;
		}
	}

	void CommandMapper::loadCommandMap(const Ogre::String& mapfile)
	{
		ConfigFile* cfg = new ConfigFile();
		cfg->loadFromResourceSystem("rastullah_input.cfg", 
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		
		for (ConfigFile::SettingsIterator it = cfg->getSettingsIterator("Movement");
			it.hasMoreElements();)
		{
			String key = it.peekNextKey();
			String setting = it.getNext();

			mMovementCommands[InputManager::getSingleton().getScanCode(key)] = getMovement(setting);
			Logger::getSingleton().log(Logger::UI, LML_TRIVIAL,
				Ogre::String("Key ") + key	+ " (" 
				+ StringConverter::toString(InputManager::getSingleton().getScanCode(key))
				+ ") is assigned to movement "	+ setting+" ("
				+ StringConverter::toString(getMovement(setting))+")");
		}

		for (ConfigFile::SettingsIterator it = cfg->getSettingsIterator("Keys off combat");
			it.hasMoreElements();)
		{
			String key = it.peekNextKey();
			String setting = it.getNext();

			mKeyCommandsOffCombat[getKeyCode(key)] = CeGuiString(setting);
			Logger::getSingleton().log(Logger::UI, LML_TRIVIAL,
				Ogre::String("Key ") + key	+ " (" + StringConverter::toString(getKeyCode(key))
				+ ") is assigned to command " + setting+" while not in combat");
		}

		for (ConfigFile::SettingsIterator it = cfg->getSettingsIterator("Keys in combat");
			it.hasMoreElements();)
		{
			String key = it.peekNextKey();
			String setting = it.getNext();

			mKeyCommandsInCombat[getKeyCode(key)] = CeGuiString(setting);
			Logger::getSingleton().log(Logger::UI, LML_TRIVIAL,
				Ogre::String("Key ") + key	+ " (" + StringConverter::toString(getKeyCode(key))
				+ ") is assigned to command " + setting+" while in combat");
		}

		for (ConfigFile::SettingsIterator it = cfg->getSettingsIterator("Mouse off combat");
			it.hasMoreElements();)
		{
			String key = it.peekNextKey();
			String setting = it.getNext();

			mMouseCommandsOffCombat[getMouseButtonCode(key)] = CeGuiString(setting);
			Logger::getSingleton().log(Logger::UI, LML_TRIVIAL,
				Ogre::String("Mouse Button ") + key	+ " (" + StringConverter::toString(getMouseButtonCode(key))
				+ ") is assigned to command " + setting+" while not in combat");
		}

		for (ConfigFile::SettingsIterator it = cfg->getSettingsIterator("Mouse in combat");
			it.hasMoreElements();)
		{
			String key = it.peekNextKey();
			String setting = it.getNext();

			mMouseCommandsInCombat[getMouseButtonCode(key)] = CeGuiString(setting);
			Logger::getSingleton().log(Logger::UI, LML_TRIVIAL,
				Ogre::String("Mouse Button ") + key	+ " (" + StringConverter::toString(getMouseButtonCode(key))
				+ ") is assigned to command " + setting+" while in combat");
		}

		delete cfg;
	}

	int CommandMapper::getKeyCode(const Ogre::String &keyDescription)
	{
		StringVector parts = StringUtil::split(keyDescription, "+");
		
		int modifiers = 0;
		for(size_t i = 0; i<parts.size()-1; i++)
		{
			modifiers |= InputManager::getSingleton().getSystemCode(parts[i]);
		}
		
		return encodeKey(InputManager::getSingleton().getScanCode(parts[parts.size()-1]), modifiers);
	}

	int CommandMapper::getMouseButtonCode(int buttonNum)
	{
		return 1 << (buttonNum+4);
	}

	int CommandMapper::getMouseButtonCode(const Ogre::String &buttonDescription)
	{
		StringVector parts = StringUtil::split(buttonDescription, "+");
		
		int modifiers = 0;
		for(size_t i = 0; i<parts.size()-1; i++)
		{
			modifiers |= InputManager::getSingleton().getSystemCode(parts[i]);
		}
		
		int buttonNum = StringConverter::parseInt(parts[parts.size()-1].substr(6));
		return encodeKey(getMouseButtonCode(buttonNum), modifiers);
	}

	MovementState CommandMapper::getMovement(const Ogre::String &movementDescription)
	{
		if (movementDescription == "left")
		{
			return MOVE_LEFT;
		}
		else if (movementDescription == "right")
		{
			return MOVE_RIGHT;
		}
		else if (movementDescription == "forward")
		{
			return MOVE_FORWARD;
		}
		else if (movementDescription == "backward")
		{
			return MOVE_BACKWARD;
		}
		else if (movementDescription == "turn_left")
		{
			return TURN_LEFT;
		}
		else if (movementDescription == "turn_right")
		{
			return TURN_RIGHT;
		}
		else if (movementDescription == "run")
		{
			return MOVE_RUN;
		}
		else if (movementDescription == "sneak")
		{
			return MOVE_SNEAK;
		}
		else if (movementDescription == "jump")
		{
			return MOVE_JUMP;
		}
		else if (movementDescription == "run_lock")
		{
			return MOVE_RUN_LOCK;
		}

		return MOVE_NONE;
	}
}

