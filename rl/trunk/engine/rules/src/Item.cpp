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
#include "Exception.h"

using namespace std;

namespace rl
{
    Item::Item(const CeGuiString name, const CeGuiString description)
        : GameObject(name, description),
		mWeight(0),
		mItemType(ITEMTYPE_OTHER),
		mImageName(""),
		mMeshName(""),
		mIsContainer(false),
		mContainerLayout(),
		mCapacity(make_pair<int,int>(0,0)),
		mSize(pair<int,int>(1,1))
    {
    }

	Item::Item(const CeGuiString name, const CeGuiString description, const CeGuiString imageName, const CeGuiString meshName, Item::ItemType type, std::pair<int,int> size)
		: GameObject(name, description),
		mWeight(0),
		mItemType(type),
		mImageName(imageName),
		mMeshName(meshName),
		mIsContainer(false),
		mContainerLayout(),
		mCapacity(make_pair<int,int>(0,0)),
		mSize(size)
	{
	}

    Item::~Item(void)
    {
    }
	
	Item* Item::clone()
	{
		Item* item = new Item(mName, mDescription, mImageName, mMeshName, mItemType, mSize);
		item->setWeight(mWeight);
		item->setContainer(mIsContainer,mCapacity);
		return item;
	}


	CeGuiString Item::getFormattedText(const CeGuiString &unformattedText)
	{
		// Anzahl der Zeichen pro Zeile hässlich, soll später durch Konstante ersetzt werden...
		unsigned maxChars = 40;

		if (unformattedText.length() <= maxChars)
			return unformattedText;
		
		
		int lastPosWithSpace = 0;

		while (unformattedText.find(" ", lastPosWithSpace+1) < 40)
		{
			lastPosWithSpace = unformattedText.find(" ", lastPosWithSpace+1);
		}
		if (lastPosWithSpace > 0) {
			// Es gibt ein gültiges letztes Leerzeichen
			return (unformattedText.substr(0,lastPosWithSpace) + "\r\n" + 
				getFormattedText(unformattedText.substr(lastPosWithSpace+1)));
		}
		else {
			// String an nicht-Leerzeichen zerschneiden
			return (unformattedText.substr(0,maxChars) + "-\r\n" + 
				getFormattedText(unformattedText.substr(maxChars+1)));
		}
	}

	const CeGuiString Item::getItemTypeString(ItemType type){
		switch(type){
			case ITEMTYPE_WEAPON:
				return CeGuiString("Waffe");
				break;
			case ITEMTYPE_SHIELD:
				return CeGuiString("Schild");
				break;
			case ITEMTYPE_RING:
				return CeGuiString("Ring");
				break;
			case ITEMTYPE_GLOVES:
				return CeGuiString("Handschuhe");
				break;
			case ITEMTYPE_BRACELET:
				return CeGuiString("Armreif");
				break;
			case ITEMTYPE_ARMOR:
				return CeGuiString("Rüstung");
				break;
			case ITEMTYPE_CAPE:
				return CeGuiString("Umhang");
				break;
			case ITEMTYPE_BRACERS:
				return CeGuiString("Armschienen");
				break;
			case ITEMTYPE_BACKPACK:
				return CeGuiString("Rucksack");
				break;
			case ITEMTYPE_BELT:
				return CeGuiString("Gürtel");
				break;
			case ITEMTYPE_NECKLACE:
				return CeGuiString("Amulett");
				break;
			case ITEMTYPE_HELMET:
				return CeGuiString("Helm");
				break;
			case ITEMTYPE_TROUSERS:
				return CeGuiString("Hose");
				break;
			case ITEMTYPE_SHINBONE:
				return CeGuiString("Beinschienen");
				break;
			case ITEMTYPE_BOOTS:
				return CeGuiString("Stiefel");
				break;
			case ITEMTYPE_OTHER:
				return CeGuiString("Anderes");
				break;
		}
		return CeGuiString("undefined");
	}

	const CeGuiString Item::getDescription() const
	{
		CeGuiString desc = mName + "\r\n\r\n" + 
			getFormattedText(mDescription)+
			"\r\n\r\n"+
			"Gewicht: ";
		if (mWeight >= 40)
		{
			desc += Ogre::StringConverter::toString(mWeight / 40)+ " Stein";
		}
		else
		{
			desc += Ogre::StringConverter::toString(mWeight)+ " Unzen";
		}
		return desc;

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

	void Item::setImageName(const CeGuiString name)
	{
		mImageName = name;
	}
	void Item::setMeshName(const CeGuiString name)
	{
		mMeshName = name;
	}

	const CeGuiString Item::getImageName()
	{
		return mImageName;
	}
	const CeGuiString Item::getMeshName()
	{
		return mMeshName;
	}

	void Item::setPrice(int price)
	{
		mPrice = price;
	}
	
	int Item::getPrice()
	{
		return mPrice;
	}


	bool Item::isContainer()
	{
		return mIsContainer;
	}
	void Item::setContainer(bool isContainer, pair<int,int> dim)
	{
		mIsContainer = isContainer;
		mCapacity = dim;
		if (isContainer) {
			initContainer();
		}
	}

	pair<int,int> Item::getCapacity()
	{
		return mCapacity;
	}

	void Item::initContainer()
	{
		mContainerLayout = ContainerLayout(mCapacity.first);

		for (unsigned z = 0; z< mContainerLayout.size(); z++){
			ContainerColumn temp = ContainerColumn(mCapacity.second);
			mContainerLayout[z] = temp;
		}
		
		for (unsigned int x = 0; x < mContainerLayout.size(); x++){
			for (unsigned int y = 0; y < mContainerLayout[0].size(); y++){
				mContainerLayout[x][y] = NULL;
			}
		}

	}

	pair<int,int> Item::getSize()
	{
		return mSize;
	}

	void Item::setSize(int widthSize,int heightSize)
	{
		mSize = pair<int,int>(widthSize,heightSize);
	}

	ContainerLayout &Item::getContainerLayout()
	{
		return mContainerLayout;
	}
}
