/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
    class Slot;
    class Container;

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
            ITEMTYPE_CLOTHES    = 1 << 15,
			ITEMTYPE_OTHER      = 1 << 16,
            ITEMTYPE_ALL_ITEMS  = ~(0)
		};


        static const Ogre::String CLASS_NAME;

        static const Ogre::String PROPERTY_ITEMTYPE;
        static const Ogre::String PROPERTY_IMAGENAME;
        static const Ogre::String PROPERTY_SIZE;
        static const Ogre::String PROPERTY_SUBMESHNAME;

		Item(unsigned int id);
        virtual ~Item(void);

		void setItemType(ItemType itemType);
		ItemType getItemType() const;

		const CeGuiString getImageName() const;
		void setImageName(const CeGuiString name);

        const CeGuiString getSubmeshName() const;
        void setSubmeshName(const CeGuiString &name);

		/**
		 * @return ob es sich um ein Item handelt,
		 * in dem andere gespeichert werden koennen
		 */
        virtual bool isContainer() const;
		
		std::pair<int,int> getSize() const;
		void setSize(int width, int height);


        /**
         * Fügt das Verhalten für die GO-States: GOS_READY, GOS_HELD und GOS_IN_POSSESION hinzu.
         * @warning Bevor einer der drei States gesetzt wird, sollte zunächst die Funktion removeOldState
         * aufgerufen werden, dann müssen Owner und ParentSlot/ParentContainer gesetzt werden und
         * eventuell doCreateActor. Als letztes muss diese Funktion mit dem neuen State aufgerufen werden.
         * Die Reihenfolge ist wichtig, damit die onStateChange-Funktionen funktionieren können!
         **/
        virtual void setState(GameObjectState state);
        virtual void removeOldState();

        void setParentSlot(Slot* slot);
        Slot* getParentSlot() const;

        void setParentContainer(Container* cont);
        Container* getParentContainer() const;

        virtual void setOwner(GameObject* go);
        GameObject* getOwner() const;

        virtual const Property getProperty(const CeGuiString& key) const;
        virtual void setProperty(const CeGuiString& key, const Property& value);
        virtual PropertyKeys getAllPropertyKeys() const;

        void doCreateActor();
    protected:
		ItemType mItemType;
		CeGuiString mImageName;
        CeGuiString mSubmeshName;
		std::pair<int,int> mSize;
        GameObject* mOwner;
        Slot* mParentSlot;
        Container* mParentContainer;

		void doLoose();
	};
}

#endif
