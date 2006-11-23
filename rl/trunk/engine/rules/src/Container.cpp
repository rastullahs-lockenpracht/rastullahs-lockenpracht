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

#include "Container.h"
#include "Exception.h"
#include "Item.h"
#include <algorithm>

using namespace std;

namespace rl {

    const Ogre::String Container::CLASS_NAME = "Container";

    const Ogre::String Container::PROPERTY_CAPACITY = "capacity";
    const Ogre::String Container::PROPERTY_VOLUME = "volume";

    Container::Container(unsigned int id)
        : Item(id),
          mCapacity(0.0)
    {
    }

    Container::~Container()
    {
    }

    Ogre::Real Container::getCapacity() const
    {
        return mCapacity;
    }

    void Container::setCapacity(const Ogre::Real capacity)
    {
        mCapacity = capacity;
    }

    void Container::setVolume(unsigned int x, unsigned int y)
    {
        mVolume = make_pair(x, y);
    }

    std::pair<unsigned int,unsigned int> Container::getVolume() const
    {
        return mVolume;
    }

    Ogre::Real Container::getContentWeight() const
    {
        Ogre::Real rval = 0;
        for (ItemSet::const_iterator it = mItems.begin(); it != mItems.end(); it++)
        {
            rval += (*it)->getWeight();
        }
        return rval;
    }

    bool Container::isContainer() const
    {
        return true;
    }

    ItemSet Container::getItems() const
    {
        return mItems;
    }

    void Container::addItem(Item* item)
    {
        if(item == 0)
        {
            Throw(NullPointerException, "Item ist null.");
        }
        pair<unsigned int, unsigned int> pos = findPositionWithEnoughSpace(item->getSize());
        mItemPositions[item] = pos;
        mItems.insert(item);
        item->setState(GOS_IN_POSSESSION);
    }

    void Container::removeItem(Item* item)
    {
        ItemSet::iterator it = mItems.find(item);
        if (it != mItems.end())
        {
            mItems.erase(it);
            mItemPositions.erase(mItemPositions.find(item));
            item->setState(GOS_LOADED);
        }
        else
        {
            Throw(IllegalArgumentException, "Item not in Container.");
        }
    }

	int Container::getItemCount() const
	{
		return mItems.size();
	}

    bool Container::canPlaceAt(Item* item, unsigned int xPos, unsigned int yPos) const
	{
		int xSize = item->getSize().first;
		int ySize = item->getSize().second;

		bool free = true;

        // Es wird versucht, das Item außerhalb des Containers zu platzieren
        if (xPos + xSize >= mVolume.first 
            || yPos + ySize >= mVolume.second)
        {
            return false;
        }

		for (unsigned int x = xPos; x < (xPos + xSize); x++)
        {
			for (unsigned int y = yPos; y < (yPos + ySize); y++)
            {
                LOG_DEBUG2(Logger::RULES,
					Ogre::String("Checking Point in Backpack: Point x:")
					+ Ogre::StringConverter::toString(x) 
					+ ", Point y:"
					+ Ogre::StringConverter::toString(y),
                    "Container::canPlaceAt()");

                if (getItemAt(x, y) != item)
                {				
                    // Siehe nach, ob ein anderes Item im Weg ist
					return false;
				}
			}
		}
		
		return true;
	}


    void Container::setItemPosition(Item* item, unsigned int xPos, unsigned int yPos)
    {
        if (mItemPositions.find(item) == mItemPositions.end())
        {
            Throw(IllegalArgumentException, "Item not in Container.");
        }

        mItemPositions[item] = make_pair(xPos, yPos);
    }

    pair<unsigned int, unsigned int> Container::findPositionWithEnoughSpace(pair<unsigned int, unsigned int> space) const
    {
		for (unsigned int x = 0; x < mVolume.first; x++)
        {
			for (unsigned int y = 0; y < mVolume.second; y++)
            {
				if (isFree(x, y) && checkSpace(x, y, space))
                {
					return pair<unsigned int, unsigned int>(x,y);
				}
			}
		}
		Throw(IllegalStateException, "Rucksack hat keinen Platz für das Item");
	}

    bool Container::checkSpace(unsigned int xStart, unsigned int yStart, pair<unsigned int,unsigned int> space) const
    {
		// Falls Kästchen nicht mehr im Rucksack, ist auch kein Platz mehr :)
        if ((xStart+space.first) >= mVolume.first 
            || (yStart+space.second) >= mVolume.second)
        {
			return false;
		}

		for (unsigned int x = 0; x < space.first; x++)
        {
			for (unsigned int y = 0; y < space.second; y++)
            {
                if (!isFree(xStart+x, yStart+y))
                {
                    return false;				
                }
			}
		}
		return true;
	}

    bool Container::isFree(unsigned int x, unsigned int y) const
    {
        return getItemAt(x, y) == NULL;
    }

    Item* Container::getItemAt(unsigned int x, unsigned int y) const
    {
        for (map<Item*, pair<unsigned int, unsigned int> >::const_iterator iter = mItemPositions.begin(); 
            iter != mItemPositions.end(); iter++)
        {
            Item* item = (*iter).first;
            pair<unsigned int, unsigned int> pos = (*iter).second;

            if (pos.first <= x 
                && pos.second <= y 
                && x <= pos.first + item->getSize().first
                && y <= pos.second + item->getSize().second)
            {
                return item;
            }
        }

        return NULL;
    }

    Ogre::Real Container::getWeight() const
    {
        return mWeight + getContentWeight();
    }

    void Container::setProperty(const Ogre::String &key, const rl::Property &value)
    {
        if (key == Container::PROPERTY_CAPACITY)
        {
            if (value.isInt())
            {
                mCapacity = value.toInt();
            }
            else if (value.isReal())
            {
                mCapacity = value.toReal();
            }
        }
        else if (key == Container::PROPERTY_VOLUME)
        {
            mVolume = value.toIntPair();
        }
        else
        {
            Item::setProperty(key, value);
        }
    }

    const Property Container::getProperty(const Ogre::String &key) const
    {
        if (key == Container::PROPERTY_CAPACITY)
        {
            return Property(mCapacity);
        }
        else if (key == Container::PROPERTY_VOLUME)
        {
            return Property(mVolume);
        }
        else
        {
            return Item::getProperty(key);
        }
    }

    PropertySet* Container::getAllProperties() const
    {
        PropertySet* ps = Item::getAllProperties();
        ps->setProperty(Container::PROPERTY_CAPACITY, Property(mCapacity));
        ps->setProperty(Container::PROPERTY_VOLUME, Property(mVolume));

        return ps;
    }

}
