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
#include "stdinc.h" //precompiled header

/**
 * \file PropertiesWindow.cpp
 *
 * Implementation of the PropertyHolder window
 */

#include <CEGUI/PropertyHelper.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/widgets/ListboxTextItem.h>
#include <boost/bind.hpp>

#include "GameObject.h"
#include "Properties.h"
#include "PropertiesWindow.h"

using namespace CEGUI;

namespace rl
{

    //------------------------------------------------------- Constructor

    PropertiesWindow::PropertiesWindow(PropertyHolder* object)
        : AbstractWindow("propertieswindow.xml", WIT_MOUSE_INPUT)
        , mObject(object)
    {
        // Get access to the tab control
        mTabPane = getTabControl("PropertiesWindow/PropertiesTabPane");
        RlAssert(mTabPane != NULL, "PropertiesWindow/PropertiesTabPane is null");

        // Get a access to the main Property table
        mMainTable = getMultiColumnList("PropertiesWindow/PropertiesTabControl/MainTable");
        RlAssert(mMainTable != NULL, "PropertiesWindow/PropertiesTabPane is null");

        mMainTable->addColumn("Key", 0, cegui_reldim(0.3));
        mMainTable->addColumn("Type", 1, cegui_reldim(0.3));
        mMainTable->addColumn("Value", 2, cegui_reldim(0.4));

        centerWindow();

        bindDestroyWindowToXButton();
        bindDestroyWindowToClick(getWindow("PropertiesWindow/CloseButton"));

        initialize(mObject);
    }

    //------------------------------------------------------- Destructor

    PropertiesWindow::~PropertiesWindow()
    {
    }

    //------------------------------------------------------- initialize

    void PropertiesWindow::initialize(PropertyHolder* object)
    {
        // Fill header text fields with name and description
        fillHeader(object);

        // Get the property set of the object
        PropertyRecordPtr props = object->getAllProperties();

        int row = 0;
        //  Iterate through all properties and add a row for
        //  every Entry
        for (PropertyRecord::PropertyRecordMap::const_iterator propIter = props->begin(); propIter != props->end();
             propIter++)
        {
            CeGuiString key = propIter->first;

            if (key == "description" || key == "name")
                continue;

            // Check for Int
            if (propIter->second.isInt())
            {
                addPropertyInt(propIter->second, mMainTable, key);
            }
            // Check for IntPair
            else if (propIter->second.isIntPair())
            {
                addPropertyIntPair(propIter->second, mMainTable, key);
                int rowCount = mMainTable->getRowCount();
                mMainTable->addRow(rowCount);
                mMainTable->setItem(new ListboxTextItem(""), 0, rowCount);
            }
            // Check for IntTriple
            else if (propIter->second.isIntTriple())
            {
                addPropertyIntTriple(propIter->second, mMainTable, key);
                int rowCount = mMainTable->getRowCount();
                mMainTable->addRow(rowCount);
                mMainTable->setItem(new ListboxTextItem(""), 0, rowCount);
            }
            // Check for String
            else if (propIter->second.isString())
            {
                addPropertyString(propIter->second, mMainTable, key);
            }
            // Check for Bool
            else if (propIter->second.isBool())
            {
                addPropertyBool(propIter->second, mMainTable, key);
            }
            // Check for Real
            else if (propIter->second.isReal())
            {
                addPropertyReal(propIter->second, mMainTable, key);
            }
            // Check for Vector3
            else if (propIter->second.isVector3())
            {
                addPropertyArray3(propIter->second, mMainTable, key);
                int rowCount = mMainTable->getRowCount();
                mMainTable->addRow(rowCount);
                mMainTable->setItem(new ListboxTextItem(""), 0, rowCount);
            }
            // Check for Quaternion
            else if (propIter->second.isQuaternion())
            {
                addPropertyQuaternion(propIter->second, mMainTable, key);
                int rowCount = mMainTable->getRowCount();
                mMainTable->addRow(rowCount);
                mMainTable->setItem(new ListboxTextItem(""), 0, rowCount);
            }
            // Check for Array
            else if (propIter->second.isArray())
            {
                addPropertyArray(propIter->second, key);
            }
        }

        mMainTable->autoSizeColumnHeader(0);
        mMainTable->autoSizeColumnHeader(1);
        mMainTable->autoSizeColumnHeader(2);
    }

    void PropertiesWindow::update()
    {
        while (mTabPane->getTabCount() > 1)
        {
            mTabPane->removeTab(mTabPane->getTabContentsAtIndex(1)->getName());
        }

        while (mMainTable->getRowCount() > 0)
        {
            mMainTable->removeRow(0);
        }

        initialize(mObject);
    }

    //------------------------------------------------------- fillHeader

    void PropertiesWindow::fillHeader(PropertyHolder* object)
    {
        GameObject* go = dynamic_cast<GameObject*>(object);
        if (go)
        {
            getWindow("PropertiesWindow/HeaderSheet/ObjectName")->setText(go->getName());
            getWindow("PropertiesWindow/HeaderSheet/ObjectDescription")->setText(go->getDescription());
        }
    }

    //------------------------------------------------------- addPropertyInt

    void PropertiesWindow::addPropertyInt(const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key)
    {
        // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        unsigned int colCount = table->getColumnCount();
        unsigned int rowCount = table->getRowCount();

        // Table has the three columns Key, Type, Value
        if (colCount == 3)
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("Int  "), 1, rowCount);
            table->setItem(new ListboxTextItem(PropertyHelper<int>::toString(prop.toInt())), 2, rowCount);
        }
        // Table has the two columns Type, Value
        else if (colCount == 2)
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem("Int  "), 0, rowCount);
            table->setItem(new ListboxTextItem(PropertyHelper<int>::toString(prop.toInt())), 1, rowCount);
        }
    }

    //------------------------------------------------------- addPropertyIntPair

    void PropertiesWindow::addPropertyIntPair(
        const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key)
    {
        // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        unsigned int colCount = table->getColumnCount();
        unsigned int rowCount = table->getRowCount();

        IntPair intPair = prop.toIntPair();

        // Table has the three columns Key, Type, Value
        if (colCount == 3)
        {
            // Add row for the first IntPair value
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("IntPair  "), 1, rowCount);
            table->setItem(new ListboxTextItem("( " + PropertyHelper<int>::toString(intPair.first) + ", "
                               + PropertyHelper<int>::toString(intPair.second) + " )"),
                2, rowCount);
        }
        // Table has the two columns Type, Value
        else if (colCount == 2)
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem("IntPair  "), 0, rowCount);
            table->setItem(new ListboxTextItem("( " + PropertyHelper<int>::toString(intPair.first) + ", "
                               + PropertyHelper<int>::toString(intPair.second) + " )"),
                1, rowCount);
        }
    }

    //------------------------------------------------------- addPropertyIntTriple

    void PropertiesWindow::addPropertyIntTriple(
        const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key)
    {
        // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        int colCount = table->getColumnCount();
        int rowCount = table->getRowCount();

        Tripel<int> intTriple = prop.toIntTriple();

        // Table has the three columns Key, Type, Value
        if (colCount == 3)
        {
            // Add row for the first IntTriple value
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("IntTriple  "), 1, rowCount);
            table->setItem(new ListboxTextItem("( " + PropertyHelper<int>::toString(intTriple.first) + ", "
                               + PropertyHelper<int>::toString(intTriple.second) + ", "
                               + PropertyHelper<int>::toString(intTriple.third) + " )"),
                2, rowCount);
        }
        // Table has the two columns Type, Value
        else if (colCount == 2)
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem("IntTriple  "), 0, rowCount);
            table->setItem(new ListboxTextItem("( " + PropertyHelper<int>::toString(intTriple.first) + ", "
                               + PropertyHelper<int>::toString(intTriple.second) + ", "
                               + PropertyHelper<int>::toString(intTriple.third) + " )"),
                1, rowCount);
        }
    }

    //------------------------------------------------------- addPropertyString

    void PropertiesWindow::addPropertyString(
        const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key)
    {
        // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        int colCount = table->getColumnCount();
        int rowCount = table->getRowCount();

        // Table has the three columns Key, Type, Value
        if (colCount == 3)
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("String    "), 1, rowCount);
            table->setItem(new ListboxTextItem(prop.toString()), 2, rowCount);
        }
        // Table has the two columns Type, Value
        else if (colCount == 2)
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem("String    "), 0, rowCount);
            table->setItem(new ListboxTextItem(prop.toString()), 1, rowCount);
        }
    }

    //------------------------------------------------------- addPropertyBool

    void PropertiesWindow::addPropertyBool(const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key)
    {
        // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        int colCount = table->getColumnCount();
        int rowCount = table->getRowCount();

        // Table has the three columns Key, Type, Value
        if (colCount == 3)
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("Bool  "), 1, rowCount);
            table->setItem(new ListboxTextItem(PropertyHelper<bool>::toString(prop.toBool())), 2, rowCount);
        }
        // Table has the two columns Type, Value
        else if (colCount == 2)
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem("Bool  "), 0, rowCount);
            table->setItem(new ListboxTextItem(PropertyHelper<bool>::toString(prop.toBool())), 1, rowCount);
        }
    }

    //------------------------------------------------------- addPropertyReal

    void PropertiesWindow::addPropertyReal(const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key)
    {
        // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        int colCount = table->getColumnCount();
        int rowCount = table->getRowCount();

        char buf[50];
        sprintf(buf, "%1.2f", prop.toReal());

        // Table has the three columns Key, Type, Value
        if (colCount == 3)
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("Real  "), 1, rowCount);
            table->setItem(new ListboxTextItem(CEGUI::String(buf)), 2, rowCount);
        }
        // Table has the two columns Type, Value
        else if (colCount == 2)
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem("Real  "), 0, rowCount);
            table->setItem(new ListboxTextItem(CEGUI::String(buf)), 1, rowCount);
        }
    }

    //------------------------------------------------------- addPropertyArray3

    void PropertiesWindow::addPropertyArray3(
        const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key)
    {
        // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        int colCount = table->getColumnCount();
        int rowCount = table->getRowCount();

        Ogre::Vector3 vec = prop.toVector3();

        char buf1[50];
        sprintf(buf1, "%1.2f", vec.x);
        char buf2[50];
        sprintf(buf2, "%1.2f", vec.y);
        char buf3[50];
        sprintf(buf3, "%1.2f", vec.z);

        // Table has the three columns Key, Type, Value
        if (colCount == 3)
        {
            // Add row for the first IntTriple value
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("Vector3  "), 1, rowCount);
            table->setItem(new ListboxTextItem("( " + CEGUI::String(buf1) + ", " + CEGUI::String(buf2) + ", "
                               + CEGUI::String(buf3) + " )"),
                2, rowCount);
        }
        // Table has the two columns Type, Value
        else if (colCount == 2)
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem("Vector3  "), 0, rowCount);
            table->setItem(new ListboxTextItem("( " + CEGUI::String(buf1) + ", " + CEGUI::String(buf2) + ", "
                               + CEGUI::String(buf3) + " )"),
                1, rowCount);
        }
    }

    //------------------------------------------------------- addPropertyQuaternion

    void PropertiesWindow::addPropertyQuaternion(
        const Property& prop, CEGUI::MultiColumnList* table, const CeGuiString& key)
    {
        // Check column count
        // if count = 3 | MainTable
        // if count = 2 | Array table
        int colCount = table->getColumnCount();
        int rowCount = table->getRowCount();

        Ogre::Quaternion quat = prop.toQuaternion();
        Ogre::Degree angle;
        Ogre::Vector3 axis;

        quat.ToAngleAxis(angle, axis);

        char buf_angle[50];
        sprintf(buf_angle, "%1.2f", angle.valueDegrees());
        char buf_v1[50];
        sprintf(buf_v1, "%1.2f", axis.x);
        char buf_v2[50];
        sprintf(buf_v2, "%1.2f", axis.y);
        char buf_v3[50];
        sprintf(buf_v3, "%1.2f", axis.z);

        // Table has the three columns Key, Type, Value
        if (colCount == 3)
        {
            // Add row for the first IntTriple value
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem(key + "  "), 0, rowCount);
            table->setItem(new ListboxTextItem("Quaternion  "), 1, rowCount);
            table->setItem(new ListboxTextItem("Axis:   ( " + CEGUI::String(buf_v1) + ", " + CEGUI::String(buf_v2)
                               + ", " + CEGUI::String(buf_v3) + " )"),
                2, rowCount);

            // Add second for the second IntTriple value
            table->addRow(rowCount + 1);
            table->setItem(new ListboxTextItem("Degree: " + CEGUI::String(buf_angle)), 2, rowCount + 1);
        }
        // Table has the two columns Type, Value
        else if (colCount == 2)
        {
            table->addRow(rowCount);
            table->setItem(new ListboxTextItem("Quaternion  "), 0, rowCount);
            table->setItem(new ListboxTextItem("Axis:   ( " + CEGUI::String(buf_v1) + ", " + CEGUI::String(buf_v2)
                               + ", " + CEGUI::String(buf_v3) + " )"),
                2, rowCount);

            // Add second for the second IntTriple value
            table->addRow(rowCount + 1);
            table->setItem(new ListboxTextItem("Degree " + CEGUI::String(buf_angle)), 2, rowCount + 1);
        }
    }

    //------------------------------------------------------- addPropertyArray

    void PropertiesWindow::addPropertyArray(const Property& prop, const CeGuiString& key)
    {
        // Create the new MultiColumnList with two columns
        // Type and Value and set tab text to key
        auto tabCount = mTabPane->getTabCount();
        CEGUI::MultiColumnList* newTable
            = static_cast<CEGUI::MultiColumnList*>(CEGUI::WindowManager::getSingleton().createWindow(
                "RastullahLook/MultiColumnList", "PropertiesWindow/PropertiesTabControl/" + key));

        // Set table properties
        newTable->setText(key);
        newTable->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
        newTable->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
        newTable->setUserSortControlEnabled(false);
        newTable->setFont("Vera Serif-8");

        newTable->addColumn("Type", 0, cegui_reldim(0.3));
        newTable->addColumn("Value", 1, cegui_reldim(0.7));

        // Add the MultiColumnList to the tab pane
        mTabPane->addTab(newTable);

        // Get access to the vector
        PropertyArray vProp = prop.toArray();

        // Iterate through the vector entries and add them
        // to the table
        for (PropertyArray::const_iterator it = vProp.begin(); it != vProp.end(); it++)
        {
            // Check for Int
            if (it->isInt())
            {
                addPropertyInt(*it, newTable);
            }
            // Check for IntPair
            else if (it->isIntPair())
            {
                addPropertyIntPair(*it, newTable);
                int rowCount = newTable->getRowCount();
                newTable->addRow(rowCount);
                newTable->setItem(new ListboxTextItem(""), 0, rowCount);
            }
            // Check for IntTriple
            else if (it->isIntTriple())
            {
                addPropertyIntTriple(*it, newTable);
                int rowCount = newTable->getRowCount();
                newTable->addRow(rowCount);
                newTable->setItem(new ListboxTextItem(""), 0, rowCount);
            }
            // Check for String
            else if (it->isString())
            {
                addPropertyString(*it, newTable);
            }
            // Check for Bool
            else if (it->isBool())
            {
                addPropertyBool(*it, newTable);
            }
            // Check for Real
            else if (it->isReal())
            {
                addPropertyReal(*it, newTable);
            }
            // Check for Vector3
            else if (it->isVector3())
            {
                addPropertyArray3(*it, newTable);
                int rowCount = newTable->getRowCount();
                newTable->addRow(rowCount);
                newTable->setItem(new ListboxTextItem(""), 0, rowCount);
            }
            // Check for Quaternion
            else if (it->isQuaternion())
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
