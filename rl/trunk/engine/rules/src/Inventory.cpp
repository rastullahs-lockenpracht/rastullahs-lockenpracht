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
#include "Slot.h"

using namespace std;

namespace rl
{

    /**
       Just to remember all parts of the inventory

	//Ring links
	//Ring rechts
	//in Hand links
	//in Hand rechts
	//Handschuhe
	//Armreif links
	//Armreif rechts
	//Oberkoerper Ruestung
	//Umhang
	//Armschienen
	//Oberkoerper Ruecken (Rucksack, Schwertscheiden)
	//Guertel  (+Slots fuer Guerteltaschen+Scheiden)
	//Halskette
	//Stirnband, Helm, Diadem
	//Hose
	//Beinschienen
	//Stiefel

    */


	Inventory::Inventory(Creature* owner) :
		mOwner(owner),
        mSlots(),
		mWeapons(),
		mCurrentWeight(0),
		mCurrentBeByWeight(0),
		mCurrentBe(0),
		mCurrentRs(0),
		mValuesUpToDate(false)
	{
	}

	Inventory::~Inventory() 
	{
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
			(mCurrentWeight / (mOwner->getEigenschaft(E_KOERPERKRAFT) * 20) - 1) :
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

	bool Inventory::isFreeInContainer(Item* item, pair<int,int> posKaestchen, Item* container)
	{
		int xSize = item->getSize().first;
		int ySize = item->getSize().second;

		bool free = true;
		for (int x = posKaestchen.first; x < (posKaestchen.first + xSize); x++){
			for (int y = posKaestchen.second; y < (posKaestchen.second + ySize); y++){
				
				LOG_MESSAGE("InventoryWindow",
					Ogre::String("Checking Point in Backpack: Point x:")
					+ Ogre::StringConverter::toString(x) 
					+ ", Point y:"
					+ Ogre::StringConverter::toString(y));
				
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

	void Inventory::removeItem(Item* item)
    {

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
            LOG_ERROR(Logger::RULES, "Inventory::addItemToContainer fehgeschlagen.");
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

	bool Inventory::checkSpace(int xStart, int yStart, pair<unsigned int,unsigned int> space, ContainerLayout containerLayout){
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
	void Inventory::switchToWeapon(int weaponId)
    {

    }

    void Inventory::addToInventory(Item* item, const CeGuiString& containerName)
    {
    }

    void Inventory::hold(Item* item, const CeGuiString& slotName)
    {
        std::map<CeGuiString, Slot*>::iterator slotIter = mSlots.find(slotName);
        if (slotIter == mSlots.end())
        {
            Throw(rl::IllegalArgumentException, Ogre::String("Slot '")+slotName.c_str()+"' doesn't exist.");
        }
        
        (*slotIter).second->setItem(item);
    }

    Item* Inventory::getItem(const CeGuiString& slotName) const
    {
        return NULL;
    }

    void Inventory::ready(Item* item)
    {
    }

    void Inventory::addSlot(const CeGuiString& name, const Ogre::String& bone, int itemMask)
    {
        if (mSlots.find(name) != mSlots.end())
        {
            Throw(rl::IllegalArgumentException, Ogre::String("Slot '")+name.c_str()+"' already exists.");
        }
        mSlots[name] = new Slot(mOwner, name, bone, itemMask);
    }

}
