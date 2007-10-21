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

#include "Item.h"

#include "Actor.h"
#include "Container.h"
#include "Exception.h"
#include "Slot.h"

using namespace std;

namespace rl
{
    const Ogre::String Item::CLASS_NAME = "Item";

    const Ogre::String Item::PROPERTY_IMAGENAME = "imagename";
    const Ogre::String Item::PROPERTY_ITEMTYPE = "itemtype";
    const Ogre::String Item::PROPERTY_SIZE = "size";

    Item::Item(unsigned int id)
        : GameObject(id),
		mItemType(ITEMTYPE_OTHER),
		mImageName(""),
		mSize(pair<int,int>(1,1)),
        mOwner(NULL),
        mParentSlot(NULL),
        mParentContainer(NULL)
    {
        mQueryFlags |= QUERYFLAG_ITEM;
    }

    Item::~Item(void)
    {
    }

	void Item::setItemType(ItemType itemType)
	{
		mItemType = itemType;
	}

	Item::ItemType Item::getItemType() const
	{
		return mItemType;
	}

	void Item::setImageName(const CeGuiString name)
	{
		mImageName = name;
	}

	const CeGuiString Item::getImageName() const
	{
		return mImageName;
	}

	bool Item::isContainer() const
	{
		return false;
	}

	pair<int,int> Item::getSize() const
	{
		return mSize;
	}

	void Item::setSize(int widthSize,int heightSize)
	{
		mSize = pair<int,int>(widthSize,heightSize);
	}

    void Item::doHold()
    {
        setActor(createActor());
        mState = GOS_HELD;
    }

    void Item::doLoose()
    {
        if (mActor != NULL)
        {
            mActor->detachFromParent();
            destroyActor();
        }
    }

    void Item::removeOldState()
    {
        setState(GOS_LOADED);
    }

// --------------- Warning ------------
// do not change this function without
// having a look at the containers and slots
    void Item::setState(GameObjectState targetstate)
    {
        if (mState == targetstate)
        {
            return;
        }

        GameObjectState oldState = mState;


        if( targetstate != GOS_HELD &&
            targetstate != GOS_IN_POSSESSION &&
            targetstate != GOS_READY )
        {
            if( mState == GOS_HELD ||
                mState == GOS_IN_POSSESSION ||
                mState == GOS_READY )
            {
                mState = GOS_LOADED; // <- this is important to avoid endless recursion!
                onStateChange(oldState, mState);

                if( getParentSlot() )
                    getParentSlot()->setItem(NULL);
                setParentSlot(NULL);
                if( getParentContainer() )
                    getParentContainer()->removeItem(this);
                setParentContainer(NULL);
                doLoose();
                setOwner(NULL);
            }

            GameObject::setState(targetstate);
        }
        else
        {
            if( mState == GOS_HELD ||
                mState == GOS_IN_POSSESSION ||
                mState == GOS_READY )
            {
                LOG_WARNING(Logger::RULES, 
                    "Item::removeOldState() or Item::setState(GOS_LOADED) should be called  \
                    before setting one of the States GOS_HELD, GOS_IN_POSSESSION or GOS_READY!");
            }
            else
            {
                if( mState != GOS_LOADED )
                {
                    GameObject::setState(GOS_LOADED);
                    oldState = GOS_LOADED;
                }

                mState = targetstate;
                onStateChange(oldState, mState);
            }
        }
    }

    void Item::setProperty(const Ogre::String &key, const rl::Property &value)
    {
        if (key == Item::PROPERTY_IMAGENAME)
        {
            mImageName = value.toString();
        }
        else if (key == Item::PROPERTY_SIZE)
        {
            mSize = value.toIntPair();
        }
        else if (key == Item::PROPERTY_ITEMTYPE)
        {
            mItemType = static_cast<Item::ItemType>(value.toInt());
        }
        else
        {
            GameObject::setProperty(key, value);
        }
    }

    const Property Item::getProperty(const Ogre::String &key) const
    {
        if (key == Item::PROPERTY_IMAGENAME)
        {
            return Property(mImageName);
        }
        else if (key == Item::PROPERTY_SIZE)
        {
            return Property(mSize);
        }
        else if (key == Item::PROPERTY_ITEMTYPE)
        {
            return Property(mItemType);
        }
        else
        {
            return GameObject::getProperty(key);
        }
    }

    PropertyRecord* Item::getAllProperties() const
    {
        PropertyRecord* ps = GameObject::getAllProperties();
        ps->setProperty(Item::PROPERTY_IMAGENAME, Property(mImageName));
        ps->setProperty(Item::PROPERTY_SIZE, Property(mSize));
        ps->setProperty(Item::PROPERTY_ITEMTYPE, Property(mItemType));

        return ps;
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
}
