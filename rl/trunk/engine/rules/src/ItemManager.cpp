/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
	

#include "ItemManager.h"
#include "ItemDataLoader.h"

#include "Item.h"
#include "Weapon.h"
#include "Armor.h"
#include "Exception.h"

#include "ActorManager.h"


using namespace XERCES_CPP_NAMESPACE;

using Ogre::Singleton;
using Ogre::StringConverter;

template<> rl::ItemManager* Singleton<rl::ItemManager>::ms_Singleton = 0;

namespace rl {


	ItemManager& ItemManager::getSingleton()
	{
		return Singleton<ItemManager>::getSingleton();
	}
	ItemManager* ItemManager::getSingletonPtr()
	{
		return Singleton<ItemManager>::getSingletonPtr();
	}

	ItemManager::ItemManager() :
		mAvailableItems(),
		mItems()
	{
	}

	ItemManager::~ItemManager()
	{
	}

	Item* ItemManager::createItem(const CeGuiString &uniqueName)
	{
		if (mAvailableItems.find(uniqueName) == mAvailableItems.end())
		{
			// Keine Iteminfos vorhanden
			Throw(IllegalStateException, "Keine Iteminfos zu Item vorhanden!");
		}

		// Suche Eintrag in der Item-Config Liste
		Item* prototype = mAvailableItems[uniqueName];
		Item* newItem = prototype->clone();

		try
		{
			if (newItem->getMeshName().length() > 0){
				newItem->setActor(ActorManager::getSingleton().createMeshActor(
					Ogre::StringConverter::toString(newItem->getId())+Ogre::String(newItem->getName().c_str()), 
					Ogre::String(newItem->getMeshName().c_str())));
			}
		}
		catch (...)
		{
			Logger::getSingleton().log("ItemManager",Logger::LL_MESSAGE,"Item "+newItem->getName()+" hat keine gültige Meshinformationen! Kein Aktor wird erstellt");
		}


		// speichere Item in Map
		mItems[newItem->getId()] = newItem;

		return newItem;
	}


	void ItemManager::deleteItem(int id)
	{
		ItemsInGame::iterator it = mItems.find(id);

		if (it != mItems.end())
		{
            Item* item = it->second;
            if (item->getActor())
            {
                ActorManager::getSingleton().destroyActor(item->getActor());
            }
	    	delete item;
		}
        else
        {
            Throw(IllegalArgumentException,
                String("No item with ID ") + StringConverter::toString(id) + " found.");
        }
	}

	void ItemManager::_addItem(rl::Item *item)
	{
		mAvailableItems.insert(make_pair(item->getName(), item));
	}

	void ItemManager::loadItemDataFile(const std::string& filename)
	{
		ItemDataLoader::loadData(filename);
	}
}
