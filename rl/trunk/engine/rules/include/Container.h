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

#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include "RulesPrerequisites.h"

#include "Item.h"

namespace rl
{
    typedef std::set<Item*> ItemSet;
	typedef std::pair<unsigned int, unsigned int> UintPair;

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
        Container(unsigned int id);
        virtual ~Container(void);

		/// Get the weight capacity (in Stein)
        Ogre::Real getCapacity() const;

		/// Set the weight capacity (in Stein)
        void setCapacity(Ogre::Real capacity);

		/// Set the "volume" to x (width) * y (height) spaces
		void setVolume(unsigned int x, unsigned int y);

		/// Get the container's "volume" spaces
		UintPair getVolume() const;

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
        bool addItem(Item* item);
		bool addItem(Item* item, UintPair position);
        void removeItem(Item* item);

        ItemSet getItems() const;

		int getItemCount() const;

        bool isFree(unsigned int x, unsigned int y) const;
        Item* getItemAt(unsigned int x, unsigned int y) const;
        void putItemAt(Item* item, unsigned int x, unsigned int y);
        bool canPlaceAt(Item* item, unsigned int xPos, unsigned int yPos) const;
        void setItemPosition(Item* item, unsigned int xPos, unsigned int yPos);
        UintPair getItemPosition(Item* item) const;

        virtual const Property getProperty(const Ogre::String& key) const;
        virtual void setProperty(const Ogre::String& key, const Property& value);
        virtual PropertySet* getAllProperties() const;

    private:
		static const UintPair NO_SPACE_FOR_ITEM;

        Ogre::Real mCapacity;
		std::pair<unsigned int,unsigned int> mVolume;
		
		// Speichert, wo die Items sich im Container befinden.
		// Speichert also die IDs der Objekte in die einzelnen Volumenfelder
		int objIDMap [1][1];

        ItemSet mItems;

        std::map<Item*, UintPair> mItemPositions;

        std::pair<unsigned int, unsigned int> 
			findPositionWithEnoughSpace(UintPair space) const;
        bool checkSpace(
			unsigned int xStart, 
			unsigned int yStart, 
			UintPair space) const;
    };
}

#endif //__CONTAINER_H__
