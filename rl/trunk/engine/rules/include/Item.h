/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

	typedef std::vector<Item*> ContainerColumn;
	typedef std::vector<ContainerColumn> ContainerLayout;


    /// Aufnehmbare Objekte in der Spielwelt.
    class _RlRulesExport Item : public GameObject
    {
    public:
		enum ItemType{
			ITEMTYPE_WEAPON     = 1 << 0,
			ITEMTYPE_SHIELD     = 1 << 1,
			ITEMTYPE_RING       = 1 << 2,
			ITEMTYPE_GLOVES     = 1 << 3,
			ITEMTYPE_BRACELET   = 1 << 4,
			ITEMTYPE_ARMOR      = 1 << 5,
			ITEMTYPE_CAPE       = 1 << 6,
			ITEMTYPE_BRACERS    = 1 << 7,
			ITEMTYPE_BACKPACK   = 1 << 8,
			ITEMTYPE_BELT       = 1 << 9,
			ITEMTYPE_NECKLACE   = 1 << 10,
			ITEMTYPE_HELMET     = 1 << 11,
			ITEMTYPE_TROUSERS   = 1 << 12,
			ITEMTYPE_SHINBONE   = 1 << 13,
			ITEMTYPE_BOOTS      = 1 << 14,
			ITEMTYPE_OTHER      = 1 << 15,
            ITEMTYPE_ALL_ITEMS  = ~(0)
		};


        static const Ogre::String CLASS_NAME;

        static const Ogre::String PROPERTY_ITEMTYPE;
        static const Ogre::String PROPERTY_IMAGENAME;
        static const Ogre::String PROPERTY_SIZE;

		Item(unsigned int id);
        virtual ~Item(void);

	    void setWeight(Ogre::Real weight);
        Ogre::Real getWeight() const;

		void setItemType(ItemType itemType);
		ItemType getItemType() const;

		void setPrice(int price);
		int getPrice() const;
 
		const CeGuiString getImageName() const;
		void setImageName(const CeGuiString name);

		/**
		 * @return ob es sich um ein Item handelt,
		 * in dem andere gespeichert werden koennen
		 */
        virtual bool isContainer() const;
		
		std::pair<int,int> getSize() const;
		void setSize(int width, int height);

        virtual void setState(GameObjectState state);
        void hold();

        virtual const Property getProperty(const Ogre::String& key) const;
        virtual void setProperty(const Ogre::String& key, const Property& value);
        virtual PropertySet* getAllProperties() const;

    protected:
        /// Masse in Unzen.
        Ogre::Real mWeight;
		ItemType mItemType;

		// Der Standard-Preis (kann variiert werden) in Silbertalern
		unsigned int mPrice;

		CeGuiString mImageName;

		std::pair<int,int> mSize;

		void doHold();
	};
}

#endif
