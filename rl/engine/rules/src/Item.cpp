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

#include "Item.h"

#include "Actor.h"
#include "Container.h"
#include "Exception.h"
#include "GameObjectManager.h"
#include "Slot.h"

using namespace std;

namespace rl
{
    const Ogre::String Item::CLASS_NAME = "Item";

    const Ogre::String Item::PROPERTY_ITEMTYPE = "itemtype";
    const Ogre::String Item::PROPERTY_SIZE = "size";
    const Ogre::String Item::PROPERTY_SUBMESHNAME = "submeshfile";

    Item::Item(unsigned int id)
        : GameObject(id)
        , mItemType(ITEMTYPE_OTHER)
        , mSize(pair<int, int>(1, 1))
        , mOwner(NULL)
        , mParentSlot(NULL)
        , mParentContainer(NULL)
        , mSubmeshName("")
    {
        mQueryFlags |= QUERYFLAG_ITEM;
    }

    Item::~Item(void)
    {
        removeOldState(); // so the parent-container etc knows this item doesn't exists any more
    }

    void Item::setItemType(ItemType itemType)
    {
        mItemType = itemType;
    }

    Item::ItemType Item::getItemType() const
    {
        return mItemType;
    }

    bool Item::isContainer() const
    {
        return false;
    }

    pair<int, int> Item::getSize() const
    {
        return mSize;
    }

    void Item::setSize(int widthSize, int heightSize)
    {
        mSize = pair<int, int>(widthSize, heightSize);
    }

    void Item::doLoose()
    {
        if (mActor != NULL)
        {
            mActor->detachFromParent();
        }
        if (mParentContainer != NULL)
        {
            mParentContainer->_doRemoveItem(this);
            mParentContainer = NULL;
        }
        if (mParentSlot != NULL)
        {
            mParentSlot->setItem(NULL);
            mParentSlot = NULL;
        }
        setOwner(NULL);
    }

    void Item::removeOldState()
    {
        setState(GOS_LOADED);
    }

    // --------------- Warning ------------
    // do not change this function without
    // having a look at the containers and slots
    void Item::setState(GameObjectState targetState)
    {
        if (mState == targetState)
        {
            return;
        }

        // do only things that are possible
        if (targetState & (GOS_HELD | GOS_READY | GOS_IN_POSSESSION | GOS_IN_SCENE)
            && mState & (GOS_HELD | GOS_READY | GOS_IN_POSSESSION | GOS_IN_SCENE))
        {
            LOG_ERROR(Logger::RULES,
                "Item '" + getName() + "' could not change state from '" + Ogre::StringConverter::toString(mState)
                    + "' to state '" + Ogre::StringConverter::toString(targetState) + "'!"
                    + "\nYou need to call 'Item::removeOldState()' first.");

            // first remove the old state (thats a recursive function call)
            // removeOldState();
        }

        // everything else is not handled here, so give it to the parent
        if ((targetState == GOS_IN_SCENE || targetState == GOS_LOADED)
            && (mState == GOS_IN_SCENE || mState == GOS_LOADED))
        {
            GameObject::setState(targetState);
            return;
        }

        GameObjectState oldState = mState;
        onBeforeStateChange(oldState, targetState);

        if (targetState == GOS_LOADED && (mState == GOS_HELD || mState == GOS_READY || GOS_IN_POSSESSION))
        {
            mState = targetState; // this is needed here to prevent an endless recursion
            // "remove old state"
            doLoose();
            destroyActor();
        }
        else if (mState == GOS_LOADED && (targetState == GOS_HELD || targetState == GOS_READY))
        {
            createActor();
            mState = targetState;
            // do nothing, the user has to do what he needs himself
        }
        else if ((mState == GOS_LOADED && targetState == GOS_IN_POSSESSION)
            || (mState == GOS_IN_POSSESSION && targetState == GOS_LOADED))
        {
            mState = targetState;
        }
        else
        {
            Throw(rl::IllegalStateException, "Unhandled state change");
        }

        onAfterStateChange(oldState, targetState);
        GameObjectManager::getSingleton().gameObjectStateChanged(this, oldState, targetState);
    }

    void Item::setProperty(const CeGuiString& key, const Property& value)
    {
        if (key == Item::PROPERTY_SIZE)
        {
            mSize = value.toIntPair();
        }
        else if (key == Item::PROPERTY_ITEMTYPE)
        {
            mItemType = static_cast<Item::ItemType>(value.toInt());
        }
        else if (key == Item::PROPERTY_SUBMESHNAME)
        {
            mSubmeshName = value.toString();
        }
        else
        {
            GameObject::setProperty(key, value);
        }
    }

    const Property Item::getProperty(const CeGuiString& key) const
    {
        if (key == Item::PROPERTY_SIZE)
        {
            return Property(mSize);
        }
        else if (key == Item::PROPERTY_ITEMTYPE)
        {
            return Property(mItemType);
        }
        else if (key == Item::PROPERTY_SUBMESHNAME)
        {
            return Property(mSubmeshName);
        }
        else
        {
            return GameObject::getProperty(key);
        }
    }

    PropertyKeys Item::getAllPropertyKeys() const
    {
        PropertyKeys keys(GameObject::getAllPropertyKeys());
        keys.insert(Item::PROPERTY_SUBMESHNAME);
        keys.insert(Item::PROPERTY_SIZE);
        keys.insert(Item::PROPERTY_ITEMTYPE);
        return keys;
    }

    void Item::setOwner(GameObject* owner)
    {
        mOwner = owner;
    }

    GameObject* Item::getOwner() const
    {
        return mOwner;
    }

    void Item::setParentSlot(Slot* slot)
    {
        mParentSlot = slot;
    }

    Slot* Item::getParentSlot() const
    {
        return mParentSlot;
    }

    void Item::doCreateActor()
    {
        setActor(createActor());
    }

    void Item::setParentContainer(Container* cont)
    {
        mParentContainer = cont;
    }

    Container* Item::getParentContainer() const
    {
        return mParentContainer;
    }

    const CeGuiString& Item::getSubmeshName() const
    {
        return mSubmeshName;
    }

    void Item::setSubmeshName(const CeGuiString& name)
    {
        mSubmeshName = name;
    }
}
