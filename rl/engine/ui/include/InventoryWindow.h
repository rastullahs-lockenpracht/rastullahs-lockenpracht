/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "ItemDragContainer.h"

namespace rl
{

    class CameraObject;
    class Container;
    class ContainerContentWindow;
    class Inventory;
    class Item;
    class RaySelector;

    class _RlUiExport InventoryWindow : public AbstractWindow, public ItemDragContainerDestroyListener
    {
    public:
        static const Ogre::String SLOTNAME;

        InventoryWindow(const Ogre::String& inventoryWindow, Inventory* inventory);
        ~InventoryWindow();

        void showContainerContent(Container* container);
        void notifyContainerContentWindowClosed(Container* container); // no error if not found!

        void setVisible(bool visible, bool destroyAfterHide = false);

        bool showPossibleSlots(const Item* item);

        void notifyItemDragContainerDestroyed(ItemDragContainer* cont);

    private:
        typedef std::map<CeGuiString, ItemDragContainer*> DndContainerMap;
        typedef std::map<CeGuiString, CEGUI::Window*> SlotWindowMap;

        Inventory* mInventory;
        SlotWindowMap mSlotWindows;
        CEGUI::Window* mWorldBackground;
        DndContainerMap mWorldDragContainers;
        DndContainerMap mSlotDragContainers;
        RaySelector* mMouseSelector;
        bool mShowAllWorldItems;

        void createSlotWindows(Inventory* inventory);
        void initInventoryWindow(Inventory* inventory);

        ItemDragContainer* createItemDragContainer(
            Item* item, bool showdescription, const CeGuiString& containerName = "");
        ItemDragContainer* getItemDragContainer(const Item* item, bool description);
        CeGuiString getDragContainerNameFromItem(const Item* item, bool description);
        bool handleItemDroppedOnSlot(const CEGUI::EventArgs& evt);
        bool handleItemEntersSlot(const CEGUI::EventArgs& evt);
        bool handleItemLeavesSlot(const CEGUI::EventArgs& evt);
        bool handleItemDroppedOnWorld(const CEGUI::EventArgs& evt);
        bool handleItemDragEnded(const Item* item, bool description);
        bool handleItemDragStarted(const Item* item, bool description);
        bool handleMouseMovedInWorld(const CEGUI::EventArgs& evt);
        bool handleKeys(const CEGUI::EventArgs& evt, bool down);

        Ogre::Rect getCeGuiRectFromWorldAABB(CameraObject* camera, const Ogre::AxisAlignedBox& aabb) const;
        typedef std::map<Container*, ContainerContentWindow*> ContainerMap;
        ContainerMap mOpenContainerMap;
    };
}

#endif //__InventoryWindow_H__
