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
        if( mItem != NULL )
            mItem->removeOldState();
    }

    Item* Slot::getItem() const
    {
        return mItem;
    }

    bool Slot::isAllowed(const Item *item) const
    {
        if (item == NULL)
            return false;
        int type = item->getItemType();
        return (type & mItemReadyMask) == type || (type & mItemHeldMask) == type;
    }

    bool Slot::canReady(const Item *item) const
    {
        if (item == NULL)
            return false;
        int type = item->getItemType();
        return (type & mItemReadyMask) == type;
    }

    bool Slot::setItem(Item* item)
    {
        if(mItem == item)
            return false;
        if (item)
        {
            if (isAllowed(item) && isEmpty())
            {
                item->removeOldState();
                item->setOwner(mOwner);
                item->setParentSlot(this);
                if (canReady(item))
                    item->setState(GOS_READY);
                else
                    item->setState(GOS_HELD);

                mItem = item;
            }
            else
            {
                LOG_ERROR(Logger::RULES,
                    "Item '" + item->getName() + "' konnte nicht in den Slot '" + mName
                    + "' gelegt werden: " + (isAllowed(item) ? "'falscher Typ' " : "")
                    + (isEmpty() ? "'Slot schon belegt'" : "") + "!");
                return false;
            }
        }
        else
        {
            // this is the case, if the item is removed automatically
            // don't change this without looking at Item::setState
            mItem->removeOldState();
            mItem = NULL;
        }

        return true;
    }


    BoneSlot::BoneSlot(Creature* owner, const CeGuiString& name, int itemReadyMask, int itemHeldMask, const Ogre::String& bone)
        : Slot(owner, name, itemReadyMask, itemHeldMask), mBone(bone)
    {
    }

    bool BoneSlot::setItem(Item* item)
    {
        if (!Slot::setItem(item))
        {
            return false;
        }

        if (item)
        {
            if (mOwner->getState() == GOS_IN_SCENE
                || mOwner->getState() == GOS_HELD
                || mOwner->getState() == GOS_READY)
            {
                if (!item->getActor())
                {
                    item->doCreateActor();
                }
                mOwner->getActor()->attachToSlot(item->getActor(), mBone);
            }
        }

        return true;
    }

    void BoneSlot::update()
    {
        if( mItem && (mOwner->getState() == GOS_IN_SCENE
                || mOwner->getState() == GOS_HELD
                || mOwner->getState() == GOS_READY) )
        {
            if( !mItem->getActor() )
            {
                mItem->doCreateActor();
                mOwner->getActor()->attachToSlot(mItem->getActor(), mBone);
            }
            else
            {
                if( !mOwner->getActor()->hasChild(mItem->getActor()) )
                    mOwner->getActor()->attachToSlot(mItem->getActor(), mBone);
            }
        }
    }


    SubmeshSlot::SubmeshSlot(Creature* owner, const CeGuiString& name, int itemReadyMask, int itemHeldMask, const Ogre::String& submesh)
        : Slot(owner, name, itemReadyMask, itemHeldMask), mSubmesh(submesh)
    {
    }

    bool SubmeshSlot::setItem(Item* item)
    {
        Item* oldItem = mItem;
        if (!Slot::setItem(item))
        {
            return false;
        }

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
                    CeGuiString file = item->getSubmeshName();
                    if( file == "" )
                        file = item->getMeshfile();
                    else
                        file = mOwner->getSubmeshPreName()+file;

				    mmo->replaceSubmesh(
					    mSubmesh,
					    file.c_str());
			    }
		    }

            if (mOwner->getActor()
                && mOwner->getActor()->getPhysicalThing())
            {
                mOwner->getActor()->getPhysicalThing()->updatePhysicsProxy();
            }
	    }

        return true;
    }

    void SubmeshSlot::update()
    {
	    if (mOwner->getActor())
	    {
		    MergeableMeshObject* mmo = dynamic_cast<MergeableMeshObject*>(
			    mOwner->getActor()->getControlledObject());

		    if (mmo)
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

			    if (mItem)
			    {
                    mItem->doCreateActor();
                    CeGuiString file = mItem->getSubmeshName();
                    if( file == "" )
                        file = mItem->getMeshfile();
                    else
                        file = mOwner->getSubmeshPreName()+file;

				    mmo->replaceSubmesh(
					    mSubmesh,
					    file.c_str());
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

    bool MaterialSlot::setItem(Item* item)
    {
        if (!Slot::setItem(item))
        {
            return false;
        }

        if (item)
        {
            try
            {
                CeGuiString mat = item->getProperty("material").toString();
                CeGuiString mesh = item->getSubmeshName();

                ///@todo: what to do if actor is null?, think about changing the inventory of an gameobject not in scene
		        if (mOwner->getActor())
		        {
			        MeshObject* mo = dynamic_cast<MeshObject*>(
				        mOwner->getActor()->getControlledObject());

                    MergeableMeshObject* mmo = dynamic_cast<MergeableMeshObject*>(
				        mo);

                    if (mmo && !mesh.empty())
                    {
                        mmo->replaceSubmesh(mSubmesh, mesh.c_str());
                    }

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

        return true;
    }

    void MaterialSlot::update()
    {
        if (mItem)
        {
            try
            {
                CeGuiString mat = mItem->getProperty("material").toString();
                CeGuiString mesh = mItem->getSubmeshName();

                ///@todo: what to do if actor is null?, think about changing the inventory of an gameobject not in scene
		        if (mOwner->getActor())
		        {
			        MeshObject* mo = dynamic_cast<MeshObject*>(
				        mOwner->getActor()->getControlledObject());

                    MergeableMeshObject* mmo = dynamic_cast<MergeableMeshObject*>(
				        mo);

                    if (mmo && !mesh.empty())
                    {
                        mmo->replaceSubmesh(mSubmesh, mesh.c_str());
                    }

			        if (mo)
			        {                        
                        mo->setMaterial(mat.c_str(), mSubmesh);
                    }
                }
            }
            catch (const IllegalArgumentException&)
            {
                LOG_ERROR(Logger::RULES, "Item " + mItem->getName() + " has no property material.");
            }
            catch (const WrongFormatException&)
            {
                LOG_ERROR(Logger::RULES, "Item " + mItem->getName() + " has a property material, but it is no string property.");
            }
        }
    }

} // namespace rl
