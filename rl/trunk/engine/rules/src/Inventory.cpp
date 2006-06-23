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


#include "Item.h"
#include "Armor.h"
#include "Weapon.h"
#include "Inventory.h"
#include "Exception.h"
#include "ActorManager.h"
#include "Creature.h"
#include "Eigenschaft.h"
#include "ItemManager.h"

using namespace std;

namespace rl
{

	Inventory::Inventory(Creature* owner) :
		mOwner(NULL),
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
		mWeapons(),
		mCurrentWeight(0),
		mCurrentBeByWeight(0),
		mCurrentBe(0),
		mCurrentRs(0),
		mValuesUpToDate(false)
	{

		mOwner = owner;


		// *****************
		// TODO: Entfernen, wenn der ItemManager in Skripte eingebunden ist...
		// *****************
		Item* backpack = ItemManager::getSingleton().createItem("Rucksack");
		mBackpack = backpack;

		Item* trank = ItemManager::getSingleton().createItem("Heiltrank");
		addItem(trank);

		
		Item* kurzschwert = ItemManager::getSingleton().createItem("Kurzschwert");
		addItem(kurzschwert);

		Item* fackel = ItemManager::getSingleton().createItem("Fackel");
		addItem(fackel);
		
		Item* meinUmhang = ItemManager::getSingleton().createItem("Fellumhang");
		addItem(meinUmhang);

		Item* meineRuestung = ItemManager::getSingleton().createItem("Krötenhaut");
		mArmor = meineRuestung;

		Item* meineStiefel = ItemManager::getSingleton().createItem("Lederstiefel");
		addItem(meineStiefel);

		Item* meineHandschuhe = ItemManager::getSingleton().createItem("Lederhandschuhe");
		addItem(meineHandschuhe);

		Item* meinRing = ItemManager::getSingleton().createItem("Rubinring");
		addItem(meinRing);
		
		Item* meinArmreif = ItemManager::getSingleton().createItem("Perlenarmreif");
		mBraceletLeft = meinArmreif;

	}

	Inventory::~Inventory() 
	{
		// Lösche Alle Objekte aus dem Rucksack
		/*for (unsigned int x = 0; x < mBackpackLayout.size(); x++){
			for (unsigned int y = 0; y < mBackpackLayout[0].size(); y++){
				if (mBackpackLayout[x][y] != NULL){
					delete mBackpackLayout[x][y];
				}
			}
		}
		*/
	}

	
	void Inventory::markDirty()
	{
		mValuesUpToDate = false;
	}

	Inventory::ItemList Inventory::getAllItems()
	{
		ItemList allItems(0);
		ItemList wornItems = getWornItems();

		// Container durchgehen
		ItemList::iterator it = wornItems.begin();

		while (it != wornItems.end())
		{
			allItems.push_back(*it);
			if ((*it)->isContainer())
			{
				addContainerItemsToList(*(*it), allItems);
			}
			it++;
		}
		return allItems;
	}

	void Inventory::addContainerItemsToList(Item &container, ItemList &itemList)
	{
		assert( container.isContainer());
		for (unsigned int x = 0; x < container.getContainerLayout().size(); x++){
			for (unsigned int y = 0; y < container.getContainerLayout()[0].size(); y++){
				if (container.getContainerLayout()[x][y] != NULL){
			
					// Item im Container -> überprüfe ob schon zur Liste hinzugefügt
					ItemList::iterator it = itemList.begin();
					bool found = false;

					// Durchsuche, ob schon in Liste
					while (it != itemList.end())
					{
						if ((*it) == container.getContainerLayout()[x][y])
						{
							found = true;
							break;
						}
						it++;
					}
					if (!found) { // Nur wenn noch nicht in Liste, soll das Item zur Liste hinzugefügt werden
						itemList.push_back(container.getContainerLayout()[x][y]);
						
						// rekursiv aufrufen, falls es sich um ein Container handelt
						if (container.getContainerLayout()[x][y]->isContainer())
							addContainerItemsToList(*(container.getContainerLayout()[x][y]), itemList);
					}
				}
			}
		}
	}

	Inventory::ItemList Inventory::getAllContainers()
	{
		ItemList allContainers(0);
		ItemList wornItems = getWornItems();

		// Alle Items durchsuchen
		ItemList::iterator it = wornItems.begin();
		
		while (it != wornItems.end())
		{
			if ((*it)->isContainer())
			{
				allContainers.push_back(*it);
				addContainersToList(*(*it), allContainers);
			}
			it++;
		}
		return allContainers;
	}

	void Inventory::addContainersToList(Item &container, ItemList &itemList)
	{
		assert( container.isContainer());
		for (unsigned int x = 0; x < container.getContainerLayout().size(); x++){
			for (unsigned int y = 0; y < container.getContainerLayout()[0].size(); y++){
				if (container.getContainerLayout()[x][y] != NULL && container.getContainerLayout()[x][y]->isContainer()){
			
					// Item im Container -> überprüfe ob schon zur Liste hinzugefügt
					ItemList::iterator it = itemList.begin();
					bool found = false;

					// Durchsuche, ob schon in Liste
					while (it != itemList.end())
					{
						if ((*it) == container.getContainerLayout()[x][y])
						{
							found = true;
							break;
						}
						it++;
					}
					if (!found) { // Nur wenn noch nicht in Liste, soll das Item zur Liste hinzugefügt werden
						itemList.push_back(container.getContainerLayout()[x][y]);
						
						// rekursiv aufrufen
						addContainersToList(*(container.getContainerLayout()[x][y]), itemList);
					}
				}
			}
		}
	}

	int Inventory::getOverallWeight()
	{
		if (!mValuesUpToDate)
			updateStats();
		return mCurrentWeight;
	}

	pair<int,int> Inventory::getOverallBe()
	{
		if (!mValuesUpToDate)
			updateStats();
		return make_pair<int,int>(mCurrentBe, mCurrentBeByWeight);
	}

	int Inventory::getOverallRs()
	{
		if (!mValuesUpToDate)
			updateStats();
		return mCurrentRs;
	}

	Inventory::ItemList Inventory::getWornItems()
	{
		ItemList wornItems(0);
		if (mBackpack)
			wornItems.push_back(mBackpack);
		if (mRingLeft)
			wornItems.push_back(mRingLeft);
		if (mRingRight)
			wornItems.push_back(mRingRight);
		if (mHandLeft)
			wornItems.push_back(mHandLeft);
		if (mHandRight)
			wornItems.push_back(mHandRight);
		if (mGloves)
			wornItems.push_back(mGloves);
		if (mBraceletLeft)
			wornItems.push_back(mBraceletLeft);
		if (mBraceletRight)
			wornItems.push_back(mBraceletRight);
		if (mArmor)
			wornItems.push_back(mArmor);
		if (mCape)
			wornItems.push_back(mCape);
		if (mBracers)
			wornItems.push_back(mBracers);
		if (mBelt)
			wornItems.push_back(mBelt);
		if (mNecklace)
			wornItems.push_back(mNecklace);
		if (mHelmet)
			wornItems.push_back(mHelmet);
		if (mTrousers)
			wornItems.push_back(mTrousers);
		if (mShinbone)
			wornItems.push_back(mShinbone);
		if (mBoots)
			wornItems.push_back(mBoots);

		return wornItems;
	}

	
	void Inventory::updateStats()
	{
		calculateWeight(getAllItems());
		calculateRsAndBe();
		// flag setzen: Werte sind up to Date
		mValuesUpToDate = true;
	}

	// Berechnungsmethoden
	void Inventory::calculateWeight(ItemList items)
	{
		ItemList::iterator it = items.begin();

		unsigned int totalWeight = 0;

		while (it != items.end())
		{
			totalWeight += (*it)->getWeight();

			it++;
		}
		mCurrentWeight = totalWeight;
	}

	void Inventory::calculateRsAndBe()
	{
		mCurrentBe = 0;
		mCurrentRs = 0;

		// Behinderung durch Traglast ist 1 Punkt pro KK * 40 Unzen / 2 ab KK Stein gewicht
		mCurrentBeByWeight = (mCurrentWeight > mOwner->getEigenschaft(E_KOERPERKRAFT) * 40)?
			mCurrentWeight / mOwner->getEigenschaft(E_KOERPERKRAFT) * 40 :
			0;

		ItemList wornItems = getWornItems();

		ItemList::iterator it = wornItems.begin();
		while (it != wornItems.end())
		{
			// Beachte nur Rüstungen
			if (dynamic_cast<Armor*>(*it) != 0)
			{
				mCurrentRs += (dynamic_cast<Armor*>(*it))->getGRS();
				mCurrentBe += (dynamic_cast<Armor*>(*it))->getGBE();
			}
			it++;
		}
	}

	/*Item* Inventory::createItem(const CeGuiString name, const CeGuiString description, const CeGuiString imageName, Item::ItemType type, pair<int,int> size)
	{
		Item* item = new Item(name, description);
		item->setImageName(imageName);
		item->setItemType(type);
		item->setSize(size.first,size.second);

		return item;
	}
*/
	// TODO: Anordnung der Items
	/*set<Item*> Inventory::getAllItemsInContainer()
	{
		set<Item*> items;
		
		for (int x = 0; x < mBackpackDimension.first; x++){
			for(int y = 0; y < mBackpackDimension.second; y++){
				items.insert(mBackpackLayout[x][y]);
			}
		}
		return items;
	}*/
	

	bool Inventory::isFreeInContainer(Item* item, pair<int,int> posKaestchen, Item* container)
	{
		int xSize = item->getSize().first;
		int ySize = item->getSize().second;

		bool free = true;
		for (int x = posKaestchen.first; x < (posKaestchen.first + xSize); x++){
			for (int y = posKaestchen.second; y < (posKaestchen.second + ySize); y++){
				
				Logger::getSingletonPtr()->log("InventoryWindow",Logger::LL_MESSAGE,
					CEGUI::String("Checking Point in Backpack: Point x:")
					+ StringConverter::toString(x) 
					+ ", Point y:"
					+ StringConverter::toString(y));
				
				if (x >= int(container->getContainerLayout().size()) || y >= int(container->getContainerLayout()[0].size())){
					// Es wird versucht, das Item außerhalb des Containers zu platzieren
					free = false;
				} else if (container->getContainerLayout()[x][y] == item) {
					// kein Problem, item wird nur ein bisschen verschoben
				} else {
					// Siehe nach, ob ein anderes Item im Weg ist
					free = free && (!container->getContainerLayout()[x][y]);
				}
			}
		}
		
		return free;
	}

	void Inventory::setItemContainerPosition(Item* item, int xPos, int yPos, Item* container)
	{
		int xSize = item->getSize().first;
		int ySize = item->getSize().second;
		
		// Itempointer auf neuer Position eintragen
		for (int x = 0; x < xSize; x++){
			for (int y = 0; y < ySize; y++) {
				container->getContainerLayout()[xPos+x][yPos+y] = item;
			}
		}
	}

	void Inventory::removeItem(Item* item){

		switch (item->getItemType()) {
			case Item::ITEMTYPE_WEAPON:
				if (getHandRight() == item){
					removeHandRight();
				}
				break;
			case Item::ITEMTYPE_SHIELD:
				if (getHandLeft() == item){
					removeHandLeft();
				}
				break;
			case Item::ITEMTYPE_RING:
				if (getRingLeft() == item){
					removeRingLeft();
				} else if (getRingRight() == item){
					removeRingRight();
				}
				break;
			case Item::ITEMTYPE_GLOVES:
				if (getGloves() == item){
					removeGloves();
				}
				break;
			case Item::ITEMTYPE_BRACELET:
				if (getBraceletLeft() == item){
					removeBraceletLeft();
				} else if (getBraceletRight() == item){
					removeBraceletRight();
				}
				break;
			case Item::ITEMTYPE_ARMOR:
				if (getArmor() == item){
					removeArmor();
				}
				break;

			case Item::ITEMTYPE_CAPE:
				if (getCape() == item){
					removeCape();
				}
				
			case Item::ITEMTYPE_BRACERS:
				if (getBracers() == item){
					removeBracers();
				}
				break;
			case Item::ITEMTYPE_BACKPACK:
				if (getBackpack() == item){
					removeBackpack();
				}
				break;
			case Item::ITEMTYPE_BELT:
				if (getBelt() == item){
					removeBelt();
				}
				break;
			case Item::ITEMTYPE_NECKLACE:
				if (getNecklace() == item){
					removeNecklace();
				}
				break;
			case Item::ITEMTYPE_HELMET:
				if (getHelmet() == item){
					removeHelmet();
				}
				break;
			case Item::ITEMTYPE_TROUSERS:
				if (getTrousers() == item){
					removeTrousers();
				}
				break;
			case Item::ITEMTYPE_SHINBONE:
				if (getShinbone() == item){
					removeShinbone();
				} 
				break;
			case Item::ITEMTYPE_BOOTS:
				if (getBoots() == item){
					removeBoots();
				}
				break;
			case Item::ITEMTYPE_OTHER:
				// Kann nicht in Slots sein
				break;
		}
		// Gehe die Container durch und entferne das Item
		removeItemFromContainers(item);

		markDirty();
	}

	void Inventory::removeItemFromContainers(Item* item)
	{
		ItemList allItems = getAllItems();
		
		ItemList::iterator itemIterator = allItems.begin();
		
		while (itemIterator != allItems.end()){
			Item* currentItem = *itemIterator++;
			if (currentItem->isContainer())
			{
				removeItemFromContainer(item, currentItem);
			}
		}
	}

	void Inventory::removeItemFromContainer(Item* item, Item* container)
	{
		for (unsigned int x = 0; x < container->getContainerLayout().size(); x++){
			for (unsigned int y = 0; y < container->getContainerLayout()[0].size(); y++){
				if (container->getContainerLayout()[x][y] == item){
					container->getContainerLayout()[x][y] = NULL;
				}
			}
		}
	}

	bool Inventory::addItemToContainer(Item* item, Item* container) {
		// Platz suchen
		try {
			if (item->getItemType() == Item::ITEMTYPE_BACKPACK) {
				// Ein Rucksack soll auf den Boden, wenn Platz ist
				Throw(IllegalStateException, "Ein Rucksack kann nicht in den Rucksack gepackt werden!");
			}
			pair<int,int> pos = findPositionWithEnoughSpace(item->getSize(), container->getContainerLayout());
			// Platz gefunden, platziere Item:
			for (int x = pos.first; x < pos.first+item->getSize().first; x++){
				for (int y = pos.second; y < pos.second+item->getSize().second; y++){
					container->getContainerLayout()[x][y] = item;
				}
			}
			return true;
		} catch (IllegalStateException ie){
			// Kein Platz mehr im Container... 
			// TODO: Auf den Boden legen
			Logger::getSingleton().log("Inventory",Logger::LL_ERROR, "addItemToContainer fehgeschlagen.");
			return false;
		}
	}
	
	pair<pair<int,int>, Item*> Inventory::getItemPositionInContainer(Item* item)
	{
		// Durchsuche Alle Container
		ItemList containers = getAllContainers();

		ItemList::iterator it = containers.begin();
		while (it != containers.end())
		{
			// Container absuchen
			for (unsigned int x = 0; x < (*it)->getContainerLayout().size(); x++){
				for (unsigned int y = 0; y < (*it)->getContainerLayout()[0].size(); y++){
					if ((*it)->getContainerLayout()[x][y] == item)
						return make_pair<pair<int,int>,Item*>(make_pair<int,int>(x,y), (*it));
				}
			}
			it++;
		}

		// Nicht gefunden
		Throw(IllegalArgumentException,"Item konnte nicht in den Containern im Inventar gefunden werden");
	}

	pair<int,int> Inventory::findPositionWithEnoughSpace(pair<int,int> space, ContainerLayout containerLayout){
		pair<int,int> pos;
		for (unsigned int x = 0; x < containerLayout.size(); x++){
			for (unsigned int y = 0; y < containerLayout[0].size(); y++){
				if (containerLayout[x][y] == NULL && checkSpace(x,y,space, containerLayout)){
					pos = pair<int,int>(x,y);
					return pos;
				}
			}
		}
		Throw(IllegalStateException, "Rucksack hat keinen Platz für das Item");
	}

	bool Inventory::checkSpace(int xStart, int yStart, pair<int,int> space, ContainerLayout containerLayout){
		bool free = true;
		for (unsigned int x = 0; x < space.first; x++){
			for (unsigned int y = 0; y < space.second; y++){
				// Falls Kästchen nicht mehr im Rucksack, ist auch kein Platz mehr :)
				if ((xStart+x) >= containerLayout.size() || (yStart+y) >= containerLayout[0].size()){
					return false;
				}
				free = free && (containerLayout[xStart+x][yStart+y] == NULL);
			}
		}
		return free;
	}

	
	bool Inventory::addItem(Item* item){

		ItemList allItems = getAllItems();
		
		ItemList::iterator itemIterator = allItems.begin();
		// Durchlaufe alle Container, und schau, ob Platz für das Item ist.
		// TODO: Fix Itemtypen
		while (itemIterator != allItems.end()){
			Item* currentItem = *itemIterator++;
			if (currentItem->isContainer() && addItemToContainer(item, currentItem))
				return true;	
		}
		return false;
	}

	// TODO
	void Inventory::addWeapon(Weapon* weapon)
	{
		WeaponMap::const_iterator it = mWeapons.find(weapon->getId());
		if (it != mWeapons.end())
		{
			Throw(IllegalArgumentException, 
				"weaponId bereits in mWeapons enthalten.");
		}
		mWeapons.insert(make_pair(weapon->getId(), weapon));
		
		
		// Ueberpruefe ob in der Waffenhand etwas ist
		if (mHandRight) {
			// Lege die Waffe in den Rucksack
			addItem(weapon);
		}
		else 
		{
			setHandRight(weapon);
		}
	}

	// TODO
	Weapon* Inventory::getWeapon(int weaponId)
	{
		WeaponMap::const_iterator it = mWeapons.find(weaponId);
		if (it == mWeapons.end())
		{
			Throw(IllegalArgumentException, "weaponId nicht in mWeapons gefunden.");
		}
		return (*it).second;	
	}

	// TODO
	Weapon* Inventory::removeWeapon(int weaponId)
	{
		WeaponMap::iterator it = mWeapons.find(weaponId);
		if (it == mWeapons.end())
		{
			Throw(IllegalArgumentException, "weaponId nicht in mWeapons gefunden.");
		}
		Weapon* rval = (*it).second;
		mWeapons.erase(it);
		return rval;
	}

	// TODO
	void Inventory::switchToWeapon(int weaponId){

		WeaponMap::iterator it = mWeapons.find(weaponId);
		if (it == mWeapons.end())
		{
			Throw(IllegalArgumentException, "weaponId nicht in mWeapons gefunden.");
		}
		
		Weapon* weapon = (*it).second;

		if (mHandRight && mHandRight != weapon){
			// Alte Waffe wegnehmen
			Item* oldWeapon = removeHandRight();
			setHandRight(weapon);
			addItem(oldWeapon);

		}

	}


	Item* Inventory::getRingLeft()
	{
		return mRingLeft;
	}
	
	Item* Inventory::getRingRight()
	{
		return mRingRight;
	}

	Item* Inventory::getHandLeft()
	{
		return mHandLeft;
	}

	Item* Inventory::getHandRight()
	{
		return mHandRight;
	}

	Item* Inventory::getGloves()
	{
		return mGloves;
	}

	Item* Inventory::getBraceletLeft()
	{
		return mBraceletLeft;
	}
	
	Item* Inventory::getBraceletRight()
	{
		return mBraceletRight;
	}
	
	Item* Inventory::getArmor()
	{
		return mArmor;
	}
	
	Item* Inventory::getCape()
	{
		return mCape;
	}

	Item* Inventory::getBracers()
	{
		return mBracers;
	}
	
	Item* Inventory::getBackpack()
	{
		return mBackpack;
	}
	
	Item* Inventory::getBelt()
	{
		return mBelt;
	}
	
	Item* Inventory::getNecklace()
	{
		return mNecklace;
	}

	Item* Inventory::getHelmet()
	{
		return mHelmet;
	}
	
	Item* Inventory::getTrousers()
	{
		return mTrousers;
	}

	Item* Inventory::getShinbone()
	{
		return mShinbone;
	}
	
	Item* Inventory::getBoots()
	{
		return mBoots;
	}

	Item* Inventory::removeRingLeft()
	{
		Item* retItem = mRingLeft;
		mRingLeft = NULL;
		return retItem;
	}

	Item* Inventory::removeRingRight()
	{
		Item* retItem = mRingRight;
		mRingRight = NULL;
		return retItem;
	}

	Item* Inventory::removeHandLeft(){
		Item* temp = mHandLeft;
		mHandLeft = NULL;
		return temp;
	}
	Item* Inventory::removeHandRight(){
		if (mHandRight && mHandRight->getActor())
		{
			mOwner->detachWeapon();
		}
		Item* temp = mHandRight;
		mHandRight = NULL;
		return temp;
	}
	Item* Inventory::removeGloves(){
		Item* temp = mGloves;
		mGloves = NULL;
		return temp;
	}
	Item* Inventory::removeBraceletLeft(){
		Item* temp = mBraceletLeft;
		mBraceletLeft = NULL;
		return temp;
	}
	Item* Inventory::removeBraceletRight(){
		Item* temp = mBraceletRight;
		mBraceletRight = NULL;
		return temp;
	}
	Item* Inventory::removeArmor(){
		Item* temp = mArmor;
		mArmor = NULL;
		return temp;
	}
	Item* Inventory::removeCape(){
		Item* temp = mCape;
		mCape = NULL;
		return temp;
	}
	Item* Inventory::removeBracers(){
		Item* temp = mBracers;
		mBracers = NULL;
		return temp;
	}
	Item* Inventory::removeBackpack(){
		Item* temp = mBackpack;
		mBackpack = NULL;
		return temp;
	}
	Item* Inventory::removeBelt(){
		Item* temp = mBelt;
		mBelt = NULL;
		return temp;
	}
	Item* Inventory::removeNecklace(){
		Item* temp = mNecklace;
		mNecklace = NULL;
		return temp;
	}
	Item* Inventory::removeHelmet(){
		Item* temp = mHelmet;
		mHelmet = NULL;
		return temp;
	}

	Item* Inventory::removeTrousers(){
		Item* temp = mTrousers;
		mTrousers = NULL;
		return temp;
	}
	Item* Inventory::removeShinbone(){
		Item* temp = mShinbone;
		mShinbone = NULL;
		return temp;
	}
	Item* Inventory::removeBoots(){
		Item* temp = mBoots;
		mBoots = NULL;
		return temp;
	}

	void Inventory::setRingLeft(Item* item)
	{
		mRingLeft = item;
	}
	void Inventory::setRingRight(Item* item)
	{
		mRingRight = item;
	}
	void Inventory::setHandLeft(Item* item)
	{
		mHandLeft = item;
	}
	void Inventory::setHandRight(Item* item)
	{
		if (item && item->getActor() && (dynamic_cast<Weapon*>(item) != 0)){
			mOwner->attachWeapon(dynamic_cast<Weapon*>(item));
		}
		mHandRight = item;
	}
	void Inventory::setGloves(Item* item)
	{
		mGloves = item;
	}
	void Inventory::setBraceletLeft(Item* item)
	{
		mBraceletLeft = item;
	}
	void Inventory::setBraceletRight(Item* item)
	{
		mBraceletRight = item;
	}
	void Inventory::setArmor(Item* item)
	{
		mArmor = item;
	}
	void Inventory::setCape(Item* item)
	{
		mCape = item;
	}
	void Inventory::setBracers(Item* item)
	{
		mBracers = item;
	}
	void Inventory::setBackpack(Item* item)
	{
		mBackpack = item;
	}
	void Inventory::setBelt(Item* item)
	{
		mBelt = item;
	}
	void Inventory::setNecklace(Item* item)
	{
		mNecklace = item;
	}
	void Inventory::setHelmet(Item* item)
	{
		mHelmet = item;
	}
	void Inventory::setTrousers(Item* item)
	{
		mTrousers = item;
	}
	void Inventory::setShinbone(Item* item)
	{
		mShinbone = item;
	}
	void Inventory::setBoots(Item* item)
	{
		mBoots = item;
	}
}
