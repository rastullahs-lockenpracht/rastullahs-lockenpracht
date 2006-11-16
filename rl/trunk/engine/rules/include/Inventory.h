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

#include "RulesPrerequisites.h"

#include "Container.h"

namespace rl {

    class Creature;
    class Item;
    class Slot;

/**
 * @brief Verwaltet das Inventar des Charakters
 */
class _RlRulesExport Inventory
{
public:
///////////////////////////////////////////////////////////////////////////////
// Typedefs

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


    // Das Inventar der Kreatur wird durch das Inventarobjekt verwaltet.
    void addToInventory(Item* item, const CeGuiString& containerName);
    void hold(Item* item, const CeGuiString& slotName);
    void ready(Item* item);

    Item* getItem(const CeGuiString& slotName) const;
    void addSlot(const CeGuiString& name, const Ogre::String& bone, int itemMask);

private:
    typedef std::map<CeGuiString, Slot*> SlotMap;

	bool mValuesUpToDate;
	int mCurrentWeight;
	int mCurrentBeByWeight;
	int mCurrentBe;
	int mCurrentRs;

    SlotMap mSlots;

	Creature* mOwner;

	/**
	* Setzt Flag, dass die Werte neu berechnet werden müssen
	*/
	void markDirty();

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


