#ifndef __CommandMapperWindow_H__
#define __CommandMapperWindow_H__

#include "UiPrerequisites.h"
#include "CeGuiWindow.h"

namespace rl {

	class _RlUiExport CommandMapperWindow :
		public CeGuiWindow
	{
	public:
		CommandMapperWindow();
			
	private:
		bool handleChangeButton();
		bool handleCloseButton();
		bool handleChangeMovement();
		bool handleChangeInBattle();
		bool handleChangeOffBattle();
	
		void subscribeEventsToElements();
		void subscribeEventsToCollectCommand();
	};

}

#endif
