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
#include "stdinc.h" //precompiled header

#include "ContainerContentWindow.h"

#include <boost/bind.hpp>
#include <CEGUIImageset.h>
#include <CEGUIImagesetManager.h>
#include <CEGUIPropertyHelper.h>
#include <CEGUIWindowManager.h>

#include "Container.h"
#include "Inventory.h"
#include "ItemIconDragContainer.h"
#include "WindowFactory.h"

using namespace CEGUI;

namespace rl {

	int ContainerContentWindow::sItemCount = 0;

	ContainerContentWindow::ContainerContentWindow(Container* container)
		: AbstractWindow("containercontentwindow.xml", WIT_MOUSE_INPUT),
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

		bindDestroyWindowToXButton();
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

            itemWindow->subscribeEvent(
                Window::EventMouseClick,
                boost::bind(&ContainerContentWindow::handleItemMouseClick, this, _1, item));

            itemWindow->subscribeEvent(
                Window::EventMouseDoubleClick,
                boost::bind(&ContainerContentWindow::handleItemDoubleClick, this, _1, item));

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

				dragcont->getParent()->removeChildWindow(dragcont);
				std::pair<unsigned int, unsigned int> pos = mContainer->getItemPosition(item);
				mContentWindow->addChildWindow(dragcont);
				dragcont->setPosition(
					UVector2(
						cegui_absdim(pos.first*30),
						cegui_absdim(pos.second*30)));
				dragcont->setItemParent(mContainer);

				///@todo Swap with old content (if there is some)
			}
			else if (dragcont->getItemParentSlot() != "")
			{
				dragcont->getItemParentInventory()->dropItem(dragcont->getItemParentSlot());
				mContainer->addItem(item);

				dragcont->getParent()->removeChildWindow(dragcont);
				std::pair<unsigned int, unsigned int> pos = mContainer->getItemPosition(item);
				mContentWindow->addChildWindow(dragcont);
				dragcont->setPosition(
					UVector2(
						cegui_absdim(pos.first*30),
						cegui_absdim(pos.second*30)));
				dragcont->setItemParent(mContainer);

				///@todo Swap with old content (if there is some)
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
			icon = ItemDragContainer::ICON_UNKNOWN_ITEM;
		}

   //     LOG_MESSAGE("IW", icon);
   //     Window* itemWindow =
	  //      CEGUI::WindowManager::getSingletonPtr()->createWindow(
		 //   "RastullahLook/StaticImage",
			//mWindow->getName() + "/" + icon
			//+ "/" + CEGUI::PropertyHelper::intToString(sItemCount++));
   //     itemWindow->setProperty("Image", icon);
   //     itemWindow->setPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));
   //     itemWindow->setSize(UVector2(cegui_reldim(1), cegui_reldim(1)));
		CeGuiString dragContainerName =
			mWindow->getName() +  "/item/"
			+ Ogre::StringConverter::toString(item->getId())+"_DragContainer";

		ItemDragContainer* itemhandler = new ItemIconDragContainer(item,
			dragContainerName);
		itemhandler->setItemParent(mContainer);
		itemhandler->setPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));

		return itemhandler;
	}

    bool ContainerContentWindow::handleItemMouseClick(const EventArgs& evt, Item* item)
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

    bool ContainerContentWindow::handleItemDoubleClick(const EventArgs& evt, Item* item)
    {
        const MouseEventArgs& mevt = static_cast<const MouseEventArgs&>(evt);
        if (mevt.button == LeftButton)
        {
            item->doDefaultAction(NULL,NULL);
            return true;
        }
        else
        {
            return false;
        }
    }
}
