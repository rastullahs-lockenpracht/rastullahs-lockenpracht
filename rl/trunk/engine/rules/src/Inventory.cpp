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

using namespace std;

namespace rl
{

	Inventory::Inventory() 
	{
	}

	Inventory::~Inventory() 
	{
		if (mBackpack){
			delete mBackpack;
		}
	}

	vector<Item*> Inventory::getAllItems()
	{
		vector<Item*> is(1);
		is[0] = mRingLeft;
		return is;
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
	
	Item* Inventory::getBackpack()
	{
		return mBackpack;
	}
	
	Item* Inventory::getArmorFront()
	{
		return mArmorFront;
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
	
	Item* Inventory::getCrotch()
	{
		return mCrotch;
	}
	
	Item* Inventory::getTrousers()
	{
		return mTrousers;
	}

	Item* Inventory::getShinboneLeft()
	{
		return mShinboneLeft;
	}
	
	Item* Inventory::getShinboneRight()
	{
		return mShinboneRight;
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
		return NULL;
	}
	Item* Inventory::removeHandRight(){
		return NULL;
	}
	Item* Inventory::removeGloves(){
		return NULL;
	}
	Item* Inventory::removeBraceletLeft(){
		return NULL;
	}
	Item* Inventory::removeBraceletRight(){
		return NULL;
	}
	Item* Inventory::removeArmor(){
		return NULL;
	}
	Item* Inventory::removeBackpack(){
		return NULL;
	}
	Item* Inventory::removeArmorFront(){
		return NULL;
	}
	Item* Inventory::removeBelt(){
		return NULL;
	}
	Item* Inventory::removeNecklace(){
		return NULL;
	}
	Item* Inventory::removeHelmet(){
		return NULL;
	}
	Item* Inventory::removeCrotch(){
		return NULL;
	}
	Item* Inventory::removeTrousers(){
		return NULL;
	}
	Item* Inventory::removeShinboneLeft(){
		return NULL;
	}
	Item* Inventory::removeShinboneRight(){
		return NULL;
	}
	Item* Inventory::removeBoots(){
		return NULL;
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
	void Inventory::setBackpack(Item* item)
	{
		mBackpack = item;
	}
	void Inventory::setArmorFront(Item* item)
	{
		mArmorFront = item;
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
	void Inventory::setCrotch(Item* item)
	{
		mCrotch = item;
	}
	void Inventory::setTrousers(Item* item)
	{
		mTrousers = item;
	}
	void Inventory::setShinboneLeft(Item* item)
	{
		mShinboneLeft = item;
	}
	void Inventory::setShinboneRight(Item* item)
	{
		mShinboneRight = item;
	}
	void Inventory::setBoots(Item* item)
	{
		mBoots = item;
	}
}
