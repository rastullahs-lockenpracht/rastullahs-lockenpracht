#ifndef __InventoryWindow_H__
#define __InventoryWindow_H__

#include "UiPrerequisites.h"
#include <CEGUI.h>

namespace rl {

	class Creature;

	class _RlUiExport InventoryWindow : public CeGuiWindow
	{
	public:
		InventoryWindow();

		setCharacter(Creature* character);
		update();

	private:
        Creature* mCharacter;
	}

}

#endif
