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
 * \file GameObjectWindow.cpp
 *
 * Implementation of the GameObject window
 */

#include <boost/bind.hpp>
#include "GameObjectWindow.h"
#include "GameObject.h"

using namespace CEGUI;

namespace rl {

    //------------------------------------------------------- Constructor

	GameObjectWindow::GameObjectWindow() :
        CeGuiWindow("gameobjectwindow.xml", WND_MOUSE_INPUT)
	{
        // Get access to the tab control
        mTabPane = getTabControl("GameObjectWindow/PropertiesTabPane");
        RlAssert(mTabPane != NULL, "GameObjectWindow/PropertiesTabPane is null");

        // Get a access to the main Property table
        mMainTable = getMultiColumnList("GameObjectWindow/PropertiesTabControl/MainTable");
        RlAssert(mMainTable != NULL, "GameObjectWindow/PropertiesTabPane is null");

        mMainTable->addColumn( (utf8*)"Key", 0, cegui_reldim(0.3));
        mMainTable->addColumn( (utf8*)"Type", 1, cegui_reldim(0.3));
        mMainTable->addColumn( (utf8*)"Value", 2, cegui_reldim(0.4));
        
        centerWindow();

        bindCloseToCloseButton();
        bindClickToCloseWindow(getWindow("GameObjectWindow/CloseButton"));
	    
	}

    //------------------------------------------------------- Destructor

	GameObjectWindow::~GameObjectWindow() 
	{        
	}

    //------------------------------------------------------- initialize

	void GameObjectWindow::initialize( GameObject* object )
	{
        // Fill header text fields with name and description
        fillHeader(object);    

        // Get the property set of the object
        PropertySet* props = object->getAllProperties();

        int row = 0;
        //  Iterate through all properties and add a row for
        //  every Entry
        for( PropertyMap::const_iterator propIter = props->begin();
            propIter != props->end(); propIter++ )
        {
            CEGUI::String key = propIter->first;
            
            if( key == "description" || key == "name" )
                continue;

            // Check for Int
            if( propIter->second.isInt() )
            {
                addPropertyInt(propIter->second, mMainTable, key );
            }
            // Check for IntPair
            else if( propIter->second.isIntPair() )
            {
                addPropertyIntPair(propIter->second, mMainTable, key);
                int rowCount = mMainTable->getRowCount();
                mMainTable->addRow(rowCount);
                mMainTable->setItem(new ListboxTextItem(""), 0, rowCount);
            }
            // Check for IntTriple
            else if( propIter->second.isIntTriple() )
            {
                addPropertyIntTriple(propIter->second, mMainTable, key);
                int rowCount = mMainTable->getRowCount();
                mMainTable->addRow(rowCount);
                mMainTable->setItem(new ListboxTextItem(""), 0, rowCount);
            }
            // Check for String
            else if( propIter->second.isString() )
            {
                addPropertyString(propIter->second, mMainTable, key);
            }
            // Check for Bool
            else if( propIter->second.isBool() )
            {
                addPropertyBool(propIter->second, mMainTable, key);
            }
            // Check for Real
            else if( propIter->second.isReal() )
            {
                addPropertyReal(propIter->second, mMainTable, key);
            }
            // Check for Vector3
            else if( propIter->second.isVector3() )
            {
                addPropertyVector3(propIter->second, mMainTable, key);
                int rowCount = mMainTable->getRowCount();
                mMainTable->addRow(rowCount);
                mMainTable->setItem(new ListboxTextItem(""), 0, rowCount);
            }
            // Check for Quaternion
            else if( propIter->second.isQuaternion() )
            {
                addPropertyQuaternion(propIter->second, mMainTable, key);
                int rowCount = mMainTable->getRowCount();
                mMainTable->addRow(rowCount);
                mMainTable->setItem(new ListboxTextItem(""), 0, rowCount);
            }
            // Check for Array
            else if( propIter->second.isArray() )
            {
                addPropertyArray(propIter->second, key);
            }
        }

        mMainTable->autoSizeColumnHeader(0);
        mMainTable->autoSizeColumnHeader(1);
        mMainTable->autoSizeColumnHeader(2);
	}

    //------------------------------------------------------- fillHeader

    void GameObjectWindow::fillHeader(GameObject* object)
    {
        getWindow("GameObjectWindow/HeaderSheet/ObjectName")->setText(object->getName());
        getWindow("GameObjectWindow/HeaderSheet/ObjectDescription")->setText(object->getDescription());
    }

    //------------------------------------------------------- addPropertyInt

    void GameObjectWindow::addPropertyInt(const Property& prop, 
        CEGUI::MultiColumnList* table, const CEGUI::String& key)
    {
        // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        int colCount = table->getColumnCount();
        int rowCount = table->getRowCount();

        // Table has the three columns Key, Type, Value
        if ( colCount == 3 )
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("Int  "), 1, rowCount);
            table->setItem(new ListboxTextItem(PropertyHelper::intToString(prop.toInt())), 
                2, 
                rowCount);
        }
        // Table has the two columns Type, Value
        else if ( colCount == 2 )
        {
            table->setItem(new ListboxTextItem("Int  "), 0, rowCount);
            table->setItem(new ListboxTextItem(PropertyHelper::intToString(prop.toInt())), 
                1, 
                rowCount);
        }
    }

    //------------------------------------------------------- addPropertyIntPair

    void GameObjectWindow::addPropertyIntPair(const Property& prop, 
        CEGUI::MultiColumnList* table, const CEGUI::String& key)
    {
        // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        int colCount = table->getColumnCount();
        int rowCount = table->getRowCount();

        IntPair intPair = prop.toIntPair();

        // Table has the three columns Key, Type, Value
        if ( colCount == 3 )
        {
            // Add row for the first IntPair value
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("IntPair  "), 1, rowCount);
            table->setItem(new ListboxTextItem(
                "First " + PropertyHelper::intToString(intPair.first)), 
                2, 
                rowCount);

            // Add second for the second IntPair value
            table->addRow(rowCount + 1);
            table->setItem(new ListboxTextItem(
                "Second " + PropertyHelper::intToString(intPair.second)), 
                2, 
                rowCount + 1);
        }
        // Table has the two columns Type, Value
        else if ( colCount == 2 )
        {
            table->setItem(new ListboxTextItem("IntPair  "), 0, rowCount);
            table->setItem(new ListboxTextItem(
                "First " + PropertyHelper::intToString(intPair.first)), 
                1, 
                rowCount);

            // Add second for the second IntPair value
            table->addRow(rowCount + 1);
            table->setItem(new ListboxTextItem(
                "Second " + PropertyHelper::intToString(intPair.second)), 
                2, 
                rowCount + 1);
        }
    }

    //------------------------------------------------------- addPropertyIntTriple

    void GameObjectWindow::addPropertyIntTriple(const Property& prop, 
        CEGUI::MultiColumnList* table, const CEGUI::String& key)
    {
        // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        int colCount = table->getColumnCount();
        int rowCount = table->getRowCount();

        Tripel<int> intTriple = prop.toIntTriple();

        // Table has the three columns Key, Type, Value
        if ( colCount == 3 )
        {
            // Add row for the first IntTriple value
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("IntTriple  "), 1, rowCount);
            table->setItem(new ListboxTextItem(
                "First " + PropertyHelper::intToString(intTriple.first)), 
                2, 
                rowCount);

            // Add second for the second IntTriple value
            table->addRow(rowCount + 1);
            table->setItem(new ListboxTextItem(
                "Second " + PropertyHelper::intToString(intTriple.second)), 
                2, 
                rowCount + 1);

            // Add third for the third IntTriple value
            table->addRow(rowCount + 2);
            table->setItem(new ListboxTextItem(
                "Third " + PropertyHelper::intToString(intTriple.third)), 
                2, 
                rowCount + 2);
        }
        // Table has the two columns Type, Value
        else if ( colCount == 2 )
        {
            table->setItem(new ListboxTextItem("IntTriple  "), 0, rowCount);
            table->setItem(new ListboxTextItem(
                "First " + PropertyHelper::intToString(intTriple.first)), 
                1, 
                rowCount);

            // Add second for the second IntPair value
            table->addRow(rowCount + 1);
            table->setItem(new ListboxTextItem(
                "Second " + PropertyHelper::intToString(intTriple.second)), 
                2, 
                rowCount + 1);

            // Add third for the third IntTriple value
            table->addRow(rowCount + 2);
            table->setItem(new ListboxTextItem(
                "Third " + PropertyHelper::intToString(intTriple.third)), 
                2, 
                rowCount + 2);
        } 
    }

    //------------------------------------------------------- addPropertyString

    void GameObjectWindow::addPropertyString(const Property& prop, 
        CEGUI::MultiColumnList* table, const CEGUI::String& key)
    {
        // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        int colCount = table->getColumnCount();
        int rowCount = table->getRowCount();

        // Table has the three columns Key, Type, Value
        if ( colCount == 3 )
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("String  "), 1, rowCount);
            table->setItem(new ListboxTextItem(prop.toString()), 
                2, 
                rowCount);
        }
        // Table has the two columns Type, Value
        else if ( colCount == 2 )
        {
            table->setItem(new ListboxTextItem("String  "), 0, rowCount);
            table->setItem(new ListboxTextItem(prop.toString()), 
                1, 
                rowCount);
        }
    }

    //------------------------------------------------------- addPropertyBool

    void GameObjectWindow::addPropertyBool(const Property& prop, 
        CEGUI::MultiColumnList* table, const CEGUI::String& key)
    {
        // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        int colCount = table->getColumnCount();
        int rowCount = table->getRowCount();

        // Table has the three columns Key, Type, Value
        if ( colCount == 3 )
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("Bool  "), 1, rowCount);
            table->setItem(new ListboxTextItem(PropertyHelper::boolToString(prop.toBool())), 
                2, 
                rowCount);
        }
        // Table has the two columns Type, Value
        else if ( colCount == 2 )
        {
            table->setItem(new ListboxTextItem("Bool  "), 0, rowCount);
            table->setItem(new ListboxTextItem(PropertyHelper::boolToString(prop.toBool())), 
                1, 
                rowCount);
        }
    }

    //------------------------------------------------------- addPropertyReal

    void GameObjectWindow::addPropertyReal(const Property& prop, 
        CEGUI::MultiColumnList* table, const CEGUI::String& key)
    {
       // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        int colCount = table->getColumnCount();
        int rowCount = table->getRowCount();

        // Table has the three columns Key, Type, Value
        if ( colCount == 3 )
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("Real  "), 1, rowCount);
            table->setItem(new ListboxTextItem(PropertyHelper::floatToString(prop.toReal())), 
                2, 
                rowCount);
        }
        // Table has the two columns Type, Value
        else if ( colCount == 2 )
        {
            table->setItem(new ListboxTextItem("Real  "), 0, rowCount);
            table->setItem(new ListboxTextItem(PropertyHelper::floatToString(prop.toReal())), 
                1, 
                rowCount);
        }
    }

    //------------------------------------------------------- addPropertyVector3

    void GameObjectWindow::addPropertyVector3(const Property& prop, 
        CEGUI::MultiColumnList* table, const CEGUI::String& key)
    {
        // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        int colCount = table->getColumnCount();
        int rowCount = table->getRowCount();

        Ogre::Vector3 vec = prop.toVector3();

        // Table has the three columns Key, Type, Value
        if ( colCount == 3 )
        {
            // Add row for the first IntTriple value
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("Vector3  "), 1, rowCount);
            table->setItem(new ListboxTextItem("X " + PropertyHelper::floatToString(vec.x)), 
                2, 
                rowCount);

            // Add second for the second IntTriple value
            table->addRow(rowCount + 1);
            table->setItem(new ListboxTextItem("Y " + PropertyHelper::floatToString(vec.y)), 
                2, 
                rowCount + 1);

            // Add third for the third IntTriple value
            table->addRow(rowCount + 2);
            table->setItem(new ListboxTextItem("Z " + PropertyHelper::floatToString(vec.z)), 
                2, 
                rowCount + 2);
        }
        // Table has the two columns Type, Value
        else if ( colCount == 2 )
        {
            table->setItem(new ListboxTextItem("Vector3  "), 0, rowCount);
            table->setItem(new ListboxTextItem("X " + PropertyHelper::floatToString(vec.x)), 
                1, 
                rowCount);

            // Add second for the second IntPair value
            table->addRow(rowCount + 1);
            table->setItem(new ListboxTextItem("Y " + PropertyHelper::floatToString(vec.y)), 
                2, 
                rowCount + 1);

            // Add third for the third IntTriple value
            table->addRow(rowCount + 2);
            table->setItem(new ListboxTextItem("Z " + PropertyHelper::floatToString(vec.z)), 
                2, 
                rowCount + 2);
        } 
    }

    //------------------------------------------------------- addPropertyQuaternion

    void GameObjectWindow::addPropertyQuaternion(const Property& prop, 
        CEGUI::MultiColumnList* table, const CEGUI::String& key)
    {
        // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        int colCount = table->getColumnCount();
        int rowCount = table->getRowCount();

        Ogre::Quaternion quat = prop.toQuaternion();
        Ogre::Radian angle;
        Ogre::Vector3 axis;

        quat.ToAngleAxis( angle, axis );

        // Table has the three columns Key, Type, Value
        if ( colCount == 3 )
        {
            // Add row for the first IntTriple value
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("Quaternion  "), 1, rowCount);
            table->setItem(new ListboxTextItem("Axis " +
                PropertyHelper::floatToString(axis.x) + ", " +
                PropertyHelper::floatToString(axis.y) + ", " +
                PropertyHelper::floatToString(axis.z)), 
                2, 
                rowCount);

            // Add second for the second IntTriple value
            table->addRow(rowCount + 1);
            table->setItem(new ListboxTextItem("Radian " + 
                PropertyHelper::floatToString(angle.valueRadians())), 
                2, 
                rowCount + 1);
        }
        // Table has the two columns Type, Value
        else if ( colCount == 2 )
        {
            table->setItem(new ListboxTextItem("Quaternion  "), 0, rowCount);
            table->setItem(new ListboxTextItem("Axis " +
                PropertyHelper::floatToString(axis.x) + ", " +
                PropertyHelper::floatToString(axis.y) + ", " +
                PropertyHelper::floatToString(axis.z)), 
                2, 
                rowCount);

            // Add second for the second IntTriple value
            table->addRow(rowCount + 1);
            table->setItem(new ListboxTextItem("Radian " + 
                PropertyHelper::floatToString(angle.valueRadians())), 
                2, 
                rowCount + 1);
        } 

    }

    //------------------------------------------------------- addPropertyArray

    void GameObjectWindow::addPropertyArray(const Property& prop, const CEGUI::String& key)
    {
        // Create the new MultiColumnList with two columns
        // Type and Value and set tab text to key
        int tabCount = mTabPane->getTabCount();
        CEGUI::MultiColumnList* newTable = new MultiColumnList(
            "RastullahLook/MultiColumnList",
            "GameObjectWindow/PropertiesTabControl/PropertyTab_" + 
            CEGUI::PropertyHelper::intToString(tabCount) );
        newTable->setText(key);
        newTable->addColumn("Type", 0, cegui_reldim(0.3));
        newTable->addColumn("Value", 1, cegui_reldim(0.7));
        
        // Add the MultiColumnList to the tab pane
        mTabPane->addTab(newTable);

        // Get access to the vector
        std::vector<Property> vProp = prop.toArray();
        std::vector<Property>::iterator it;

        // Iterate through the vector entries and add them
        // to the table
        for(it = vProp.begin(); it != vProp.end(); it++)
        {
            // Check for Int
            if( it->isInt() )
            {
                addPropertyInt(*it, newTable);
            }
            // Check for IntPair
            else if( it->isIntPair() )
            {
                addPropertyIntPair(*it, newTable);
                int rowCount = newTable->getRowCount();
                newTable->addRow(rowCount);
                newTable->setItem(new ListboxTextItem(""), 0, rowCount);
            }
            // Check for IntTriple
            else if( it->isIntTriple() )
            {
                addPropertyIntTriple(*it, newTable);
                int rowCount = newTable->getRowCount();
                newTable->addRow(rowCount);
                newTable->setItem(new ListboxTextItem(""), 0, rowCount);
            }
            // Check for String
            else if( it->isString() )
            {
                addPropertyString(*it, newTable);
            }
            // Check for Bool
            else if( it->isBool() )
            {
                addPropertyBool(*it, newTable);
            }
            // Check for Real
            else if( it->isReal() )
            {
                addPropertyReal(*it, newTable);
            }
            // Check for Vector3
            else if( it->isVector3() )
            {
                addPropertyVector3(*it, newTable);
                int rowCount = newTable->getRowCount();
                newTable->addRow(rowCount);
                newTable->setItem(new ListboxTextItem(""), 0, rowCount);
            }
            // Check for Quaternion
            else if( it->isQuaternion() )
            {
                addPropertyQuaternion(*it, newTable);
                int rowCount = newTable->getRowCount();
                newTable->addRow(rowCount);
                newTable->setItem(new ListboxTextItem(""), 0, rowCount);
            }
        }
        newTable->autoSizeColumnHeader(0);
        newTable->autoSizeColumnHeader(1);
    }

} // namespace rl
