#ifndef __CommandExecutor_h__
#define __CommandExecutor_h__

#include "UiPrerequisites.h"
#include <vector>

namespace rl {

	/**
	* Diese Klasse enthält die ausführbaren Spieleraktionen, welche mit einer Taste verbunden werden
	* können
	* 
	* pure virtual - von dieser Klasse muss in Ruby geerbt werden, hier ist nur das Interface definiert
	*/
	class _RlUiExport CommandExecutor
	{
	public:
		virtual ~CommandExecutor();

		virtual CeGuiStringVector getCommandsInBattle();
		virtual CeGuiStringVector getCommandsOffBattle();
		virtual void executeCommand(CeGuiString command);
	};

}

#endif
