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
        : GameObject(name, description)
    {
    }

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

	CEGUI::String Item::getItemTypeString(ItemType type){
		switch(type){
			case ITEMTYPE_WEAPON:
				return CEGUI::String("Weapon");
				break;
			case ITEMTYPE_SHIELD:
				return CEGUI::String("Shield");
				break;
			case ITEMTYPE_RING:
				return CEGUI::String("Ring");
				break;
			case ITEMTYPE_GLOVE:
				return CEGUI::String("Glove");
				break;
			case ITEMTYPE_BRACELET:
				return CEGUI::String("Bracelet");
				break;
			case ITEMTYPE_ARMOR:
				return CEGUI::String("Armor");
				break;
			case ITEMTYPE_BRACER:
				return CEGUI::String("Bracer");
				break;
			case ITEMTYPE_BACKPACK:
				return CEGUI::String("Backpack");
				break;
			case ITEMTYPE_BELT:
				return CEGUI::String("Belt");
				break;
			case ITEMTYPE_NECKLACE:
				return CEGUI::String("Necklace");
				break;
			case ITEMTYPE_HELMET:
				return CEGUI::String("Helmet");
				break;
			case ITEMTYPE_TROUSERS:
				return CEGUI::String("Trousers");
				break;
			case ITEMTYPE_SHINBONE:
				return CEGUI::String("Shinbone");
				break;
			case ITEMTYPE_BOOTS:
				return CEGUI::String("Boots");
				break;
			case ITEMTYPE_OTHER:
				return CEGUI::String("Other");
				break;
		}
		return CEGUI::String("undefined");
	}
}
