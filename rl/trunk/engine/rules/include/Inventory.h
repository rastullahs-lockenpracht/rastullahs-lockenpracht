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

#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "Item.h"
#include "Weapon.h"
#include "Container.h"
#include "RulesPrerequisites.h"



namespace rl {
class _RlRulesExport Inventory;
class _RlRulesExport Creature;

/**
 * @brief Verwaltet das Inventar des Charakters
 */
class _RlRulesExport Inventory
{
public:
///////////////////////////////////////////////////////////////////////////////
// Typedefs

	/** @brief Eine Map mit Zeigern auf alle Waffen, die die Kreatur zur
	*   Zeit einsetzen kann.
	**/
	typedef map<int, Weapon*> WeaponMap;

	/** 
	* @brief Eine Liste mit Zeigern auf Items
	**/
	typedef std::list<Item*> ItemList;

	Inventory(Creature* owner);
	virtual ~Inventory();

///////////////////////////////////////////////////////////////////////////////
// Methoden

	/**
	* Liefert alle Items im Inventar inm einer Liste
	* Wichtig:
	* NUR die erste Hierarchieebene der Items wird zurückgegeben
	* Was in den Items drinnen ist, ist vernachlaessigt
	*/
	ItemList getAllItems();


	/**
	* Liefert die gesamte Last des Inventars in Unzen
	*/
	int getOverallWeight();

	/**
	 * @return Die errechnete Behinderung
	 **/
	pair<int,int> getOverallBe();

	/**
	* @return die gesamte Rüstung des Chars
	*/
	int getOverallRs();

	/**
	 * @brief fügt das Item dem Inventar hinzu, wenn Platz ist,
	 * gibt sonst false zurück
	 * @param item Das Item, dass aufgenommen werden soll
	 **/
	bool addItem(Item* item);

	bool addItemToContainer(Item* item, Item* container);

	/**
	* Durchsucht alle Container und entfernt das Item daraus 
	*/
	void removeItemFromContainers(Item* item);

	/**
	* Durchsucht einen Container und entfernt das Item daraus
	*/
	void removeItemFromContainer(Item* item, Item* container);

	bool isFreeInContainer(Item* item, pair<int,int> posKaestchen, Item* container);

	/**
	* Liefert die Position und das Containeritem, das den Gegenstand trägt/beinhaltet
	*/
	pair<pair<int,int>, Item*> getItemPositionInContainer(Item* item);

	void addWeapon(Weapon* weapon);

	Weapon* getWeapon(int weaponId);
	Weapon* removeWeapon(int weaponId);
	void switchToWeapon(int weaponId);

	void setItemContainerPosition(Item* item, int xPosDraggedTo, int yPosDraggedTo, Item* container);

	void removeItem(Item* item);

	//Ring links
	Item* getRingLeft();
	//Ring rechts
	Item* getRingRight();
	//in Hand links
	Item* getHandLeft();
	//in Hand rechts
	Item* getHandRight();
	//Handschuhe
	Item* getGloves();
	//Armreif links
	Item* getBraceletLeft();
	//Armreif rechts
	Item* getBraceletRight();
	//Oberkoerper Ruestung
	Item* getArmor();
	//Umhang
	Item* getCape();
	//Armschienen
	Item* getBracers();
	//Oberkoerper Ruecken (Rucksack, Schwertscheiden)
	Item* getBackpack();
	//Guertel  (+Slots fuer Guerteltaschen+Scheiden)
	Item* getBelt();
	//Halskette
	Item* getNecklace();
	//Stirnband, Helm, Diadem
	Item* getHelmet();
	//Hose
	Item* getTrousers();
	//Beinschienen
	Item* getShinbone();
	//Stiefel
	Item* getBoots();

	Item* removeRingLeft();
	Item* removeRingRight();
	Item* removeHandLeft();
	Item* removeHandRight();
	Item* removeGloves();
	Item* removeBraceletLeft();
	Item* removeBraceletRight();
	Item* removeArmor();
	Item* removeCape();
	Item* removeBracers();
	Item* removeBackpack();
	Item* removeBelt();
	Item* removeNecklace();
	Item* removeHelmet();
	Item* removeTrousers();
	Item* removeShinbone();
	Item* removeBoots();

	void setRingLeft(Item* item);
	void setRingRight(Item* item);
	void setHandLeft(Item* item);
	void setHandRight(Item* item);
	void setGloves(Item* item);
	void setBraceletLeft(Item* item);
	void setBraceletRight(Item* item);
	void setBracers(Item* item);
	void setArmor(Item* item);
	void setCape(Item* item);
	void setBackpack(Item* item);
	void setBelt(Item* item);
	void setNecklace(Item* item);
	void setHelmet(Item* item);
	void setTrousers(Item* item);
	void setShinbone(Item* item);
	void setBoots(Item* item);

    		/**
		 *  Gibt der Spielfigur eine Waffe in die Hand. Wird gesteuert durch 
         *  das Inventar.
		 *  @param weapon Waffe, die in die Hand soll
		 */
		void attachWeapon(Weapon* weapon);

		/**
		 *  Entfernt der Kreatur die Waffe aus der Hand.
		 **/
		void detachWeapon();

// Das Inventar der Kreatur wird durch das Inventarobjekt verwaltet.
        void addToInventory(Item* item, const CeGuiString& containerName);
        void hold(Item* item, const CeGuiString& slotName);
        void ready(Item* item);
		
		/**
		 *  Heftet Container an die Kreatur.
		 *  @param container Ein Zeiger auf den hinzuzufuegenden Container.
		 *  @throws InvalidArgumentException Nullpointer uebergeben.
		 *  @throws InvalidArgumentException Container bereits in
		 *   \c mContainer enthalten.
         *  @ingroup CreatureRubyExports
		 **/
		void addContainer(Container* container);
		/**
		 *  Liefert den Container \a containerName zurueck.
		 *  @param containerName Bezeichnet den Container in mContainer.
		 *  @return Liefert einen Zeiger auf den Container.
		 *  @throws InvalidArgumentException Der Container wurde nicht
		 *  in \c mContainer gefunden.
         *  @ingroup CreatureRubyExports
		 **/
		Container* getContainer(const CeGuiString containerName) const;
		/**
		 *  Entfernt den Container containerName. Der Container wird aus 
         *  \c mContainer geloescht und ein Zeiger darauf zurueckgegeben.
		 *  @param containerName Bezeichnet den Container in \c mContainer.
		 *  @throws InvalidArgumentException Container nicht in
		 *   \c mContainer gefunden.
         *  @ingroup CreatureRubyExports
		 **/
		Container* removeContainer(const CeGuiString containerName);


private:
	bool mValuesUpToDate;
	int mCurrentWeight;
	int mCurrentBeByWeight;
	int mCurrentBe;
	int mCurrentRs;

	WeaponMap mWeapons;

	Creature* mOwner;

	Item* mRingLeft;
	Item* mRingRight;
	Item* mHandLeft;
	Item* mHandRight;
	Item* mGloves;
	Item* mBraceletLeft;
	Item* mBraceletRight;
	Item* mArmor;
	Item* mCape;
	Item* mBracers;
	Item* mBackpack;
	Item* mBelt;
	Item* mNecklace;
	Item* mHelmet;
	Item* mTrousers;
	Item* mShinbone;
	Item* mBoots;


	/**
	* Setzt Flag, dass die Werte neu berechnet werden müssen
	*/
	void markDirty();

	/** @brief Liefert die nächste freie Position für den Gegenstand zurueck.
	 *  @param space Die Ausmaße des Gegenstandes.
	 *  @return Die Anfangsposition des Platzesm an den das Item gesetzt werden kann.
	 *  @exception IllegalStateException Es gibt nicht genug Platz für das Item.
	 **/
	pair<int,int> findPositionWithEnoughSpace(pair<int,int> space, ContainerLayout containerLayout);

	/**
	* Überprüft die Stelle im Inventar auf genug Platz für ein Item der Größe space
	*/
	bool checkSpace(int xStart, int yStart, pair<unsigned int,unsigned int> space, ContainerLayout container);

	/**
	* Fügt alle Items in einem Container rekursiv der Liste hinzu
	*/
	void addContainerItemsToList(Item &container, ItemList &itemList);

	/**
	* Erzeugt eine Liste aus Items, die am Körper getragen werden
	*/
	ItemList getWornItems();

	/**
	* Erzeugt eine Liste aus Containern, die der Character mit sich trägt
	*/
	ItemList getAllContainers();


	/**
	* Fügt alle Container in den Items rekursiv zur Liste hinzu
	*/
	void addContainersToList(Item &container, ItemList &itemList);

	/**
	* Update der Werte für Gewicht, Rs und Behinderung
	*/
	void updateStats();

	/**
	* Berechnet das Gewicht und aktualisiert mCurrentWeight
	*/
	void calculateWeight(ItemList allItems);
	
	/**
	* @pre: calculateWeight() wurde zuvor aufgerufen
	* @post: berechnet die Behinderung (mCurrentRs und mCurrentBe werden aktualisiert)
	*/
	void calculateRsAndBe();



};

}
#endif


