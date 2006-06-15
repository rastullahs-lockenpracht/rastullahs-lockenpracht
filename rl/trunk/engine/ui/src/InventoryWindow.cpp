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
using namespace Ogre;

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
		if (invWin->mDroppedItem)
		{
			Logger::getSingletonPtr()->log("InventoryWindow",Logger::LL_ERROR, "Item placed");
			invWin->updateItemPosition();
		}
	}

	/*!
	*		Überprüft, ob das Item von dem Slot akzeptiert wird
	*/
	bool checkTypeAccepted(CEGUI::Window* window, CEGUI::DragContainer* draggedItem){
		if (window->getUserString("ItemType").compare(draggedItem->getUserString("ItemType")) == 0)
		{
			return true;
		} 
		else 
		{
			return false;
		}
	}

	/**
	* Berechnet aus relativen und Absoluten Mousekoordinaten die neue Position im Container
	*/
	std::pair<int,int> calculateNewPosition(const DragDropEventArgs& ddea)
	{
		// Errechnung der Koordinaten, in welchem Kästchen denn nun gedroppt wird
		Point absMouse = MouseCursor::getSingleton().getPosition();
		Point scrnPt = ddea.window->windowToScreen( Point(0,0) ); 
		Point relMouse = absMouse- ddea.dragDropItem->windowToScreen( Point(0,0) );

		//Bug in CeGUI (1. rel Mouse koordinate spinnt
		if (relMouse.d_x < 0) {
			relMouse.d_x = 15;
		}

		Logger::getSingletonPtr()->log(
			"InventoryWindow",
			Logger::LL_MESSAGE,
			Ogre::String("relMouse: Point x:")
			+ StringConverter::toString(relMouse.d_x)
			+ ", Point y:"
			+ StringConverter::toString(relMouse.d_y));

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

		int xKaestchen = (int) (pointInBackpack.d_x / 30);
		int yKaestchen = (int) (pointInBackpack.d_y / 30);

		std::pair<int,int> result = std::make_pair<int,int>(xKaestchen, yKaestchen);
		return result;
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
		InventoryWindow* invWin = WindowFactory::getSingletonPtr()->getInventoryWindow();

		if (ddea.window->getUserData()){
			// Es handelt sich um einen Container
			Item* container = static_cast<Item*>(ddea.window->getUserData());
			if (container == invWin->getGroundItem())
			{
				// Boden (nimmt alles)
				ddea.window->setProperty("ContainerColour", WindowFactory::getSingletonPtr()->getInventoryWindow()->mColorAccept);
				return true;
			} 
			else if (container && container->getItemType() == Item::ITEMTYPE_BACKPACK)
			{
				// Ein Container kann keinen Rucksack aufnehmen
				if (!(ddea.dragDropItem->getUserString("ItemType").compare(Item::getItemTypeString(Item::ITEMTYPE_BACKPACK))))
				{
					ddea.window->setProperty("ContainerColour", WindowFactory::getSingletonPtr()->getInventoryWindow()->mColorReject);
					return false;
				}
				else 
				{
					ddea.window->setProperty("ContainerColour", WindowFactory::getSingletonPtr()->getInventoryWindow()->mColorAccept);
					return true;
				}
			}
			else {
				// Beliebiger anderer Container am Körper
				// TODO: Beschränkung auf Itemtypen... 

				ddea.window->setProperty("ContainerColour", WindowFactory::getSingletonPtr()->getInventoryWindow()->mColorAccept);
				return true;
			}
		}
		else if (checkTypeAccepted(ddea.window, ddea.dragDropItem))
		{
			ddea.window->setProperty("ContainerColour", WindowFactory::getSingletonPtr()->getInventoryWindow()->mColorAccept);
			return true;
		} 
		else 
		{
			ddea.window->setProperty("ContainerColour", WindowFactory::getSingletonPtr()->getInventoryWindow()->mColorReject);
			return false;
		}
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


		if (ddea.window->getUserData()){
			// Es handelt sich um einen Container
			Item* container = static_cast<Item*>(ddea.window->getUserData());
			if (container == invWin->getGroundItem())
			{
				// Boden (nimmt alles)
			} 
			else if (container && container->getItemType() == Item::ITEMTYPE_BACKPACK)
			{
				// Rucksack (nimmt alles außer dem Rucksack selbst)
				if (!(ddea.dragDropItem->getUserString("ItemType").compare(Item::getItemTypeString(Item::ITEMTYPE_BACKPACK))))
				{
					return false;
				}
			}
			else {
				// Beliebiger anderer Container am Körper
				// TODO: Beschränkung auf Itemtypen... 
			}

			std::pair<int,int> newPos =  calculateNewPosition(ddea);

			if (invWin->isFreeInContainer(
				static_cast<Item*>(ddea.dragDropItem->getUserData()),
				newPos, static_cast<Item*>(ddea.window->getUserData())))
			{
				ddea.window->addChildWindow(ddea.dragDropItem);

				invWin->mPosDraggedTo=CEGUI::Point(newPos.first*30,newPos.second*30);
				invWin->mDroppedItem = ddea.dragDropItem;
				invWin->mContainerDraggedTo = ddea.window;

				invWin->updateInventory();
				return true;
			} 
			else 
			{
				return false;
			}


		} else {
			// Es handelt sich um einen Slot am Körper
			if (checkTypeAccepted(ddea.window, ddea.dragDropItem)){
				// Nur wenn das Item in den Slot passt, soll es auch dort gedroppt werden können				

				ddea.window->addChildWindow(ddea.dragDropItem);

				invWin->mPosDraggedTo=CEGUI::Point(0.0,0.0);
				invWin->mDroppedItem = ddea.dragDropItem;
				invWin->mContainerDraggedTo = ddea.window;

				invWin->updateInventory();
				return true;
			} else {
				// Item passt nicht zum Slot
				return false;
			}
		}
	} 


	/* Konstruktor */
	InventoryWindow::InventoryWindow()
		: CeGuiWindow("inventorywindow.xml", WND_MOUSE_INPUT),
		mPosDraggedTo(),
		mContainerDraggedTo(NULL),
		mDroppedItem(NULL),
		mContainerTabs(NULL),
		mContainerContents(NULL),
		mGroundTab(NULL),
		mGroundItem(NULL),
		mGroundContainer(NULL),
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
		mBoots(NULL),
		mGroundDimension(make_pair<int,int>(12,15))
	{
		initSlots();
		GameLoopManager::getSingletonPtr()->addSynchronizedTask(new InventoryArrangeTask(), FRAME_ENDED);
	}
	
	InventoryWindow::~InventoryWindow()
	{
		/*if(mBackpackContent){
			mBackpackContent->destroy();
		}*/
	}


	/** Methoden */

	void InventoryWindow::setInventory(Inventory* inventory)
	{
		//if (inventory != NULL && mInventory != inventory){
			// Entleere Slots zur Sicherheit (falls anderes Inventar zuvor gezeigt wurde)
			emptySlots();
			// Speichere das neue Inventar
			mInventory = inventory;
			// Fuelle das Fenster entsprechend dem neuen Inventar, das angezeigt werden soll
			fillSlots();
		//}
	}

	void update()
	{

	}

	void InventoryWindow::updateItemPosition(){
		
		// Positionieren
		mDroppedItem->setPosition(CEGUI::Absolute, mPosDraggedTo);
		mDroppedItem = NULL;

		// Loggen
		Logger::getSingletonPtr()->log(
			"InventoryWindow",
			Logger::LL_MESSAGE,
			Ogre::String("Position set to: Point x:")
			+ StringConverter::toString(mPosDraggedTo.d_x)
			+ ", Point y:"
			+ StringConverter::toString(mPosDraggedTo.d_y));

		Logger::getSingletonPtr()->log("InventoryWindow",Logger::LL_MESSAGE,Ogre::String("updateItemPosition finished"));
	}

	void InventoryWindow::updateInventory(){
		// Nach Drop
		Item* item = static_cast<Item*>(mDroppedItem->getUserData());

		// Gegenstand von alter Position entfernen
		mInventory->removeItem(item);

		// Gegenstand vom Boden entfernen
		mInventory->removeItemFromContainer(item, mGroundItem);

		// Welchem Item wird das Item hinzugefügt? -> speichere das in die Variable container
		Item* container = static_cast<Item*>(mContainerDraggedTo->getUserData());

		// Extra Behandlung, falls Item auf den Boden gelegt wird
		if (container == mGroundItem)
		{
			mInventory->setItemContainerPosition(
				item,
				(int)(mPosDraggedTo.d_x/30), 
				(int)(mPosDraggedTo.d_y/30),
				container);

			// Zusätzlich wenn Item Container war, muss Tab entfernt werden (einfach Inventar neu aufbauen)
			if (item->isContainer())
			{
				removeContainerAndContent(item);
			}

			return;
		} 
				
		// Item im Container platzieren
		std::list<CEGUI::Window*>::iterator it = mContainerContents.begin();

		while (it != mContainerContents.end())
		{
			if (mContainerDraggedTo == *it++){
				mInventory->setItemContainerPosition(
					item,
					(int)(mPosDraggedTo.d_x/30), 
					(int)(mPosDraggedTo.d_y/30),
					container);
				break;
			}
		}

		if (item->isContainer())
		{
			// ContainerTab hinzufügen, wenn nicht schon vorhanden
			createAndFillContainer(item);

		}
		

		// Gegenstand in Slot setzen / Schon dagewesenen Gegenstand zurück in den Rucksack setzen
		if (mContainerDraggedTo == mArmor){
			if (mInventory->getArmor() != NULL){
				//Pack die alte Rüstung ins Inventar
				mInventory->addItemToContainer(mInventory->getArmor(), mInventory->getBackpack());
			}
			mInventory->setArmor(item);
		}
		if (mContainerDraggedTo == mCape){
			if (mInventory->getCape() != NULL){
				//Pack die alte Rüstung ins Inventar
				mInventory->addItemToContainer(mInventory->getCape(), mInventory->getBackpack());
			}
			mInventory->setCape(item);
		}
		if (mContainerDraggedTo == mBackpack) {
			if (mInventory->getBackpack() != NULL){
				//Pack den alten Rucksack auf den Boden
				mInventory->addItemToContainer(mInventory->getBackpack(), mInventory->getBackpack());
			}
			mInventory->setBackpack(item);
		}
		if (mContainerDraggedTo == mBelt) {
			if (mInventory->getBelt() != NULL){
				//Pack den alten Gürtel ins Inventar
				mInventory->addItemToContainer(mInventory->getBelt(), mInventory->getBackpack());
			}
			mInventory->setBelt(item);
		}
		if (mContainerDraggedTo == mBoots) {
			if (mInventory->getBoots() != NULL){
				//pack die alten Stiefel ins Inventar
				mInventory->addItemToContainer(mInventory->getBoots(), mInventory->getBackpack());
			}
			mInventory->setBoots(item);
		}
		if (mContainerDraggedTo == mBraceletLeft) {
			if (mInventory->getBraceletLeft() != NULL){
				//pack den alten Armreif ins Inventar
				mInventory->addItemToContainer(mInventory->getBraceletLeft(), mInventory->getBackpack());
			}
			mInventory->setBraceletLeft(item);
		}
		if (mContainerDraggedTo == mBraceletRight) {
			if (mInventory->getBraceletRight() != NULL){
				//pack den alten Armreif ins Inventar
				mInventory->addItemToContainer(mInventory->getBraceletRight(), mInventory->getBackpack());
			}
			mInventory->setBraceletRight(item);
		}
		if (mContainerDraggedTo == mRingLeft) {
			if (mInventory->getRingLeft() != NULL){
				//pack den alten Ring ins Inventar
				mInventory->addItemToContainer(mInventory->getRingLeft(), mInventory->getBackpack());
			}
			mInventory->setRingLeft(item);
		}
		if (mContainerDraggedTo == mRingRight) {
			if (mInventory->getRingRight() != NULL){
				//pack den alten Ring ins Inventar
				mInventory->addItemToContainer(mInventory->getRingRight(), mInventory->getBackpack());
			}
			mInventory->setRingRight(item);
		}
		if (mContainerDraggedTo == mHelmet) {
			if (mInventory->getHelmet() != NULL){
				//pack den alten Helm ins Inventar
				mInventory->addItemToContainer(mInventory->getHelmet(), mInventory->getBackpack());
			}
			mInventory->setHelmet(item);
		}
		if (mContainerDraggedTo == mBracers) {
			if (mInventory->getBracers() != NULL){
				//pack die alte Armschienen ins Inventar
				mInventory->addItemToContainer(mInventory->getBracers(), mInventory->getBackpack());
			}
			mInventory->setBracers(item);
		}
		if (mContainerDraggedTo == mNecklace) {
			if (mInventory->getNecklace() != NULL){
				//pack das alte Amulett ins Inventar
				mInventory->addItemToContainer(mInventory->getNecklace(), mInventory->getBackpack());
			}
			mInventory->setNecklace(item);
		}
		if (mContainerDraggedTo == mGloves) {
			if (mInventory->getGloves() != NULL){
				//pack die alten Handschuhe ins Inventar
				mInventory->addItemToContainer(mInventory->getGloves(), mInventory->getBackpack());
			}
			mInventory->setGloves(item);
		}
		if (mContainerDraggedTo == mTrousers) {
			if (mInventory->getTrousers() != NULL){
				//pack die alte Hose ins Inventar
				mInventory->addItemToContainer(mInventory->getTrousers(), mInventory->getBackpack());
			}
			mInventory->setTrousers(item);
		}
		if (mContainerDraggedTo == mShinbone) {
			if (mInventory->getShinbone() != NULL){
				//pack die alte Hose ins Inventar
				mInventory->addItemToContainer(mInventory->getShinbone(), mInventory->getBackpack());
			}
			mInventory->setShinbone(item);
		}
		if (mContainerDraggedTo == mHandLeft) {
			if (mInventory->getHandLeft() != NULL){
				//pack das alte Schild ins Inventar
				mInventory->addItemToContainer(mInventory->getHandLeft(), mInventory->getBackpack());
			}
			mInventory->setHandLeft(item);
		}
		if (mContainerDraggedTo == mHandRight) {
			if (mInventory->getHandRight() != NULL){
				//pack die alte Waffe ins Inventar
				mInventory->addItemToContainer(mInventory->getHandRight(), mInventory->getBackpack());
			}
			mInventory->setHandRight(item);
		}
	}

	bool InventoryWindow::isFreeInContainer(Item* item, pair<int,int> kaestchenPos, Item* container)
	{
		return mInventory->isFreeInContainer(item, kaestchenPos, container);
	}


	Item* InventoryWindow::getGroundItem()
	{
		return mGroundItem;
	}

	void InventoryWindow::initSlots()
	{

		// Das "BodenItem" initiieren
		mGroundItem = new Item("Boden", "Dieses Item repräsentiert den Boden");
		mGroundItem->setImageName("Trank");
		mGroundItem->setItemType(Item::ITEMTYPE_OTHER);
		mGroundItem->setSize(1,1);
		mGroundItem->setContainer(true, std::make_pair<int,int>(mGroundDimension.first,mGroundDimension.second));


		mContainerTabs = getWindow("InventoryWindow/Tabs");
		
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


		// Fenster für ContainerItems erstellen
		createContainerWindows();
	}


	void InventoryWindow::createContainerWindows() {
		

		std::list<Item*> allItems = mInventory->getAllItems();

		std::list<Item*>::iterator itemIterator = allItems.begin();
		
		while (itemIterator != allItems.end()){
			Item* currentItem = *itemIterator++;

			if (currentItem->isContainer()){
				// Tab für Item erzeugen, da es als Container funktioniert
				createAndFillContainer(currentItem);
			}
		}
		// Den Boden hinzufuegen
		createAndFillContainer(mGroundItem);
	}

	void InventoryWindow::createAndFillContainer(Item* container){
		assert(mContainerContents.size() == mContainers.size());

		std::list<CEGUI::Window*>::iterator it = mContainerContents.begin();
		bool found = false;
		// überprüfe, ob schon vorhanden
		while (it != mContainerContents.end())
		{
			if (container == static_cast<Item*>((*it)->getUserData()))
			{
				found = true;
				break;
			}
			it++;
		}
		
		// Nur wenn noch nicht vorhanden, soll Containertab erzeugt werden
		if (!found)
		{

			// Erzeuge Tab im Inventar für den Container
			CEGUI::Window* containerWindow = CEGUI::WindowManager::getSingleton().createWindow("DefaultGUISheet", "InventoryWindow/Tabs/"+container->getName());
			// Name für das Tab
			containerWindow->setText(container->getName());


			mContainerTabs->addChildWindow(containerWindow);

			// Tab in die Liste einfuegen
			mContainers.push_back(containerWindow);

			CEGUI::StaticImage* containerSpace = static_cast<CEGUI::StaticImage*>(CEGUI::WindowManager::getSingletonPtr()->createWindow("RastullahLook/Container", "InventoryWindow/Tabs/"+container->getName()+"/Content"));
			
			containerSpace->setVerticalAlignment(CEGUI::VA_CENTRE);
			containerSpace->setHorizontalAlignment(CEGUI::HA_CENTRE);
			containerSpace->setWindowSize(UVector2(cegui_absdim(container->getCapacity().first*30), cegui_absdim(container->getCapacity().second*30)));
			containerSpace->setUserData(container);
			containerWindow->addChildWindow(containerSpace);

			// Inhalt in die Liste Einfügen
			mContainerContents.push_back(containerSpace);

			// Der Rucksack soll alle Itemtypen akzeptieren (Bis auf den Rucksack selbst)
			addDropListener(containerSpace);

			// Items im Container erstellen
			std::set<Item*> itemsInContainer;

			ContainerLayout temp = container->getContainerLayout();
			for (unsigned x = 0; x < temp.size(); x++){
				for (unsigned y = 0; y < temp[0].size(); y++) {
					if (temp[x][y] != NULL && (itemsInContainer.find(temp[x][y]) == itemsInContainer.end())){
						DragContainer* itemhandler = createItem(temp[x][y],containerSpace);
						itemhandler->setPosition(CEGUI::Absolute,CEGUI::Point(x*30,y*30));
						itemsInContainer.insert(temp[x][y]);
						Logger::getSingletonPtr()->log(
							"InventoryWindow",
							Logger::LL_MESSAGE,
							Ogre::String("erzeuge Bild im Container: ") + temp[x][y]->getName());
					}
				}
			}
		}
	}


	void InventoryWindow::removeContainerAndContent(Item* container)
	{
		assert(mContainerContents.size() == mContainers.size());
		std::list<CEGUI::Window*>::iterator it = mContainerContents.begin();
		std::list<CEGUI::Window*>::iterator itParent = mContainers.begin();
		CEGUI::Window* windowToEmpty = NULL;
		CEGUI::Window* parentTabWindow = NULL;

		bool found = false;
		while (it != mContainerContents.end())
		{
			if (container == static_cast<Item*>((*it)->getUserData()))
			{
				// Aus Liste entfernen
				windowToEmpty = *it;
				parentTabWindow = *itParent;
				found = true;
				break;
			}
			it++;
			itParent++;
		}
		if (found)
		{
			// Entferne die Elemente aus den Listen
			mContainerContents.remove(windowToEmpty);
			mContainers.remove(parentTabWindow);


			emptySlot(windowToEmpty);
			parentTabWindow->removeChildWindow(windowToEmpty);
			WindowManager::getSingletonPtr()->destroyWindow(windowToEmpty);

			mContainerTabs->removeChildWindow(parentTabWindow);

			WindowManager::getSingletonPtr()->destroyWindow(parentTabWindow);
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

		// Zusätzlich alle Container durchgehen und entleeren
		
		std::list<CEGUI::Window*>::iterator it = mContainerContents.begin();
		
		while (it != mContainerContents.end())
		{
			emptySlot(*it);
			WindowManager::getSingletonPtr()->destroyWindow(*it);
			it++;
		} // alle Container wurden entleert

		it = mContainers.begin();
		while (it != mContainers.end())
		{
			emptySlot(*it);
			mContainerTabs->removeChildWindow(*it);
			WindowManager::getSingletonPtr()->destroyWindow(*it);
			it++;
		} // alle ContainerTabs wurden entleert
		mContainerContents.clear();
		mContainers.clear();
	}

	void InventoryWindow::emptySlot(CEGUI::Window* slot)
	{
		while(slot->getChildCount() > 0){
			CEGUI::Window* windowToDestroy = slot->getChildAtIdx(0);
			WindowManager::getSingletonPtr()->destroyWindow(windowToDestroy);
			slot->removeChildWindow(windowToDestroy);
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

			CeGuiString tempName = item->getName();

			// Erzeuge das Hintergrundfenster des Items
			Window* itemWindow = 
				CEGUI::WindowManager::getSingletonPtr()->createWindow(
					"RastullahLook/Item", 
					tempName + "Item");

			itemWindow->setWindowPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));
			itemWindow->setWindowSize(UVector2(cegui_absdim(item->getSize().first*30), cegui_absdim(item->getSize().second*30)));
			itemWindow->disable(); 

			itemhandler->addChildWindow(itemWindow);

			// Gib dem Item noch ein Bild...
			Window* itemIcon = 
				CEGUI::WindowManager::getSingletonPtr()->createWindow(
					"RastullahLook/StaticImage", 
					tempName + "Icon");

			itemIcon->setWindowPosition(UVector2(cegui_reldim(0.0), cegui_reldim(0.0)));
			itemIcon->setWindowSize(UVector2(cegui_reldim(1), cegui_reldim(1)));
			itemIcon->setProperty("Image", CeGuiString("set:ModelThumbnails image:") + item->getImageName());
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

	}
}
