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
#include "InventoryWindow.h"
#include "ItemIconDragContainer.h"
#include "WindowFactory.h"

using namespace CEGUI;

namespace rl {

	int ContainerContentWindow::sItemCount = 0;

	ContainerContentWindow::ContainerContentWindow(Container* container, InventoryWindow* parent)
		: AbstractWindow("containercontentwindow.xml", WIT_MOUSE_INPUT),
		mContainer(container),
        mInventoryWindow(parent)
	{
		mContentWindow = getWindow("ContainerContentWindow/Content");
		mContentWindow->setUserData(container);
		mContentWindow->subscribeEvent(
			Window::EventDragDropItemDropped,
			boost::bind(&ContainerContentWindow::handleItemDroppedOnContainer, this, _1));
		mContentWindow->subscribeEvent(
            Window::EventDragDropItemEnters,
			boost::bind(&ContainerContentWindow::handleItemEntersContainer, this, _1));
		mContentWindow->subscribeEvent(
            Window::EventDragDropItemLeaves,
			boost::bind(&ContainerContentWindow::handleItemLeavesContainer, this, _1));

        UVector2 size = UVector2(
			cegui_absdim(container->getVolume().first*30),
            cegui_absdim(container->getVolume().second*30));
		mContentWindow->setSize(size);
        size.d_x += cegui_absdim(40);
        size.d_y += cegui_absdim(50);
        mContentWindow->getParent()->setMaxSize(size);
        mContentWindow->getParent()->setMinSize(size);

		initializeContent();

		bindDestroyWindowToXButton();
	}

    bool ContainerContentWindow::handleItemEntersContainer(const CEGUI::EventArgs& evt)
    {
		const DragDropEventArgs& evtArgs = static_cast<const DragDropEventArgs&>(evt);

		if (evtArgs.dragDropItem->testClassName("ItemDragContainer"))
		{
			ItemDragContainer* dragcont = static_cast<ItemDragContainer*>(
				evtArgs.dragDropItem);
			Item* item = dragcont->getItem();

            if( item->getParentContainer() == mContainer )
                return true;

            if( !mContainer->canHold(item) )
            {
                mContentWindow->setProperty("ContainerColour", 
                    mContentWindow->getProperty("ContainerColour_DropImpossible"));
            }


            return true;
        }
        return false;
    }

    bool ContainerContentWindow::handleItemLeavesContainer(const CEGUI::EventArgs& evt)
    {
        const DragDropEventArgs& evtArgs = static_cast<const DragDropEventArgs&>(evt);

		if (evtArgs.dragDropItem->testClassName("ItemDragContainer"))
		{
            mContentWindow->setProperty("ContainerColour", 
                mContentWindow->getProperty("ContainerColour_Standard"));

            return true;
        }
        return false;
    }

    void ContainerContentWindow::setVisible(bool visible, bool destroyAfterHide)
    {
        if( !visible && destroyAfterHide )
        {
            if( mInventoryWindow )
                mInventoryWindow->notifyContainerContentWindowClosed(mContainer);
        }

        AbstractWindow::setVisible(visible, destroyAfterHide);
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

            if( item->getParentContainer() == mContainer )
                return false;

			if( mContainer->addItem(item) )
            {
			    dragcont->getParent()->removeChildWindow(dragcont);
			    std::pair<unsigned int, unsigned int> pos = mContainer->getItemPosition(item);
			    mContentWindow->addChildWindow(dragcont);
			    dragcont->setPosition(
				    UVector2(
					    cegui_absdim(pos.first*30),
					    cegui_absdim(pos.second*30)));
			    dragcont->setItemParent(mContainer);

                handleItemLeavesContainer(evt);
			    return true;
            }
		}
        handleItemLeavesContainer(evt);
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
        if( mInventoryWindow )
        {
            itemhandler->subscribeEvent(DragContainer::EventDragStarted,
                boost::bind(&rl::InventoryWindow::showPossibleSlots, mInventoryWindow, item));
            itemhandler->subscribeEvent(DragContainer::EventDragEnded,
                boost::bind(&InventoryWindow::showPossibleSlots, mInventoryWindow, (Item*)NULL));
        }

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
