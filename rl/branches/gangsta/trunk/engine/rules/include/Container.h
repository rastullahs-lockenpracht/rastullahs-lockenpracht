/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
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
        Container(int id, const CeGuiString& name, const CeGuiString& description);
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

    private:
        int mCapacity;
        ItemSet mItems;
    };
}

#endif
