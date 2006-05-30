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


#include <boost/bind.hpp>

#include "InventoryWindow.h"
#include "RulesSubsystem.h"
#include "Inventory.h"
#include "Creature.h"
#include "WindowFactory.h"
#include "GameLoop.h"


#include <CEGUIPropertyHelper.h>

using namespace CEGUI;

namespace rl {

	// @FIXME Wenn CeGUI Bug beseitigt hat:
	InventoryArrangeTask::InventoryArrangeTask() : GameTask()
	{
	}
	InventoryArrangeTask::~InventoryArrangeTask()
	{
	}

	void InventoryArrangeTask::run(Ogre::Real elapsedTime)
	{
		static InventoryWindow* invWin = WindowFactory::getSingletonPtr()->getInventoryWindow();
		if (invWin->droppedItem){
			Logger::getSingletonPtr()->log("InventoryWindow",Logger::LL_ERROR,String("Item placed"));
			invWin->updateItemPosition();
		}
	}

	/*!
	*		Überprüft, ob das Item von dem Slot akzeptiert wird
	*/
	bool checkTypeAccepted(CEGUI::Window* window, CEGUI::DragContainer* draggedItem){
		if (window->getUserString("ItemType").compare(draggedItem->getUserString("ItemType"))){
			return false;
		} else {
			return true;
		}
	}

	/*!
	* Behandlung für Mouse-over mit Item in der Maus
	* Überprüft, ob das Item passt, und färbt dementsprechend den Behälter ein
	*/
	bool handleDragEnter(const CEGUI::EventArgs& args)
	{
		using namespace CEGUI;
		// Event zu einem DragDropEvent machen
		const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);

		if (checkTypeAccepted(ddea.window, ddea.dragDropItem)){
			ddea.window->setProperty("ContainerColour", WindowFactory::getSingletonPtr()->getInventoryWindow()->colorAccept);
		} else {
			ddea.window->setProperty("ContainerColour", WindowFactory::getSingletonPtr()->getInventoryWindow()->colorReject);
			return false;
		}
		return true;
	} 
	bool handleDragLeave(const CEGUI::EventArgs& args)
	{
		using namespace CEGUI;

		const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
		ddea.window->setProperty("ContainerColour", WindowFactory::getSingletonPtr()->getInventoryWindow()->colorNormal);
		return true;
	} 
	bool handleDragDropped(const CEGUI::EventArgs& args)
	{
		using namespace CEGUI;

		InventoryWindow* invWin = WindowFactory::getSingletonPtr()->getInventoryWindow();
		const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
		ddea.window->setProperty("ContainerColour", invWin->colorNormal);


		Point relMouse = ddea.dragDropItem->windowToScreen( Point(0,0) );

		// Nur wenn das Item in den Slot passt, soll es auch dort gedroppt werden können
		if (checkTypeAccepted(ddea.window, ddea.dragDropItem)){
			ddea.window->addChildWindow(ddea.dragDropItem);

			invWin->posDraggedTo=CEGUI::Point(0.0,0.0);
			invWin->droppedItem = ddea.dragDropItem;
			invWin->containerDraggedTo = ddea.window;
			return true;
		} else {
			return false;
		}
	} 

	bool handleDragEnterBackpack(const CEGUI::EventArgs& args)
	{
		using namespace CEGUI;

		const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
		ddea.window->setProperty("ContainerColour", WindowFactory::getSingletonPtr()->getInventoryWindow()->colorAccept);
		
		return true;

	} 
	bool handleDragDroppedBackpack(const CEGUI::EventArgs& args)
	{
		using namespace CEGUI;
		InventoryWindow* invWin = WindowFactory::getSingletonPtr()->getInventoryWindow();

		char buf1[5];
		char buf2[5];

		const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
		ddea.window->setProperty("ContainerColour", invWin->colorNormal);
		
		
		
		Point absMouse = MouseCursor::getSingleton().getPosition();
		Point scrnPt = ddea.window->windowToScreen( Point(0,0) ); 
		Point relMouse = absMouse- ddea.dragDropItem->windowToScreen( Point(0,0) );

		//Bug in CeGUI (1. rel Mouse koordinate spinnt
		if (relMouse.d_x < 0) {
			relMouse.d_x = 15;
		}

		Logger::getSingletonPtr()->log("InventoryWindow",Logger::LL_MESSAGE,
			String("relMouse: Point x:").append(itoa(int(relMouse.d_x),buf1,10))
			.append(", Point y:")
			.append(itoa(int(relMouse.d_y),buf2,10)));

		Point pointInBackpack = absMouse-scrnPt;
		pointInBackpack -= relMouse;

		// Position des nächsten Kästchens bestimmen		
		if (pointInBackpack.d_x < 0){
			pointInBackpack.d_x = 0;
		}
		if (pointInBackpack.d_y < 0) {
			pointInBackpack.d_y = 0;
		}

		if (pointInBackpack.d_x > 16){
			pointInBackpack.d_x = (int(pointInBackpack.d_x) + 14) - ((int(pointInBackpack.d_x) + 14) % 30);
		} else {
			pointInBackpack.d_x = 0;
		}
		if (pointInBackpack.d_y > 16){
			pointInBackpack.d_y = (int(pointInBackpack.d_y) + 14 ) - ((int(pointInBackpack.d_y) + 14) % 30);
		} else {
			pointInBackpack.d_y = 0;
		}

		int xKaestchen = pointInBackpack.d_x / 30;
		int yKaestchen = pointInBackpack.d_y / 30;

		int width = ddea.dragDropItem->getAbsoluteWidth();
		int height = ddea.dragDropItem->getAbsoluteHeight();

		width = width / 30;
		height = height / 30;

		if (invWin->getInventory()->isFreeInBackpack(pair<int,int>(xKaestchen,yKaestchen),pair<int,int>(width,height) ))
		{
			ddea.window->addChildWindow(ddea.dragDropItem);

			invWin->posDraggedTo=pointInBackpack;
			invWin->droppedItem = ddea.dragDropItem;
			invWin->containerDraggedTo = ddea.window;

			Logger::getSingletonPtr()->log("InventoryWindow",Logger::LL_MESSAGE,
				String("Point in Backpack: Point x:").append(itoa(int(pointInBackpack.d_x),buf1,10))
				.append(", Point y:")
				.append(itoa(int(pointInBackpack.d_y),buf2,10)));
			return true;
		} 
		else 
		{
			return false;
		}
	}


	/* Konstruktor */
	InventoryWindow::InventoryWindow()
		: CeGuiWindow("inventorywindow.xml", WND_MOUSE_INPUT)
	{
		initSlots();

		// Farbschema für Containerkästchen initiieren 
		colorAccept = CEGUI::String("FF22FF22");
		colorReject = CEGUI::String("FFFF2222");
		colorNormal = CEGUI::String("FFFFFFFF");


		DragContainer* item1 = createItem("meinTrank",Item::ITEMTYPE_OTHER,pair<int,int>(1,1),"Trank");
		item1->setWindowPosition(UVector2(cegui_reldim(0.0f), cegui_reldim(0.0f)));

		DragContainer* item2 = createItem("trankName",Item::ITEMTYPE_GLOVE,pair<int,int>(2,2),"Handschuhe");
		item2->setWindowPosition(UVector2(cegui_absdim(30), cegui_absdim(0)));

		droppedItem = NULL;
		containerDraggedTo = NULL;


		GameLoopManager::getSingletonPtr()->addSynchronizedTask(new InventoryArrangeTask(), FRAME_ENDED);
	}
	
	InventoryWindow::~InventoryWindow()
	{
		if(mBackpackContent){
			mBackpackContent->destroy();
		}
	}


	/** Methoden */

	void InventoryWindow::setCharacter(Creature* character)
	{
		mCharacter = character;
		if (character){
			mInventory = mCharacter->getInventory();
		} else {
			mInventory = NULL;
		}
	}

	void update()
	{

	}

	Inventory* InventoryWindow::getInventory(){
		return mInventory;
	}
	
	void InventoryWindow::updateItemPosition(){
		
		// Positionieren
		droppedItem->setPosition(CEGUI::Absolute, posDraggedTo);
		droppedItem = NULL;

		// Loggen
		char buf1[5];
		char buf2[5];
		Logger::getSingletonPtr()->log("InventoryWindow",Logger::LL_MESSAGE,
			String("Position set to: Point x:").append(itoa(int(posDraggedTo.d_x),buf1,10))
			.append(", Point y:")
			.append(itoa(int(posDraggedTo.d_y),buf2,10)));

		Logger::getSingletonPtr()->log("InventoryWindow",Logger::LL_MESSAGE,String("updateItemPosition finished"));
	}

	void InventoryWindow::initSlots(){

		initBackpack(pair<int,int>(12,7));

		// Hole die einzelnen Slots aus dem XML-File
		mHelmet = getStaticImage("InventoryWindow/Helmet");
		mHelmet->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_HELMET));
		
		mRingLeft = getStaticImage("InventoryWindow/RingLeft");
		mRingLeft->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_RING));
		
		mRingRight = getStaticImage("InventoryWindow/RingRight");
		mRingRight->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_RING));
		
		mHandLeft = getStaticImage("InventoryWindow/HandLeft");
		mHandLeft->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_WEAPON));

		mHandRight = getStaticImage("InventoryWindow/HandRight");
		mHandRight->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_SHIELD));

		mGloves = getStaticImage("InventoryWindow/Gloves");
		mGloves->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_GLOVE));

		mBraceletLeft = getStaticImage("InventoryWindow/BraceletLeft");
		mBraceletLeft->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BRACELET));

		mBraceletRight = getStaticImage("InventoryWindow/BraceletRight");
		mBraceletRight->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BRACELET));

		mArmor = getStaticImage("InventoryWindow/Armor");
		mArmor->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_ARMOR));

		mBracerRight = getStaticImage("InventoryWindow/BracerRight");
		mBracerRight->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BRACER));

		mBracerLeft = getStaticImage("InventoryWindow/BracerLeft");
		mBracerLeft->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BRACER));

		mBackpack = getStaticImage("InventoryWindow/Backpack");
		mBackpack->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BACKPACK));

		mBelt = getStaticImage("InventoryWindow/Belt");
		mBelt->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BELT));

		mNecklace = getStaticImage("InventoryWindow/Necklace");
		mNecklace->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_NECKLACE));

		mTrousers = getStaticImage("InventoryWindow/Trousers");
		mTrousers->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_TROUSERS));

		mShinboneLeft = getStaticImage("InventoryWindow/ShinboneLeft");
		mShinboneLeft->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_SHINBONE));

		mShinboneRight = getStaticImage("InventoryWindow/ShinboneRight");
		mShinboneRight->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_SHINBONE));

		mBoots = getStaticImage("InventoryWindow/Boots");
		mBoots->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BOOTS));

		// gib ihnen die Funktionalität Items aufzunehmen
		addDropListener(mHelmet);
		addDropListener(mRingLeft);
		addDropListener(mRingRight);
		addDropListener(mHandLeft);
		addDropListener(mHandRight);
		addDropListener(mGloves);
		addDropListener(mBraceletLeft);
		addDropListener(mBraceletRight);
		addDropListener(mArmor);
		addDropListener(mBracerRight);
		addDropListener(mBracerLeft);
		addDropListener(mBackpack);
		addDropListener(mBelt);
		addDropListener(mNecklace);
		addDropListener(mTrousers);
		addDropListener(mShinboneLeft);
		addDropListener(mShinboneRight);
		addDropListener(mBoots);
	}

	void InventoryWindow::addDropListener(CEGUI::Window* slot){
		// onMouseOver (beim Drag)
		slot->subscribeEvent(Window::EventDragDropItemEnters, &handleDragEnter);
		// onMouseOut (beim Drag)
		slot->subscribeEvent(Window::EventDragDropItemLeaves, &handleDragLeave);
		// onItemDropped
		slot->subscribeEvent(Window::EventDragDropItemDropped, &handleDragDropped); 
	}

	DragContainer* InventoryWindow::createItem(CEGUI::String name,Item::ItemType type,pair<int,int> dim, CEGUI::String imgName)
	{

		DragContainer* itemhandler = static_cast<DragContainer*>(
		CEGUI::WindowManager::getSingletonPtr()->createWindow("DragContainer", name));
		itemhandler->setWindowPosition(UVector2(cegui_reldim(0.0f), cegui_reldim(0.0f)));
		itemhandler->setWindowSize(UVector2(cegui_absdim(dim.first*30), cegui_absdim(dim.second*30))); 
		itemhandler->setUserString("ItemType",Item::getItemTypeString(type));
		
		mBackpackContent->addChildWindow(itemhandler);

		Window* item = CEGUI::WindowManager::getSingletonPtr()->createWindow("RastullahLook/Item", name.append(CEGUI::String("Item")));

		item->setWindowPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));
		item->setWindowSize(UVector2(cegui_absdim(dim.first*30), cegui_absdim(dim.second*30)));
		item->disable(); 
		itemhandler->addChildWindow(item);


		Window* itemIcon = CEGUI::WindowManager::getSingletonPtr()->createWindow("RastullahLook/StaticImage", name.append(CEGUI::String("Bild")));
		item->addChildWindow(itemIcon);
		itemIcon->setWindowPosition(UVector2(cegui_reldim(0.0), cegui_reldim(0.0)));
		itemIcon->setWindowSize(UVector2(cegui_reldim(1), cegui_reldim(1)));
		itemIcon->setProperty("Image", CEGUI::String("set:ModelThumbnails image:").append(imgName));
		// disable to allow inputs to pass through.
		itemIcon->disable();

		return itemhandler;
	}

	void InventoryWindow::initBackpack(pair<int,int> dim)
	{
		// Initiiere den Rucksack-Container für das Inventar
		mBackpackWindow = getWindow("InventoryWindow/Tabs/Backpack");
		mBackpackContent = CEGUI::WindowManager::getSingletonPtr()->createWindow("RastullahLook/Container", "InventoryWindow/Tabs/Backpack/Content");
		mBackpackContent->setVerticalAlignment(CEGUI::VA_CENTRE);
		mBackpackContent->setHorizontalAlignment(CEGUI::HA_CENTRE);
		mBackpackContent->setWindowSize(UVector2(cegui_absdim(dim.first*30), cegui_absdim(dim.second*30)));
		mBackpackWindow->addChildWindow(mBackpackContent);

		// Der Rucksack soll ale Itemtypen akzeptieren
		mBackpackContent->subscribeEvent(Window::EventDragDropItemEnters, &handleDragEnterBackpack);
		mBackpackContent->subscribeEvent(Window::EventDragDropItemLeaves, &handleDragLeave);
		mBackpackContent->subscribeEvent(Window::EventDragDropItemDropped, &handleDragDroppedBackpack); 
	}
}
