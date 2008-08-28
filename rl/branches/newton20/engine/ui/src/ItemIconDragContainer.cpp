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

#include "ItemIconDragContainer.h"

#include <boost/bind.hpp>

#include "AbstractWindow.h"
#include "Item.h"

namespace CEGUI{
    CEGUI_DEFINE_WINDOW_FACTORY(ItemIconDragContainer)
}
namespace rl {

    const CeGuiString ItemIconDragContainer::WidgetTypeName("ItemIconDragContainer");



	ItemIconDragContainer::ItemIconDragContainer(const CeGuiString &type, const CeGuiString& name)
		: ItemDragContainer(type, name)
	{
		CeGuiString prefix = name;
		mContentWindow = this; //AbstractWindow::loadWindow("itemicondragcontainer.xml", prefix);


		for (size_t idx = 0; idx < mContentWindow->getChildCount(); ++idx)
		{
			LOG_MESSAGE(Logger::UI, mContentWindow->getChildAtIdx(idx)->getName());
		}

		addChildWindow(mContentWindow);
                mContentWindow->setDestroyedByParent(true);
	}

        void ItemIconDragContainer::setItem(Item* item)
        {
            ItemDragContainer::setItem(item);




		CeGuiString icon = item->getImageName();

		if (icon == "")
		{
			icon = ICON_UNKNOWN_ITEM;
		}
                const CeGuiString &name = getName();



		mContentWindow->getChild(name+"/Icon")
			->setProperty("Image", icon);

		mContentWindow->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&ItemDragContainer::_handleItemMouseClick, this, _1, item));

		mContentWindow->subscribeEvent(
			Window::EventMouseDoubleClick,
			boost::bind(&ItemDragContainer::_handleItemDoubleClick, this, _1, item));

		setSize(CEGUI::UVector2(
			cegui_absdim(item->getSize().first*30),
			cegui_absdim(item->getSize().second*30)));
		mContentWindow->setSize(CEGUI::UVector2(
			cegui_absdim(item->getSize().first*30),
			cegui_absdim(item->getSize().second*30)));


        }
}
