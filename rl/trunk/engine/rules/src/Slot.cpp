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
#include "stdinc.h" //precompiled header

#include "Slot.h"

#include "Actor.h"
#include "Creature.h"
#include "Item.h"
#include "MeshObject.h"
#include "MergeableMeshObject.h"
#include "PhysicalThing.h"

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
		///@todo: what to do if actor is null?, think about changing the inventory of an gameobject not in scene
		if (mOwner->getActor())
		{
			MergeableMeshObject* mmo = dynamic_cast<MergeableMeshObject*>(
				mOwner->getActor()->getControlledObject());

			if (mmo)
			{
				if (item)
				{
					if (isAllowed(item))
					{
						item->setState(GOS_IN_POSSESSION);
						mmo->replaceSubmesh(
							mSubmesh,
							item->getMeshfile().c_str());
						mItem = item;
					}
				}
				else
				{
					MeshPartMap::const_iterator it = mOwner->getMeshParts().find(mSubmesh);
					if (it != mOwner->getMeshParts().end())
					{
						mmo->replaceSubmesh(
							mSubmesh,
							(*it).second);
					}
					else
					{
						mmo->removeSubmesh(mSubmesh);
					}
					mItem = NULL;
				}
			}

            if (mOwner->getActor()
                && mOwner->getActor()->getPhysicalThing())
            {
                mOwner->getActor()->getPhysicalThing()->updatePhysicsProxy();
            }
		}
    }

    MaterialSlot::MaterialSlot(Creature* owner, const CeGuiString& name, int itemMask, const Ogre::String& submesh)
      : Slot(owner, name, itemMask),
        mSubmesh(submesh)
    {
    }

    void MaterialSlot::setItem(Item* item)
    {
        if (item)
        {
            try
            {
                CeGuiString mat = item->getProperty("material").toString();

                ///@todo: what to do if actor is null?, think about changing the inventory of an gameobject not in scene
		        if (mOwner->getActor())
		        {
			        MeshObject* mo = dynamic_cast<MeshObject*>(
				        mOwner->getActor()->getControlledObject());

			        if (mo)
			        {
                        mo->setMaterial(mat.c_str(), mSubmesh);
                    }
                }
            }
            catch (const IllegalArgumentException&)
            {
                LOG_ERROR(Logger::RULES, "Item " + item->getName() + " has no property material.");
            }
            catch (const WrongFormatException&)
            {
                LOG_ERROR(Logger::RULES, "Item " + item->getName() + " has a property material, but it is no string property.");
            }
        }
        else
        {
            ///@todo reset material?
        }
    }

} // namespace rl
