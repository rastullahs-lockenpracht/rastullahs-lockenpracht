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

#ifndef __PropertyWriter_H__
#define __PropertyWriter_H__

#include "CommonPrerequisites.h"

#include <tinyxml2.h>

#include "Properties.h"
#include "XmlProcessor.h"

namespace rl
{
    class Property;
    class PropertyRecord;

    typedef std::pair<Ogre::String, Property> PropertyEntry;

    class _RlCommonExport XmlPropertyWriter : public XmlProcessor
    {
    public:
        XmlPropertyWriter();
        virtual ~XmlPropertyWriter();

        tinyxml2::XMLDocument* getDocument();

        PropertyRecordPtr getPropertyRecords();
        void setPropertyRecords(PropertyRecordVector sets);
        void addPropertyRecord(PropertyRecordPtr set);

        tinyxml2::XMLElement* processProperty(tinyxml2::XMLElement* parent, const PropertyEntry& entry);
        tinyxml2::XMLElement* processPropertyRecord(
            tinyxml2::XMLElement* parent, const char* const name, const PropertyRecord& set);
        tinyxml2::XMLElement* processPropertyArray(
            tinyxml2::XMLElement* parent, const char* const name, const PropertyArray& vector);
        tinyxml2::XMLElement* processPropertyMap(
            tinyxml2::XMLElement* parent, const char* const name, const PropertyMap& map);

        void writeEachPropertyToElem(tinyxml2::XMLElement* parent, const PropertyMap& map);

    protected:
        PropertyRecordVector mPropertyRecords;

        tinyxml2::XMLDocument* mDocument;

        Property getProperty(const Ogre::String& key, const CeGuiString& value);
    };
} // namespace rl

#endif //__PropertyWriter_H__
