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

	vector<Item*> getAllItems();

	set<Item*> getAllItemsInBackpack();

	vector<vector<Item*> > getBackpackLayout();

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

	bool isFreeInBackpack(pair<int,int> posKaestchen, pair<int,int> dimKaestchen);

	void addWeapon(Weapon* weapon);

	Weapon* getWeapon(int weaponId);
	Weapon* removeWeapon(int weaponId);
	void switchToWeapon(int weaponId);

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
	
	//Armschiene links
	Item* getBracerLeft();
	//Armschiene rechts
	Item* getBracerRight();
	

	//Oberkoerper Ruecken (Rucksack, Schwertscheiden)
	Item* getBackpack();
	
	//Oberkoerper Front (MesserGuertelquer, Seil)
	//Item* getArmorFront();
	
	//Guertel  (+Slots fuer Guerteltaschen+Scheiden)
	Item* getBelt();
	//Halskette
	Item* getNecklace();
	//Stirnband, Helm, Diadem
	Item* getHelmet();
	
	//Schritt (Prinz Albert, Unterleibsschutz)
	//Item* getCrotch();
	
	//Hose
	Item* getTrousers();
	//Schienbein links
	Item* getShinboneLeft();
	//Schienbein rechts
	Item* getShinboneRight();
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
	Item* removeBracerLeft();
	Item* removeBracerRight();
	Item* removeBackpack();
	//Item* removeArmorFront();
	Item* removeBelt();
	Item* removeNecklace();
	Item* removeHelmet();
	//Item* removeCrotch();
	Item* removeTrousers();
	Item* removeShinboneLeft();
	Item* removeShinboneRight();
	Item* removeBoots();

	void setRingLeft(Item* item);
	void setRingRight(Item* item);
	void setHandLeft(Item* item);
	void setHandRight(Item* item);
	void setGloves(Item* item);
	void setBraceletLeft(Item* item);
	void setBraceletRight(Item* item);
	void setBracerLeft(Item* item);
	void setBracerRight(Item* item);
	void setArmor(Item* item);
	void setBackpack(Item* item);
//	void setArmorFront(Item* item);
	void setBelt(Item* item);
	void setNecklace(Item* item);
	void setHelmet(Item* item);
//	void setCrotch(Item* item);
	void setTrousers(Item* item);
	void setShinboneLeft(Item* item);
	void setShinboneRight(Item* item);
	void setBoots(Item* item);

private:
	Item* mRingLeft;
	Item* mRingRight;
	Item* mHandLeft;
	Item* mHandRight;
	Item* mGloves;
	Item* mBraceletLeft;
	Item* mBraceletRight;
	Item* mArmor;
	Item* mBracerLeft;
	Item* mBracerRight;
	Item* mBackpack;
//	Item* mArmorFront;
	Item* mBelt;
	Item* mNecklace;
	Item* mHelmet;
//	Item* mCrotch;
	Item* mTrousers;
	Item* mShinboneLeft;
	Item* mShinboneRight;
	Item* mBoots;

	vector< vector<Item*> > mBackpackLayout;
	pair<int,int> mBackpackDimension;
};

}
#endif


