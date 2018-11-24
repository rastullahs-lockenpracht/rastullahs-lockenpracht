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
#ifndef __ItemDragContainer_H__
#define __ItemDragContainer_H__

#include "UiPrerequisites.h"

#include <CEGUI/widgets/DragContainer.h>

namespace rl
{
    class Container;
    class Inventory;
    class Item;
    class Slot;
    class ItemDragContainerDestroyListener;

    class ItemDragContainer : public CEGUI::DragContainer
    {
    public:
        static const Ogre::String ICON_UNKNOWN_ITEM;

        virtual ~ItemDragContainer();

        void setItemParent(Container* container);
        void setItemParent(Inventory* inventory, const CeGuiString& slotname);
        Container* getItemParentContainer() const;
        const CeGuiString& getItemParentSlot() const;
        Inventory* getItemParentInventory() const;
        Item* getItem() const;
        virtual void setItem(Item*); // can only be used once to initialize the Container
        CEGUI::Window* getContentWindow() const;

        bool _handleItemMouseClick(const CEGUI::EventArgs& evt, Item* item);
        bool _handleItemDoubleClick(const CEGUI::EventArgs& evt, Item* item);

        bool fadeOutAndHide(Ogre::Real delay);
        bool stopFadeOut();

        virtual void destroy(void); // inherited from CEGUI::WINDOW

        // void destroyWindow(); // this is internally done with a windowfadejob to prevent problems

        void setDestroyListener(ItemDragContainerDestroyListener* listener);

    protected:
        CEGUI::Window* mContentWindow;

        ItemDragContainer(const CeGuiString& type, const CeGuiString& name);

    private:
        Item* mItem;
        Container* mParentContainer;
        CeGuiString mParentSlot;
        Inventory* mInventory;
        unsigned long mHideAndDestroyJobTicket;
        bool mMarkedToHideAndDestroy;
        ItemDragContainerDestroyListener* mDestroyListener;
    };

    class ItemDragContainerDestroyListener
    {
    public:
        ItemDragContainerDestroyListener();
        virtual ~ItemDragContainerDestroyListener();
        virtual void notifyItemDragContainerDestroyed(ItemDragContainer* cont) = 0;

    protected:
        void addDragContainer(ItemDragContainer* dragcont);
        void removeDragContainer(ItemDragContainer* dragcont);
        friend class ItemDragContainer;

    private:
        typedef std::map<CeGuiString, ItemDragContainer*> DndContainerMap;
        DndContainerMap mContainers;
        bool mIsDestroying;
    };

} // namespace rl

#endif //__ItemDragContainer_H__
