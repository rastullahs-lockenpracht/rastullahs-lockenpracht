#include "CommandExecutor.h"

namespace rl {

	CommandExecutor::~CommandExecutor()
	{
	}

	CeGuiStringVector CommandExecutor::getCommandsInBattle()
	{
		static CeGuiStringVector emptyvec;
		return emptyvec;
	}
	
	CeGuiStringVector CommandExecutor::getCommandsOffBattle()
	{
		static CeGuiStringVector emptyvec;
		return emptyvec;
	}
	
	void CommandExecutor::executeCommand(CeGuiString command)
	{
	}
}