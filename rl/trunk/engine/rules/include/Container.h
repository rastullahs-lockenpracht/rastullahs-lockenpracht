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

    /// Behaelter fr Items.
    class _RlRulesExport Container : public Item
    {
    public:
        static const Ogre::String CLASS_NAME;

        static const Ogre::String PROPERTY_VOLUME;
        static const Ogre::String PROPERTY_CAPACITY;
        static const Ogre::String PROPERTY_CONTENT;

        Container(unsigned int id);
        virtual ~Container(void);

        /// Fassungsvermoegen in Unzen
        Ogre::Real getCapacity() const;

        /// Fassungsvermoegen in Unzen
        void setCapacity(Ogre::Real capacity);

	// Volumen in x (breite) * y (hoehe)
	void setVolume(unsigned int x, unsigned int y);
        std::pair<unsigned int, unsigned int> getVolume() const;

        /// ist dieser Gegenstand ein Container
        virtual bool isContainer() const;

		/// Liefert Gesamtgewicht des Inhalts.
        Ogre::Real getContentWeight() const;
        virtual Ogre::Real getWeight() const;

        void addItem(Item* item);
        void removeItem(Item* item);

        ItemSet getItems() const;

	int getItemCount() const;

        bool isFree(unsigned int x, unsigned int y) const;
        Item* getItemAt(unsigned int x, unsigned int y) const;
        void putItemAt(Item* item, unsigned int x, unsigned int y);
        bool canPlaceAt(Item* item, unsigned int xPos, unsigned int yPos) const;
        void setItemPosition(Item* item, unsigned int xPos, unsigned int yPos);

        virtual const Property getProperty(const Ogre::String& key) const;
        virtual void setProperty(const Ogre::String& key, const Property& value);
        virtual PropertySet* getAllProperties() const;

    private:
        Ogre::Real mCapacity;
	std::pair<unsigned int,unsigned int> mVolume;
		
		// Speichert, wo die Items sich im Container befinden.
		// Speichert also die IDs der Objekte in die einzelnen Volumenfelder
	int objIDMap [1][1];

        ItemSet mItems;

        std::map<Item*, std::pair<unsigned int, unsigned int> > mItemPositions;

        std::pair<unsigned int, unsigned int> findPositionWithEnoughSpace(std::pair<unsigned int, unsigned int> space) const;
        bool checkSpace(unsigned int xStart, unsigned int yStart, std::pair<unsigned int,unsigned int> space) const;
    };
}

#endif //__CONTAINER_H__
