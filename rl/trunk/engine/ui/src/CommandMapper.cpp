#include "CommandMapper.h"
#include "UiSubsystem.h"
#include "Person.h"

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

		mKeyCommandsInBattle.insert(make_pair(KC_F3, make_pair("ShowActionMenuAction", "Aktionen")));
		mKeyCommandsOffBattle.insert(make_pair(KC_F3, make_pair("ShowActionMenuAction", "Aktionen")));
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
		KeyCommandMap* commandMap;
		if (UiSubsystem::getSingleton().isInBattleMode())
			commandMap = &mKeyCommandsInBattle;
		else
			commandMap = &mKeyCommandsOffBattle;

		KeyCommandMap::const_iterator command = commandMap->find(keycode);

		if (command != commandMap->end())
		{
			Person* chara = UiSubsystem::getSingleton().getActiveCharacter();
			chara->doAction((*command).second.first, (*command).second.second, chara, chara);
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

}

