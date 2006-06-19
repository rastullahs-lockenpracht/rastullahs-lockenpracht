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

#ifndef __ITEMMANAGER_H__
#define __ITEMMANAGER_H__

#include "RulesPrerequisites.h"

#include <OgreSingleton.h>

namespace rl {

	class Item;

	class _RlRulesExport ItemManager : public Ogre::Singleton<ItemManager>
	{
	public:
		static ItemManager& getSingleton();
		static ItemManager* getSingletonPtr();
		
		ItemManager();
		~ItemManager();

		Item* createItem(const CeGuiString &uniqueName);

		void deleteItem(int id);

		void _addItem(Item* item);


		void ItemManager::loadItemDataFile(const std::string& filename);


	private:
		/**
		* Alle Items, die der Manager erzeugen kann werdein in dieser Map gespeichert
		*/
		typedef std::map<CeGuiString, Item*> AvailableItems;

		typedef std::map<int, Item*> ItemsInGame;



		/**
		* @brief Enthält zu jedem String eine Propertymap, die die Eigenschaften des
		* Items speichert
		* 
		* 
		*/
		AvailableItems mAvailableItems;

		/**
		* @brief Enthält die Items im Spiel
		*
		*/
		ItemsInGame mItems;
	};
}
#endif //__ITEMMANAGER_H__