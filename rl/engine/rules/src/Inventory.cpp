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

#include "Inventory.h"

#include "ActorManager.h"
#include "Armor.h"
#include "Creature.h"
#include "Eigenschaft.h"
#include "Exception.h"
#include "GameObjectManager.h"
#include "Item.h"
#include "Slot.h"
#include "Weapon.h"

using namespace std;

namespace rl
{

    const Ogre::String Inventory::PROPERTY_CONTENT = "content";
    const Ogre::String Inventory::PROPERTY_SLOTS = "slots";

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

    Inventory::Inventory(Creature* owner)
        : mOwner(owner)
        , mSlots()
        , mCurrentWeight(0)
        , mCurrentBeByWeight(0)
        , mCurrentBe(0)
        , mCurrentRs(0)
        , mValuesUpToDate(false)
    {
    }

    Inventory::~Inventory()
    {
        for (SlotMap::iterator iter = mSlots.begin(); iter != mSlots.end(); ++iter)
        {
            delete iter->second;
        }
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

    pair<int, int> Inventory::getOverallBe()
    {
        if (!mValuesUpToDate)
            updateStats();
        return { mCurrentBe, mCurrentBeByWeight };
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
        // mCurrentBeByWeight = (mCurrentWeight > mOwner->getEigenschaft(E_KOERPERKRAFT) * 40)?
        //	(mCurrentWeight / (mOwner->getEigenschaft(E_KOERPERKRAFT) * 20) - 1) :
        //	0;

        // ItemList wornItems = getWornItems();

        // ItemList::iterator it = wornItems.begin();
        // while (it != wornItems.end())
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
            Throw(rl::IllegalArgumentException, Ogre::String("Slot '") + slotName.c_str() + "' doesn't exist.");
        }
        // slotIter->second->getItem()->setOwner(NULL);
        slotIter->second->setItem(NULL);
    }

    void Inventory::hold(Item* item, const CeGuiString& slotName)
    {
        std::map<CeGuiString, Slot*>::iterator slotIter = mSlots.find(slotName);
        if (slotIter == mSlots.end())
        {
            Throw(rl::IllegalArgumentException, Ogre::String("Slot '") + slotName.c_str() + "' doesn't exist.");
        }

        slotIter->second->setItem(item);
        // item->setOwner(getOwner());
    }

    bool Inventory::canHold(const Item* item, const CeGuiString& slotName) const
    {
        std::map<CeGuiString, Slot*>::const_iterator slotIter = mSlots.find(slotName);
        if (slotIter == mSlots.end())
        {
            Throw(rl::IllegalArgumentException, Ogre::String("Slot '") + slotName.c_str() + "' doesn't exist.");
        }

        return slotIter->second->isAllowed(item);
    }

    bool Inventory::canReady(const Item* item, const CeGuiString& slotName) const
    {
        std::map<CeGuiString, Slot*>::const_iterator slotIter = mSlots.find(slotName);
        if (slotIter == mSlots.end())
        {
            Throw(rl::IllegalArgumentException, Ogre::String("Slot '") + slotName.c_str() + "' doesn't exist.");
        }

        return slotIter->second->canReady(item);
    }

    Item* Inventory::getItem(const CeGuiString& slotName) const
    {
        std::map<CeGuiString, Slot*>::const_iterator slotIter = mSlots.find(slotName);
        if (slotIter == mSlots.end())
        {
            Throw(rl::IllegalArgumentException, Ogre::String("Slot '") + slotName.c_str() + "' doesn't exist.");
        }

        return (*slotIter).second->getItem();
    }

    void Inventory::ready(Item* item, const CeGuiString& slotName)
    {
        std::map<CeGuiString, Slot*>::iterator slotIter = mSlots.find(slotName);
        if (slotIter == mSlots.end())
        {
            Throw(rl::IllegalArgumentException, Ogre::String("Slot '") + slotName.c_str() + "' doesn't exist.");
        }

        if (slotIter->second->canReady(item))
        {
            slotIter->second->setItem(item);
        }
        else
        {
            Throw(rl::IllegalArgumentException,
                Ogre::String("Item '") + item->getName().c_str() + "' cannot be readied in slot '" + slotName.c_str()
                    + "'.");
        }
        LOG_MESSAGE("Inventory", "Item " + item->getName() + " readied in slot " + slotName + ".");
    }

    std::vector<Weapon*> Inventory::getReadiedWeapons() const
    {
        std::vector<Weapon*> rval;
        for (SlotMap::const_iterator it = mSlots.begin(); it != mSlots.end(); ++it)
        {
            if (it->second->isReady() && (it->second->getItem()->getItemType() & Item::ITEMTYPE_WEAPON) != 0)
            {
                rval.push_back(dynamic_cast<Weapon*>(it->second->getItem()));
            }
        }
        return rval;
    }

    void Inventory::addSlot(
        const CeGuiString& name, const Ogre::String& meshpartname, int itemReadyMask, int itemHeldMask, SlotType type)
    {
        if (mSlots.find(name) != mSlots.end())
        {
            LOG_ERROR(Logger::RULES, "Slot '" + name + "' already exists.");
        }
        else
        {
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
            case SLOT_DEFAULT:
                mSlots[name] = new Slot(mOwner, name, itemReadyMask, itemHeldMask);
                break;
            }
        }
    }

    const Inventory::SlotMap& Inventory::getAllSlots() const
    {
        return mSlots;
    }

    const Property Inventory::getProperty(const CeGuiString& key) const
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
                    contentProp[(*it).first] = GameObjectManager::getSingleton().toProperty(curSlot->getItem());
                }
            }

            prop.setValue(contentProp);
        }
        return prop;
    }

    void Inventory::setProperty(const CeGuiString& key, const Property& value)
    {
        if (key == Inventory::PROPERTY_SLOTS)
        {
            PropertyArray slotVec = value.toArray();

            for (PropertyArray::const_iterator it = slotVec.begin(); it != slotVec.end(); ++it)
            {
                PropertyMap slotProps = it->toMap();

                CeGuiString name = slotProps["name"].toString();
                int holdItems = Item::ITEMTYPE_ALL_ITEMS;
                if (slotProps.find("holds") != slotProps.end())
                {
                    holdItems = slotProps["holds"].toInt();
                }
                int readyItems = Item::ITEMTYPE_ALL_ITEMS;
                if (slotProps.find("readies") != slotProps.end())
                {
                    readyItems = slotProps["readies"].toInt();
                }

                CeGuiString type = slotProps.find("type")->second.toString();
                if (type == "bone")
                {
                    CeGuiString bone = slotProps["bone"].toString();
                    LOG_MESSAGE("Inventory", "Add bone slot " + bone);
                    addSlot(name, bone.c_str(), readyItems, holdItems, SLOT_BONE);
                }
                else if (type == "submesh")
                {
                    CeGuiString submesh = slotProps["submesh"].toString();
                    LOG_MESSAGE("Inventory", "Add submesh slot " + submesh);
                    addSlot(name, submesh.c_str(), readyItems, holdItems, SLOT_SUBMESH);
                }
                else if (type == "material")
                {
                    CeGuiString submesh = slotProps["submesh"].toString();
                    LOG_MESSAGE("Inventory", "Add material slot " + submesh);
                    addSlot(name, submesh.c_str(), readyItems, holdItems, SLOT_MATERIAL);
                }
                else if (type == "default")
                {
                    LOG_MESSAGE("Inventory", "Add default slot " + name);
                    addSlot(name, "", readyItems, holdItems, SLOT_DEFAULT);
                }
                else
                {
                    LOG_ERROR(Logger::RULES, "Unknown slot type '" + type + "' in inventory properties.");
                }
            }
        }
        else if (key == Inventory::PROPERTY_CONTENT)
        {
            PropertyMap bonesContent = value.toMap();
            for (PropertyMap::const_iterator it = bonesContent.begin(); it != bonesContent.end(); ++it)
            {
                Item* item
                    = dynamic_cast<Item*>(GameObjectManager::getSingleton().createGameObjectFromProperty((*it).second));
                if (item)
                {
                    LOG_MESSAGE("Inventory", "Add item " + it->second.toString() + " to slot " + (*it).first);
                    hold(item, (*it).first);
                }
            }
        }
    }

    PropertyKeys Inventory::getAllPropertyKeys() const
    {
        PropertyKeys keys;
        keys.insert(Inventory::PROPERTY_SLOTS);
        keys.insert(Inventory::PROPERTY_CONTENT);
        return keys;
    }
}
