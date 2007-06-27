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
#include "ItemDragContainer.h"

namespace rl {

	ItemDragContainer::ItemDragContainer(Item* item, const CeGuiString& name)
		: CEGUI::DragContainer("DragContainer", name),
		mItem(item),
		mParentContainer(NULL),
		mParentSlot("")
	{
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

	void ItemDragContainer::setContentWindow(CEGUI::Window* window)
	{
		if (window)
		{
			window->setPosition(CEGUI::UVector2(cegui_reldim(0), cegui_reldim(0)));
			window->setSize(CEGUI::UVector2(cegui_reldim(1), cegui_reldim(1)));
			addChildWindow(window);
		}
		mContentWindow = window;
	}
}
