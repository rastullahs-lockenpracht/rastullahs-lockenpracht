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

#include "Item.h"

#include "Actor.h"
#include "Exception.h"

using namespace std;

namespace rl
{
    const Ogre::String Item::CLASS_NAME = "Item";

    const Ogre::String Item::PROPERTY_IMAGENAME = "imagename";
    const Ogre::String Item::PROPERTY_ITEMTYPE = "itemtype";
    const Ogre::String Item::PROPERTY_SIZE = "size";

    Item::Item(unsigned int id)
        : GameObject(id),
		mWeight(0),
		mItemType(ITEMTYPE_OTHER),
		mImageName(""),
		mSize(pair<int,int>(1,1))
    {
        mQueryFlags |= QUERYFLAG_ITEM;
    }

    Item::~Item(void)
    {
    }
	
    void Item::setWeight(Ogre::Real weight)
    {
        mWeight = weight;
    }

    Ogre::Real Item::getWeight() const
    {
        return mWeight;
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

	void Item::setPrice(int price)
	{
		mPrice = price;
	}
	
	int Item::getPrice() const
	{
		return mPrice;
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

	void Item::hold()
	{
		setState(GOS_HELD);
	}

    void Item::doHold()
    {
        setActor(createActor());
        mState = GOS_HELD;
    }

    void Item::setState(GameObjectState targetstate)
    {
        if (mState == targetstate)
        {
            return;
        }

        bool stateChanged = false;

        if (targetstate == GOS_IN_POSSESSION)
        {
            if (mState == GOS_LOADED)
            {
                stateChanged = true;
            }
            else if (mState == GOS_IN_SCENE)
            {
                doRemoveFromScene();
                stateChanged = true;
            }
            else if (mState == GOS_HELD)
            {
                mActor->detachFromParent();
                destroyActor();
                stateChanged = true;
            }
        }
        else if (targetstate == GOS_LOADED)
        {
            if (mState == GOS_IN_POSSESSION)
            {
                stateChanged = true;
            }
            if (mState == GOS_HELD)
            {
                mActor->detachFromParent();
                destroyActor();
                stateChanged = true;
            }
        }
        else if (targetstate == GOS_IN_SCENE)
        {
            if (mState == GOS_IN_POSSESSION)
            {
                doRemoveFromScene();
                stateChanged = true;
            }
            if (mState == GOS_HELD)
            {
                mActor->detachFromParent();
                stateChanged = true;
            }
        }
        else if (targetstate == GOS_HELD)
        {
            if (mState == GOS_LOADED)
            {
                doHold();
                stateChanged = true;
            }
            else if (mState == GOS_IN_SCENE)
            {
                mActor->removeFromScene();
                doHold();
                stateChanged = true;
            }
            else if (mState == GOS_IN_POSSESSION)
            {
                doHold();
                stateChanged = true;
            }
        }
        else if (targetstate == GOS_READY)
        {
            ///@todo
        }

        if (stateChanged)
        {
            mState = targetstate;   
        }
        else
        {
            GameObject::setState(targetstate);
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

    PropertySet* Item::getAllProperties() const
    {
        PropertySet* ps = GameObject::getAllProperties();
        ps->setProperty(Item::PROPERTY_IMAGENAME, Property(mImageName));
        ps->setProperty(Item::PROPERTY_SIZE, Property(mSize));
        ps->setProperty(Item::PROPERTY_ITEMTYPE, Property(mItemType));

        return ps;
    }
}
