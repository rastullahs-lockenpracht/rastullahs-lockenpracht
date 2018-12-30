/*
 * (C) 2003-2008. Team Pantheon. www.team-pantheon.de
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
 *  along with this program; if not you can get it here:
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm
 */
#ifndef __Properties_H__
#define __Properties_H__

#include "CommonPrerequisites.h"
#include "Property.h"

namespace rl
{

    class PropertyRecord;
    using PropertyRecordPtr = std::shared_ptr<PropertyRecord>;
    typedef std::set<CeGuiString> PropertyKeys;
    /**
     * This is the base class of all objects having Propertys
     */
    class _RlCommonExport PropertyHolder
    {
    public:
        virtual const Property getProperty(const CeGuiString& key) const = 0;
        virtual void setProperty(const CeGuiString& key, const Property& value) = 0;
        void setPropertyAsString(const Ogre::String& key, const CeGuiString& value);
        PropertyRecordPtr getAllProperties() const;
        virtual PropertyKeys getAllPropertyKeys() const = 0;
        void setProperties(const PropertyRecordPtr props);
        void setProperties(const PropertyMap& propmap);
    };

    /**
     * A PropertyRecord holds all Propertys of one object,
     */
    class _RlCommonExport PropertyRecord : public PropertyHolder
    {
    public:
        typedef std::map<const CeGuiString, Property> PropertyRecordMap;

        PropertyRecord();
        PropertyRecord(const PropertyRecordPtr ps);
        PropertyRecord(const PropertyMap& propmap);

        virtual const Property getProperty(const CeGuiString& key) const;
        virtual void setProperty(const CeGuiString& key, const Property& value);
        bool hasProperty(const CeGuiString& key) const;
        virtual PropertyKeys getAllPropertyKeys() const;
        const PropertyRecordMap::const_iterator begin() const;
        const PropertyRecordMap::const_iterator end() const;
        PropertyMap toPropertyMap() const;

        PropertyRecordPtr getDifference(const PropertyRecordPtr differenceTo) const;

    private:
        PropertyRecordMap mProperties;
    };

    using PropertyRecordVector = std::vector<PropertyRecordPtr>;
}

#endif //__Properties_H__
