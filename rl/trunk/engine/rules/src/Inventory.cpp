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

#include "Inventory.h"

#include "Item.h"
#include "Armor.h"
#include "Weapon.h"
#include "Exception.h"
#include "ActorManager.h"
#include "Creature.h"
#include "Eigenschaft.h"
#include "Slot.h"
#include "GameObjectManager.h"

using namespace std;

namespace rl
{

	const Ogre::String Inventory::PROPERTY_CONTENT = "content";

    /**
       Just to remember all parts of the inventory

	//Ring links
	//Ring rechts
	//in Hand links
	//in Hand rechts
	//Handschuhe
	//Armreif links
	//Armreif rechts
	//Oberkoerper Ruestung
	//Umhang
	//Armschienen
	//Oberkoerper Ruecken (Rucksack, Schwertscheiden)
	//Guertel  (+Slots fuer Guerteltaschen+Scheiden)
	//Halskette
	//Stirnband, Helm, Diadem
	//Hose
	//Beinschienen
	//Stiefel

    */


	Inventory::Inventory(Creature* owner) :
		mOwner(owner),
        mSlots(),
		mCurrentWeight(0),
		mCurrentBeByWeight(0),
		mCurrentBe(0),
		mCurrentRs(0),
		mValuesUpToDate(false)
	{
	}

	Inventory::~Inventory()
	{
	}

	void Inventory::markDirty()
	{
		mValuesUpToDate = false;
	}

	Creature* Inventory::getOwner() const
	{
		return mOwner;
	}

	Inventory::ItemList Inventory::getAllItems() const
	{
		ItemList allItems(0);

        for (SlotMap::const_iterator iter = mSlots.begin(); iter != mSlots.end(); ++iter)
        {
            Item* item = (*iter).second->getItem();
            if (item != NULL)
            {
                allItems.push_back(item);
            }
        }

		return allItems;
	}

	int Inventory::getOverallWeight()
	{
		if (!mValuesUpToDate)
			updateStats();
		return mCurrentWeight;
	}

	pair<int,int> Inventory::getOverallBe()
	{
		if (!mValuesUpToDate)
			updateStats();
		return make_pair<int,int>(mCurrentBe, mCurrentBeByWeight);
	}

	int Inventory::getOverallRs()
	{
		if (!mValuesUpToDate)
			updateStats();
		return mCurrentRs;
	}

	void Inventory::updateStats()
	{
		calculateWeight(getAllItems());
		calculateRsAndBe();
		// flag setzen: Werte sind up to Date
		mValuesUpToDate = true;
	}

	// Berechnungsmethoden
	void Inventory::calculateWeight(ItemList items)
	{
		ItemList::iterator it = items.begin();

		Ogre::Real totalWeight = 0.0;

		while (it != items.end())
		{
			totalWeight += (*it)->getMass();

			it++;
		}
		mCurrentWeight = totalWeight;
	}

	void Inventory::calculateRsAndBe()
	{
		mCurrentBe = 0;
		mCurrentRs = 0;

		// Behinderung durch Traglast ist 1 Punkt pro KK * 40 Unzen / 2 ab KK Stein gewicht
		//mCurrentBeByWeight = (mCurrentWeight > mOwner->getEigenschaft(E_KOERPERKRAFT) * 40)?
		//	(mCurrentWeight / (mOwner->getEigenschaft(E_KOERPERKRAFT) * 20) - 1) :
		//	0;

		//ItemList wornItems = getWornItems();

		//ItemList::iterator it = wornItems.begin();
		//while (it != wornItems.end())
		//{
		//	// Beachte nur Rstungen
		//	if (dynamic_cast<Armor*>(*it) != 0)
		//	{
		//		mCurrentRs += (dynamic_cast<Armor*>(*it))->getGRS();
		//		mCurrentBe += (dynamic_cast<Armor*>(*it))->getGBE();
		//	}
		//	it++;
		//}
	}

	void Inventory::dropItem(const CeGuiString& slotName)
	{
		std::map<CeGuiString, Slot*>::iterator slotIter = mSlots.find(slotName);
        if (slotIter == mSlots.end())
        {
            Throw(rl::IllegalArgumentException, Ogre::String("Slot '")+slotName.c_str()+"' doesn't exist.");
        }
        //slotIter->second->getItem()->setOwner(NULL);
        slotIter->second->setItem(NULL);

	}

    void Inventory::hold(Item* item, const CeGuiString& slotName)
    {
        std::map<CeGuiString, Slot*>::iterator slotIter = mSlots.find(slotName);
        if (slotIter == mSlots.end())
        {
            Throw(rl::IllegalArgumentException, Ogre::String("Slot '")+slotName.c_str()+"' doesn't exist.");
        }

        slotIter->second->setItem(item);
        //item->setOwner(getOwner());
    }

	bool Inventory::canHold(Item* item, const CeGuiString& slotName) const
    {
        std::map<CeGuiString, Slot*>::const_iterator slotIter = mSlots.find(slotName);
        if (slotIter == mSlots.end())
        {
            Throw(rl::IllegalArgumentException, Ogre::String("Slot '")+slotName.c_str()+"' doesn't exist.");
        }

		return slotIter->second->isAllowed(item);
    }

	bool Inventory::canReady(Item* item, const CeGuiString& slotName) const
    {
        std::map<CeGuiString, Slot*>::const_iterator slotIter = mSlots.find(slotName);
        if (slotIter == mSlots.end())
        {
            Throw(rl::IllegalArgumentException, Ogre::String("Slot '")+slotName.c_str()+"' doesn't exist.");
        }

		return slotIter->second->canReady(item);
    }

    Item* Inventory::getItem(const CeGuiString& slotName) const
    {
        std::map<CeGuiString, Slot*>::const_iterator slotIter = mSlots.find(slotName);
        if (slotIter == mSlots.end())
        {
            Throw(rl::IllegalArgumentException, Ogre::String("Slot '")+slotName.c_str()+"' doesn't exist.");
        }

        return (*slotIter).second->getItem();
    }

    void Inventory::ready(Item* item)
    {
    }

    void Inventory::addSlot(const CeGuiString& name, const Ogre::String& meshpartname, int itemReadyMask, int itemHeldMask, SlotType type)
    {
        if (mSlots.find(name) != mSlots.end())
        {
            Throw(rl::IllegalArgumentException, Ogre::String("Slot '")+name.c_str()+"' already exists.");
        }

		switch (type)
        {
            case SLOT_BONE:
			    mSlots[name] = new BoneSlot(mOwner, name, itemReadyMask, itemHeldMask, meshpartname);
                break;
            case SLOT_SUBMESH:
			    mSlots[name] = new SubmeshSlot(mOwner, name, itemReadyMask, itemHeldMask, meshpartname);
                break;
            case SLOT_MATERIAL:
			    mSlots[name] = new MaterialSlot(mOwner, name, itemReadyMask, itemHeldMask, meshpartname);
                break;
		}
    }

    const Inventory::SlotMap& Inventory::getAllSlots() const
    {
        return mSlots;
    }

	const Property Inventory::getProperty(const Ogre::String& key) const
	{
		Property prop;
		if (key == Inventory::PROPERTY_CONTENT)
		{
			PropertyMap contentProp;

			SlotMap slots = getAllSlots();
			for (SlotMap::const_iterator it = slots.begin(); it != slots.end(); ++it)
			{
				Slot* curSlot = (*it).second;
				if (curSlot->getItem())
				{
					contentProp[(*it).first] =
						GameObjectManager::getSingleton().toProperty(curSlot->getItem());
				}
			}

			prop.setValue(contentProp);
		}
		return prop;
	}

	void Inventory::setProperty(const Ogre::String& key, const Property& value)
	{
		if (key == Inventory::PROPERTY_CONTENT)
		{
			PropertyMap bonesContent = value.toMap();
			for (PropertyMap::const_iterator it = bonesContent.begin();
				it != bonesContent.end(); ++it)
			{
				Item* item = dynamic_cast<Item*>(
					GameObjectManager::getSingleton().createGameObjectFromProperty(
						(*it).second));
				if (item)
				{
					hold(item, (*it).first);
				}
			}
		}
	}

    PropertySet* Inventory::getAllProperties() const
	{
		PropertySet* ps = new PropertySet();
		ps->setProperty(Inventory::PROPERTY_CONTENT, getProperty(Inventory::PROPERTY_CONTENT));
		return ps;
	}

}
