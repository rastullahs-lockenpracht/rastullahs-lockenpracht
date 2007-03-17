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

/**
 * \file GameObjectWindow.h
 *
 * Declaration of the GameObject window
 */

#ifndef __GameObjectWindow_H__
#define __GameObjectWindow_H__

#include "UiPrerequisites.h"
#include "AbstractWindow.h"

namespace rl {

	class GameObject;
    class Property;

    /**
     * The GameObject window class
     *
     * This window displays all properties of a game object by looping
     * through the property set of the given object.
     */
    class _RlUiExport GameObjectWindow : public AbstractWindow
	{
	public:

        /**
         * Constructor of the GameObject window
         */
		GameObjectWindow();
		
        /**
         * Destructor of the GameObject window
         */
        virtual ~GameObjectWindow();

        /**
         * Initializes the window with the specified game object and
         * fills the window with the properties of the GameObject.
         *
         * @param object The GameObject to be displayed
         */
		virtual void initialize(GameObject* object);
	
	protected:

        /**
         * Fills the header part of the window with the Name and Description
         * property of the GameObject.
         *
         * @param object The GameObject to be displayed
         */
        virtual void fillHeader(GameObject* object);
 
        /**
         * Adds an Int value to the property table.
         *
         * @param key The property to add.
         */
        virtual void addPropertyInt(const Property& prop, 
            CEGUI::MultiColumnList* table,
            const Ogre::String& key = "");

        /**
         * Adds the IntPair values to the property table.
         *
         * @param key The property to add.
         */
        virtual void addPropertyIntPair(const Property& prop, 
            CEGUI::MultiColumnList* table,
            const Ogre::String& key = "");

        /**
         * Adds the IntTriple values to the property table.
         *
         * @param key The property to add.
         */
        virtual void addPropertyIntTriple(const Property& prop, 
            CEGUI::MultiColumnList* table,
            const Ogre::String& key = "");

        /**
         * Adds a String value to the property table.
         *
         * @param key The property to add.
         */
        virtual void addPropertyString(const Property& prop, 
            CEGUI::MultiColumnList* table,
            const Ogre::String& key = "");

        /**
         * Adds a Bool value to the property table.
         *
         * @param key The property to add.
         */
        virtual void addPropertyBool(const Property& prop, 
            CEGUI::MultiColumnList* table,
            const Ogre::String& key = "");

        /**
         * Adds a Real value to the property table.
         *
         * @param key The property to add.
         */
        virtual void addPropertyReal(const Property& prop, 
            CEGUI::MultiColumnList* table,
            const Ogre::String& key = "");

        /**
         * Adds the Vector3 values to the property table.
         *
         * @param key The property to add.
         */
        virtual void addPropertyVector3(const Property& prop, 
            CEGUI::MultiColumnList* table,
            const Ogre::String& key = "");

        /**
         * Adds the Quaternion values to the property table.
         *
         * @param key The property to add.
         */
        virtual void addPropertyQuaternion(const Property& prop, 
            CEGUI::MultiColumnList* table,
            const Ogre::String& key = "");

        /**
         * Adds a new tab to the tabpane and lists
         * the array values.
         *
         * @param key The property to add.
         */
        virtual void addPropertyArray(const Property& prop,
            const Ogre::String& key = "");

        
        CEGUI::TabControl *mTabPane;        //!< TabControl for the property tabs
        CEGUI::MultiColumnList *mMainTable; //!< Main table for the GameObject properties 
	};
}

#endif
