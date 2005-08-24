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
#ifndef __InGameMenuWindow_H__
#define __InGameMenuWindow_H__

#include "UiPrerequisites.h"
#include "CeGuiWindow.h"

namespace rl {
	
	class GameObject;
	class Action;

	class _RlUiExport InGameMenuWindow : public CeGuiWindow
	{
	public:
		InGameMenuWindow();
		virtual ~InGameMenuWindow();

		void setAction(CEGUI::MenuItem* item, Action* action);
		void createMenu(CEGUI::MenuBase* menu);

		void update();
	private:
		bool handleAction(Action* action);
	};
}

#endif
