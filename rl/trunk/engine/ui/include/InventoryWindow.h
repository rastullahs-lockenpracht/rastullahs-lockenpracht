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

#ifndef __InventoryWindow_H__
#define __InventoryWindow_H__

#include "UiPrerequisites.h"
#include "CeGuiWindow.h"
#include "Inventory.h"
#include "GameTask.h"

namespace rl {

	class Creature;

	class _RlUiExport InventoryArrangeTask : public GameTask
	{
	public:
		InventoryArrangeTask();
		~InventoryArrangeTask();
		
		void run(Ogre::Real elapsedTime);
	};

	class _RlUiExport InventoryWindow : public CeGuiWindow
	{
	public:
		InventoryWindow();
		~InventoryWindow();

		void setCharacter(Creature* character);
		
		CEGUI::DragContainer* createItem(CEGUI::String name,Item::ItemType type,pair<int,int> dimension, CEGUI::String imgName);

		void update();
		void updateItemPosition();

		CEGUI::Point posBeforeDrag;
		CEGUI::Point posDraggedTo;
		CEGUI::DragContainer* droppedItem;
		CEGUI::Window* containerDraggedTo;

		CEGUI::String colorAccept;
		CEGUI::String colorReject;
		CEGUI::String colorNormal;

		Inventory* getInventory();


	private:
        Creature* mCharacter;
		Inventory* mInventory;

		// Das Fenster, in das der Rucksackcontainer soll
		CEGUI::Window* mBackpackWindow;
		// Der Rucksackslot (besonders, da alle Arten von Items akzeptiert)
		CEGUI::Window* mBackpackContent;

		// Alle verschiedenen Inventoryslots
		CEGUI::StaticImage* mRingLeft;
		CEGUI::StaticImage* mRingRight;
		CEGUI::StaticImage* mHandLeft;
		CEGUI::StaticImage* mHandRight;
		CEGUI::StaticImage* mGloves;
		CEGUI::StaticImage* mBraceletLeft;
		CEGUI::StaticImage* mBraceletRight;
		CEGUI::StaticImage* mArmor;
		CEGUI::StaticImage* mBracerRight;
		CEGUI::StaticImage* mBracerLeft;
		CEGUI::StaticImage* mBackpack;
		CEGUI::StaticImage* mBelt;
		CEGUI::StaticImage* mNecklace;
		CEGUI::StaticImage* mHelmet;
		CEGUI::StaticImage* mTrousers;
		CEGUI::StaticImage* mShinboneLeft;
		CEGUI::StaticImage* mShinboneRight;
		CEGUI::StaticImage* mBoots;

		/**
		* Initiiert die Slots des Inventars
		*/
		void initSlots();

		/** 
		* Gibt dem Slot die Fähigkeit Dragcontainer zu akzeptieren
		*/
		void addDropListener(CEGUI::Window* slot);

		/*!
		* fügt dem Inventar den Container für den Rucksack zu,
		* gibt ihm die nötige Funktionalität für drag&drop
		*/
		void initBackpack(pair<int,int> dim);
	};
}
#endif
