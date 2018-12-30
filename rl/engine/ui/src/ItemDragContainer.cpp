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

#include "ItemDragContainer.h"

#include <CEGUI/EventSet.h>
#include <CEGUI/WindowManager.h>

#include "AbstractWindow.h"
#include "Creature.h"
#include "Inventory.h"
#include "Item.h"
#include "JobScheduler.h"
#include "WindowFactory.h"
#include "WindowFadeJob.h"

namespace rl
{
    const Ogre::String ItemDragContainer::ICON_UNKNOWN_ITEM = "set:ModelThumbnails image:item_unknown";

    ItemDragContainer::ItemDragContainer(const CeGuiString& type, const CeGuiString& name)
        : CEGUI::DragContainer(type, name)
        , mItem(NULL)
        , mParentContainer(NULL)
        , mParentSlot("")
        , mContentWindow(NULL)
        , mInventory(NULL)
        , mMarkedToHideAndDestroy(false)
        , mDestroyListener(NULL)
    {
    }

    void ItemDragContainer::setItem(Item* item)
    {
        if (mItem != NULL)
            Throw(IllegalArgumentException, "The item of an ItemDragContainer can only be set once!");
        mItem = item;
    }

    ItemDragContainer::~ItemDragContainer()
    {
        if (mDestroyListener)
            mDestroyListener->notifyItemDragContainerDestroyed(this);
        setDestroyListener(NULL);

        stopFadeOut();
    }

    void ItemDragContainer::destroy()
    {
        if (mDestroyListener)
            mDestroyListener->notifyItemDragContainerDestroyed(this);
        setDestroyListener(NULL);
        stopFadeOut();

        CEGUI::DragContainer::destroy();

        /*
                    hide();

                    if (getParent())
                    {
                        getParent()->removeChild(this);
                    }

                    removeAllEvents();
                    if( mContentWindow )
                    mContentWindow->removeAllEvents();
                    removeChild(mContentWindow);
                    CEGUI::WindowManager::getSingleton().destroyWindow(mContentWindow);
                    CEGUI::WindowManager::getSingleton().destroyWindow(this);
        */
    }
    /*
        void ItemDragContainer::destroyWindow()
        {
            if(mDestroyListener)
                mDestroyListener->notifyItemDragContainerDestroyed(this);
            mDestroyListener = NULL;
            stopFadeOut();

            hide();
            if (getParent())
            {
                getParent()->removeChild(this);
            }

            JobScheduler::getSingleton().addJob(
                new WindowFadeJob(this,
                WindowFadeJob::FADE_OUT_AND_DESTROY, 1.0f, 9999999999999.9f),
                JobScheduler::JP_NORMAL,
                0.0f);
        }
    */
    void ItemDragContainer::setItemParent(Container* container)
    {
        mParentContainer = container;
        mParentSlot = "";
        mInventory = NULL;
    }

    void ItemDragContainer::setItemParent(Inventory* inventory, const CeGuiString& slotname)
    {
        mParentSlot = slotname;
        mParentContainer = NULL;
        mInventory = inventory;
    }

    Container* ItemDragContainer::getItemParentContainer() const
    {
        return mParentContainer;
    }

    const CeGuiString& ItemDragContainer::getItemParentSlot() const
    {
        return mParentSlot;
    }

    Inventory* ItemDragContainer::getItemParentInventory() const
    {
        return mInventory;
    }

    Item* ItemDragContainer::getItem() const
    {
        return mItem;
    }

    CEGUI::Window* ItemDragContainer::getContentWindow() const
    {
        return mContentWindow;
    }

    bool ItemDragContainer::_handleItemMouseClick(const CEGUI::EventArgs& evt, Item* item)
    {
        const CEGUI::MouseEventArgs& mevt = static_cast<const CEGUI::MouseEventArgs&>(evt);
        if (mevt.button == CEGUI::RightButton)
        {
            WindowFactory::getSingleton().showActionChoice(item);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool ItemDragContainer::_handleItemDoubleClick(const CEGUI::EventArgs& evt, Item* item)
    {
        const CEGUI::MouseEventArgs& mevt = static_cast<const CEGUI::MouseEventArgs&>(evt);
        if (mevt.button == CEGUI::LeftButton)
        {
            item->doDefaultAction(static_cast<Creature*>(item->getOwner()), NULL);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool ItemDragContainer::fadeOutAndHide(Ogre::Real delay)
    {
        stopFadeOut();

        mHideAndDestroyJobTicket = JobScheduler::getSingleton().addJob(
            new WindowFadeJob(this, WindowFadeJob::FADE_OUT, 1.0f, 0.5f), JobScheduler::JP_NORMAL, delay);
        mMarkedToHideAndDestroy = true;
        return true;
    }

    bool ItemDragContainer::stopFadeOut()
    {
        if (mMarkedToHideAndDestroy)
        {
            JobScheduler::getSingleton().removeJob(mHideAndDestroyJobTicket);
        }
        mMarkedToHideAndDestroy = false;
        setAlpha(1.0f);

        return true;
    }

    void ItemDragContainer::setDestroyListener(ItemDragContainerDestroyListener* listener)
    {
        if (listener == mDestroyListener)
            return;

        if (mDestroyListener)
            mDestroyListener->removeDragContainer(this);

        mDestroyListener = listener;

        if (listener)
            mDestroyListener->addDragContainer(this);
    }

    ItemDragContainerDestroyListener::ItemDragContainerDestroyListener()
        : mIsDestroying(false)
    {
    }

    ItemDragContainerDestroyListener::~ItemDragContainerDestroyListener()
    {
        // remove all dragContainers from list
        mIsDestroying = true;
        for (DndContainerMap::iterator it = mContainers.begin(); it != mContainers.end(); it++)
            it->second->setDestroyListener(NULL);
    }

    void ItemDragContainerDestroyListener::addDragContainer(ItemDragContainer* dragcont)
    {
        if (dragcont == NULL)
            Throw(NullPointerException,
                "Parameter dragcont in ItemDragContainerDestroyListener::addDragContainer should not be NULL!");

        DndContainerMap::iterator it = mContainers.find(dragcont->getName());
        if (it != mContainers.end())
        {
            Throw(IllegalArgumentException,
                "ItemDragContainerDestroyListener::addDragContainer: ItemDragContainer with name '"
                    + dragcont->getName() + "' already added!");
        }

        mContainers.insert(std::make_pair(dragcont->getName(), dragcont));
    }

    void ItemDragContainerDestroyListener::removeDragContainer(ItemDragContainer* dragcont)
    {
        if (mIsDestroying) // if this Listener is destroyed, we don't need to care about the list
            return;

        if (dragcont == NULL)
            Throw(NullPointerException,
                "Parameter dragcont in ItemDragContainerDestroyListener::removeDragContainer should not be NULL!");

        DndContainerMap::iterator it = mContainers.find(dragcont->getName());
        if (it == mContainers.end())
        {
            Throw(IllegalArgumentException,
                "ItemDragContainerDestroyListener::removeDragContainer: ItemDragContainer with name '"
                    + dragcont->getName() + "' was not added before!");
        }

        mContainers.erase(it);
    }
}
