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

#include "ItemDescriptionDragContainer.h"

#include <boost/bind.hpp>

#include "AbstractWindow.h"
#include "Item.h"

namespace rl {
	ItemDescriptionDragContainer::ItemDescriptionDragContainer(Item* item, const CeGuiString& name)
		: ItemDragContainer(item, name)
	{
		CeGuiString icon = item->getImageName();

		if (icon == "")
		{
			icon = ICON_UNKNOWN_ITEM;
		}

		CeGuiString prefix = name;
		mContentWindow = AbstractWindow::loadWindow("itemdescriptiondragcontainer.xml", prefix);

		mContentWindow->getChild(
			name+"ItemDescriptionDragContainer/Icon")
			->setProperty("Image", icon);
		mContentWindow->getChild(
			name+"ItemDescriptionDragContainer/Name")
			->setText(item->getName());
		mContentWindow->getChild(
			name+"ItemDescriptionDragContainer/Description")
			->setText(item->getDescription());

		mContentWindow->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&ItemDragContainer::_handleItemMouseClick, this, _1, item));

		mContentWindow->subscribeEvent(
			Window::EventMouseDoubleClick,
			boost::bind(&ItemDragContainer::_handleItemDoubleClick, this, _1, item));

		setSize(mContentWindow->getSize());
		addChildWindow(mContentWindow);
	}
}
