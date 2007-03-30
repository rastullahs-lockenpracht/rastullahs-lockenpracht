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
#include <elements/CEGUIDragContainer.h>
#include <elements/CEGUIFrameWindow.h>


using namespace CEGUI;
using namespace std;

namespace rl {

    InventoryWindow::InventoryWindow(const Ogre::String& inventoryWindow, Inventory* inventory)
        : AbstractWindow(inventoryWindow, AbstractWindow::WND_MOUSE_INPUT)
    {
        mInventory = inventory;

        mContainerTabs = getTabControl("InventoryWindow/Tabs");
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

        Window* containers = getWindow("InventoryWindow/Slots");

        for (size_t idx = 0; idx < containers->getChildCount(); idx++)
        {
            Window* curWnd = containers->getChildAtIdx(idx);
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
            if (item != NULL)
            {
                LOG_MESSAGE(Logger::UI, (*it).first + " - " + item->getDescription());
                CeGuiString icon = item->getImageName();
                if (icon != "" /*&& icon.find("/") != CeGuiString::npos*/)
                {
                    LOG_MESSAGE("IW", icon);
                    Window* itemWindow = 
				        CEGUI::WindowManager::getSingletonPtr()->createWindow(
					    "RastullahLook/StaticImage", 
                        mWindow->getName() + "/" + slotName + "/" + icon);
                    itemWindow->setProperty("Image", icon);
                    (*it).second->addChildWindow(itemWindow);
                    itemWindow->setPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));
                    itemWindow->setSize(
                        UVector2(cegui_absdim(item->getSize().first*30),
                                 cegui_absdim(item->getSize().second*30)));
                }
            }
        }
    }
}
