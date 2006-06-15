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

/**
 * @brief Verwaltet das Inventar des Charakters
 */
class _RlRulesExport Inventory
{
public:
	Inventory();
	virtual ~Inventory();

	/**
	* Liefert alle Items im Inventar inm einer Liste
	* Wichtig:
	* NUR die erste Hierarchieebene der Items wird zurückgegeben
	* Was in den Items drinnen ist, ist vernachlaessigt
	*/
	std::list<Item*> getAllItems();

	/**
	 * @return Die errechnete Behinderung
	 **/
	int getCurrentBe();

	/**
	 * @brief fügt das Item dem Inventar hinzu, wenn Platz ist,
	 * gibt sonst false zurück
	 * @param item Das Item, dass aufgenommen werden soll
	 **/
	bool addItem(Item* item);

	void addItemToContainer(Item* item, Item* container);

	/**
	* Durchsucht alle Container und entfernt das Item daraus 
	*/
	void removeItemFromContainers(Item* item);

	/**
	* Durchsucht einen Container und entfernt das Item daraus
	*/
	void removeItemFromContainer(Item* item, Item* container);

	bool isFreeInContainer(Item* item, pair<int,int> posKaestchen, Item* container);

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

private:

	/** @brief Liefert die nächste freie Position für den Gegenstand zurueck.
	 *  @param space Die Ausmaße des Gegenstandes.
	 *  @return Die Anfangsposition des Platzesm an den das Item gesetzt werden kann.
	 *  @exception IllegalStateException Es gibt nicht genug Platz für das Item.
	 **/
	pair<int,int> findPositionWithEnoughSpace(pair<int,int> space, ContainerLayout containerLayout);

	/**
	* Überprüft die Stelle im Inventar auf genug Platz für ein Item der Größe space
	*/
	bool checkSpace(int xStart, int yStart, pair<int,int> space, ContainerLayout container);

	/**
	* Erzeugt ein Item mit den angegebenen Parametern
	* @param: name Der Name des Items
	* @param: description Die Beschreibung des Gegenstandes
	* @param: imageName Der Name des Bildes, das dem Item zugeordnet werden soll
	* @param: type Der Itemtyp
	* @param: size die Groesse des Gegenstandes im Inventar
	*
	* @return: Item* der Zeiger auf das neue Item
	*/
	Item* createItem(const CeGuiString name, const CeGuiString description, const CeGuiString imageName, Item::ItemType type, pair<int,int> size);

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
};

}
#endif


