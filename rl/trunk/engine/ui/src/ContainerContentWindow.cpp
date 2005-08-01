#include <CEGUIImagesetManager.h>
#include <CEGUIImage.h>
#include <CEGUIImageset.h>
#include <boost/bind.hpp>

#include "ContainerContentWindow.h"
#include "Container.h"
#include "ListboxImageItem.h"
#include "UiSubsystem.h"

#include "Item.h"

namespace rl {

	using namespace CEGUI;

	ContainerContentWindow::ContainerContentWindow(Container* container) 
		:  CeGuiWindow("containercontentwindow.xml", WND_MOUSE_INPUT) {
		mItemList = getListbox("ContainerContentWindow/ContentList");
		
		ItemSet items = container->getItems();

		for(ItemSet::iterator iter = items.begin(); iter != items.end(); iter++) {
			Item* gameitem = *iter;
			CeGuiString thumbname = gameitem->getName();
			const Image& image = ImagesetManager::getSingleton().getImageset("ModelThumbnails")->getImage(thumbname);
			ListboxImageItem* item = 
				new ListboxImageItem(&image, 0, gameitem);

			mItemList->addItem(item);
		}

		mItemList->subscribeEvent(
			Window::EventMouseClick, 
			boost::bind(&ContainerContentWindow::handleItemChoose, this));

		addToRoot(mWindow);	
	}

	bool ContainerContentWindow::handleItemChoose()
	{
		Item* currentItem = static_cast<Item*>(mItemList->getFirstSelectedItem()->getUserData());
		UiSubsystem::getSingleton().showActionChoice(currentItem);
		return true;
	}
}