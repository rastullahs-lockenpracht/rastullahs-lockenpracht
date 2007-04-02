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
#ifndef __ItemDragContainer_H__
#define __ItemDragContainer_H__

#include "UiPrerequisites.h"

#include <elements/CEGUIDragContainer.h>

namespace rl 
{
	class Container;
	class Inventory;
	class Item;
	class Slot;

	class ItemDragContainer : public CEGUI::DragContainer
	{
	public:
		ItemDragContainer(Item* item, const CeGuiString& name);
		
		void setItemParent(Container* container);
		void setItemParent(Inventory* inventory, const CeGuiString& slotname);
		Container* getItemParentContainer() const;
		const CeGuiString& getItemParentSlot() const;
		Inventory* getItemParentInventory() const;
		Item* getItem() const;

	protected:
		virtual bool testClassName_impl(const CEGUI::String& class_name) const;

	private:
		Item* mItem;
		Container* mParentContainer;
		CeGuiString mParentSlot;
		Inventory* mInventory;
	};

} // namespace rl

#endif //__ItemDragContainer_H__