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

#ifndef __ITEM_H__
#define __ITEM_H__

#include "GameObject.h"
//#include <set>

namespace rl
{
    /// Aufnehmbare Objekte in der Spielwelt.
    class _RlRulesExport Item : public GameObject
    {
    public:
		Item(const CeGuiString name, const CeGuiString description);

		//Item(const CeGuiString name, const CeGuiString description, CEGUI::String imageName, ItemType type, pair<int,int> size);

        virtual ~Item(void);

		enum ItemType{
			ITEMTYPE_WEAPON = 0,
			ITEMTYPE_SHIELD,
			ITEMTYPE_RING,
			ITEMTYPE_GLOVES,
			ITEMTYPE_BRACELET,
			ITEMTYPE_ARMOR,
			ITEMTYPE_CAPE,
			ITEMTYPE_BRACERS,
			ITEMTYPE_BACKPACK,
			ITEMTYPE_BELT,
			ITEMTYPE_NECKLACE,
			ITEMTYPE_HELMET,
			ITEMTYPE_TROUSERS,
			ITEMTYPE_SHINBONE,
			ITEMTYPE_BOOTS,
			ITEMTYPE_OTHER
		};

        void setWeight(int weight);
        int getWeight() const;

		void setItemType(ItemType itemType);
		ItemType getItemType() const;

		static const CeGuiString getItemTypeString(ItemType type);
 
		const CeGuiString getImageName();

		void setImageName(const CeGuiString name);
		
		
		/**
		 * @return ob es sich um ein Item handelt,
		 * in dem andere gespeichert werden können
		 */
		bool isContainer();
		
		void setContainer(bool isContainer, pair<int,int> dim = pair<int,int>(0,0));

		pair<int,int> getCapacity();
		void setCapacity(int width, int height);

		pair<int,int> getSize();
		void setSize(int width, int height);

		
    private:
        /// Masse in Unzen.
        int mWeight;
		ItemType mItemType;

		CeGuiString mImageName;

		bool mIsContainer;
		pair<int,int> mCapacity;

		pair<int,int> mSize;

    };

    class ItemByIdOrdering : public std::binary_function<Item*, Item*, bool> {
    public:
        bool operator()(const Item* const lhs, const Item* const rhs)
        {
            return lhs->getId() < rhs->getId();
        }
    };

    class FindItemById : public std::binary_function<int, Item*, bool> {
    public:
        bool operator()(int id, const Item* const item) const
        {
            return item->getId() == id;
        }
    };

    typedef std::set<Item*, ItemByIdOrdering> ItemSet;
}

#endif
