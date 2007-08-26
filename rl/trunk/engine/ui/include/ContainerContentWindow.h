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

#ifndef __ContainerContentWindow_H__
#define __ContainerContentWindow_H__

#include "UiPrerequisites.h"
#include "AbstractWindow.h"
#include "InventoryWindow.h"

namespace rl {

	class Container;
	class Item;
	
    /// this class is intended to be used with a inventorywindow
	class _RlUiExport ContainerContentWindow :
		public AbstractWindow
	{
	public:
		ContainerContentWindow(Container* container, InventoryWindow* parent);

        void setVisible(bool visible, bool destroyAfterHide = false);

	private:
		CEGUI::Window* mContentWindow;
		Container* mContainer;
        InventoryWindow* mInventoryWindow;

		static int sItemCount;

		bool handleItemDroppedOnContainer(const CEGUI::EventArgs& evt);
        bool handleItemEntersContainer(const CEGUI::EventArgs& evt);
        bool handleItemLeavesContainer(const CEGUI::EventArgs& evt);
        bool handleItemMouseClick(const CEGUI::EventArgs& evt, Item* item);
        bool handleItemDoubleClick(const CEGUI::EventArgs& evt, Item* item);
		void initializeContent();
		CEGUI::Window* createItemWindow(Item* item);
	};
}

#endif
