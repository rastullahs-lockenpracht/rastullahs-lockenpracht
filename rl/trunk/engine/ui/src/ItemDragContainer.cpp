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

#include "ItemDragContainer.h"

#include <CEGUIWindowManager.h>
#include <CEGUIEventSet.h>

#include "AbstractWindow.h"
#include "Creature.h"
#include "Inventory.h"
#include "Item.h"
#include "JobScheduler.h"
#include "WindowFactory.h"
#include "WindowFadeJob.h"

namespace rl {
	const Ogre::String ItemDragContainer::ICON_UNKNOWN_ITEM = "set:ModelThumbnails image:item_unknown";

	ItemDragContainer::ItemDragContainer(Item* item, const CeGuiString& name)
		: CEGUI::DragContainer("DragContainer", name),
		mItem(item),
		mParentContainer(NULL),
		mParentSlot(""),
		mContentWindow(NULL),
        mInventory(NULL),
        mMarkedToHideAndDestroy(false),
        mDestroyListener(NULL)
	{
	}

	ItemDragContainer::~ItemDragContainer()
	{
        if(mDestroyListener)
            mDestroyListener->notifyItemDragContainerDestroyed(this);
        stopFadeOut();

        hide();
		if (getParent())
		{
			getParent()->removeChildWindow(this);
		}
		removeAllEvents();
		mContentWindow->removeAllEvents();
		removeChildWindow(mContentWindow);
		CEGUI::WindowManager::getSingleton().destroyWindow(mContentWindow);
        CEGUI::WindowManager::getSingleton().destroyWindow(this);
	}

    void ItemDragContainer::destroyWindow()
    {
        if(mDestroyListener)
            mDestroyListener->notifyItemDragContainerDestroyed(this);
        mDestroyListener = NULL;
        stopFadeOut();

        hide();
		if (getParent())
		{
			getParent()->removeChildWindow(this);
		}

        JobScheduler::getSingleton().addJob(
            new WindowFadeJob(this,
            WindowFadeJob::FADE_OUT_AND_DESTROY, 1.0f, 9999999999999.9f),
            JobScheduler::JP_NORMAL,
            0.0f);
    }

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

	bool ItemDragContainer::testClassName_impl(const CEGUI::String& class_name) const
	{
		if (class_name=="ItemDragContainer")	return true;
		return DragContainer::testClassName_impl(class_name);
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

        mHideAndDestroyJobTicket =
                JobScheduler::getSingleton().addJob(
                    new WindowFadeJob(this,
                    WindowFadeJob::FADE_OUT, 1.0f, 0.5f),
                    JobScheduler::JP_NORMAL,
                    delay);
        mMarkedToHideAndDestroy = true;
        return true;
    }

    bool ItemDragContainer::stopFadeOut()
    {
        if( mMarkedToHideAndDestroy )
        {
            JobScheduler::getSingleton().removeJob(mHideAndDestroyJobTicket);
        }
        mMarkedToHideAndDestroy = false;
        setAlpha(1.0f);

        return true;
    }
}
