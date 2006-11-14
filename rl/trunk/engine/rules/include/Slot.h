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
#ifndef __SLOT_H__
#define __SLOT_H__

#include "RulesPrerequisites.h"

namespace rl {

class Creature;
class Item;

class _RlRulesExport Slot
{
public:
    Slot(Creature* owner, const CeGuiString& name, const Ogre::String& bone, int itemMask);
    ~Slot();

    void setItem(Item* item);
    Item* getItem() const;
    bool isAllowed(Item* item) const;

private:
    Creature* mOwner;
    CeGuiString mName;
    Ogre::String mBone;
    int mItemMask;
    Item* mItem;
};

} // namespace rl

#endif //__SLOT_H__