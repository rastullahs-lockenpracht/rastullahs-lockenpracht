/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#ifndef __CommandMapperWindow_H__
#define __CommandMapperWindow_H__

#include "UiPrerequisites.h"
#include "CeGuiWindow.h"

namespace rl {

	class GameObject;

	class CommandMapperInputWindow :
		public CeGuiWindow
	{
	public:
		CommandMapperInputWindow();

		void setVisible(bool visible);
	};

	class _RlUiExport CommandMapperWindow :
		public CeGuiWindow
	{
	public:
		CommandMapperWindow(GameObject* actionHolder);
			
	private:
		bool handleChangeButton();
		bool handleCloseButton();
		bool handleChangeMovement();
		bool handleChangeInBattle();
		bool handleChangeOffBattle();
		bool handleKeyDown(const CEGUI::EventArgs& evt);
		bool handleMouseButton(const CEGUI::EventArgs& evt);
	
		void muteElements(bool mute);
		void muteWindow(bool mute);

		GameObject* mActionHolder;
		CommandMapperInputWindow* mInputWindow;
		CEGUI::MultiColumnList* mTableInBattle;
		CEGUI::MultiColumnList* mTableOffBattle;
		CEGUI::MultiColumnList* mTableMovement;
		CEGUI::TabPane* mTabPane;
		CEGUI::Window* mSelectedTable;
	};

	

}

#endif
