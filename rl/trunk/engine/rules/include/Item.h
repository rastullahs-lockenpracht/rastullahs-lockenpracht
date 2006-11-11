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

namespace rl
{
	class _RlRulesExport Item;

	typedef vector<Item*> ContainerColumn;
	typedef vector<ContainerColumn> ContainerLayout;


    /// Aufnehmbare Objekte in der Spielwelt.
    class _RlRulesExport Item : public GameObject
    {
    public:
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

        static const Ogre::String CLASS_NAME;

		Item(unsigned int id);

        virtual ~Item(void);

		const CeGuiString getDescription() const;

        void setWeight(int weight);
        int getWeight() const;

		void setItemType(ItemType itemType);
		ItemType getItemType() const;

		void setPrice(int price);
		int getPrice();

		static const CeGuiString getItemTypeString(ItemType type);
 
		const CeGuiString getImageName();
		void setImageName(const CeGuiString name);

		/**
		 * @return ob es sich um ein Item handelt,
		 * in dem andere gespeichert werden können
		 */
		bool isContainer();
		
		void setContainer(bool isContainer, std::pair<int,int> dim = std::make_pair(0,0) );

		ContainerLayout &getContainerLayout();
		std::pair<int,int> getCapacity();
		
		std::pair<int,int> getSize();
		void setSize(int width, int height);

		
    protected:
        /// Masse in Unzen.
        int mWeight;
		ItemType mItemType;

		// Der Standard-Preis (kann variiert werden) in Silbertalern
		unsigned int mPrice;

		CeGuiString mImageName;

		bool mIsContainer;
		ContainerLayout mContainerLayout;
		std::pair<int,int> mCapacity;

		std::pair<int,int> mSize;


		/**
		* Generiert einen Text mit Zeilenumbrüchen, aus dem String.
		* @param unformattedText Der zu formatierende Text
		* @return CeGuiString der Text mit eingefügten Enter-symbolen
		*/
		static CeGuiString getFormattedText(const CeGuiString &unformattedText);

	private:

		/**
		* initiiert einen Container mit null-pointern
		* gemaess der Kapazitaet des Items
		*/
		void initContainer();
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
