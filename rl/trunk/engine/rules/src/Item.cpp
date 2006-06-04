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

using namespace std;

namespace rl
{
    Item::Item(const CeGuiString& name, const CeGuiString& description)
        : GameObject(name, description),
		mWeight(0),
		mItemType(ITEMTYPE_OTHER),
		mImageName(""),
		mCapacity(pair<int,int>(1,1)),
		mSize(pair<int,int>(1,1))
    {
    }

	/*Item::Item(const CeGuiString& name, const CeGuiString& description, CEGUI::String imageName, ItemType type, pair<int,int> size)
		: GameObject(name, description)
	{
		mCapacity = pair<int,int> (1,1);
		boolContainer = false;

		mItemType = type;
		mImageName = imageName;
		mSize = size;
	}*/

    Item::~Item(void)
    {
    }

    void Item::setWeight(int weight)
    {
        mWeight = weight;
    }

    int Item::getWeight() const
    {
        return mWeight;
    }

	void Item::setItemType(ItemType itemType)
	{
		mItemType = itemType;
	}

	Item::ItemType Item::getItemType() const
	{
		return mItemType;
	}

	void Item::setImageName(const CeGuiString& name)
	{
		mImageName = name;
	}

	const CeGuiString& Item::getImageName()
	{
		return mImageName;
	}


	const CeGuiString Item::getItemTypeString(ItemType type){
		switch(type){
			case ITEMTYPE_WEAPON:
				return CeGuiString("Weapon");
				break;
			case ITEMTYPE_SHIELD:
				return CeGuiString("Shield");
				break;
			case ITEMTYPE_RING:
				return CeGuiString("Ring");
				break;
			case ITEMTYPE_GLOVES:
				return CeGuiString("Glove");
				break;
			case ITEMTYPE_BRACELET:
				return CeGuiString("Bracelet");
				break;
			case ITEMTYPE_ARMOR:
				return CeGuiString("Armor");
				break;
			case ITEMTYPE_CAPE:
				return CeGuiString("Cape");
				break;
			case ITEMTYPE_BRACERS:
				return CeGuiString("Bracer");
				break;
			case ITEMTYPE_BACKPACK:
				return CeGuiString("Backpack");
				break;
			case ITEMTYPE_BELT:
				return CeGuiString("Belt");
				break;
			case ITEMTYPE_NECKLACE:
				return CeGuiString("Necklace");
				break;
			case ITEMTYPE_HELMET:
				return CeGuiString("Helmet");
				break;
			case ITEMTYPE_TROUSERS:
				return CeGuiString("Trousers");
				break;
			case ITEMTYPE_SHINBONE:
				return CeGuiString("Shinbone");
				break;
			case ITEMTYPE_BOOTS:
				return CeGuiString("Boots");
				break;
			case ITEMTYPE_OTHER:
				return CeGuiString("Other");
				break;
		}
		return CeGuiString("undefined");
	}


	bool Item::isContainer()
	{
		return mIsContainer;
	}
	void Item::setContainer(bool isContainer, pair<int,int> dim)
	{
		mIsContainer = isContainer;
		mCapacity = dim;
	}
	pair<int,int> Item::getCapacity()
	{
		return mCapacity;
	}

	void Item::setCapacity(int widthCapacity,int heightCapacity)
	{
		mCapacity = pair<int,int> (widthCapacity,heightCapacity);
	}

	pair<int,int> Item::getSize()
	{
		return mSize;
	}

	void Item::setSize(int widthSize,int heightSize)
	{
		mSize = pair<int,int>(widthSize,heightSize);
	}
}
