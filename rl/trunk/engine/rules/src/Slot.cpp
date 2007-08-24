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

    Slot::Slot(Creature* owner, const CeGuiString& name, int itemReadyMask, int itemHeldMask)
        : mOwner(owner), mName(name), mItemReadyMask(itemReadyMask), mItemHeldMask(itemHeldMask), mItem(NULL)
    {
    }

    Slot::~Slot()
    {
    }

    Item* Slot::getItem() const
    {
        return mItem;
    }

    bool Slot::isAllowed(const Item *item) const
    {
        if( item == NULL )
            return false;
        int type = item->getItemType();
        return (type & mItemReadyMask) == type || (type & mItemHeldMask) == type;
    }

    bool Slot::canReady(const Item *item) const


    {
        if( item == NULL )
            return false;
        int type = item->getItemType();
        return (type & mItemReadyMask) == type;
    }

    bool Slot::standardSetItem(Item* item)

    {
        if (item)
        {
            if( isAllowed(item) && isEmpty())
            {
                item->removeOldState();
                item->setOwner(mOwner);
                item->setParentSlot(this);
                if( canReady(item) )
                    item->setState(GOS_READY);
                else
                    item->setState(GOS_HELD);

                mItem = item;
            }
            else
            {
                LOG_MESSAGE(Logger::RULES,
                    "Item '" + item->getName() + "' konnte nicht in den Slot '" + mName
                    + "' gelegt werden: " + (isAllowed(item) ? "'falscher Typ' " : "")
                    + (isEmpty() ? "'Slot schon belegt" : "") + "!");
                return false;
            }
        }
        else
        {
            // this is the case, if the item is removed automatically
            // don't change this without looking at Item::setState
            if( mItem->getState() != GOS_LOADED )
                mItem->setState(GOS_LOADED);
            mItem = NULL;

        }

        return true;
    }


    BoneSlot::BoneSlot(Creature* owner, const CeGuiString& name, int itemReadyMask, int itemHeldMask, const Ogre::String& bone)
        : Slot(owner, name, itemReadyMask, itemHeldMask), mBone(bone)
    {
    }

    void BoneSlot::setItem(Item* item)
    {
        if( !standardSetItem(item) )
            return;

        if(item)
        {
            if (mOwner->getState() == GOS_IN_SCENE)
            {
                mItem->doCreateActor();
                mOwner->getActor()->attachToSlot(mItem->getActor(), mBone);
            }
        }
    }


    SubmeshSlot::SubmeshSlot(Creature* owner, const CeGuiString& name, int itemReadyMask, int itemHeldMask, const Ogre::String& submesh)
        : Slot(owner, name, itemReadyMask, itemHeldMask), mSubmesh(submesh)
    {
    }

    void SubmeshSlot::setItem(Item* item)
    {
        Item* oldItem = mItem;
        if( !standardSetItem(item) )
            return;


	    if (mOwner->getActor())
	    {
		    MergeableMeshObject* mmo = dynamic_cast<MergeableMeshObject*>(
			    mOwner->getActor()->getControlledObject());

		    if (mmo)
		    {
                if (oldItem != mItem)
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
			    }

			    if (item)
			    {
                    item->doCreateActor();
				    mmo->replaceSubmesh(
					    mSubmesh,
					    item->getMeshfile().c_str());
			    }
		    }

            if (mOwner->getActor()
                && mOwner->getActor()->getPhysicalThing())
            {
                mOwner->getActor()->getPhysicalThing()->updatePhysicsProxy();
            }
	    }
    }

    MaterialSlot::MaterialSlot(Creature* owner, const CeGuiString& name, int itemReadyMask, int itemHeldMask, const Ogre::String& submesh)
      : Slot(owner, name, itemReadyMask, itemHeldMask),
        mSubmesh(submesh)
    {
    }

    void MaterialSlot::setItem(Item* item)
    {
        if( !standardSetItem(item) )
            return;


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
