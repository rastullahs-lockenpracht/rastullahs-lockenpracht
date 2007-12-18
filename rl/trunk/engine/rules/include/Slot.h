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
#ifndef __SLOT_H__
#define __SLOT_H__

#include "RulesPrerequisites.h"

namespace rl {

class Creature;
class Item;

class _RlRulesExport Slot
{
public:
    // provides standard behaviour, sets
    // item-state etc, returns true, if succeeded to set item, false indicates to to nothing
    // subclasses MUST call this method when overriding
    virtual bool setItem(Item* item);
    Item* getItem() const;
    bool isAllowed(const Item* item) const;
    bool isEmpty() const {return mItem == NULL;}
    bool canReady(const Item* item) const;
    CeGuiString getName() const {return mName;}
    virtual ~Slot();

protected:
    Slot(Creature* owner, const CeGuiString& name, int itemReadyMask, int itemHeldMask);


    Creature* mOwner;
    Item* mItem;

private:
    CeGuiString mName;
    /// die mItemReadyMask hat Vorrang vor der mItemHeldMask
    int mItemHeldMask;
    /// die mItemReadyMask hat Vorrang vor der mItemHeldMask
    int mItemReadyMask;
};

class BoneSlot : public Slot
{
public:
    BoneSlot(Creature* owner, const CeGuiString& name, int itemReadyMask, int itemHeldMask, const Ogre::String& bone);
    virtual bool setItem(Item* item);

private:
    Ogre::String mBone;
};

class SubmeshSlot : public Slot
{
public:
    SubmeshSlot(Creature* owner, const CeGuiString& name, int itemReadyMask, int itemHeldMask, const Ogre::String& submesh);
    virtual bool setItem(Item* item);

private:
    Ogre::String mSubmesh;
};

class MaterialSlot : public Slot
{
public:
    MaterialSlot(Creature* owner, const CeGuiString& name, int itemReadyMask, int itemHeldMask, const Ogre::String& submesh);
    virtual bool setItem(Item* item);

private:
    Ogre::String mSubmesh;
};

} // namespace rl

#endif //__SLOT_H__
