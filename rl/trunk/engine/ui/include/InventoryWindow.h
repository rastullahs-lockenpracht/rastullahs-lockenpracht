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

#include <OgreRectangle.h>

#include "AbstractWindow.h"

namespace rl {

	class CameraObject;
    class Inventory;
	class Item;
	class ItemDragContainer;
	class RaySelector;

	class _RlUiExport InventoryWindow : public AbstractWindow
	{
	public:
		static const Ogre::String SLOTNAME;

		InventoryWindow(const Ogre::String& inventoryWindow, Inventory* inventory);
		~InventoryWindow();

    private:
		typedef std::map<CeGuiString, ItemDragContainer*> DndContainerMap;

		Inventory* mInventory;
        std::map<CeGuiString, CEGUI::Window*> mSlotWindows;
		CEGUI::Window* mWorldBackground;
		DndContainerMap mWorldDragContainers;
		DndContainerMap mSlotDragContainers;
		RaySelector* mMouseSelector;

		void createSlotWindows(Inventory* inventory);
        void initInventoryWindow(Inventory* inventory);

		ItemDragContainer* createItemDragContainer(Item* item, bool showdescription, const CeGuiString& containerName = "");
        //ItemDragContainer* getItemDragContainer(Item* item, bool description);
        CeGuiString getDragContainerNameFromItem(const Item* item, bool description);
		bool handleItemDroppedOnSlot(const CEGUI::EventArgs& evt);
		bool handleItemDroppedOnWorld(const CEGUI::EventArgs& evt);
		bool handleMouseMovedInWorld(const CEGUI::EventArgs& evt);
		bool handleKeys(const CEGUI::EventArgs& evt, bool down);
		bool destroyDragContainer(ItemDragContainer* cont);

		Ogre::Rectangle getCeGuiRectFromWorldAABB(
			CameraObject* camera,
			const Ogre::AxisAlignedBox& aabb) const;
    };
}

#endif //__InventoryWindow_H__
