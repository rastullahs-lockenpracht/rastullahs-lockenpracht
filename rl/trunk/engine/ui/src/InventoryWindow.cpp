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

#include "InventoryWindow.h"

#include <boost/bind.hpp>
#include <CEGUIImageset.h>
#include <CEGUIImagesetManager.h>
#include <CEGUIWindowManager.h>

#include "Actor.h"
#include "ActorManager.h"
#include "CameraObject.h"
#include "Creature.h"
#include "Inventory.h"
#include "Item.h"
#include "ItemDescriptionDragContainer.h"
#include "ItemIconDragContainer.h"
#include "Selector.h"
#include "WindowFactory.h"

using namespace CEGUI;
using namespace Ogre;
using namespace std;

namespace rl {

	const Ogre::String InventoryWindow::SLOTNAME = "slotname";

    InventoryWindow::InventoryWindow(const Ogre::String& inventoryWindow, Inventory* inventory)
        : AbstractWindow(inventoryWindow, AbstractWindow::WIT_MOUSE_INPUT)
    {
        mInventory = inventory;

        //mSquareSize = ...;
		mWorldBackground = getWindow("InventoryWindow/Background");

        createSlotWindows(inventory);
        initInventoryWindow(inventory);

		mMouseSelector = new RaySelector(QUERYFLAG_ITEM, true);
    }

    InventoryWindow::~InventoryWindow()
    {
		mWorldBackground->removeAllEvents();
		delete mMouseSelector;
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
				Window* itemWindow = createItemDragContainer(item, false, slotName);
				if (itemWindow != NULL)
				{
					slotWindow->addChildWindow(itemWindow);
				}
            }
        }

		mWorldBackground->setDragDropTarget(true);
		mWorldBackground->subscribeEvent(Window::EventDragDropItemDropped,
			boost::bind(&InventoryWindow::handleItemDroppedOnWorld, this, _1));
		mWorldBackground->subscribeEvent(Window::EventMouseMove,
			boost::bind(&InventoryWindow::handleMouseMovedInWorld, this, _1));
		mWorldBackground->subscribeEvent(Window::EventKeyDown,
			boost::bind(&InventoryWindow::handleKeys, this, _1, true));
		mWorldBackground->subscribeEvent(Window::EventKeyUp,
			boost::bind(&InventoryWindow::handleKeys, this, _1, false));
    }

	ItemDragContainer* InventoryWindow::createItemDragContainer(
		Item* item, bool showdescription, const CeGuiString& slotName)
	{
		CeGuiString dragContainerName =
			mWindow->getName() +  "/item/"
			+ Ogre::StringConverter::toString(item->getId())+"_DragContainer";
		ItemDragContainer* itemhandler = NULL;

		DndContainerMap::iterator it = mDragContainers.find(dragContainerName);
		if (it != mDragContainers.end())
		{
			//itemhandler = it->second;
			return NULL; ///@todo just a test
		}
		else
		{
			if (showdescription)
				itemhandler = new ItemDescriptionDragContainer(item, dragContainerName);
			else
				itemhandler = new ItemIconDragContainer(item, dragContainerName);

			if (slotName != "")
			{
				itemhandler->setItemParent(mInventory, slotName);
			}

			Window* itemWindow = itemhandler->getContentWindow();

			itemWindow->subscribeEvent(
				Window::EventMouseClick,
				boost::bind(&InventoryWindow::handleItemMouseClick, this, _1, item));

			itemWindow->subscribeEvent(
				Window::EventMouseDoubleClick,
				boost::bind(&InventoryWindow::handleItemDoubleClick, this, _1, item));

			itemhandler->setPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));

			mDragContainers[dragContainerName] = itemhandler;
		}

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
				else
				{
					dragcont->removeEvent(Window::EventMouseLeaves);
					mDragContainers.erase(dragcont->getName());
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

	bool InventoryWindow::handleItemDroppedOnWorld(const EventArgs& evt)
	{
		const DragDropEventArgs& evtArgs = static_cast<const DragDropEventArgs&>(evt);
		if (evtArgs.dragDropItem->testClassName("ItemDragContainer"))
		{
			ItemDragContainer* dragcont = static_cast<ItemDragContainer*>(
				evtArgs.dragDropItem);
			Item* item = dragcont->getItem();
			Ogre::Vector3 targetPosWindow(
				dragcont->getPixelRect().d_left / getRoot()->getPixelSize().d_width,
				dragcont->getPixelRect().d_top / getRoot()->getPixelSize().d_height,
				-1);

			if (dragcont->getItemParentContainer() != NULL)
			{
				dragcont->getItemParentContainer()->removeItem(item);
				dragcont->getParent()->removeChildWindow(dragcont);
				CEGUI::WindowManager::getSingleton().destroyWindow(dragcont->getContentWindow());
				CEGUI::WindowManager::getSingleton().destroyWindow(dragcont);
			}
			else if (dragcont->getItemParentSlot() != "")
			{
				dragcont->getItemParentInventory()->dropItem(dragcont->getItemParentSlot());

				dragcont->getParent()->removeChildWindow(dragcont);
				CEGUI::WindowManager::getSingleton().destroyWindow(dragcont->getContentWindow());
				CEGUI::WindowManager::getSingleton().destroyWindow(dragcont);
			}
			else
			{
				destroyDragContainer(dragcont);
			}

			Ogre::Vector3 targetPosWorldSpace =
				mInventory->getOwner()->getPosition()
				+ mInventory->getOwner()->getOrientation()
				* targetPosWindow; ///@todo check why coordinates are negative
			item->placeIntoScene();
			item->setPosition(targetPosWorldSpace);

			LOG_DEBUG(Logger::UI,
				"Dropped item "+item->getName()
				+" to position "+Ogre::StringConverter::toString(targetPosWorldSpace));

			return true;

		}
		return false;
	}

	bool InventoryWindow::handleMouseMovedInWorld(const EventArgs& evt)
	{
		const MouseEventArgs& mevt = static_cast<const MouseEventArgs&>(evt);

		Actor* cameraActor = ActorManager::getSingleton().getActor("DefaultCamera");
		CameraObject* camera = static_cast<CameraObject*>(cameraActor->getControlledObject());

		CEGUI::Point mousePos = mevt.position;
		mousePos.d_x /= getRoot()->getPixelSize().d_width;
		mousePos.d_y /= getRoot()->getPixelSize().d_height;
		Ogre::Ray camToWorld = camera->getCameraToViewportRay(
			mousePos.d_x, mousePos.d_y);
		Ogre::Vector3 rayStart = camera->getCamera()->getWorldPosition();
		Ogre::Vector3 rayDir = camera->getDirectionFromScreenPosition(
			mousePos.d_x, mousePos.d_y);

		mMouseSelector->setRay(camToWorld.getOrigin(), camToWorld.getPoint(3));
		mMouseSelector->updateSelection();
		Selector::GameObjectVector objs = mMouseSelector->getAllSelectedObjects();

		///@todo select, ...
		if (!objs.empty())
		{
			LOG_MESSAGE(Logger::UI,
				"Selected "+Ogre::StringConverter::toString(objs.size())+" items.");
			for (Selector::GameObjectVector::const_iterator it = objs.begin();
				it != objs.end(); ++it)
			{
				LOG_MESSAGE(Logger::UI,
					"Selected " + (*it)->getDescription());

				ItemDragContainer* cont =
					createItemDragContainer(static_cast<Item*>(*it), true);
				if (cont)
				{
					mWorldBackground->addChildWindow(cont);
					cont->setVisible(true);

					Ogre::Rectangle aabb = getCeGuiRectFromWorldAABB(camera,
							(*it)->getActor()->_getSceneNode()->_getWorldAABB());
					UVector2 posCont = UVector2(
							UDim((aabb.left+aabb.right)/2.0, 0),
							UDim((aabb.top+aabb.bottom)/2.0, 0));
					posCont -= cont->getSize() / UVector2(UDim(2, 2), UDim(2, 2));
					cont->setPosition(posCont);

					cont->subscribeEvent(
						Window::EventMouseLeaves,
						boost::bind(&InventoryWindow::destroyDragContainer, this, cont));
				}
			}
		}

		//camera->getPointOnScreen(

		return true;
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

	bool InventoryWindow::handleKeys(const CEGUI::EventArgs &evt, bool down)
	{
		const KeyEventArgs& kevt = static_cast<const KeyEventArgs&>(evt);
		if (kevt.scancode == CEGUI::Key::Tab)
		{
			if (down)
			{
				Actor* cameraActor = ActorManager::getSingleton().getActor("DefaultCamera");
				CameraObject* camera = static_cast<CameraObject*>(cameraActor->getControlledObject());

				HalfSphereSelector sel(QUERYFLAG_ITEM);
				sel.setPosition(cameraActor->getWorldPosition());
				sel.setOrientation(cameraActor->getWorldOrientation());
				sel.setRadius(2.0);

				sel.updateSelection();
				Selector::GameObjectVector v = sel.getAllSelectedObjects();
				for (Selector::GameObjectVector::iterator
					it = v.begin(); it != v.end(); ++it)
				{
					ItemDragContainer* cont =
						createItemDragContainer(static_cast<Item*>(*it), true);

					if (cont)
					{
						mWorldBackground->addChildWindow(cont);

						Ogre::Rectangle aabb = getCeGuiRectFromWorldAABB(camera,
							(*it)->getActor()->_getSceneNode()->_getWorldAABB());
						UVector2 posCont = UVector2(
							UDim((aabb.left+aabb.right)/2.0, 0),
							UDim((aabb.top+aabb.bottom)/2.0, 0));
						posCont -= cont->getSize() / UVector2(UDim(2, 2), UDim(2, 2));
						cont->setPosition(posCont);

						cont->subscribeEvent(
							Window::EventMouseLeaves,
							boost::bind(&InventoryWindow::destroyDragContainer, this, cont));
					}
				}
			}
			else
			{
				while (!mDragContainers.empty())
				{
					destroyDragContainer(mDragContainers.begin()->second);
				}
			}

			return true;
		}

		return false;
	}

	bool InventoryWindow::destroyDragContainer(rl::ItemDragContainer* cont)
	{
		cont->removeAllEvents();
		mWorldBackground->removeChildWindow(cont);
		mDragContainers.erase(cont->getName());
		CEGUI::WindowManager::getSingleton().destroyWindow(cont->getContentWindow());

		return true;
	}

	Ogre::Rectangle InventoryWindow::getCeGuiRectFromWorldAABB(
		CameraObject* camera,
        const AxisAlignedBox& aabb) const
    {
        // Initialise each to the value of the opposite side, so that min/max work smoothly.
        Real left = 1.0f, bottom = 1.0f, right = -1.0f, top = -1.0f;

        // Determine screen pos of all corners and widen the rect if needed
		const Ogre::Vector3* corners = aabb.getAllCorners();
        for (size_t i = 0; i < 8; ++i)
        {
			Ogre::Vector3 screenSpacePos = camera->getPointOnCeGuiScreen(corners[i]);
            if (screenSpacePos.z > 0) continue; // Behind camera

            left   = std::min(left,   screenSpacePos.x);
            right  = std::max(right,  screenSpacePos.x);
            bottom = std::min(bottom, screenSpacePos.y);
            top    = std::max(top,    screenSpacePos.y);
        }

        Ogre::Rectangle rval = {left,top, right, bottom};
        return rval;
    }
}
