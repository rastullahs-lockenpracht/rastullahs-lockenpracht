#include "ContainerContentWindow.h"
#include "Container.h"
#include "ListboxImageItem.h"
#include <CEGUIImagesetManager.h>
#include <CEGUIImage.h>
#include <CEGUIImageset.h>
#include "Item.h"

namespace rl {

	using namespace CEGUI;

	ContainerContentWindow::ContainerContentWindow(Container* container) 
		:  CeGuiWindow("containercontentwindow.xml", WND_MOUSE_INPUT) {
		mItemList = getListbox("ContainerContentWindow/ContentList");
		
		ItemSet items = container->getItems();

		for(ItemSet::iterator iter = items.begin(); iter != items.end(); iter++) {
			Item* gameitem = *iter;
			const Image& image = ImagesetManager::getSingleton().getImageset("ModelThumbnails")->getImage(gameitem->getName());
			ListboxImageItem* item = 
				new ListboxImageItem(&image, 0, gameitem);

			mItemList->addItem(item);
		}
	}

}