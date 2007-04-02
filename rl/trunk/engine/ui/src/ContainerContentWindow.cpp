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
#include "ContainerContentWindow.h"

#include <boost/bind.hpp>
#include <CEGUIImageset.h>
#include <CEGUIImagesetManager.h>
#include <CEGUIPropertyHelper.h>
#include <CEGUIWindowManager.h>

#include "Container.h"
#include "InventoryWindow.h"
#include "ItemDragContainer.h"

using namespace CEGUI;

namespace rl {

	int ContainerContentWindow::sItemCount = 0;

	ContainerContentWindow::ContainerContentWindow(Container* container) 
		: AbstractWindow("containercontentwindow.xml", WND_MOUSE_INPUT),
		mContainer(container)
	{
		mContentWindow = getWindow("ContainerContentWindow/Content");
		mContentWindow->setUserData(container);
		mContentWindow->subscribeEvent(
			Window::EventDragDropItemDropped,
			boost::bind(&ContainerContentWindow::handleItemDroppedOnContainer, this, _1));

		mContentWindow->setSize(UVector2(
			cegui_absdim(container->getVolume().first*30),
            cegui_absdim(container->getVolume().second*30)));

		initializeContent();

		bindCloseToCloseButton();
		centerWindow();
	}

	void ContainerContentWindow::initializeContent()
	{
		ItemSet items = mContainer->getItems();
		for (ItemSet::const_iterator it = items.begin(); it != items.end(); it++)
		{
			Item* item = *it;
			Window* itemWindow = createItemWindow(item);

			std::pair<unsigned int, unsigned int> pos = mContainer->getItemPosition(item);
			itemWindow->setPosition(
				UVector2(
					cegui_absdim(pos.first*30),
					cegui_absdim(pos.second*30)));

			mContentWindow->addChildWindow(itemWindow);
		}
	}

	bool ContainerContentWindow::handleItemDroppedOnContainer(const EventArgs& evt)
	{
		const DragDropEventArgs& evtArgs = static_cast<const DragDropEventArgs&>(evt);

		if (evtArgs.dragDropItem->testClassName("ItemDragContainer"))
		{
			ItemDragContainer* dragcont = static_cast<ItemDragContainer*>(
				evtArgs.dragDropItem);
			Item* item = dragcont->getItem();

			if (dragcont->getItemParentContainer() != NULL)
			{
				dragcont->getItemParentContainer()->removeItem(item);
				mContainer->addItem(item);
			}
			else if (dragcont->getItemParentSlot() != "")
			{
				dragcont->getItemParentInventory()->dropItem(dragcont->getItemParentSlot());
				mContainer->addItem(item);
			}
			
			return true;
		}
		return false;
	}

	Window* ContainerContentWindow::createItemWindow(Item* item)
	{
        CeGuiString icon = item->getImageName();

		if (icon == "")
		{
			icon = InventoryWindow::ICON_UNKNOWN_ITEM;
		}

        LOG_MESSAGE("IW", icon);
        Window* itemWindow = 
	        CEGUI::WindowManager::getSingletonPtr()->createWindow(
		    "RastullahLook/StaticImage", 
			mWindow->getName() + "/" + icon 
			+ "/" + CEGUI::PropertyHelper::intToString(sItemCount++));
        itemWindow->setProperty("Image", icon);
        itemWindow->setPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));
        itemWindow->setSize(
            UVector2(cegui_absdim(item->getSize().first*30),
                     cegui_absdim(item->getSize().second*30)));

		ItemDragContainer* itemhandler = new ItemDragContainer(item, 
			"DragContainer_"+itemWindow->getName());
		itemhandler->setItemParent(mContainer);
		itemhandler->setPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));
		itemhandler->setSize(
			UVector2(cegui_absdim(item->getSize().first*30),
					 cegui_absdim(item->getSize().second*30))); 
		itemhandler->setUserData(item);
		itemhandler->setTooltipText(item->getName());
		itemhandler->addChildWindow(itemWindow);
		

		return itemhandler;
	}
}
