/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#include "Container.h"
#include "Exception.h"
#include <algorithm>

using namespace std;

namespace rl {

    Container::Container(int id, const string& name, const string& description)
        : GameObject(id, name, description), mCapacity(1000)
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
            Throw(InvalidArgumentException, "Item nicht in Container.");
        }
        return rval;
    }
}
