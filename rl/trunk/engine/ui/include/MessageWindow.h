#ifndef __MessageWindow_H__
#define __MessageWindow_H__

#include "UiPrerequisites.h"
#include "CeGuiWindow.h"

namespace rl {
	
	class _RlUiExport MessageWindow :
		public CeGuiWindow
	{
	public:
		MessageWindow();
		
		void setText(const CeGuiString& message);
	
	private:
		CEGUI::Window* mTextField;	
	};
}

#endif
