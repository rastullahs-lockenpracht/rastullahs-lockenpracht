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

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include "Property.h"
#include "XmlProcessor.h"

namespace rl 
{
    class Property;
    class PropertyRecord;

    typedef std::pair<Ogre::String, Property> PropertyEntry;

    class _RlCommonExport XmlPropertyWriter
        : public XmlProcessor
    {
    public:
        XmlPropertyWriter();
        virtual ~XmlPropertyWriter();

        XERCES_CPP_NAMESPACE::DOMDocument* getDocument();

        std::vector<PropertyRecord*> getPropertyRecords();
        void setPropertyRecords(std::vector<PropertyRecord*> sets);
        void addPropertyRecord(PropertyRecord* set);

        XERCES_CPP_NAMESPACE::DOMElement* processProperty(XERCES_CPP_NAMESPACE::DOMElement* parent, PropertyEntry entry);
        XERCES_CPP_NAMESPACE::DOMElement* processPropertyRecord(XERCES_CPP_NAMESPACE::DOMElement* parent, const char* const name, PropertyRecord set);
        XERCES_CPP_NAMESPACE::DOMElement* processPropertyArray(XERCES_CPP_NAMESPACE::DOMElement* parent, const char* const name, PropertyArray vector);
        XERCES_CPP_NAMESPACE::DOMElement* processPropertyMap(XERCES_CPP_NAMESPACE::DOMElement* parent, const char* const name, PropertyMap map);

        void writeEachPropertyToElem(XERCES_CPP_NAMESPACE::DOMElement* parent, const PropertyMap &map);
    protected:
        std::vector<PropertyRecord*> mPropertyRecords;

        XERCES_CPP_NAMESPACE::DOMDocument* mDocument;
        XERCES_CPP_NAMESPACE::DOMImplementation* mImplementation;
        XERCES_CPP_NAMESPACE::DOMWriter* mWriter;
        XERCES_CPP_NAMESPACE::XMLFormatTarget* mTarget;

        Property getProperty(const Ogre::String& key, const CeGuiString& value);
    };
} // namespace rl

#endif //__PropertyWriter_H__
