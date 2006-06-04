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
		if (invWin->mDroppedItem){
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
			ddea.window->setProperty("ContainerColour", WindowFactory::getSingletonPtr()->getInventoryWindow()->mColorAccept);
		} else {
			ddea.window->setProperty("ContainerColour", WindowFactory::getSingletonPtr()->getInventoryWindow()->mColorReject);
			return false;
		}
		return true;
	} 
	bool handleDragLeave(const CEGUI::EventArgs& args)
	{
		using namespace CEGUI;

		const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
		ddea.window->setProperty("ContainerColour", WindowFactory::getSingletonPtr()->getInventoryWindow()->mColorNormal);
		return true;
	} 
	bool handleDragDropped(const CEGUI::EventArgs& args)
	{
		using namespace CEGUI;

		InventoryWindow* invWin = WindowFactory::getSingletonPtr()->getInventoryWindow();
		const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
		ddea.window->setProperty("ContainerColour", invWin->mColorNormal);


		Point relMouse = ddea.dragDropItem->windowToScreen( Point(0,0) );

		// Nur wenn das Item in den Slot passt, soll es auch dort gedroppt werden können
		if (checkTypeAccepted(ddea.window, ddea.dragDropItem)){
			

			ddea.window->addChildWindow(ddea.dragDropItem);

			invWin->mPosDraggedTo=CEGUI::Point(0.0,0.0);
			invWin->mDroppedItem = ddea.dragDropItem;
			invWin->mContainerDraggedTo = ddea.window;

			invWin->updateInventory();
			return true;
		} else {
			return false;
		}
	} 

	bool handleDragEnterBackpack(const CEGUI::EventArgs& args)
	{
		using namespace CEGUI;

		const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
		ddea.window->setProperty("ContainerColour", WindowFactory::getSingletonPtr()->getInventoryWindow()->mColorAccept);
		
		return true;

	} 
	bool handleDragDroppedBackpack(const CEGUI::EventArgs& args)
	{
		using namespace CEGUI;
		InventoryWindow* invWin = WindowFactory::getSingletonPtr()->getInventoryWindow();

		char buf1[5];
		char buf2[5];

		const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
		ddea.window->setProperty("ContainerColour", invWin->mColorNormal);
		
		// Den Rucksack darf man nicht in den Rucksack droppen...
		if (!(ddea.dragDropItem->getUserString("ItemType").compare(Item::getItemTypeString(Item::ITEMTYPE_BACKPACK)))){
			return false;
		}
		
		// Errechnung der Koordinaten, in welchem Kästchen denn nun gedroppt wird
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

		if (invWin->isFreeInBackpack(
			static_cast<Item*>(ddea.dragDropItem->getUserData()),pair<int,int>(xKaestchen,yKaestchen)))
		{
			ddea.window->addChildWindow(ddea.dragDropItem);

			invWin->mPosDraggedTo=pointInBackpack;
			invWin->mDroppedItem = ddea.dragDropItem;
			invWin->mContainerDraggedTo = ddea.window;

			invWin->updateInventory();
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
		: CeGuiWindow("inventorywindow.xml", WND_MOUSE_INPUT),
		mPosDraggedTo(),
		mContainerDraggedTo(NULL),
		mDroppedItem(NULL),
		mBackpackWindow(NULL),
		mBackpackContent(NULL),
		mColorAccept("FF22FF22"),
		mColorReject("FFFF2222"),
		mColorNormal("FFFFFFFF"),
		mRingLeft(NULL),
		mRingRight(NULL),
		mHandLeft(NULL),
		mHandRight(NULL),
		mGloves(NULL),
		mBraceletLeft(NULL),
		mBraceletRight(NULL),
		mArmor(NULL),
		mCape(NULL),
		mBracers(NULL),
		mBackpack(NULL),
		mBelt(NULL),
		mNecklace(NULL),
		mHelmet(NULL),
		mTrousers(NULL),
		mShinbone(NULL),
		mBoots(NULL)
	{
		initSlots();
		GameLoopManager::getSingletonPtr()->addSynchronizedTask(new InventoryArrangeTask(), FRAME_ENDED);
	}
	
	InventoryWindow::~InventoryWindow()
	{
		if(mBackpackContent){
			mBackpackContent->destroy();
		}
	}


	/** Methoden */

	void InventoryWindow::setInventory(Inventory* inventory)
	{
		if (inventory != NULL && mInventory != inventory){
			emptySlots();
			mInventory = inventory;
			fillSlots();
		}
	}

	void update()
	{

	}

	void InventoryWindow::updateItemPosition(){
		
		// Positionieren
		mDroppedItem->setPosition(CEGUI::Absolute, mPosDraggedTo);
		mDroppedItem = NULL;

		// Loggen
		char buf1[5];
		char buf2[5];
		Logger::getSingletonPtr()->log("InventoryWindow",Logger::LL_MESSAGE,
			String("Position set to: Point x:").append(itoa(int(mPosDraggedTo.d_x),buf1,10))
			.append(", Point y:")
			.append(itoa(int(mPosDraggedTo.d_y),buf2,10)));

		Logger::getSingletonPtr()->log("InventoryWindow",Logger::LL_MESSAGE,String("updateItemPosition finished"));
	}

	void InventoryWindow::updateInventory(){
		Item* item = static_cast<Item*>(mDroppedItem->getUserData());
		
		// Gegenstand aus dem Rucksack entfernen
		mInventory->removeItemFromBackpack(item);
		// Gegenstand aus den Slots entfernen
		mInventory->removeItemFromSlots(item);

		if (mContainerDraggedTo == mBackpackContent)
			mInventory->setItemBackpackPosition(item, mPosDraggedTo.d_x/30, mPosDraggedTo.d_y/30);
		else {
			// Gegenstand in Slot setzen / Schon dagewesenen Gegenstand zurück in den Rucksack setzen
			if (mContainerDraggedTo == mArmor){
				if (mInventory->getArmor() != NULL){
					//Pack die alte Rüstung ins Inventar
					mInventory->addItemToBackpack(mInventory->getArmor());
				}
				mInventory->setArmor(item);
			}
			if (mContainerDraggedTo == mCape){
				if (mInventory->getCape() != NULL){
					//Pack die alte Rüstung ins Inventar
					mInventory->addItemToBackpack(mInventory->getCape());
				}
				mInventory->setCape(item);
			}
			if (mContainerDraggedTo == mBackpack) {
				if (mInventory->getBackpack() != NULL){
					//Pack den alten Rucksack auf den Boden
					mInventory->addItemToBackpack(mInventory->getBackpack());
				}
				mInventory->setBackpack(item);
			}
			if (mContainerDraggedTo == mBelt) {
				if (mInventory->getBelt() != NULL){
					//Pack den alten Gürtel ins Inventar
					mInventory->addItemToBackpack(mInventory->getBelt());
				}
				mInventory->setBelt(item);
			}
			if (mContainerDraggedTo == mBoots) {
				if (mInventory->getBoots() != NULL){
					//pack die alten Stiefel ins Inventar
					mInventory->addItemToBackpack(mInventory->getBoots());
				}
				mInventory->setBoots(item);
			}
			if (mContainerDraggedTo == mBraceletLeft) {
				if (mInventory->getBraceletLeft() != NULL){
					//pack den alten Armreif ins Inventar
					mInventory->addItemToBackpack(mInventory->getBraceletLeft());
				}
				mInventory->setBraceletLeft(item);
			}
			if (mContainerDraggedTo == mBraceletRight) {
				if (mInventory->getBraceletRight() != NULL){
					//pack den alten Armreif ins Inventar
					mInventory->addItemToBackpack(mInventory->getBraceletRight());
				}
				mInventory->setBraceletRight(item);
			}
			if (mContainerDraggedTo == mRingLeft) {
				if (mInventory->getRingLeft() != NULL){
					//pack den alten Ring ins Inventar
					mInventory->addItemToBackpack(mInventory->getRingLeft());
				}
				mInventory->setRingLeft(item);
			}
			if (mContainerDraggedTo == mRingRight) {
				if (mInventory->getRingRight() != NULL){
					//pack den alten Ring ins Inventar
					mInventory->addItemToBackpack(mInventory->getRingRight());
				}
				mInventory->setRingRight(item);
			}
			if (mContainerDraggedTo == mHelmet) {
				if (mInventory->getHelmet() != NULL){
					//pack den alten Helm ins Inventar
					mInventory->addItemToBackpack(mInventory->getHelmet());
				}
				mInventory->setHelmet(item);
			}
			if (mContainerDraggedTo == mBracers) {
				if (mInventory->getBracers() != NULL){
					//pack die alte Armschienen ins Inventar
					mInventory->addItemToBackpack(mInventory->getBracers());
				}
				mInventory->setBracers(item);
			}
			if (mContainerDraggedTo == mNecklace) {
				if (mInventory->getNecklace() != NULL){
					//pack das alte Amulett ins Inventar
					mInventory->addItemToBackpack(mInventory->getNecklace());
				}
				mInventory->setNecklace(item);
			}
			if (mContainerDraggedTo == mGloves) {
				if (mInventory->getGloves() != NULL){
					//pack die alten Handschuhe ins Inventar
					mInventory->addItemToBackpack(mInventory->getGloves());
				}
				mInventory->setGloves(item);
			}
			if (mContainerDraggedTo == mTrousers) {
				if (mInventory->getTrousers() != NULL){
					//pack die alte Hose ins Inventar
					mInventory->addItemToBackpack(mInventory->getTrousers());
				}
				mInventory->setTrousers(item);
			}
			if (mContainerDraggedTo == mShinbone) {
				if (mInventory->getShinbone() != NULL){
					//pack die alte Hose ins Inventar
					mInventory->addItemToBackpack(mInventory->getShinbone());
				}
				mInventory->setShinbone(item);
			}
			if (mContainerDraggedTo == mHandLeft) {
				if (mInventory->getHandLeft() != NULL){
					//pack das alte Schild ins Inventar
					mInventory->addItemToBackpack(mInventory->getHandLeft());
				}
				mInventory->setHandLeft(item);
			}
			if (mContainerDraggedTo == mHandRight) {
				if (mInventory->getHandRight() != NULL){
					//pack die alte Waffe ins Inventar
					mInventory->addItemToBackpack(mInventory->getHandRight());
				}
				mInventory->setHandRight(item);
			}
		}
	}

	bool InventoryWindow::isFreeInBackpack(Item* item, pair<int,int> kaestchenPos)
	{
		return mInventory->isFreeInBackpack(item, kaestchenPos);
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
		mGloves->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_GLOVES));

		mBraceletLeft = getStaticImage("InventoryWindow/BraceletLeft");
		mBraceletLeft->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BRACELET));

		mBraceletRight = getStaticImage("InventoryWindow/BraceletRight");
		mBraceletRight->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BRACELET));

		mArmor = getStaticImage("InventoryWindow/Armor");
		mArmor->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_ARMOR));

		mCape = getStaticImage("InventoryWindow/Cape");
		mCape->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_CAPE));

		mBracers = getStaticImage("InventoryWindow/Bracers");
		mBracers->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BRACERS));

		mBackpack = getStaticImage("InventoryWindow/Backpack");
		mBackpack->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BACKPACK));

		mBelt = getStaticImage("InventoryWindow/Belt");
		mBelt->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BELT));

		mNecklace = getStaticImage("InventoryWindow/Necklace");
		mNecklace->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_NECKLACE));

		mTrousers = getStaticImage("InventoryWindow/Trousers");
		mTrousers->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_TROUSERS));

		mShinbone = getStaticImage("InventoryWindow/Shinbone");
		mShinbone->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_SHINBONE));

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
		addDropListener(mCape);
		addDropListener(mBracers);
		addDropListener(mBackpack);
		addDropListener(mBelt);
		addDropListener(mNecklace);
		addDropListener(mTrousers);
		addDropListener(mShinbone);
		addDropListener(mBoots);
	}

	void InventoryWindow::fillSlots()
	{
		createItem(mInventory->getRingLeft(), mRingLeft);
		createItem(mInventory->getRingRight(), mRingRight);
		createItem(mInventory->getHandLeft(), mHandLeft);
		createItem(mInventory->getHandRight(), mHandRight);
		createItem(mInventory->getGloves(), mGloves);
		createItem(mInventory->getBraceletLeft(), mBraceletLeft);
		createItem(mInventory->getBraceletRight(), mBraceletRight);
		createItem(mInventory->getArmor(), mArmor);
		createItem(mInventory->getCape(), mCape);
		createItem(mInventory->getBracers(), mBracers);
		createItem(mInventory->getBackpack(), mBackpack);
		createItem(mInventory->getBelt(), mBelt);
		createItem(mInventory->getNecklace(), mNecklace);
		createItem(mInventory->getHelmet(), mHelmet);
		createItem(mInventory->getTrousers(), mTrousers);
		createItem(mInventory->getShinbone(), mShinbone);
		createItem(mInventory->getBoots(), mBoots);

		// Items im Rucksack erstellen
		set<Item*> itemsInBackpack;

		vector< vector<Item*> > temp = mInventory->getBackpackLayout();
		for (unsigned x = 0; x < temp.size(); x++){
			for (unsigned y = 0; y < temp[0].size(); y++) {
				if (temp[x][y] != NULL && (itemsInBackpack.find(temp[x][y]) == itemsInBackpack.end())){
					DragContainer* itemhandler = createItem(temp[x][y],mBackpackContent);
					itemhandler->setPosition(CEGUI::Absolute,CEGUI::Point(x*30,y*30));
					itemsInBackpack.insert(temp[x][y]);
					Logger::getSingletonPtr()->log("InventoryWindow",Logger::LL_MESSAGE,String("erzeuge Bild im Backpack: ").append(temp[x][y]->getName()));
				}
			}
		}
	}

	void InventoryWindow::emptySlots()
	{
		emptySlot(mHelmet);
		emptySlot(mRingLeft);
		emptySlot(mRingRight);
		emptySlot(mHandLeft);
		emptySlot(mHandRight);
		emptySlot(mGloves);
		emptySlot(mBraceletLeft);
		emptySlot(mBraceletRight);
		emptySlot(mArmor);
		emptySlot(mCape);
		emptySlot(mBracers);
		emptySlot(mBackpack);
		emptySlot(mBelt);
		emptySlot(mNecklace);
		emptySlot(mTrousers);
		emptySlot(mShinbone);
		emptySlot(mBoots);
		emptySlot(mBackpackContent);
	}

	void InventoryWindow::emptySlot(CEGUI::Window* slot)
	{
		while(slot->getChildCount() > 0){
			CEGUI::String name = slot->getChildAtIdx(0)->getName();
			slot->removeChildWindow(slot->getChildAtIdx(0));
			WindowManager::getSingletonPtr()->destroyWindow(name);
		}
	}

	void InventoryWindow::addDropListener(CEGUI::Window* slot){
		// onMouseOver (beim Drag)
		slot->subscribeEvent(Window::EventDragDropItemEnters, &handleDragEnter);
		// onMouseOut (beim Drag)
		slot->subscribeEvent(Window::EventDragDropItemLeaves, &handleDragLeave);
		// onItemDropped
		slot->subscribeEvent(Window::EventDragDropItemDropped, &handleDragDropped); 
	}

	DragContainer* InventoryWindow::createItem(Item* item, Window* parent, UVector2 position)
	{
		// Prüfe, ob ein Item da ist
		if (item != NULL) {

			// Erzeuge einen Handler für Drag and Drop
			DragContainer* itemhandler = static_cast<DragContainer*>(
				CEGUI::WindowManager::getSingletonPtr()->createWindow("DragContainer", item->getName()));
			itemhandler->setWindowPosition(position);
			itemhandler->setWindowSize(UVector2(cegui_absdim(item->getSize().first*30), cegui_absdim(item->getSize().second*30))); 
			itemhandler->setUserString("ItemType",Item::getItemTypeString(item->getItemType()));
			itemhandler->setUserData(item);
			
			parent->addChildWindow(itemhandler);

			CEGUI::String tempName = item->getName();

			// Erzeuge das Hintergrundfenster des Items
			Window* itemWindow = CEGUI::WindowManager::getSingletonPtr()->createWindow("RastullahLook/Item", tempName.append(CEGUI::String("Item")));

			itemWindow->setWindowPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));
			itemWindow->setWindowSize(UVector2(cegui_absdim(item->getSize().first*30), cegui_absdim(item->getSize().second*30)));
			itemWindow->disable(); 

			itemhandler->addChildWindow(itemWindow);

			// Gib dem Item noch ein Bild...
			Window* itemIcon = CEGUI::WindowManager::getSingletonPtr()->createWindow("RastullahLook/StaticImage", tempName.append(CEGUI::String("Icon")));

			itemIcon->setWindowPosition(UVector2(cegui_reldim(0.0), cegui_reldim(0.0)));
			itemIcon->setWindowSize(UVector2(cegui_reldim(1), cegui_reldim(1)));
			itemIcon->setProperty("Image", CEGUI::String("set:ModelThumbnails image:").append(item->getImageName()));
			// disable to allow inputs to pass through.
			itemIcon->disable();

			itemWindow->addChildWindow(itemIcon);

			return itemhandler;
		}
		else
		{
			// Kein Item da...
			return NULL;
		}
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

		// Der Rucksack soll alle Itemtypen akzeptieren (Bis auf den Rucksack selbst)
		mBackpackContent->subscribeEvent(Window::EventDragDropItemEnters, &handleDragEnterBackpack);
		mBackpackContent->subscribeEvent(Window::EventDragDropItemLeaves, &handleDragLeave);
		mBackpackContent->subscribeEvent(Window::EventDragDropItemDropped, &handleDragDroppedBackpack); 
	}
}
