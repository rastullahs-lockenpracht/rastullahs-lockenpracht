/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
#include <CEGUIImagesetManager.h>
#include <CEGUIImage.h>
#include <CEGUIImageset.h>
#include <boost/bind.hpp>

#include "ContainerContentWindow.h"
#include "Container.h"
#include "ListboxImageItem.h"
#include "WindowManager.h"

#include "Item.h"

namespace rl {

	using namespace CEGUI;

	ContainerContentWindow::ContainerContentWindow(Container* container) 
		: CeGuiWindow("containercontentwindow.xml", WND_MOUSE_INPUT),
		mContainer(container)
	{
		mItemList = getListbox("ContainerContentWindow/ContentList");
		mItemList->setItemTooltipsEnabled(true);

		ItemSet items = container->getItems();

		for(ItemSet::iterator iter = items.begin(); iter != items.end(); iter++) {
			Item* gameitem = *iter;
			CeGuiString thumbname = gameitem->getName();
			const Image& image = ImagesetManager::getSingleton().getImageset("ModelThumbnails")->getImage(thumbname);
			ListboxImageItem* item = 
				new ListboxImageItem(&image, 0, gameitem);
			item->setTooltipText(gameitem->getDescription());

			mItemList->addItem(item);
		}

		mItemList->subscribeEvent(
			Window::EventMouseDoubleClick, 
			boost::bind(&ContainerContentWindow::handleItemUse, this));
		getWindow("ContainerContentWindow/ContentFrameWindow")->subscribeEvent(
			FrameWindow::EventCloseClicked,
			boost::bind(&ContainerContentWindow::handleClose, this));
		getWindow("ContainerContentWindow/CloseButton")->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&ContainerContentWindow::handleClose, this));

		getWindow("ContainerContentWindow/UseButton")->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&ContainerContentWindow::handleItemUse, this));
	}

	bool ContainerContentWindow::handleItemUse()
	{
		if (mItemList->getFirstSelectedItem() != NULL)
		{
			Item* currentItem = static_cast<Item*>(mItemList->getFirstSelectedItem()->getUserData());
			WindowManager::getSingleton().showActionChoice(currentItem);
		}
		return true;
	}

	bool ContainerContentWindow::handleClose()
	{
		mContainer->close();
		destroyWindow();
		return true;
	}
}
