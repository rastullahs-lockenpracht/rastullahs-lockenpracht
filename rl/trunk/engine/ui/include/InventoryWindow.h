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

#ifndef __InventoryWindow_H__
#define __InventoryWindow_H__

#include "UiPrerequisites.h"
#include "CeGuiWindow.h"
#include "Inventory.h"
#include "GameTask.h"

namespace rl {

	class Creature;
    class Item;

	class _RlUiExport InventoryArrangeTask : public GameTask
	{
	public:
		InventoryArrangeTask();
		~InventoryArrangeTask();
		
		void run(Ogre::Real elapsedTime);

        virtual const Ogre::String& getName() const;
	};

	class _RlUiExport InventoryWindow : public CeGuiWindow
	{
	public:
		InventoryWindow();
		~InventoryWindow();

		// Flag für den ArrangeTask - FIXME nach private umziehen, wenn 
		CEGUI::DragContainer* mDroppedItem;

		/**
		* Gibt dem Fenster das anzuzeigende Inventar
		*/
		void setInventory(Inventory* inventory);

		/**
		* Methode für das OnMouseOver Event
		*/
		bool handleMouseEnters(const CEGUI::EventArgs &args);
		
		/**
		* Methode für das OnMouseLeaves Event
		*/
		bool handleMouseLeaves(const CEGUI::EventArgs &args);

		/**
		* Methode für das OnMouseClick Event
		*/
		bool handleMouseClicked(const CEGUI::EventArgs &args);

		/**
		* 
		*/
		void update();

		/**
		* Übergibt änderungen ans Inventar
		*/
		void updateInventory();

		/**
		* Passt die Position eines gedroppten Items an, da CEGUI Bug, braucht man 
		* dafür ne externe Methode
		*/
		void updateItemPosition();

		// FIXME, wenn BUG im CEGUI behoben ist: entfernen

		/**
		* Schaut nach, ob das Item an der Position gedroppt werden kann.
		*/
		bool isFreeInContainer(Item* item, pair<int,int> kaestechenPos, Container* container);


		Item* getGroundItem();

	private:
		Inventory* mInventory;

		// Farbschema für die Fenster und Itemhintergründe
		const CeGuiString mColorAccept;
		const CeGuiString mColorReject;
		const CeGuiString mColorNormal;
		const CeGuiString mColorItemNormal;
		const CeGuiString mColorItemSelected;

		// TODO : Ersetze durch etwas dynamisches
		Item* mGroundItem;

		// Der Knoten an dem das zu redernde Item angebracht wird;
		Ogre::SceneNode* mInventoryItemNode;
		// Entity des jeweils zu rendernden Items
		Ogre::Entity* mRenderItemEntity;
		// 128*128 Textur für das Item-view Fenster
		Ogre::RenderTexture* mRenderTexture;
		// Viewport des RenderToTexture Features
		Ogre::Viewport* mRenderViewport;
		// Die Camera, die das Item filmt
		Actor* mCameraActor;

		CEGUI::Texture* mTexture;
		CEGUI::Imageset* mImageSet;

        InventoryArrangeTask* mInventoryArrangeTask;

		// Das Item-Renderfenster
		CEGUI::Window* mItemRenderImage;


		// Das Rüstungs- und Behinderungswert Fenster
		CEGUI::Window* mArmorValueWindow;
		CEGUI::Window* mArmorValue;
		CEGUI::Window* mHandicapValue;

		// Das Gewichtsfenster
		CEGUI::Window* mWeightWindow;
		CEGUI::Window* mTotalWeight;
		

		// Das Item-Beschreibungsfenster
		CEGUI::ScrollablePane* mDescription;

		// Das jeweils aktive Fenster, das im Beschreibungsfenster angewählt ist
		CEGUI::Window* mActiveItemWindow;

		// Das Fenster, in das der Rucksackcontainer soll
		CEGUI::TabControl* mContainerTabs;

		// Die Containerslots (besonders, da alle Arten von Items akzeptiert)
		std::list<CEGUI::Window*> mContainerContents;

		std::list<CEGUI::Window*> mContainers;

		CEGUI::Window* mGroundTab;
		CEGUI::Window* mGroundContainer;

		const std::pair<int,int> mGroundDimension;

		// Alle verschiedenen Inventoryslots
		CEGUI::Window* mRingLeft;
		CEGUI::Window* mRingRight;
		CEGUI::Window* mHandLeft;
		CEGUI::Window* mHandRight;
		CEGUI::Window* mGloves;
		CEGUI::Window* mBraceletLeft;
		CEGUI::Window* mBraceletRight;
		CEGUI::Window* mArmor;
		CEGUI::Window* mCape;
		CEGUI::Window* mBracers;
		CEGUI::Window* mBackpack;
		CEGUI::Window* mBelt;
		CEGUI::Window* mNecklace;
		CEGUI::Window* mHelmet;
		CEGUI::Window* mTrousers;
		CEGUI::Window* mShinbone;
		CEGUI::Window* mBoots;

		CEGUI::Point mPosDraggedTo;
		CEGUI::Window* mContainerDraggedTo;
		CEGUI::DragContainer* mOldItemInSlot;
        
        CEGUI::DragContainer* createItem(Item* item, CEGUI::Window* parent, CEGUI::UVector2 position = CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim(0.0f)));

		/**
		* Schliesst das Fenster bei Klick auf X
		*/
		bool handleClose();

		void initRenderToTexture();

		/**
		* Lädt die Rtt-Camera
		*/
		void loadCamera();
		void unloadCamera();

		/**
		* Rendert das Item in das Vorschaufenster im Inventar
		*/
		void renderItem(Item* item);

		/**
		* Initiiert die Slots des Inventars
		*/
		void initSlots();

		/**
		* Füllt die Slots im InventarFenster mit den Items aus dem Inventar
		*/
		void fillSlots();

		/**
		* Erzeugt ein Tab für den Container, und befuellt ihn nach Inhalt des container - Items
		*/
		void createAndFillContainer(Container* container);

		/**
		* entfernt ein ContainerTab mitsamt Inhalt aus dem Inventar
		*/
		void removeContainerAndContent(Container* container);

		/**
		* Entleert die Slots im InventarFenster wieder
		*/
		void emptySlots();


		void refreshTabs();

		/**
		* Sucht unter den Containern dasjenige, das zum gesuchten Container gehört
		*/
		CEGUI::Window* findContainer(Container* container);

		/**
		*
		* Entfernt alle Unterfenster
		*/
		void emptySlot(CEGUI::Window* slot);
		/** 
		* Gibt dem Slot die Fähigkeit Dragcontainer zu akzeptieren
		*/
		void addDropListener(CEGUI::Window* slot);

		/*!
		* fügt dem Inventar den Container für den Rucksack zu,
		* gibt ihm die nötige Funktionalität für drag&drop
		*/
		void initBackpack(std::pair<int,int> dim);


		/**
		* erzeugt für alle Items im Inventar, die Containerfunktionaltiaet haben, ein Containertab
		* im Inventar
		*/
		void createContainerWindows();

		/**
		* Überprüft, ob das gedroppte Item vom Slot akzeptiert wird oder nicht
		* @return true: Item wird akzeptiert
		*         false: Item wird nicht akzeptiert
		*/ 
		bool checkTypeAccepted(CEGUI::Window* window, CEGUI::DragContainer* draggedItem);


		/**
		* Errechnet die Kästchenposition, an der das Item im Container gedroppt wurde
		*/
		std::pair<int,int> calculateNewPosition(const CEGUI::DragDropEventArgs& ddea);

		bool handleDragEnter(const CEGUI::EventArgs& args);
		bool handleDragLeave(const CEGUI::EventArgs& args);
		bool handleDragDropped(const CEGUI::EventArgs& args);
	};
}
#endif
