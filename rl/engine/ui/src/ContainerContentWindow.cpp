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
#include "stdinc.h" //precompiled header

#include "ContainerContentWindow.h"

#include <CEGUI/PropertyHelper.h>
#include <CEGUI/WindowManager.h>

#include "CeGuiHelper.h"
#include "Container.h"
#include "Inventory.h"
#include "InventoryWindow.h"
#include "ItemIconDragContainer.h"
#include "WindowFactory.h"

using namespace CEGUI;

namespace rl
{

    int ContainerContentWindow::sItemCount = 0;

    ContainerContentWindow::ContainerContentWindow(Container* container, InventoryWindow* parent)
        : AbstractWindow("containercontentwindow.xml", WIT_MOUSE_INPUT)
        , mContainer(container)
        , mInventoryWindow(parent)
    {
        mContentWindow = getWindow("ContainerContentWindow/Content");
        mContentWindow->setUserData(container);
        mContentWindow->subscribeEvent(
            Window::EventDragDropItemDropped, &ContainerContentWindow::handleItemDroppedOnContainer, this);
        mContentWindow->subscribeEvent(
            Window::EventDragDropItemEnters, &ContainerContentWindow::handleItemEntersContainer, this);
        mContentWindow->subscribeEvent(
            Window::EventDragDropItemLeaves, &ContainerContentWindow::handleItemLeavesContainer, this);

        USize size
            = USize(cegui_absdim(container->getVolume().first * 30), cegui_absdim(container->getVolume().second * 30));
        mContentWindow->setSize(size);
        size.d_width += cegui_absdim(40);
        size.d_height += cegui_absdim(50);
        mContentWindow->getParent()->setMaxSize(size);
        mContentWindow->getParent()->setMinSize(size);

        initializeContent();

        bindDestroyWindowToXButton();
    }

    bool ContainerContentWindow::handleItemEntersContainer(const CEGUI::EventArgs& evt)
    {
        const DragDropEventArgs& evtArgs = static_cast<const DragDropEventArgs&>(evt);

        if (evtArgs.dragDropItem->getType() == "ItemDragContainer")
        {
            ItemDragContainer* dragcont = dynamic_cast<ItemDragContainer*>(evtArgs.dragDropItem);
            Item* item = dragcont->getItem();

            if (item->getParentContainer() == mContainer)
                return true;

            if (!mContainer->canHold(item))
            {
                mContentWindow->setProperty(
                    "ContainerColour", mContentWindow->getProperty("ContainerColour_DropImpossible"));
            }

            return true;
        }
        return false;
    }

    bool ContainerContentWindow::handleItemLeavesContainer(const CEGUI::EventArgs& evt)
    {
        const DragDropEventArgs& evtArgs = static_cast<const DragDropEventArgs&>(evt);

        if (evtArgs.dragDropItem->getType() == "ItemDragContainer")
        {
            mContentWindow->setProperty("ContainerColour", mContentWindow->getProperty("ContainerColour_Standard"));

            return true;
        }
        return false;
    }

    void ContainerContentWindow::setVisible(bool visible, bool destroyAfterHide)
    {
        if (!visible && destroyAfterHide)
        {
            if (mInventoryWindow)
                mInventoryWindow->notifyContainerContentWindowClosed(mContainer);
        }

        AbstractWindow::setVisible(visible, destroyAfterHide);
    }

    void ContainerContentWindow::setVisible(bool visible, bool destroyAfterHide, bool dontNotifyInventory)
    {
        if (!dontNotifyInventory)
        {
            setVisible(visible, destroyAfterHide);
            return;
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
            itemWindow->setPosition(UVector2(cegui_absdim(pos.first * 30), cegui_absdim(pos.second * 30)));

            itemWindow->subscribeEvent(Window::EventMouseClick,
                [this, item](const CEGUI::EventArgs& args) { return handleItemMouseClick(args, item); });

            itemWindow->subscribeEvent(Window::EventMouseDoubleClick,
                [this, item](const CEGUI::EventArgs& args) { return handleItemDoubleClick(args, item); });

            mContentWindow->addChild(itemWindow);
        }
    }

    bool ContainerContentWindow::handleItemDroppedOnContainer(const EventArgs& evt)
    {
        const DragDropEventArgs& evtArgs = static_cast<const DragDropEventArgs&>(evt);

        if (evtArgs.dragDropItem->getType() == "ItemDragContainer")
        {
            ItemDragContainer* dragcont = dynamic_cast<ItemDragContainer*>(evtArgs.dragDropItem);
            Item* item = dragcont->getItem();

            auto relPos = CeGuiHelper::asAbsolute(evtArgs.dragDropItem->getPosition(), dragcont->getParentPixelSize())
                - CeGuiHelper::asAbsolute(mContentWindow->getPosition(), mContentWindow->getParentPixelSize());

            // uebergangspixel
            float x = (relPos.d_x.d_offset + 14.F) / 30.F;
            float y = (relPos.d_y.d_offset + 14.F) / 30.F;

            if (mContainer->addItem(item, IntPair(static_cast<int>(x), static_cast<int>(y))))
            {
                if (dragcont != getItemWindow(item))
                {
                    CEGUI::WindowManager::getSingleton().destroyWindow(dragcont);
                    // dragcont->destroyWindow();
                    dragcont = createItemWindow(item);
                    mContentWindow->addChild(dragcont);
                }
                std::pair<unsigned int, unsigned int> pos = mContainer->getItemPosition(item);
                dragcont->setPosition(UVector2(cegui_absdim(pos.first * 30), cegui_absdim(pos.second * 30)));
                dragcont->setItemParent(mContainer);

                handleItemLeavesContainer(evt);
                return true;
            }
        }
        handleItemLeavesContainer(evt);
        return false;
    }

    ItemDragContainer* ContainerContentWindow::createItemWindow(Item* item)
    {
        ItemDragContainer* itemhandler = getItemWindow(item);
        if (itemhandler)
            return itemhandler;

        CeGuiString dragContainerName
            = mWindow->getName() + "/item/" + Ogre::StringConverter::toString(item->getId()) + "_DragContainer";

        itemhandler = dynamic_cast<ItemIconDragContainer*>(
            AbstractWindow::loadWindow("itemicondragcontainer.xml", dragContainerName));
        // CEGUI::WindowManager::getSingleton().createWindow("ItemIconDragContainer", dragContainerName));
        itemhandler->setItem(item);
        // itemhandler = new ItemIconDragContainer(item, dragContainerName);
        itemhandler->setDestroyListener(this);
        mItemDragContainerMap.insert(std::make_pair(item, itemhandler));
        itemhandler->setItemParent(mContainer);
        itemhandler->setPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));
        if (mInventoryWindow)
        {
            auto inventoryWindow = mInventoryWindow;

            itemhandler->subscribeEvent(DragContainer::EventDragStarted,
                [inventoryWindow, item]() { return inventoryWindow->showPossibleSlots(item); });
            itemhandler->subscribeEvent(DragContainer::EventDragEnded,
                [inventoryWindow]() { return inventoryWindow->showPossibleSlots(nullptr); });
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
            item->doDefaultAction(NULL, NULL);
            return true;
        }
        else
        {
            return false;
        }
    }

    ItemDragContainer* ContainerContentWindow::getItemWindow(Item* item)
    {
        ItemDragContainerMap::iterator iter = mItemDragContainerMap.find(item);
        if (iter != mItemDragContainerMap.end())
            return iter->second;

        return NULL;
    }

    void ContainerContentWindow::notifyItemDragContainerDestroyed(ItemDragContainer* cont)
    {
        ItemDragContainerMap::iterator iter = mItemDragContainerMap.begin();
        for (; iter != mItemDragContainerMap.end(); iter++)
        {
            if (iter->second == cont)
            {
                mItemDragContainerMap.erase(iter);
                return;
            }
        }

        LOG_ERROR(Logger::UI, "Could not find ItemDragContainer in InventoryWindow::notifyItemDragContainerDestroyed!");
    }
}
