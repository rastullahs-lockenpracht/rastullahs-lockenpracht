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

#include "Actor.h"
#include "ActorManager.h"
#include "CameraObject.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "Exception.h"
#include "GameLoop.h"
#include "Inventory.h"
#include "MeshObject.h"
#include "PhysicalThing.h"
#include "RulesSubsystem.h"
#include "UiSubsystem.h"
#include "WindowFactory.h"
#include "World.h"
#include "CeGuiHelper.h"

#include <CEGUIPropertyHelper.h>
#include <OgreCEGUIRenderer.h>

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
			LOG_MESSAGE("InventoryWindow", "Item placed");
			invWin->updateItemPosition();
		}
	}

    const Ogre::String& InventoryArrangeTask::getName() const
    {
        static Ogre::String NAME = "InventoryArrangeTask";

        return NAME;
    }


	// ***************************************************************
	// ***************** Konstruktor *********************************
	// ***************************************************************
	InventoryWindow::InventoryWindow()
		: CeGuiWindow("inventorywindow.xml", WND_MOUSE_INPUT),
		mDescription(NULL),
		mRenderItemEntity(NULL),
		mRenderTexture(NULL),
		mRenderViewport(NULL),
		mItemRenderImage(NULL),
		mCameraActor(NULL),
		mInventoryItemNode(NULL),
		mTexture(NULL),
		mImageSet(NULL),
		mActiveItemWindow(NULL),
		mArmorValueWindow(NULL),
		mArmorValue(NULL),
		mHandicapValue(NULL),
		mWeightWindow(NULL),
		mTotalWeight(NULL),
		mPosDraggedTo(),
		mContainerDraggedTo(NULL),
		mDroppedItem(NULL),
		mOldItemInSlot(NULL),
		mContainerTabs(NULL),
		mContainerContents(NULL),
		mGroundTab(NULL),
		mGroundItem(NULL),
		mGroundContainer(NULL),
		mColorAccept("FF22FF22"),
		mColorReject("FFFF2222"),
		mColorNormal("FFFFFFFF"),
		mColorItemNormal("FF999955"),
		mColorItemSelected("FFDDDD99"),
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
        mInventoryArrangeTask(),
		mGroundDimension(make_pair<int,int>(12,15))
	{
		initSlots();
		initRenderToTexture();

		mWindow->subscribeEvent(FrameWindow::EventCloseClicked,
			boost::bind(&InventoryWindow::handleClose, this));

        mInventoryArrangeTask = new InventoryArrangeTask();
		GameLoopManager::getSingletonPtr()->addSynchronizedTask(mInventoryArrangeTask, FRAME_ENDED);
	}
	
	InventoryWindow::~InventoryWindow()
	{
		/*if(mBackpackContent){
			mBackpackContent->destroy();
		}*/
        delete mInventoryArrangeTask;
//        delete mGroundItem;
	}


	/** Methoden */

	void InventoryWindow::setInventory(Inventory* inventory)
	{
		if (inventory != NULL && mInventory != inventory){
			// Entleere Slots zur Sicherheit (falls anderes Inventar zuvor gezeigt wurde)
			emptySlots();
			// Speichere das neue Inventar
			mInventory = inventory;
			// Fuelle das Fenster entsprechend dem neuen Inventar, das angezeigt werden soll
			fillSlots();
			update();
		}
	}

	bool InventoryWindow::handleClose()
	{
		if (isVisible())
		{
			setVisible(false);
		}
		return true;
	}

	void InventoryWindow::update()
	{
		// Aktualisiere Werte

		// Rüstung
		mArmorValue->setText(Ogre::StringConverter::toString(mInventory->getOverallRs()));

		// Behinderung
		pair<int,int> behinderung = mInventory->getOverallBe();
		mHandicapValue->setText(Ogre::StringConverter::toString(behinderung.first+behinderung.second));
		if (behinderung.second > 0)
		{
			mArmorValueWindow->setTooltipText("Rüstung / Behinderung\r\n           / +"+Ogre::StringConverter::toString(behinderung.second)+" überladen");
		} 
		else
		{
			mArmorValueWindow->setTooltipText("Rüstung / Behinderung");
		}

		// Gewicht in Unzen
		int weight = mInventory->getOverallWeight() / 40;
		// TODO: runden
		mTotalWeight->setText(Ogre::StringConverter::toString(weight));
	}

	void InventoryWindow::updateItemPosition(){
		
		// Positionieren
        mDroppedItem->setPosition(CeGuiHelper::asAbsolute(mPosDraggedTo));
		mDroppedItem = NULL;

		// Loggen
		LOG_MESSAGE(
			"InventoryWindow",
			Ogre::String("Position set to: Point x:")
			+ StringConverter::toString(mPosDraggedTo.d_x)
			+ ", Point y:"
			+ StringConverter::toString(mPosDraggedTo.d_y));

		LOG_MESSAGE("InventoryWindow", "updateItemPosition finished");
	}

	void InventoryWindow::updateInventory(){
		// Nach Drop
		Item* item = static_cast<Item*>(mDroppedItem->getUserData());

		// Gegenstand von alter Position entfernen
		//mInventory->removeItem(item);

		// Gegenstand vom Boden entfernen
		//mInventory->removeItemFromContainer(item, mGroundItem);

		// Welchem Item wird das Item hinzugefügt? -> speichere das in die Variable container
		Item* container = static_cast<Item*>(mContainerDraggedTo->getUserData());

		// Extra Behandlung, falls Item auf den Boden gelegt wird
		if (container == mGroundItem)
		{
			//mInventory->setItemContainerPosition(
			//	item,
			//	(int)(mPosDraggedTo.d_x/30), 
			//	(int)(mPosDraggedTo.d_y/30),
			//	container);

			// Zusätzlich wenn Item Container war, muss Tab entfernt werden (einfach Inventar neu aufbauen)
			if (item->isContainer())
			{
				removeContainerAndContent(dynamic_cast<Container*>(item));
				refreshTabs();
			}
			update();
			return;
		} 
				
		// Item im Container platzieren
		std::list<CEGUI::Window*>::iterator it = mContainerContents.begin();

		while (it != mContainerContents.end())
		{
			if (mContainerDraggedTo == *it++){
				//mInventory->setItemContainerPosition(
				//	item,
				//	(int)(mPosDraggedTo.d_x/30), 
				//	(int)(mPosDraggedTo.d_y/30),
				//	container);
				break;
			}
		}

		if (item->isContainer())
		{
			// ContainerTab hinzufügen, wenn nicht schon vorhanden
			createAndFillContainer(dynamic_cast<Container*>(item));
			refreshTabs();

		}
		

		bool itemSwitched = false;
		// Gegenstand in Slot setzen / Schon dagewesenen Gegenstand zurück in den Rucksack setzen
		//if (mContainerDraggedTo == mArmor){
		//	if (mInventory->getArmor() != NULL){
		//		//Pack die alte Rüstung ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeArmor(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setArmor(item);
		//}
		//if (mContainerDraggedTo == mCape){
		//	if (mInventory->getCape() != NULL){
		//		//Pack die alte Rüstung ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeCape(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setCape(item);
		//}
		//if (mContainerDraggedTo == mBackpack) {
		//	if (mInventory->getBackpack() != NULL){
		//		//Pack den alten Rucksack auf den Boden
		//		mInventory->addItemToContainer(mInventory->removeBackpack(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setBackpack(item);
		//}
		//if (mContainerDraggedTo == mBelt) {
		//	if (mInventory->getBelt() != NULL){
		//		//Pack den alten Gürtel ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeBelt(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setBelt(item);
		//}
		//if (mContainerDraggedTo == mBoots) {
		//	if (mInventory->getBoots() != NULL){
		//		//pack die alten Stiefel ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeBoots(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setBoots(item);
		//}
		//if (mContainerDraggedTo == mBraceletLeft) {
		//	if (mInventory->getBraceletLeft() != NULL){
		//		//pack den alten Armreif ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeBraceletLeft(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setBraceletLeft(item);
		//}
		//if (mContainerDraggedTo == mBraceletRight) {
		//	if (mInventory->getBraceletRight() != NULL){
		//		//pack den alten Armreif ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeBraceletRight(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setBraceletRight(item);
		//}
		//if (mContainerDraggedTo == mRingLeft) {
		//	if (mInventory->getRingLeft() != NULL){
		//		//pack den alten Ring ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeRingLeft(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setRingLeft(item);
		//}
		//if (mContainerDraggedTo == mRingRight) {
		//	if (mInventory->getRingRight() != NULL){
		//		//pack den alten Ring ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeRingRight(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setRingRight(item);
		//}
		//if (mContainerDraggedTo == mHelmet) {
		//	if (mInventory->getHelmet() != NULL){
		//		//pack den alten Helm ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeHelmet(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setHelmet(item);
		//}
		//if (mContainerDraggedTo == mBracers) {
		//	if (mInventory->getBracers() != NULL){
		//		//pack die alte Armschienen ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeBracers(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setBracers(item);
		//}
		//if (mContainerDraggedTo == mNecklace) {
		//	if (mInventory->getNecklace() != NULL){
		//		//pack das alte Amulett ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeNecklace(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setNecklace(item);
		//}
		//if (mContainerDraggedTo == mGloves) {
		//	if (mInventory->getGloves() != NULL){
		//		//pack die alten Handschuhe ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeGloves(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setGloves(item);
		//}
		//if (mContainerDraggedTo == mTrousers) {
		//	if (mInventory->getTrousers() != NULL){
		//		//pack die alte Hose ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeTrousers(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setTrousers(item);
		//}
		//if (mContainerDraggedTo == mShinbone) {
		//	if (mInventory->getShinbone() != NULL){
		//		//pack die alte Hose ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeShinbone(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setShinbone(item);
		//}
		//if (mContainerDraggedTo == mHandLeft) {
		//	if (mInventory->getHandLeft() != NULL){
		//		//pack das alte Schild ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeHandLeft(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setHandLeft(item);
		//}
		//if (mContainerDraggedTo == mHandRight) {
		//	if (mInventory->getHandRight() != NULL){
		//		//pack die alte Waffe ins Inventar
		//		mInventory->addItemToContainer(mInventory->removeHandRight(), mInventory->getBackpack());
		//		itemSwitched = true;
		//	}
		//	mInventory->setHandRight(item);
		//}
		if (itemSwitched)
		{
			try {
				//Bild muss im Inventarfenster an die neue Position gelegt werden
				//pair<pair<int,int>,Item*> posInContainer = mInventory->getItemPositionInContainer(static_cast<Item*>(mOldItemInSlot->getUserData()));

				//// Container Fenster suchen
				//CEGUI::Window* win = findContainer(posInContainer.second);
				//// Dem Fenster hinzufügen
				//win->addChildWindow(mOldItemInSlot);
				//// Positionieren		
				//mOldItemInSlot->setPosition( CEGUI::Absolute,
				//	CEGUI::Point(posInContainer.first.first * 30, posInContainer.first.second * 30));

			}
			catch (IllegalArgumentException iae)
			{
				// TODO: Item am Boden suchen
			}
		}


		// Update am Inventar durchgeführt, jetzt sollen noch die Werte aktualisiert werden
		update();
	}


	void InventoryWindow::refreshTabs()
	{
		std::list<CEGUI::Window*>::iterator it = mContainers.begin();

		// entferne alle Unterfenster
		while(mContainerTabs->getTabCount() > 0)
		{
			mContainerTabs->removeTab(mContainerTabs->getTabContents(0)->getName());
		}

		// füge sie in richtiger Reihenfolge wieder hinzu
		while (it != mContainers.end())
		{
			mContainerTabs->addTab(*it++);
		}
	}

	bool InventoryWindow::isFreeInContainer(Item* item, pair<int,int> kaestchenPos, Container* container)
	{
        return container->canPlaceAt(item, kaestchenPos.first, kaestchenPos.second);
		//return mInventory->isFreeInContainer(item, kaestchenPos, container);
        //return true;
	}


	Item* InventoryWindow::getGroundItem()
	{
		return mGroundItem;
	}

	void InventoryWindow::initSlots()
	{
		mArmorValueWindow = getWindow("InventoryWindow/ArmorValueWindow");
        mArmorValueWindow->setProperty("Image", "set:InventorySymbols image:Shield");
		mArmorValueWindow->setTooltipText("Rüstung / Behinderung");
		//TODO: An Tooltip anhängen: woraus setzt sich die Rüstung zusammen?
		mArmorValue = getWindow("InventoryWindow/ArmorValue");
		mArmorValue->setText(" 3");
		//mArmorValue->setTextColours(CEGUI::colour(0.0,0.4,0.0)); //TODO port to cegui0.5
		mArmorValue->disable();
		mHandicapValue = getWindow("InventoryWindow/HandicapValue");
		mHandicapValue->setText("2");
		//mHandicapValue->setTextColours(CEGUI::colour(0.4,0.0,0.0)); //TODO port to cegui0.5
		mHandicapValue->disable();

		mWeightWindow = getWindow("InventoryWindow/WeightWindow");
        mWeightWindow->setProperty("Image", "set:InventorySymbols image:Weight");
		mWeightWindow->setTooltipText("Getragenes Gewicht\r\nin Stein");

		mTotalWeight = getWindow("InventoryWindow/TotalWeight");
		mTotalWeight->setText("");
		// Schwarzer Text (wird rot, wenn überladen)
		//mTotalWeight->setTextColours(CEGUI::colour(0.0,0.0,0.0));  //TODO port to cegui0.5
		mTotalWeight->disable();
				

		// Das Item-Beschreibungsfeld holen
		mDescription = getScrollablePane("InventoryWindow/Description");

		// Das "BodenItem" initiieren
		mGroundItem = new Item(1/*"Boden", "Dieses Item repräsentiert den Boden"*/);
		mGroundItem->setImageName("Trank");
		mGroundItem->setItemType(Item::ITEMTYPE_OTHER);
		mGroundItem->setSize(1,1);
		//mGroundItem->setContainer(true, std::make_pair<int,int>(mGroundDimension.first,mGroundDimension.second));


		mContainerTabs = getTabControl("InventoryWindow/Tabs");
		
		// Hole die einzelnen Slots aus dem XML-File
		mHelmet = getWindow("InventoryWindow/Helmet");
		mHelmet->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_HELMET));
		
		mRingLeft = getWindow("InventoryWindow/RingLeft");
		mRingLeft->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_RING));
		
		mRingRight = getWindow("InventoryWindow/RingRight");
		mRingRight->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_RING));
		
		mHandLeft = getWindow("InventoryWindow/HandLeft");
		mHandLeft->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_SHIELD));

		mHandRight = getWindow("InventoryWindow/HandRight");
		mHandRight->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_WEAPON));

		mGloves = getWindow("InventoryWindow/Gloves");
		mGloves->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_GLOVES));

		mBraceletLeft = getWindow("InventoryWindow/BraceletLeft");
		mBraceletLeft->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BRACELET));

		mBraceletRight = getWindow("InventoryWindow/BraceletRight");
		mBraceletRight->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BRACELET));

		mArmor = getWindow("InventoryWindow/Armor");
		mArmor->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_ARMOR));

		mCape = getWindow("InventoryWindow/Cape");
		mCape->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_CAPE));

		mBracers = getWindow("InventoryWindow/Bracers");
		mBracers->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BRACERS));

		mBackpack = getWindow("InventoryWindow/Backpack");
		mBackpack->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BACKPACK));

		mBelt = getWindow("InventoryWindow/Belt");
		mBelt->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_BELT));

		mNecklace = getWindow("InventoryWindow/Necklace");
		mNecklace->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_NECKLACE));

		mTrousers = getWindow("InventoryWindow/Trousers");
		mTrousers->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_TROUSERS));

		mShinbone = getWindow("InventoryWindow/Shinbone");
		mShinbone->setUserString("ItemType",Item::getItemTypeString(Item::ITEMTYPE_SHINBONE));

		mBoots = getWindow("InventoryWindow/Boots");
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
		//createItem(mInventory->getRingLeft(), mRingLeft);
		//createItem(mInventory->getRingRight(), mRingRight);
		//createItem(mInventory->getHandLeft(), mHandLeft);
		//createItem(mInventory->getHandRight(), mHandRight);
		//createItem(mInventory->getGloves(), mGloves);
		//createItem(mInventory->getBraceletLeft(), mBraceletLeft);
		//createItem(mInventory->getBraceletRight(), mBraceletRight);
		//createItem(mInventory->getArmor(), mArmor);
		//createItem(mInventory->getCape(), mCape);
		//createItem(mInventory->getBracers(), mBracers);
		//createItem(mInventory->getBackpack(), mBackpack);
		//createItem(mInventory->getBelt(), mBelt);
		//createItem(mInventory->getNecklace(), mNecklace);
		//createItem(mInventory->getHelmet(), mHelmet);
		//createItem(mInventory->getTrousers(), mTrousers);
		//createItem(mInventory->getShinbone(), mShinbone);
		//createItem(mInventory->getBoots(), mBoots);


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
				createAndFillContainer(dynamic_cast<Container*>(currentItem));
			}
		}
		// Den Boden hinzufuegen
		//createAndFillContainer(mGroundItem);
	}

	void InventoryWindow::createAndFillContainer(Container* container){
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


			mContainerTabs->addTab(containerWindow);

			CEGUI::Window* containerSpace = static_cast<CEGUI::Window*>(CEGUI::WindowManager::getSingletonPtr()->createWindow("RastullahLook/Container", "InventoryWindow/Tabs/"+container->getName()+"/Content"));
			
			containerSpace->setVerticalAlignment(CEGUI::VA_CENTRE);
			containerSpace->setHorizontalAlignment(CEGUI::HA_CENTRE);
            containerSpace->setSize(UVector2(cegui_absdim(container->getVolume().first*30),
                cegui_absdim(container->getVolume().second*30)));
			containerSpace->setUserData(container);
			containerWindow->addChildWindow(containerSpace);


			// Inhalt in die Liste Einfügen
						// Boden an das Ende der Liste einfügen
			if (container == mGroundItem)
			{
				mContainers.push_back(containerWindow);
				mContainerContents.push_back(containerSpace);
			}
			// Rucksack vorne in die Liste einfuegen
			else if (container->getItemType() == Item::ITEMTYPE_BACKPACK)
			{
				mContainers.push_front(containerWindow);
				mContainerContents.push_front(containerSpace);
			}
			// den Rest dazwischen
			else 
			{
				if (mContainers.size() > 1)
				{
					mContainers.insert(++(mContainers.begin()),containerWindow);
					mContainerContents.insert(++(mContainerContents.begin()),containerSpace);
				} else {
					mContainers.push_front(containerWindow);
					mContainerContents.push_front(containerSpace);
				}
			}
			

			addDropListener(containerSpace);

			// Items im Container erstellen
			std::set<Item*> itemsInContainer;

			//ContainerLayout temp = container->getContainerLayout();
			//for (unsigned x = 0; x < temp.size(); x++){
			//	for (unsigned y = 0; y < temp[0].size(); y++) {
			//		if (temp[x][y] != NULL && (itemsInContainer.find(temp[x][y]) == itemsInContainer.end())){
			//			DragContainer* itemhandler = createItem(temp[x][y],containerSpace);
			//			itemhandler->setPosition(CEGUI::Absolute,CEGUI::Point(x*30,y*30));
			//			itemsInContainer.insert(temp[x][y]);
			//			LOG_MESSAGE(
			//				"InventoryWindow",
			//				Ogre::String("erzeuge Bild im Container: ") + temp[x][y]->getName());
			//		}
			//	}
			//}
		}
	}


	void InventoryWindow::removeContainerAndContent(Container* container)
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
			mContainerContents.erase(it);
			mContainers.erase(itParent);


			emptySlot(windowToEmpty);
			emptySlot(parentTabWindow);

			mContainerTabs->removeTab(parentTabWindow->getName());

			CEGUI::WindowManager::getSingletonPtr()->destroyWindow(parentTabWindow);
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
			CEGUI::WindowManager::getSingletonPtr()->destroyWindow(*it);
			it++;
		} // alle Container wurden entleert

		it = mContainers.begin();
		while (it != mContainers.end())
		{
			emptySlot(*it);
			mContainerTabs->removeTab((*it)->getName());
			CEGUI::WindowManager::getSingletonPtr()->destroyWindow(*it);
			it++;
		} // alle ContainerTabs wurden entleert
		mContainerContents.clear();
		mContainers.clear();
	}

	void InventoryWindow::emptySlot(CEGUI::Window* slot)
	{
		while(slot->getChildCount() > 0){
			CEGUI::Window* windowToDestroy = slot->getChildAtIdx(0);
			CEGUI::WindowManager::getSingletonPtr()->destroyWindow(windowToDestroy);
			slot->removeChildWindow(windowToDestroy);
		}
	}

	void InventoryWindow::addDropListener(CEGUI::Window* slot){
		// onMouseOver (beim Drag)
		slot->subscribeEvent(Window::EventDragDropItemEnters, boost::bind(&InventoryWindow::handleDragEnter,this,_1));
		// onMouseOut (beim Drag)
		slot->subscribeEvent(Window::EventDragDropItemLeaves, boost::bind(&InventoryWindow::handleDragLeave,this,_1));
		// onItemDropped
		slot->subscribeEvent(Window::EventDragDropItemDropped, boost::bind(&InventoryWindow::handleDragDropped,this,_1)); 
	}

	CEGUI::Window* InventoryWindow::findContainer(Container* container)
	{
		std::list<CEGUI::Window*>::iterator it = mContainerContents.begin();
		while (it != mContainerContents.end())
		{
			if (static_cast<Item*>((*it)->getUserData()) == container)
				return (*it);
			it++;
		}
		Throw(IllegalArgumentException, "Es gibt kein Containerfenster zu dem gesuchten Item");
	}

	DragContainer* InventoryWindow::createItem(Item* item, Window* parent, UVector2 position)
	{
		// Prüfe, ob ein Item da ist
		if (item != NULL) {

			// TODO: Ausnahmen sind Waffe und Schild (wenn parent mHand* ist, soll Hintergrund gefüllt werden)
			// Waffe:
			if (parent == mHandRight || parent == mHandLeft) 
			{
				position = UVector2(cegui_absdim(30-(item->getSize().first * 15)), cegui_absdim(75-(item->getSize().second *15)));
			}

			static int itemCnt = 0;
			itemCnt++;

			// Erzeuge einen Handler für Drag and Drop
			DragContainer* itemhandler = static_cast<DragContainer*>(
				CEGUI::WindowManager::getSingletonPtr()->createWindow("DragContainer", Ogre::StringConverter::toString(itemCnt) + item->getName()));
			itemhandler->setPosition(position);
			itemhandler->setSize(UVector2(cegui_absdim(item->getSize().first*30),
                cegui_absdim(item->getSize().second*30))); 
			itemhandler->setUserString("ItemType",Item::getItemTypeString(item->getItemType()));
			itemhandler->setUserData(item);
			itemhandler->setTooltipText(item->getName());
			itemhandler->subscribeEvent(CEGUI::Window::EventMouseClick,
				Event::Subscriber(&InventoryWindow::handleMouseClicked,this));
			
			parent->addChildWindow(itemhandler);

			CeGuiString tempName = item->getName();

			// Erzeuge das Hintergrundfenster des Items
			Window* itemWindow = 
				CEGUI::WindowManager::getSingletonPtr()->createWindow(
					"RastullahLook/Item", 
					tempName + "Item");

			itemWindow->setPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));
			itemWindow->setSize(UVector2(cegui_absdim(item->getSize().first*30),
                cegui_absdim(item->getSize().second*30)));
			itemWindow->setProperty("FrameEnabled", "false");
			itemWindow->setProperty("BackgroundColour", mColorItemNormal);
			// Gib dem Item noch ein Bild...
			itemWindow->setProperty("Image",
                CeGuiString("set:ModelThumbnails image:") + item->getImageName());
			itemWindow->disable(); 

			itemhandler->addChildWindow(itemWindow);

			return itemhandler;
		}
		else
		{
			// Kein Item da...
			return NULL;
		}
	}

	bool InventoryWindow::handleMouseEnters(const EventArgs &args) 
	{
		return true;
	}

	bool InventoryWindow::handleMouseLeaves(const EventArgs &args) 
	{
		return true;
	}


	bool InventoryWindow::handleMouseClicked(const EventArgs &args) 
	{
		// Hole das ausgewählte Item
		const CEGUI::MouseEventArgs& mea = static_cast<const MouseEventArgs&>(args);

		// Nur, wenn es sich uim einen Itemhandler handelt, soll was geschehen...
		if (mea.window->getUserData() != NULL)
		{
			Item* item = NULL;
			item = static_cast<Item*>(mea.window->getUserData());
	
			// deaktiviere das letzte aktive Item, falls es eins gab
			if (mActiveItemWindow){
				mActiveItemWindow->setProperty("BackgroundColour", mColorItemNormal);
				mActiveItemWindow->setProperty("FrameEnabled", "False");
			}
			
			assert(mea.window->getChildCount() > 0);
			
			// aktiviere das angewählte Item...
			mActiveItemWindow = mea.window->getChildAtIdx(0);
			mActiveItemWindow->setProperty("BackgroundColour", mColorItemSelected);
			mActiveItemWindow->setProperty("FrameEnabled", "True");
			
			// Schreibe Iteminfos in Description Fenster
			if (item) 
			{
				mDescription->setText(item->getDescription());
				if (item->getDescription().length() > 200)
				{
					// Scrollbalken aktivieren
                    mDescription->setShowVertScrollbar(true);
				}
				else 
				{

				}
				renderItem(item);
			}
			else
			{
				mDescription->setText("");
			}
		}
		return true;
	}

	void InventoryWindow::initRenderToTexture()
	{
		// Das Feld, in das gerendert werden soll (TransparentStaticImage)
		mItemRenderImage = getWindow("InventoryWindow/ItemPicture");

		SceneManager* tempManager = CoreSubsystem::getSingleton().
				getWorld()->getSceneManager();
	

		// Setup Render To Texture for preview window
		mRenderTexture = Root::getSingleton().getRenderSystem()->createRenderTexture( "InventoryItemRttTex", 128, 128, TEX_TYPE_2D, PF_R8G8B8 );
        {
			
			/*Ogre::Camera* itemRenderCam = tempManager->createCamera("InventoryItemRenderCamera");
			
            SceneNode* itemCamNode = 
				tempManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,-1000,0));
			
			itemCamNode->attachObject(itemRenderCam);
			itemRenderCam->pitch(Ogre::Radian(90));

            mRenderViewport = mRenderTexture->addViewport( itemRenderCam );
            mRenderTexture->setAutoUpdated(false);
			mRenderViewport->setOverlaysEnabled(false);
            mRenderViewport->setClearEveryFrame( true );
			//mRenderViewport->setClearEveryFrame(false);
            mRenderViewport->setBackgroundColour( ColourValue::Black );
			*/
			
			//loadCamera();

            
			
			
        }

        // Retrieve CEGUI texture for the RTT
		mTexture = UiSubsystem::getSingleton().getGUIRenderer()->createTexture((CEGUI::utf8*)"InventoryItemRttTex");

        mImageSet = CEGUI::ImagesetManager::getSingleton().createImageset(
                    (CEGUI::utf8*)"InventoryItemRttImageset", mTexture);

        mImageSet->defineImage((CEGUI::utf8*)"InventoryItemRttImage", 
                CEGUI::Point(0.0f, 0.0f),
                CEGUI::Size(mTexture->getWidth(), mTexture->getHeight()),
                CEGUI::Point(0.0f,0.0f));
	}

	void InventoryWindow::renderItem(Item* item)
	{
		SceneManager* tempManager = CoreSubsystem::getSingleton().
					getWorld()->getSceneManager();

		// Für einen Aufhängeknoten Sorgen
		if (mInventoryItemNode == NULL){
			mInventoryItemNode = tempManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,-999999.6,-0.2));
			//mInventoryItemNode->setPosition(0,-100,-0.2);
		}
		
		if (mRenderItemEntity) {
			// Vom Knoten entfernen
			mInventoryItemNode->detachAllObjects();
			// alte Entity entfernen
			tempManager->destroyEntity(mRenderItemEntity);
			mRenderItemEntity = NULL;
		}
		if (item->getActor())
		{
			// Camera update erforderlich, da Mapchange da was kaputt gemacht haben könnte
			unloadCamera();
			loadCamera();

			// Item setzen und Schnappschuss machen
			mRenderItemEntity = tempManager->createEntity("inventoryRenderedItem",
                (static_cast<MeshObject*>(item->getActor()->getControlledObject()))->getMeshName());
			mInventoryItemNode->attachObject(mRenderItemEntity);
		
			// Das Item hat einen Actor
			//mItemActor = ActorManager::getSingleton().createMeshActor("inventoryRenderedItem",
            //  (static_cast<MeshObject*>(item->getActor()->getControlledObject()))->getMeshName());
			//mItemActor->placeIntoScene(Ogre::Vector3(0,-101055.3,-0.2));
            mItemRenderImage->setSize(CeGuiHelper::asAbsolute(CEGUI::Vector2(128, 128)));
			mItemRenderImage->setProperty("Image",
                "set:" + mImageSet->getName() + " image:InventoryItemRttImage");
			mRenderTexture->update();

		}
        else if (item->getImageName() != "")
        {
            mItemRenderImage->setProperty("Image",
                "set:ModelThumbnails image:" + item->getImageName());

			float div = item->getSize().first / float(item->getSize().second);

			if (div < 1)
			{
				//schmales Item
                mItemRenderImage->setSize(CeGuiHelper::asAbsolute(CEGUI::Vector2(128*div, 128)));
			}
			else
			{
				// breites oder quadratisches Item
                mItemRenderImage->setSize(CeGuiHelper::asAbsolute(CEGUI::Vector2(128, 128/div)));
			}
		}
	}


	void InventoryWindow::loadCamera()
	{
		mCameraActor = ActorManager::getSingleton().createCameraActor("InventoryItemRttCam");
		mCameraActor->placeIntoScene(Ogre::Vector3(0,-999999,0));
		mCameraActor->pitch(-90);
		mCameraActor->getPhysicalThing()->freeze();

		mRenderTexture->setAutoUpdated(false);
        mRenderViewport = mRenderTexture->addViewport(
            (static_cast<CameraObject*>(mCameraActor->getControlledObject()))->getCamera());
        mRenderViewport->setOverlaysEnabled(false);
        mRenderViewport->setClearEveryFrame(true);
        mRenderViewport->setBackgroundColour( ColourValue::Black );
	}

	void InventoryWindow::unloadCamera()
	{
		// wenn loadCamera schon mal geladen wurde, soll die Camera zerstört werden
		if (mRenderViewport)
		{
			mRenderTexture->removeViewport(0);
			ActorManager::getSingleton().destroyActor(mCameraActor);
		}
	}

	/*!
	*		Überprüft, ob das Item von dem Slot akzeptiert wird
	*/
	bool InventoryWindow::checkTypeAccepted(CEGUI::Window* window, CEGUI::DragContainer* draggedItem)
	{
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
	std::pair<int,int> InventoryWindow::calculateNewPosition(const DragDropEventArgs& ddea)
	{
		// Errechnung der Koordinaten, in welchem Kästchen denn nun gedroppt wird
		Point absMouse = MouseCursor::getSingleton().getPosition();
        Point scrnPt = ddea.window->getPixelRect().getPosition();
		Point relMouse = absMouse - ddea.dragDropItem->getPixelRect().getPosition();

		//Bug in CeGUI (1. rel Mouse koordinate spinnt
		if (relMouse.d_x < 0) {
			relMouse.d_x = 15;
		}

		LOG_MESSAGE(
			"InventoryWindow",
			Ogre::String("relMouse: Point x:")
			+ StringConverter::toString(relMouse.d_x)
			+ ", Point y:"
			+ StringConverter::toString(relMouse.d_y));

		Point pointInBackpack = absMouse-scrnPt;
		pointInBackpack -= relMouse;

		// Position des nächsten Kästchens bestimmen		
		if (pointInBackpack.d_x < 0)
        {
			pointInBackpack.d_x = 0;
		}
		if (pointInBackpack.d_y < 0)
        {
			pointInBackpack.d_y = 0;
		}

		if (pointInBackpack.d_x > 16)
        {
			pointInBackpack.d_x = (int(pointInBackpack.d_x) + 14) -
                ((int(pointInBackpack.d_x) + 14) % 30);
		}
        else
        {
			pointInBackpack.d_x = 0;
		}

		if (pointInBackpack.d_y > 16)
        {
			pointInBackpack.d_y = (int(pointInBackpack.d_y) + 14 ) -
                ((int(pointInBackpack.d_y) + 14) % 30);
		}
        else
        {
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
	bool InventoryWindow::handleDragEnter(const CEGUI::EventArgs& args)
	{
		// Event zu einem DragDropEvent machen
		const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
		
		if (ddea.window->getUserData()){
			// Es handelt sich um einen Container
			Item* container = static_cast<Item*>(ddea.window->getUserData());
			if (container == getGroundItem())
			{
				// Boden (nimmt alles)
				ddea.window->setProperty("ContainerColour", mColorAccept);
				return true;
			} 
			else if (container && container->getItemType() == Item::ITEMTYPE_BACKPACK)
			{
				// Ein Container kann keinen Rucksack aufnehmen
				if (!(ddea.dragDropItem->getUserString("ItemType").compare(Item::getItemTypeString(Item::ITEMTYPE_BACKPACK))))
				{
					ddea.window->setProperty("ContainerColour", mColorReject);
					return false;
				}
				else 
				{
					ddea.window->setProperty("ContainerColour", mColorAccept);
					return true;
				}
			}
			else {
				// Beliebiger anderer Container am Körper
				// TODO: Beschränkung auf Itemtypen... 

				ddea.window->setProperty("ContainerColour", mColorAccept);
				return true;
			}
		}
		else if (checkTypeAccepted(ddea.window, ddea.dragDropItem))
		{
			ddea.window->setProperty("ContainerColour", mColorAccept);
			return true;
		}
		else
		{
			ddea.window->setProperty("ContainerColour", mColorReject);
			return false;
		}
	}

	bool InventoryWindow::handleDragLeave(const CEGUI::EventArgs& args)
	{
		// Event zu einem DragDropEvent machen
		const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
		ddea.window->setProperty("ContainerColour", mColorNormal);
		return true;
	}

	bool InventoryWindow::handleDragDropped(const CEGUI::EventArgs& args)
	{
		// Event zu einem DragDropEvent machen
		const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
		// Farbe zurücksetzen
		ddea.window->setProperty("ContainerColour", mColorNormal);


		if (ddea.window->getUserData()){
			// Es handelt sich um einen Container
			Item* container = static_cast<Item*>(ddea.window->getUserData());
			if (container == mGroundItem)
			{
				// Boden (nimmt alles)
			} 
			else if (container->getItemType() == Item::ITEMTYPE_BACKPACK)
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

			std::pair<int,int> newPos = calculateNewPosition(ddea);

			if (isFreeInContainer(
				static_cast<Item*>(ddea.dragDropItem->getUserData()),
				newPos, static_cast<Container*>(ddea.window->getUserData())))
			{
				ddea.window->addChildWindow(ddea.dragDropItem);

				mPosDraggedTo=CEGUI::Point(newPos.first*30,newPos.second*30);
				mDroppedItem = ddea.dragDropItem;
				mContainerDraggedTo = ddea.window;

				updateInventory();
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

				if (ddea.window->getChildCount() > 0)
				{
					// Slot hat schon einen anderen Gegenstand getragen
					mOldItemInSlot = static_cast<CEGUI::DragContainer*>(ddea.window->getChildAtIdx(0));
					ddea.window->removeChildWindow(mOldItemInSlot);
				}
				else
				{
					mOldItemInSlot = NULL;
				}

				ddea.window->addChildWindow(ddea.dragDropItem);

				// Waffe und Schild sollen mittig positioniert werden
				if ((!ddea.window->getUserString("ItemType").compare(Item::getItemTypeString(Item::ITEMTYPE_WEAPON)) ||
					(!ddea.window->getUserString("ItemType").compare(Item::getItemTypeString(Item::ITEMTYPE_SHIELD)))) &&
					(ddea.dragDropItem->getUserData()))
				{
					Item* item = static_cast<Item*>(ddea.dragDropItem->getUserData());
					mPosDraggedTo= CEGUI::Point(30-(item->getSize().first * 15),75-(item->getSize().second *15));
				}
				// Rest kommt in die linke obere Ecke (weil ausfüllend)
				else {
					mPosDraggedTo=CEGUI::Point(0.0,0.0);
				}
				mDroppedItem = ddea.dragDropItem;
				mContainerDraggedTo = ddea.window;

				updateInventory();
				return true;
			} else {
				// Item passt nicht zum Slot
				return false;
			}
		}
	}
}
