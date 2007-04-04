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
#include "Slot.h"

#include "Actor.h"
#include "Creature.h"
#include "Item.h"
#include "MeshObject.h"

namespace rl {

    Slot::Slot(Creature* owner, const CeGuiString& name, int itemMask)
        : mOwner(owner), mName(name), mItemMask(itemMask), mItem(NULL)
    {
    }

    Slot::~Slot()
    {
    }

    Item* Slot::getItem() const
    {
        return mItem;
    }

    bool Slot::isAllowed(Item *item) const
    {
        int type = item->getItemType();
        return (type & mItemMask) == type;
    }


    BoneSlot::BoneSlot(Creature* owner, const CeGuiString& name, int itemMask, const Ogre::String& bone)
        : Slot(owner, name, itemMask), mBone(bone)
    {
    }


    void BoneSlot::setItem(Item* item)
    {
		if (item == NULL)
		{
			mItem->setState(GOS_LOADED);
			mItem = NULL;
			return;
		}

        if (isAllowed(item))
        {
			if (mItem != NULL)
			{
				/// @todo What to do with the item which was already in the slot (or should we throw an exception if the item is not NULL)
			}

            mItem = item;
            if (mOwner->getState() == GOS_IN_SCENE)
            {
				mItem->setState(GOS_HELD);
                mOwner->getActor()->attachToSlot(mItem->getActor(), mBone);
            }
        }
    }


    SubmeshSlot::SubmeshSlot(Creature* owner, const CeGuiString& name, int itemMask, const Ogre::String& submesh)
        : Slot(owner, name, itemMask), mSubmesh(submesh)
    {
    }


    void SubmeshSlot::setItem(Item* item)
    {
        ///@todo: use meshcombiner API
    }
} // namespace rl
