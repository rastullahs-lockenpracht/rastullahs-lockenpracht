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
#include "InventoryWindow.h"

#include <boost/bind.hpp>
#include <CEGUIImageset.h>
#include <CEGUIImagesetManager.h>
#include <CEGUIWindowManager.h>

#include "ItemDragContainer.h"
#include "WindowFactory.h"

using namespace CEGUI;
using namespace std;

namespace rl {

	const Ogre::String InventoryWindow::ICON_UNKNOWN_ITEM = "set:ModelThumbnails image:item_unknown";
	const Ogre::String InventoryWindow::SLOTNAME = "slotname";

    InventoryWindow::InventoryWindow(const Ogre::String& inventoryWindow, Inventory* inventory)
        : AbstractWindow(inventoryWindow, AbstractWindow::WND_MOUSE_INPUT)
    {
        mInventory = inventory;

        //mSquareSize = ...;

        createSlotWindows(inventory);
        initInventoryWindow(inventory);
    }

    InventoryWindow::~InventoryWindow()
    {
    }

    void InventoryWindow::createSlotWindows(Inventory* inventory)
    {
        std::map<CeGuiString, Window*> slotsInWindowDefinition;

        Window* slotWindowParent = getWindow("InventoryWindow/Slots");

        for (size_t idx = 0; idx < slotWindowParent->getChildCount(); idx++)
        {
            Window* curWnd = slotWindowParent->getChildAtIdx(idx);
            CeGuiString curWndName = curWnd->getName();
            if (curWndName.find("InventoryWindow/Slots/") != CeGuiString::npos)
            {
                CeGuiString slotname = 
                    curWndName.substr(
                        curWndName.find("InventoryWindow/Slots/") + 22);
                slotsInWindowDefinition[slotname] = curWnd;
            }
            else
            {
                LOG_DEBUG(Logger::UI, "Not a slot: "+curWndName);
            }
        }

        Inventory::SlotMap slots = inventory->getAllSlots();
        for (Inventory::SlotMap::const_iterator it = slots.begin(); it != slots.end(); it++)
        {
            CeGuiString slotName = (*it).first;
            std::map<CeGuiString, Window*>::iterator 
                slotInWindowFile = slotsInWindowDefinition.find(slotName);

            if (slotInWindowFile != slotsInWindowDefinition.end())
            {
                mSlotWindows[slotName] = (*slotInWindowFile).second; 
				mSlotWindows[slotName]->setUserString(SLOTNAME, slotName);
                slotsInWindowDefinition.erase(slotInWindowFile);
                LOG_DEBUG(Logger::UI, "Found slot "+slotName);
            }
            else
            {
                LOG_DEBUG(Logger::UI, "Not found slot "+slotName);
                ///@todo Create tab
                // mSlotWindows.push_back();
            }
        }

		///@todo deactivate all remaining elements of slotsInWindowDefinition
    }

    void InventoryWindow::initInventoryWindow(Inventory* inventory)
    {
        for (std::map<CeGuiString, Window*>::iterator it = mSlotWindows.begin(); it != mSlotWindows.end(); it++)
        {
            CeGuiString slotName = (*it).first;
            Item* item = inventory->getItem(slotName);
			Window* slotWindow = (*it).second;
			slotWindow->setDragDropTarget(true);
			slotWindow->subscribeEvent(Window::EventDragDropItemDropped, 
				boost::bind(&InventoryWindow::handleItemDroppedOnSlot, this, _1));

            if (item != NULL)
            {
                LOG_MESSAGE(Logger::UI, slotName + " - " + item->getDescription());
				Window* itemWindow = createItemWindow(slotName, item);
				if (itemWindow != NULL)
				{
					slotWindow->addChildWindow(itemWindow);
				}
            }
        }
    }

	Window* InventoryWindow::createItemWindow(const CeGuiString& slotName, Item* item)
	{
        CeGuiString icon = item->getImageName();

		if (icon == "")
		{
			icon = ICON_UNKNOWN_ITEM;
		}

        LOG_MESSAGE("IW", icon);
        Window* itemWindow = 
	        CEGUI::WindowManager::getSingletonPtr()->createWindow(
		    "RastullahLook/StaticImage", 
            mWindow->getName() + "/" + slotName + "/" + icon);
        itemWindow->setProperty("Image", icon);
        itemWindow->setPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));
        itemWindow->setSize(
            UVector2(cegui_absdim(item->getSize().first*30),
                     cegui_absdim(item->getSize().second*30)));

		itemWindow->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&InventoryWindow::handleItemMouseClick, this, _1, item));

		itemWindow->subscribeEvent(
			Window::EventMouseDoubleClick,
			boost::bind(&InventoryWindow::handleItemDoubleClick, this, _1, item));
		
		ItemDragContainer* itemhandler = new ItemDragContainer(item, "DragContainer_"+itemWindow->getName());
		itemhandler->setItemParent(mInventory, slotName);

		itemhandler->setPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));
		itemhandler->setSize(
			UVector2(cegui_absdim(item->getSize().first*30),
					 cegui_absdim(item->getSize().second*30))); 
		itemhandler->setTooltipText(item->getName());
		itemhandler->addChildWindow(itemWindow);

		itemhandler->setTooltipText(item->getName());
		
		return itemhandler;
	}

	bool InventoryWindow::handleItemDroppedOnSlot(const EventArgs& evt)
	{
		const DragDropEventArgs& evtArgs = static_cast<const DragDropEventArgs&>(evt);

		if (evtArgs.dragDropItem->testClassName("ItemDragContainer"))
		{
			ItemDragContainer* dragcont = static_cast<ItemDragContainer*>(
				evtArgs.dragDropItem);
			Item* item = dragcont->getItem();
			CeGuiString targetSlot = evtArgs.window->getUserString(SLOTNAME);

			if (mInventory->canHold(item, targetSlot))
			{
				if (dragcont->getItemParentContainer() != NULL)
				{
					dragcont->getItemParentContainer()->removeItem(item);
					///@todo Swap with old content (if there is some)
				}
				else if (dragcont->getItemParentSlot() != "")
				{
					dragcont->getItemParentInventory()->dropItem(dragcont->getItemParentSlot());
					///@todo Swap with old content (if there is some)
				}

				mInventory->hold(item, targetSlot);
				dragcont->getParent()->removeChildWindow(dragcont);
				mSlotWindows[targetSlot]->addChildWindow(dragcont);
				dragcont->setPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));
				dragcont->setItemParent(mInventory, targetSlot);
				
				return true;
			}
			else
			{
				return false;
			}
		}

		return false;
	}

	bool InventoryWindow::handleItemMouseClick(const EventArgs& evt, Item* item)
	{
		const MouseEventArgs& mevt = static_cast<const MouseEventArgs&>(evt);
		if (mevt.button == RightButton)
		{
			WindowFactory::getSingleton().showActionChoice(item);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool InventoryWindow::handleItemDoubleClick(const EventArgs& evt, Item* item)
	{
		const MouseEventArgs& mevt = static_cast<const MouseEventArgs&>(evt);
		if (mevt.button == LeftButton)
		{
			item->doDefaultAction(mInventory->getOwner(), NULL);
			return true;
		}
		else
		{
			return false;
		}
	}

}
