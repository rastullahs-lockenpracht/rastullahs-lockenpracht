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
#include "Inventory.h"
#include "Exception.h"

using namespace std;

namespace rl
{

	Inventory::Inventory() :
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
		mBackpackLayout = ContainerLayout(12);

		for (unsigned z = 0; z< mBackpackLayout.size(); z++){
			ContainerColumn temp = ContainerColumn(7);
			mBackpackLayout[z] = temp;
		}
		
		for (unsigned int x = 0; x < mBackpackLayout.size(); x++){
			for (unsigned int y = 0; y < mBackpackLayout[0].size(); y++){
				mBackpackLayout[x][y] = NULL;
			}
		}
		

		Item* meineRuestung = createItem("Kroetenhaut", "Gefertigt aus Rindsleder bietet diese zusaetzlich durch Nieten verstaerkte Ruestung guten Schutz fuer den Abenteurer","Kroetenhaut",Item::ITEMTYPE_ARMOR,pair<int,int>(3,4));
		
		Item* meinTrank = createItem("meinTrank", "Erhöht die Vitalität","Trank", Item::ITEMTYPE_OTHER, pair<int,int>(1,1));

		Item* meineHandschuhe = createItem("meineHandschuhe", "Wärmen im Winter","Handschuhe",Item::ITEMTYPE_GLOVES,pair<int,int>(2,2));

		Item* meinArmreif = createItem("meinArmreif", "Verzaubert vom Shamanen Murgul","Armreif",Item::ITEMTYPE_BRACELET, pair<int,int>(1,1));

		Item* meineStiefel = createItem("meineStiefel", "Einfache Lederstiefel","Stiefel",Item::ITEMTYPE_BOOTS,pair<int,int>(2,2));
		
		Item* meinUmhang = createItem("meinUmhang", "Fellumhang von den Nivesen geklaut","Fellumhang",Item::ITEMTYPE_CAPE,pair<int,int>(2,4));
		
		Item* meinRing = createItem("meinRing","Schön gearbeiteter Ring","Rubinring",Item::ITEMTYPE_RING,pair<int,int>(1,1));

		Item* meinRing2 = createItem("meinRing2","Schön gearbeiteter Ring","Rubinring",Item::ITEMTYPE_RING,pair<int,int>(1,1));

		addItemToBackpack(meinTrank);
		addItemToBackpack(meineHandschuhe);
		addItemToBackpack(meinArmreif);
		addItemToBackpack(meineStiefel);
		addItemToBackpack(meinUmhang);
		addItemToBackpack(meinRing2);


		mRingLeft = meinRing;
		mArmor = meineRuestung;
	}

	Inventory::~Inventory() 
	{
		// Lösche Alle Objekte aus dem Rucksack
		for (unsigned int x = 0; x < mBackpackLayout.size(); x++){
			for (unsigned int y = 0; y < mBackpackLayout[0].size(); y++){
				if (mBackpackLayout[x][y] != NULL){
					delete mBackpackLayout[x][y];
				}
			}
		}
	}

	
	vector<Item*> Inventory::getAllItems()
	{
		vector<Item*> is(1);
		is[0] = mRingLeft;
		return is;
	}

	Item* Inventory::createItem(const CeGuiString name, const CeGuiString description, const CeGuiString imageName, Item::ItemType type, pair<int,int> size)
	{
		Item* item = new Item(name, description);
		item->setImageName(imageName);
		item->setItemType(type);
		item->setSize(size.first,size.second);

		return item;
	}

	// TODO: Anordnung der Items
	set<Item*> Inventory::getAllItemsInBackpack()
	{
		set<Item*> items;
		
		for (int x = 0; x < mBackpackDimension.first; x++){
			for(int y = 0; y < mBackpackDimension.second; y++){
				items.insert(mBackpackLayout[x][y]);
			}
		}
		return items;
	}
	

	bool Inventory::isFreeInBackpack(Item* item, pair<int,int> posKaestchen)
	{
		char buf1[5], buf2[5];

		int xSize = item->getSize().first;
		int ySize = item->getSize().second;

		bool free = true;
		for (int x = posKaestchen.first; x < (posKaestchen.first + xSize); x++){
			for (int y = posKaestchen.second; y < (posKaestchen.second + ySize); y++){
				
				Logger::getSingletonPtr()->log("InventoryWindow",Logger::LL_MESSAGE,
					CEGUI::String("Checking Point in Backpack: Point x:").append(itoa(x,buf1,10))
				.append(", Point y:")
				.append(itoa(y,buf2,10)));
				if (x >= int(mBackpackLayout.size()) || y >= int(mBackpackLayout[0].size())){
					// Es wird versucht, das Item außerhalb des Containers zu platzieren
					free = false;
				} else if (mBackpackLayout[x][y] == item) {
					// kein Problem, item wird nur ein bisschen verschoben
				} else {
					// Siehe nach, ob ein anderes Item im Weg ist
					free = free && (!mBackpackLayout[x][y]);
				}
			}
		}
		Logger::getSingletonPtr()->log("InventoryWindow",Logger::LL_MESSAGE,
			CEGUI::String("returning:").append(itoa(free,buf1,10)));
		
		return free;
	}

	void Inventory::setItemBackpackPosition(Item* item, int xPos, int yPos)
	{
		int xSize = item->getSize().first;
		int ySize = item->getSize().second;
		
		// Itempointer auf neuer Position eintragen
		for (int x = 0; x < xSize; x++){
			for (int y = 0; y < ySize; y++) {
				mBackpackLayout[xPos+x][yPos+y] = item;
			}
		}
	}

	void Inventory::removeItemFromSlots(Item* item){

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

				Logger::getSingletonPtr()->log("Inventory",Logger::LL_MESSAGE,"Checking Gloves..");
				if (getGloves() == item){
					Logger::getSingletonPtr()->log("Inventory",Logger::LL_MESSAGE,"Gloves exist. trying to remove them..");
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
	}
	


	void Inventory::removeItemFromBackpack(Item* item)
	{
		for (unsigned int x = 0; x < mBackpackLayout.size(); x++){
			for (unsigned int y = 0; y < mBackpackLayout[0].size(); y++){
				if (mBackpackLayout[x][y] == item){
					mBackpackLayout[x][y] = NULL;
				}
			}
		}
	}

	void Inventory::addItemToBackpack(Item* item) {
		// Platz suchen
		try {
			if (item->getItemType() == Item::ITEMTYPE_BACKPACK) {
				// Ein Rucksack soll auch auf den Boden, wenn Platz ist
				Throw(IllegalStateException, "Ein Rucksack kann nicht in den Rucksack gepackt werden!");
			}
			pair<int,int> pos = findPositionWithEnoughSpace(item->getSize());
			// Platz gefunden, platziere Item:
			for (int x = pos.first; x < pos.first+item->getSize().first; x++){
				for (int y = pos.second; y < pos.second+item->getSize().second; y++){
					mBackpackLayout[x][y] = item;
				}
			}
		} catch (IllegalStateException ie){
			// Kein Platz mehr im Rucksack... 
			// TODO: Auf den Boden legen
		}
	}


	pair<int,int> Inventory::findPositionWithEnoughSpace(pair<int,int> space){
		pair<int,int> pos;
		for (unsigned int x = 0; x < mBackpackLayout.size(); x++){
			for (unsigned int y = 0; y < mBackpackLayout[0].size(); y++){
				if (mBackpackLayout[x][y] == NULL && checkSpace(x,y,space)){
					pos = pair<int,int>(x,y);
					return pos;
				}
			}
		}
		Throw(IllegalStateException, "Rucksack hat keinen Platz für das Item");
	}

	bool Inventory::checkSpace(int xStart, int yStart, pair<int,int> space){
		bool free = true;
		for (unsigned int x = 0; x < space.first; x++){
			for (unsigned int y = 0; y < space.second; y++){
				// Falls Kästchen nicht mehr im Rucksack, ist auch kein Platz mehr :)
				if ((xStart+x) >= mBackpackLayout.size() || (yStart+y) >= mBackpackLayout[0].size()){
					return false;
				}
				free = free && (mBackpackLayout[xStart+x][yStart+y] == NULL);
			}
		}
		return free;
	}

	// TODO: Be berechnen
	int Inventory::getCurrentBe()
	{
		return 0;
	}

	vector< vector<Item*> > Inventory::getBackpackLayout(){
		return mBackpackLayout;
	}

	// TODO: Item hinzufügen, nachdem Platz gesichert ist
	bool Inventory::addItem(Item* item){
		return false;
	}

	// TODO
	void Inventory::addWeapon(Weapon* weapon)
	{
		
	}

	// TODO
	Weapon* Inventory::getWeapon(int weaponId)
	{
		return NULL;
	}

	// TODO
	Weapon* Inventory::removeWeapon(int weaponId)
	{
		return NULL;
	}

	// TODO
	void Inventory::switchToWeapon(int weaponId){

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

	void Inventory::initSlots(){

	}
}
