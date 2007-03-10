/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __CommandMapperWindow_H__
#define __CommandMapperWindow_H__

#include "UiPrerequisites.h"
#include "AbstractWindow.h"

namespace rl {

	class Creature;
	class CommandMapper;

	class CommandMapperInputWindow :
		public AbstractWindow
	{
	public:
		CommandMapperInputWindow();

		void setVisible(bool visible, bool destroyAfterHide = false);
	};

	class _RlUiExport CommandMapperWindow :
		public AbstractWindow
	{
	public:
		CommandMapperWindow(Creature* actionHolder, CommandMapper* commandMapper);
			
	private:
		bool handleChangeButton();
		bool handleCloseButton();
		bool handleChangeMovement();
		bool handleChangeInCombat();
		bool handleChangeOffCombat();
		bool handleKeyDown(const CEGUI::EventArgs& evt);
		bool handleMouseButton(const CEGUI::EventArgs& evt);
	
		void muteElements(bool mute);
		void muteWindow(bool mute);

		void refreshContent();

		Creature* mActionHolder;
		CommandMapperInputWindow* mInputWindow;
		CommandMapper* mCommandMapper;
		CEGUI::MultiColumnList* mTableInCombat;
		CEGUI::MultiColumnList* mTableOffCombat;
		CEGUI::MultiColumnList* mTableMovement;
		CEGUI::TabControl* mTabControl;
		CEGUI::Window* mSelectedTable;
	};

	

}

#endif
