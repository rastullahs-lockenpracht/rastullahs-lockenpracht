#ifndef __MainMenuWindow_H__
#define __MainMenuWindow_H__

#include "UiPrerequisites.h"
#include <CEGUIWindow.h>

namespace rl {

class _RlUiExport MainMenuWindow
{
public:
	MainMenuWindow();

	bool handleChooseModules();
	bool handleStart();

private:
	CEGUI::Window* mButtonModules;
	CEGUI::Window* mButtonStart;
};

}

#endif