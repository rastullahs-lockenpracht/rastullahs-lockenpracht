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

    Container::Container(const CeGuiString& name, const CeGuiString& description)
        : Item(name, description)
    {

    }

    Container::~Container()
    {
    }

    int Container::getCapacity() const
    {
        return mCapacity;
    }

    void Container::setCapacity(const int capacity)
    {
        mCapacity = capacity;
    }

    int Container::getContentWeight() const
    {
        int rval = 0;
        for (ItemSet::const_iterator it = mItems.begin(); it != mItems.end(); it++)
        {
            rval += (*it)->getWeight();
        }
        return rval;
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
        mItems.insert(item);
    }

    Item* Container::removeItem(int itemId)
    {
        Item* rval = 0;
        ItemSet::iterator it = find_if(mItems.begin(), mItems.end(),
            bind1st(FindItemById(), itemId));
        if (it != mItems.end())
        {
            rval = *it;
            mItems.erase(it);
        }
        else
        {
            Throw(IllegalArgumentException, "Item nicht in Container.");
        }
        return rval;
    }

	int Container::getItemCount() const
	{
		return mItems.size();
	}

	const Item* Container::getItem(int itemId) const
	{
		ItemSet::const_iterator it = find_if(mItems.begin(), mItems.end(),
			bind1st(FindItemById(), itemId));
		if (it != mItems.end())
		{
			return (*it);
		}
		else
		{
			Throw(IllegalArgumentException, "Item nicht in Container.");
		}
	}
}
