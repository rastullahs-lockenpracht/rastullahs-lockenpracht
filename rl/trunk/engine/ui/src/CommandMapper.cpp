#include "CommandMapper.h"
#include "CommandExecutor.h"

using namespace Ogre;
using namespace std;

template <> rl::CommandMapper* Singleton<rl::CommandMapper>::ms_Singleton = 0;

namespace rl {

	CommandMapper::CommandMapper()
	    : mMovementCommands(),
		  mActionCommands(),
		  mMouseCommands(),
		  mActiveMovement(MOVE_NONE),
		  mActionsInBattle(),
		  mActionsOffBattle()
	{
		mMovementCommands.insert(make_pair(KC_A, MOVE_LEFT));
		mMovementCommands.insert(make_pair(KC_D, MOVE_RIGHT));
		mMovementCommands.insert(make_pair(KC_W, MOVE_FORWARD));
		mMovementCommands.insert(make_pair(KC_S, MOVE_BACKWARD));	
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
		KeyCommandMap::const_iterator command = mActionCommands.find(keycode);
		if (command != mActionCommands.end())
		{
			//mCommandExecutor->executeCommand((*command).second);
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

	void CommandMapper::setExecutor(CommandExecutor* executor)
	{
		mCommandExecutor = executor;
		CeGuiStringVector mActionsInBattle = executor->getCommandsInBattle();
		CeGuiStringVector mActionsOffBattle = executor->getCommandsOffBattle();
	}
}

