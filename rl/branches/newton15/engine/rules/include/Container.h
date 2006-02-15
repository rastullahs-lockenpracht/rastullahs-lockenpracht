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

#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include "RulesPrerequisites.h"

#include "GameObject.h"
#include "Item.h"

namespace rl
{
    /// Behaelter für Items.
    class _RlRulesExport Container : public GameObject
    {
    public:
        Container(const CeGuiString& name, const CeGuiString& description);
        virtual ~Container(void);

        /// Fassungsvermoegen in Unzen
        int getCapacity() const;

        /// Fassungsvermoegen in Unzen
        void setCapacity(int capacity);

        /// Liefert Gesamtgewicht des Inhalts.
        int getContentWeight() const;

        void addItem(Item* item);
        Item* removeItem(int itemId);

        ItemSet getItems() const;

		int getItemCount() const;

		const Item* getItem(int itemId) const;

		virtual void close() = 0;

    private:
        int mCapacity;
        ItemSet mItems;
    };
}

#endif
