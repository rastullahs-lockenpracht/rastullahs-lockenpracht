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
#ifndef __SLOT_H__
#define __SLOT_H__

#include "RulesPrerequisites.h"

namespace rl {
    
    class Creature;
    class Item;
    
    /**
     * A class to represent a place where an Item can be attached to a creature
     *
     */
    class _RlRulesExport Slot
    {
    public:
        
        /**
         * Creates a slot
         *
         * @param owner the slot's owner
         * @param name the slot's name, it is used to find the slot when adding or removing items
         * @param itemReadyMask an item type mask for items to be readied automatically when put into the slot
         * @param itemHeldMask an item type mask for the items the slot is able to hold
         */
        Slot(Creature* owner, const CeGuiString& name, int itemReadyMask, int itemHeldMask);
        virtual ~Slot();
        
        /**
         * Puts an item into the slot, may depend on the actual slot type
         *
         * Note: this method provides standard behaviour, sets item-state etc
         * subclasses MUST call this method when overriding
         *
         * @returns true, if succeeded to set item, false indicates to to nothing
         */
        virtual bool setItem(Item* item);
        
        /**
         * Updates the slot to match the owner's state
         *
         * Note: this must only be called from Creature::doPlaceIntoScene
         */
        virtual void update();
        
        /**
         * Gets the item in the slot
         *
         * @returns the item, NULL if the slot is empty
         */
        Item* getItem() const;
        
        /**
         * Check if an item can be held in this slot
         *
         * @param item an item
         * @returns true if the item can be put into this slot, false otherwise
         */
        bool isAllowed(const Item* item) const;
        
        /** 
         * Check if the slot is empty
         *
         * @returns true if the slot is empty, false otherwise
         */
        bool isEmpty() const {return mItem == NULL;}
        
        /**
         * Check if an item is readied in this slot (e.g. trousers in the legs slot) or not (e.g. trousers in the left hand slot)
         *
         * @param item an item
         * @returns true if the item is readied in this slot, false otherwise
         */
        bool canReady(const Item* item) const;
        
        /**
         * Check if the slot holds a ready item
         *
         */
        bool isReady() { return mItem != NULL && canReady(mItem); }
        
        /**
         * Get the slot's name
         *
         * @returns the name
         */
        CeGuiString getName() const {return mName;}
        
    protected:
        
        Creature* mOwner;
        Item* mItem;
        
    private:
        CeGuiString mName;
        /// die mItemReadyMask hat Vorrang vor der mItemHeldMask
        int mItemHeldMask;
        /// die mItemReadyMask hat Vorrang vor der mItemHeldMask
        int mItemReadyMask;
    };
    
    /**
     * A slot implemented by a model bone, if objects are put into the slot, their models are attached at the bone
     */
    class BoneSlot : public Slot
    {
    public:
        
        /**
         * Creates a bone slot
         *
         * @param owner the slot's owner
         * @param name the slot's name, it is used to find the slot when adding or removing items
         * @param itemReadyMask an item type mask for items to be readied automatically when put into the slot
         * @param itemHeldMask an item type mask for the items the slot is able to hold
         * @param bone the bone name
         */
        BoneSlot(Creature* owner, const CeGuiString& name, int itemReadyMask, int itemHeldMask, const Ogre::String& bone);
        
        /**
         * Puts an item into the slot by attaching its mesh to the bone of the slot owner mesh
         *
         * @returns true, if succeeded to set item, false indicates to to nothing
         */
        virtual bool setItem(Item* item);

        /**
         * @copydoc Slot::update
         */
        virtual void update();
        
    private:
        Ogre::String mBone;
    };
    
    /**
     * A slot implemented by a submesh "baked" into the owner mesh. If an item is put into that slot it creates or 
     * replaces a specified submesh, this is used for armor to be able to animate the armor meshes on an easy way
     *
     * Note: the item meshes must have the same skeleton as the owner mesh
     */
    class SubmeshSlot : public Slot
    {
    public:

        /**
         * Creates a submesh slot
         *
         * @param owner the slot's owner
         * @param name the slot's name, it is used to find the slot when adding or removing items
         * @param itemReadyMask an item type mask for items to be readied automatically when put into the slot
         * @param itemHeldMask an item type mask for the items the slot is able to hold
         * @param submesh the submesh name
         */
        SubmeshSlot(Creature* owner, const CeGuiString& name, int itemReadyMask, int itemHeldMask, const Ogre::String& submesh);

        /**
         * Puts an item into the slot by creating or replacing a submesh in the slot owner mesh
         *
         * @returns true, if succeeded to set item, false indicates to to nothing
         */
        virtual bool setItem(Item* item);
        
        /**
         * @copydoc Slot::update
         */
        virtual void update();
        
    private:
        Ogre::String mSubmesh;
    };
    
    /**
     * A slot implemented by changing a material on a specified submesh of the owner mesh. This is used for clothes.
     */
    class MaterialSlot : public Slot
    {
    public:
        /**
         * Creates a material slot
         *
         * @param owner the slot's owner
         * @param name the slot's name, it is used to find the slot when adding or removing items
         * @param itemReadyMask an item type mask for items to be readied automatically when put into the slot
         * @param itemHeldMask an item type mask for the items the slot is able to hold
         * @param submesh the submesh name
         */
        MaterialSlot(Creature* owner, const CeGuiString& name, int itemReadyMask, int itemHeldMask, const Ogre::String& submesh);

        /**
         * Puts an item into the slot by replacing the material in one submesh of the slot owner mesh
         *
         * @returns true, if succeeded to set item, false indicates to to nothing
         */
        virtual bool setItem(Item* item);
        
        /**
         * @copydoc Slot::update
         */
        virtual void update();
        
    private:
        Ogre::String mSubmesh;
    };
    
} // namespace rl

#endif //__SLOT_H__
