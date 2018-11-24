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

#include "InventoryWindow.h"

#include "AbstractWindow.h"
#include "Actor.h"
#include "ActorManager.h"
#include "CameraObject.h"
#include "CeGuiHelper.h"
#include "CommandMapper.h"
#include "Container.h"
#include "ContainerContentWindow.h"
#include "ControlState.h"
#include "Creature.h"
#include "InputManager.h"
#include "Inventory.h"
#include "Item.h"
#include "ItemDescriptionDragContainer.h"
#include "ItemIconDragContainer.h"
#include "Selector.h"
#include "WindowFactory.h"

#include <CEGUI/WindowManager.h>
#include <CEGUI/widgets/FrameWindow.h>

using namespace CEGUI;
using namespace Ogre;
using namespace std;

namespace rl
{

    const Ogre::String InventoryWindow::SLOTNAME = "slotname";

    InventoryWindow::InventoryWindow(const Ogre::String& inventoryWindow, Inventory* inventory)
        : AbstractWindow(inventoryWindow, AbstractWindow::WIT_MOUSE_INPUT | AbstractWindow::WIT_KEYBOARD_INPUT)
        , mShowAllWorldItems(false)
    {
        mInventory = inventory;

        // mSquareSize = ...;
        mWorldBackground = getWindow("InventoryWindow/Background");
        CEGUI::Window* invWnd = getWindow("InventoryWindow");
        invWnd->subscribeEvent(FrameWindow::EventCloseClicked, [this]() { return destroyWindow(); });
        mWorldBackground->moveToBack();
        mWorldBackground->setZOrderingEnabled(false);
        invWnd->setMaxSize(invWnd->getSize());
        invWnd->setMinSize(invWnd->getSize());

        createSlotWindows(inventory);
        initInventoryWindow(inventory);

        mMouseSelector = new RaySelector(QUERYFLAG_ITEM, true);
    }

    InventoryWindow::~InventoryWindow()
    {
        mWorldBackground->removeAllEvents();
        delete mMouseSelector;
        WindowFactory::getSingleton().notifyInventoryWindowDestroyed();
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
                CeGuiString slotname = curWndName.substr(curWndName.find("InventoryWindow/Slots/") + 22);
                slotsInWindowDefinition[slotname] = curWnd;
                curWnd->setTooltipText(slotname);
            }
            else
            {
                LOG_DEBUG(Logger::UI, "Not a slot: " + curWndName);
            }
        }

        Inventory::SlotMap slots = inventory->getAllSlots();
        for (Inventory::SlotMap::const_iterator it = slots.begin(); it != slots.end(); it++)
        {
            CeGuiString slotName = (*it).first;
            std::map<CeGuiString, Window*>::iterator slotInWindowFile = slotsInWindowDefinition.find(slotName);

            if (slotInWindowFile != slotsInWindowDefinition.end())
            {
                mSlotWindows[slotName] = (*slotInWindowFile).second;
                mSlotWindows[slotName]->setUserString(SLOTNAME, slotName);
                slotsInWindowDefinition.erase(slotInWindowFile);
                LOG_DEBUG(Logger::UI, "Found slot " + slotName);
            }
            else
            {
                LOG_DEBUG(Logger::UI, "Not found slot " + slotName);
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
            slotWindow->subscribeEvent(
                Window::EventDragDropItemDropped, &InventoryWindow::handleItemDroppedOnSlot, this);
            slotWindow->subscribeEvent(Window::EventDragDropItemEnters, &InventoryWindow::handleItemEntersSlot, this);
            slotWindow->subscribeEvent(Window::EventDragDropItemLeaves, &InventoryWindow::handleItemLeavesSlot, this);

            if (item != NULL)
            {
                LOG_MESSAGE(Logger::UI, slotName + " - " + item->getDescription());
                Window* itemWindow = createItemDragContainer(item, false, slotName);
                if (itemWindow != NULL)
                {
                    slotWindow->addChild(itemWindow);
                }
            }
        }

        mWorldBackground->setDragDropTarget(true);
        mWorldBackground->subscribeEvent(
            Window::EventDragDropItemDropped, &InventoryWindow::handleItemDroppedOnWorld, this);
        mWorldBackground->subscribeEvent(Window::EventMouseMove, &InventoryWindow::handleMouseMovedInWorld, this);

        // be sure we get all key-events:
        Window* invWnd = getWindow("InventoryWindow");
        // invWnd->setDistributesCapturedInputs(false);
        invWnd->subscribeEvent(Window::EventKeyDown, [this](const CEGUI::EventArgs& args) { handleKeys(args, true); });
        invWnd->subscribeEvent(Window::EventKeyUp, [this](const CEGUI::EventArgs& args) { handleKeys(args, false); });

        // mWorldBackground->setDistributesCapturedInputs(false);
        mWorldBackground->subscribeEvent(
            Window::EventKeyDown, [this](const CEGUI::EventArgs& args) { handleKeys(args, true); });
        mWorldBackground->subscribeEvent(
            Window::EventKeyUp, [this](const CEGUI::EventArgs& args) { handleKeys(args, false); });

        invWnd->activate();
    }

    ItemDragContainer* InventoryWindow::getItemDragContainer(const Item* item, bool description)
    {
        CeGuiString dragContainerName = getDragContainerNameFromItem(item, description);

        DndContainerMap::iterator iter = mWorldDragContainers.find(dragContainerName);
        if (iter != mWorldDragContainers.end())
        {
            return iter->second;
        }
        iter = mSlotDragContainers.find(dragContainerName);
        if (iter != mSlotDragContainers.end())
        {
            return iter->second;
        }

        return NULL;
    }

    CeGuiString InventoryWindow::getDragContainerNameFromItem(const Item* item, bool description)
    {
        CeGuiString dragContainerName = mWindow->getName() + "/item/" + Ogre::StringConverter::toString(item->getId())
            + "_DragContainer" + (description ? "_D" : "_I");
        return dragContainerName;
    }

    ItemDragContainer* InventoryWindow::createItemDragContainer(
        Item* item, bool showdescription, const CeGuiString& slotName)
    {
        CeGuiString dragContainerName = getDragContainerNameFromItem(item, showdescription);
        ItemDragContainer* itemhandler = NULL;

        DndContainerMap::iterator itW = mWorldDragContainers.find(dragContainerName);
        DndContainerMap::iterator itS = mSlotDragContainers.find(dragContainerName);
        if (itW != mWorldDragContainers.end() || itS != mSlotDragContainers.end())
        {
            return NULL;
        }
        else
        {
            if (showdescription)
            {
                itemhandler = dynamic_cast<ItemDescriptionDragContainer*>(
                    AbstractWindow::loadWindow("itemdescriptiondragcontainer.xml", dragContainerName));
                // new ItemDescriptionDragContainer(item, dragContainerName);
                itemhandler->setItem(item);
            }
            else
            {
                itemhandler = dynamic_cast<ItemIconDragContainer*>(
                    AbstractWindow::loadWindow("itemicondragcontainer.xml", dragContainerName));
                // CEGUI::WindowManager::getSingleton().createWindow("ItemIconDragContainer", dragContainerName));
                itemhandler->setItem(item);
                // itemhandler = new ItemIconDragContainer(item, dragContainerName);
                itemhandler->setTooltipText(item->getName());
            }

            if (slotName != "")
            {
                itemhandler->setItemParent(mInventory, slotName);
                mSlotDragContainers[dragContainerName] = itemhandler;
            }
            else
            {
                mWorldDragContainers[dragContainerName] = itemhandler;
            }

            itemhandler->setDestroyListener(this);

            itemhandler->setRiseOnClickEnabled(true);
            itemhandler->setPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));
            itemhandler->subscribeEvent(DragContainer::EventDragStarted,
                [this, item, showdescription]() { handleItemDragStarted(item, showdescription); });
            itemhandler->subscribeEvent(DragContainer::EventDragEnded,
                [this, item, showdescription]() { handleItemDragEnded(item, showdescription); });
        }

        return itemhandler;
    }

    bool InventoryWindow::handleItemDragEnded(const Item* item, bool showdescription)
    {
        showPossibleSlots(NULL);

        if (showdescription && !mShowAllWorldItems) // this is a world item
        {
            // fade out and destroy
            ItemDragContainer* cont = getItemDragContainer(item, showdescription);
            if (cont)
                cont->fadeOutAndHide(2.0f);
        }

        return true;
    }

    bool InventoryWindow::handleItemDragStarted(const Item* item, bool showdescription)
    {
        showPossibleSlots(item);

        if (showdescription && !mShowAllWorldItems) // this is a world item
        {
            // stop fade out and destroy
            ItemDragContainer* cont = getItemDragContainer(item, showdescription);
            if (cont)
                cont->stopFadeOut();
        }

        return true;
    }

    bool InventoryWindow::handleItemDroppedOnSlot(const EventArgs& evt)
    {
        const DragDropEventArgs& evtArgs = static_cast<const DragDropEventArgs&>(evt);

        if (evtArgs.dragDropItem->getType() == "ItemDragContainer")
        {
            ItemDragContainer* dragcont = static_cast<ItemDragContainer*>(evtArgs.dragDropItem);
            Item* item = dragcont->getItem();
            CeGuiString targetSlot = evtArgs.window->getUserString(SLOTNAME);

            if (mInventory->canHold(item, targetSlot))
            {
                dragcont->getParent()->removeChild(dragcont);

                ItemDragContainer* newCont = createItemDragContainer(item, false, targetSlot);

                if (newCont)
                {
                    CEGUI::WindowManager::getSingleton().destroyWindow(dragcont);
                    //                    dragcont->destroyWindow();
                }
                else
                {
                    newCont = dragcont;
                }

                CEGUI::Window* slotWindow = mSlotWindows[targetSlot];

                mInventory->hold(item, targetSlot);

                slotWindow->addChild(newCont);
                newCont->setPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));
                newCont->setItemParent(mInventory, targetSlot);

                showPossibleSlots(NULL);
                return true;
            }
            else
            {
                showPossibleSlots(NULL);
                return false;
            }
        }

        handleItemLeavesSlot(evt);
        return false;
    }

    bool InventoryWindow::handleItemEntersSlot(const CEGUI::EventArgs& evt)
    {
        const DragDropEventArgs& evtArgs = static_cast<const DragDropEventArgs&>(evt);

        if (evtArgs.dragDropItem->getType() == "ItemDragContainer")
        {
            ItemDragContainer* dragcont = static_cast<ItemDragContainer*>(evtArgs.dragDropItem);
            Item* item = dragcont->getItem();
            CeGuiString targetSlot = evtArgs.window->getUserString(SLOTNAME);
            CEGUI::Window* slotWindow = mSlotWindows[targetSlot];

            if (mInventory->canReady(item, targetSlot))
            {
                // slotWindow->setProperty("ContainerColour",
                //    slotWindow->getProperty("ContainerColour_DropReady"));
            }
            else if (mInventory->canHold(item, targetSlot))
            {
                // slotWindow->setProperty("ContainerColour",
                //    slotWindow->getProperty("ContainerColour_DropPossible"));
            }
            else
            {
                slotWindow->setProperty("ContainerColour", slotWindow->getProperty("ContainerColour_DropImpossible"));
            }

            return true;
        }
        return false;
    }

    bool InventoryWindow::handleItemLeavesSlot(const CEGUI::EventArgs& evt)
    {
        const DragDropEventArgs& evtArgs = static_cast<const DragDropEventArgs&>(evt);

        if (evtArgs.dragDropItem->getType() == "ItemDragContainer")
        {
            ItemDragContainer* dragcont = static_cast<ItemDragContainer*>(evtArgs.dragDropItem);
            Item* item = dragcont->getItem();
            CeGuiString targetSlot = evtArgs.window->getUserString(SLOTNAME);
            CEGUI::Window* slotWindow = mSlotWindows[targetSlot];

            if (mInventory->canReady(item, targetSlot))
            {
                // slotWindow->setProperty("ContainerColour",
                //    slotWindow->getProperty("ContainerColour_Standard"));
            }
            else if (mInventory->canHold(item, targetSlot))
            {
                // slotWindow->setProperty("ContainerColour",
                //    slotWindow->getProperty("ContainerColour_Standard"));
            }
            else
            {
                slotWindow->setProperty("ContainerColour", slotWindow->getProperty("ContainerColour_Standard"));
            }

            return true;
        }
        return false;
    }

    bool InventoryWindow::handleItemDroppedOnWorld(const EventArgs& evt)
    {
        const DragDropEventArgs& evtArgs = static_cast<const DragDropEventArgs&>(evt);
        if (evtArgs.dragDropItem->getType() == "ItemDragContainer")
        {
            ItemDragContainer* dragcont = static_cast<ItemDragContainer*>(evtArgs.dragDropItem);
            Item* item = dragcont->getItem();

            auto relPos = CeGuiHelper::asRelative(dragcont->getPosition(), getRoot()->getPixelSize());

            Ogre::Vector3 targetPosWindow{ relPos.d_x.d_scale, relPos.d_y.d_scale, -1.F };

            CEGUI::WindowManager::getSingleton().destroyWindow(dragcont);
            //            dragcont->destroyWindow();

            Ogre::Vector3 targetPosWorldSpace
                = mInventory->getOwner()->getPosition() + mInventory->getOwner()->getOrientation() * targetPosWindow;
            item->removeOldState();
            item->setState(GOS_IN_SCENE);
            item->setPosition(targetPosWorldSpace);

            LOG_DEBUG(Logger::UI,
                "Dropped item " + item->getName() + " to position "
                    + Ogre::StringConverter::toString(targetPosWorldSpace));

            return true;
        }
        return false;
    }

    bool InventoryWindow::handleMouseMovedInWorld(const EventArgs& evt)
    {
        if (mShowAllWorldItems)
            return true;

        const MouseEventArgs& mevt = static_cast<const MouseEventArgs&>(evt);

        Actor* cameraActor = ActorManager::getSingleton().getActor("DefaultCamera");
        CameraObject* camera = static_cast<CameraObject*>(cameraActor->getControlledObject());

        auto mousePos = mevt.position;
        mousePos.d_x /= getRoot()->getPixelSize().d_width;
        mousePos.d_y /= getRoot()->getPixelSize().d_height;
        Ogre::Ray camToWorld = camera->getCameraToViewportRay(mousePos.d_x, mousePos.d_y);
        Ogre::Vector3 rayStart = camera->getCamera()->getDerivedPosition();
        Ogre::Vector3 rayDir = camera->getDirectionFromScreenPosition(mousePos.d_x, mousePos.d_y);

        mMouseSelector->setRay(camToWorld.getOrigin(), camToWorld.getPoint(3));
        mMouseSelector->updateSelection();
        GameObjectList objs = mMouseSelector->getAllSelectedObjects();

        ///@todo select, ...
        if (!objs.empty())
        {
            LOG_MESSAGE(Logger::UI, "Selected " + Ogre::StringConverter::toString(objs.size()) + " items.");
            for (GameObjectList::const_iterator it = objs.begin(); it != objs.end(); ++it)
            {
                if (!mInventory->getOwner()->canReachItem(static_cast<Item*>(*it)))
                    continue;

                ItemDragContainer* cont = getItemDragContainer(static_cast<Item*>(*it), true);

                if (!cont)
                {
                    cont = createItemDragContainer(static_cast<Item*>(*it), true);
                    mWorldBackground->addChild(cont);
                }
                if (cont)
                {
                    auto aabb = getCeGuiRectFromWorldAABB(camera, (*it)->getActor()->_getSceneNode()->_getWorldAABB());
                    UVector2 posCont
                        = UVector2(UDim((aabb.left + aabb.right) / 2.0F, 0), UDim((aabb.top + aabb.bottom) / 2.0F, 0));
                    posCont -= CeGuiHelper::toPosition(cont->getSize()) * 0.5F;
                    cont->setPosition(posCont);
                    cont->setVisible(true);

                    cont->fadeOutAndHide(2.0f);
                }
            }
        }

        return true;
    }

    bool InventoryWindow::handleKeys(const CEGUI::EventArgs& evt, bool down)
    {
        const KeyEventArgs& kevt = static_cast<const KeyEventArgs&>(evt);

        ///@todo das hier ueberpruefen!
        CeGuiString action = InputManager::getSingleton().getControlState()->getCommandMapper()->getControlStateAction(
            kevt.scancode, InputManager::getSingleton().getControlState()->getType());
        if (action == "")
        {
            action = InputManager::getSingleton().getControlState()->getCommandMapper()->getGlobalAction(kevt.scancode);
        }
        if (action == "inventoryshowworldobjects") // we only handle this action here
        {
            if (down)
            {
                mShowAllWorldItems = true;

                Actor* cameraActor = ActorManager::getSingleton().getActor("DefaultCamera");
                CameraObject* camera = static_cast<CameraObject*>(cameraActor->getControlledObject());

                HalfSphereSelector sel(QUERYFLAG_ITEM);
                sel.setPosition(cameraActor->getWorldPosition());
                sel.setOrientation(cameraActor->getWorldOrientation());
                sel.setRadius(10.0);

                sel.updateSelection();
                GameObjectList v = sel.getAllSelectedObjects();
                for (GameObjectList::iterator it = v.begin(); it != v.end(); ++it)
                {
                    if (!mInventory->getOwner()->canReachItem(static_cast<Item*>(*it)))
                        continue;

                    ItemDragContainer* cont = getItemDragContainer(static_cast<Item*>(*it), true);

                    if (!cont)
                    {
                        cont = createItemDragContainer(static_cast<Item*>(*it), true);
                        mWorldBackground->addChild(cont);
                    }
                    if (cont)
                    {
                        auto aabb
                            = getCeGuiRectFromWorldAABB(camera, (*it)->getActor()->_getSceneNode()->_getWorldAABB());
                        UVector2 posCont = UVector2(
                            UDim((aabb.left + aabb.right) / 2.0F, 0), UDim((aabb.top + aabb.bottom) / 2.0F, 0));
                        posCont -= CeGuiHelper::toPosition(cont->getSize()) * 0.5F;
                        cont->setPosition(posCont);
                        cont->setVisible(true);
                        cont->moveToFront();
                        cont->stopFadeOut();
                    }
                }
            }
            else
            {
                mShowAllWorldItems = false;
                DndContainerMap::iterator iter = mWorldDragContainers.begin();
                for (; iter != mWorldDragContainers.end(); iter++)
                {
                    iter->second->fadeOutAndHide(0.0f);
                }
            }

            return true;
        }

        return false;
    }

    Ogre::Rect InventoryWindow::getCeGuiRectFromWorldAABB(CameraObject* camera, const AxisAlignedBox& aabb) const
    {
        // Initialise each to the value of the opposite side, so that min/max work smoothly.
        Real left = 1.0f, bottom = 1.0f, right = -1.0f, top = -1.0f;

        // Determine screen pos of all corners and widen the rect if needed
        const auto& corners = aabb.getAllCorners();
        for (size_t i = 0; i < 8; ++i)
        {
            Ogre::Vector3 screenSpacePos = camera->getPointOnCeGuiScreen(corners[i]);
            if (screenSpacePos.z > 0)
                continue; // Behind camera

            left = std::min(left, screenSpacePos.x);
            right = std::max(right, screenSpacePos.x);
            bottom = std::min(bottom, screenSpacePos.y);
            top = std::max(top, screenSpacePos.y);
        }

        Ogre::Rect rval = { left, top, right, bottom };
        return rval;
    }

    void InventoryWindow::showContainerContent(Container* container)
    {
        ContainerMap::iterator iter = mOpenContainerMap.find(container);
        if (iter == mOpenContainerMap.end())
        {
            ContainerContentWindow* wnd = new ContainerContentWindow(container, this);
            mOpenContainerMap.insert(make_pair(container, wnd));
            mWorldBackground->addChild(wnd->getWindow());
            wnd->setVisible(true);
        }
        else
        {
            iter->second->setVisible(true);
            iter->second->getWindow()->moveToFront();
        }
    }

    void InventoryWindow::setVisible(bool visible, bool destroyAfterHide)
    {
        if (!visible && destroyAfterHide)
        {
            ContainerMap::iterator iter = mOpenContainerMap.begin();
            for (; iter != mOpenContainerMap.end(); iter++)
            {
                if (iter->second != NULL)
                {
                    iter->second->setVisible(
                        false, true, true); // if we call notifyInventoryWindow here, the iterator isn't valid any more!
                }
            }
            mOpenContainerMap.erase(mOpenContainerMap.begin(), mOpenContainerMap.end());
        }

        AbstractWindow::setVisible(visible, destroyAfterHide);
    }

    void InventoryWindow::notifyContainerContentWindowClosed(Container* container)
    {
        ContainerMap::iterator iter = mOpenContainerMap.find(container);
        if (iter != mOpenContainerMap.end())
        {
            mOpenContainerMap.erase(iter);
        }
    }

    bool InventoryWindow::showPossibleSlots(const Item* item)
    {
        SlotWindowMap::iterator iter = mSlotWindows.begin();

        for (; iter != mSlotWindows.end(); iter++)
        {
            if (item)
            {
                if (mInventory->canReady(item, iter->first))
                {
                    iter->second->setProperty(
                        "ContainerColour", iter->second->getProperty("ContainerColour_DropReady"));
                    continue;
                }
                else if (mInventory->canHold(item, iter->first))
                {
                    iter->second->setProperty(
                        "ContainerColour", iter->second->getProperty("ContainerColour_DropPossible"));
                    continue;
                }
            }
            iter->second->setProperty("ContainerColour", iter->second->getProperty("ContainerColour_Standard"));
        }
        return true;
    }

    void InventoryWindow::notifyItemDragContainerDestroyed(ItemDragContainer* cont)
    {
        DndContainerMap::iterator iter = mWorldDragContainers.begin();
        for (; iter != mWorldDragContainers.end(); iter++)
        {
            if (iter->second == cont)
            {
                mWorldDragContainers.erase(iter);
                return;
            }
        }

        iter = mSlotDragContainers.begin();
        for (; iter != mSlotDragContainers.end(); iter++)
        {
            if (iter->second == cont)
            {
                mSlotDragContainers.erase(iter);
                return;
            }
        }

        LOG_ERROR(Logger::UI, "Could not find ItemDragContainer in InventoryWindow::notifyItemDragContainerDestroyed!");
    }
}
