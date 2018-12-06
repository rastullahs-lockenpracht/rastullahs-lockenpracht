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

/**
 * \file PropertiesWindow.h
 *
 * Declaration of the Property window
 */

#ifndef __PropertiesWindow_H__
#define __PropertiesWindow_H__

#include "AbstractWindow.h"
#include "UiPrerequisites.h"

namespace rl
{

    class Property;
    class PropertyHolder;

    /**
     * The Property window class
     *
     * This window displays all properties of a game object by looping
     * through the property set of the given object.
     */
    class _RlUiExport PropertiesWindow : public AbstractWindow
    {
    public:
        /**
         * Constructor of the Property window
         */
        PropertiesWindow(PropertyHolder* ph);

        /**
         * Destructor of the Property window
         */
        virtual ~PropertiesWindow();

        /**
         * Refreshes the shown data
         */
        void update();

    private:
        /**
         * Initializes the window with the specified game object and
         * fills the window with the properties of the PropertyHolder.
         *
         * @param object The PropertyHolder to be displayed
         */
        void initialize(PropertyHolder* object);

        /**
         * Fills the header part of the window with the Name and Description
         * property of the PropertyHolder.
         *
         * @param object The PropertyHolder to be displayed
         */
        void fillHeader(PropertyHolder* object);

        /**
         * Adds an Int value to the property table.
         *
         * @param key The property to add.
         */
        void addPropertyInt(const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key = "");

        /**
         * Adds the IntPair values to the property table.
         *
         * @param key The property to add.
         */
        void addPropertyIntPair(const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key = "");

        /**
         * Adds the IntTriple values to the property table.
         *
         * @param key The property to add.
         */
        void addPropertyIntTriple(const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key = "");

        /**
         * Adds a String value to the property table.
         *
         * @param key The property to add.
         */
        void addPropertyString(const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key = "");

        /**
         * Adds a Bool value to the property table.
         *
         * @param key The property to add.
         */
        void addPropertyBool(const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key = "");

        /**
         * Adds a Real value to the property table.
         *
         * @param key The property to add.
         */
        void addPropertyReal(const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key = "");

        /**
         * Adds the Vector3 values to the property table.
         *
         * @param key The property to add.
         */
        void addPropertyArray3(const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key = "");

        /**
         * Adds the Quaternion values to the property table.
         *
         * @param key The property to add.
         */
        void addPropertyQuaternion(const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key = "");

        /**
         * Adds a new tab to the tabpane and lists
         * the array values.
         *
         * @param key The property to add.
         */
        void addPropertyArray(const Property& prop, const CeGuiString& key = "");

        CEGUI::TabControl* mTabPane; //!< TabControl for the property tabs
        CEGUI::MultiColumnList* mMainTable; //!< Main table for the PropertyHolder properties
        PropertyHolder* mObject;
    };
}

#endif
