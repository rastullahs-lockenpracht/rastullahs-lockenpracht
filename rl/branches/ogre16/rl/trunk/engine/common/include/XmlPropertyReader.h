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

#ifndef __PropertyReader_H__
#define __PropertyReader_H__

#include "CommonPrerequisites.h"

#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include "Properties.h"
#include "XmlProcessor.h"

namespace rl 
{
    class Property;
    class PropertyRecord;

    typedef std::pair<Ogre::String, Property> PropertyEntry;

    class _RlCommonExport XmlPropertyReader
         : public XmlProcessor
    {
    public:
        XmlPropertyReader();
        virtual ~XmlPropertyReader();

        void parseGameObjectFile(Ogre::DataStreamPtr &stream, const Ogre::String &groupName);
        PropertyRecordVector getPropertyRecords();

        PropertyEntry processProperty(XERCES_CPP_NAMESPACE::DOMElement* domElem) const;
		PropertyEntry processProperty(XERCES_CPP_NAMESPACE::DOMAttr* domAttr) const;

        virtual PropertyRecordPtr getPropertiesAsRecord(XERCES_CPP_NAMESPACE::DOMElement* parent);
    private:
        PropertyRecordVector mPropertyRecords;

        Property getProperty(const Ogre::String& key, const CeGuiString& value);
    };
} // namespace rl

#endif //__PropertyReader_H__
