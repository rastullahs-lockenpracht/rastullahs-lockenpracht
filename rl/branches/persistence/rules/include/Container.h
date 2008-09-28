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

#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include "RulesPrerequisites.h"

#include "Item.h"

namespace rl
{
    typedef std::set<Item*> ItemSet;

    /// Behaelter fr Items.
    class _RlRulesExport Container : public Item
    {
    public:
        static const Ogre::String CLASS_NAME;

        static const Ogre::String PROPERTY_VOLUME;
        static const Ogre::String PROPERTY_CAPACITY;
        static const Ogre::String PROPERTY_CONTENT;
        static const Ogre::String PROPERTY_CONTENT_OBJECTS;
        static const Ogre::String PROPERTY_CONTENT_POSITIONS;

		/** Creates a new container
		 * @param id the gameobject ID
		 */
		Container(const CeGuiString &id);
        virtual ~Container(void);

		/// Get the weight capacity (in Stein)
        Ogre::Real getCapacity() const;

		/// Set the weight capacity (in Stein)
        void setCapacity(Ogre::Real capacity);

		/// Set the "volume" to x (width) * y (height) spaces
		void setVolume(int x, int y);

		/// Get the container's "volume" spaces
		IntPair getVolume() const;

        /** Returns whether this item is a container
		 * @return always <code>true</code>
		 */
        virtual bool isContainer() const;

		/// Liefert Gesamtgewicht des Inhalts.
        Ogre::Real getContentWeight() const;

		virtual Ogre::Real getMass() const;

		/**
		 * Add an item to the container's content
		 * @param item the item
		 * @return <code>true</code> if adding was successful, <code>false</code> otherwise (e.g. not enough space)
		 */
        bool addItem(Item* item, IntPair position = IntPair(0,0));
        void removeItem(Item* item);

        ItemSet getItems() const;

		int getItemCount() const;

        bool isFree(int x, int y) const;
        Item* getItemAt(int x, int y) const;
        void putItemAt(Item* item, int x, int y);
        bool canPlaceAt(Item* item, int xPos, int yPos) const;
        void setItemPosition(Item* item, int xPos, int yPos);
        IntPair getItemPosition(Item* item) const;

        virtual const Property getProperty(const CeGuiString& key) const;
        virtual void setProperty(const CeGuiString& key, const Property& value);
        virtual PropertyKeys getAllPropertyKeys() const;

        bool canHold(Item* item) const;

        // in order to set the owner of the items in this container correctly override this function
        void setOwner(GameObject *go);

    private:
		static const IntPair NO_SPACE_FOR_ITEM;

        Ogre::Real mCapacity;
		IntPair mVolume;
		
		// Speichert, wo die Items sich im Container befinden.
		// Speichert also die IDs der Objekte in die einzelnen Volumenfelder
		int objIDMap [1][1];

        ItemSet mItems;

        std::map<Item*, IntPair> mItemPositions;

        IntPair	findPositionWithEnoughSpace(IntPair space) const;
        bool checkSpace(
			int xStart, 
			int yStart, 
			IntPair space) const;

        /*
         * recursive function
         * @param cont a container
         * @return true, if cont is this container or any of his parents
        */
        bool isParent(Container* cont) const;
    };
}

#endif //__CONTAINER_H__
