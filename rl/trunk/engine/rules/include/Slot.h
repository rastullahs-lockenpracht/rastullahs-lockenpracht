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
    virtual void setItem(Item* item) = 0;
    Item* getItem() const;
    bool isAllowed(Item* item) const;
    virtual ~Slot();

protected:
    Slot(Creature* owner, const CeGuiString& name, int itemMask);

    Creature* mOwner;
    Item* mItem;

private:
    CeGuiString mName;
    int mItemMask;
};

class BoneSlot : public Slot
{
public:
    BoneSlot(Creature* owner, const CeGuiString& name, int itemMask, const Ogre::String& bone);
    virtual void setItem(Item* item);

private:
    Ogre::String mBone;
};

class SubmeshSlot : public Slot
{
public:
    SubmeshSlot(Creature* owner, const CeGuiString& name, int itemMask, const Ogre::String& submesh);
    virtual void setItem(Item* item);

private:
    Ogre::String mSubmesh;
};

class MaterialSlot : public Slot
{
public:
    MaterialSlot(Creature* owner, const CeGuiString& name, int itemMask, const Ogre::String& submesh);
    virtual void setItem(Item* item);

private:
    Ogre::String mSubmesh;
};

} // namespace rl

#endif //__SLOT_H__
