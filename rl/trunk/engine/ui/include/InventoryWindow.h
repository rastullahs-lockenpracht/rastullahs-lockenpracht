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

#ifndef __InventoryWindow_H__
#define __InventoryWindow_H__

#include "UiPrerequisites.h"

#include "AbstractWindow.h"

namespace rl {

    class Inventory;
	class Item;
	class ItemDragContainer;

	class _RlUiExport InventoryWindow : public AbstractWindow
	{
	public:
		static const Ogre::String ICON_UNKNOWN_ITEM;
		static const Ogre::String SLOTNAME;

		InventoryWindow(const Ogre::String& inventoryWindow, Inventory* inventory);
		~InventoryWindow();

    private:
		typedef std::map<CeGuiString, ItemDragContainer*> DndContainerMap;

		Inventory* mInventory;
        std::map<CeGuiString, CEGUI::Window*> mSlotWindows;
		CEGUI::Window* mWorldBackground;
		DndContainerMap mDragContainers;

		void createSlotWindows(Inventory* inventory);
        void initInventoryWindow(Inventory* inventory);

		ItemDragContainer* createItemDragContainer(Item* item, const CeGuiString& containerName = "");
		bool handleItemDroppedOnSlot(const CEGUI::EventArgs& evt);
		bool handleItemDroppedOnWorld(const CEGUI::EventArgs& evt);
		bool handleItemMouseClick(const CEGUI::EventArgs& evt, Item* item);
		bool handleItemDoubleClick(const CEGUI::EventArgs& evt, Item* item);
		bool handleMouseMovedInWorld(const CEGUI::EventArgs& evt);
		bool handleKeys(const CEGUI::EventArgs& evt, bool down);
		bool destroyDragContainer(ItemDragContainer* cont);
    };
}

#endif //__InventoryWindow_H__
