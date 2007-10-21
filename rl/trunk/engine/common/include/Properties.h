/* 
 * (C) 2003-2007. Team Pantheon. www.team-pantheon.de
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
 
namespace rl {

    class PropertyRecord;
    class PropertyRecordPtr;

    typedef std::map<const Ogre::String, Property> PropertyRecordMap;

    /**
     * This is the base class of all objects having Propertys
     */
    class _RlCommonExport PropertyHolder
    {
    public:
        virtual const Property getProperty(const Ogre::String& key) const = 0;
        virtual void setProperty(const Ogre::String& key, const Property& value) = 0;
        virtual PropertyRecord* getAllProperties() const = 0;
        void setProperties(const PropertyRecord* props);
        void setProperties(const PropertyMap& propmap);
    };

    /**
     * A PropertyRecord holds all Propertys of one object,
     */
    class _RlCommonExport PropertyRecord : public PropertyHolder
    {
	public:
        PropertyRecord();
        PropertyRecord(const PropertyRecord* ps);
        PropertyRecord(const PropertyMap& propmap);

        virtual const Property getProperty(const Ogre::String& key) const;
        virtual void setProperty(const Ogre::String& key, const Property& value);
        bool hasProperty(const Ogre::String& key) const;
        virtual PropertyRecord* getAllProperties() const;
        const PropertyRecordMap::const_iterator begin() const;
        const PropertyRecordMap::const_iterator end() const;
        PropertyMap toPropertyMap() const;
        
    private:
        PropertyRecordMap mProperties;
	};

    class _RlCommonExport PropertyRecordPtr :
        public Ogre::SharedPtr<PropertyRecord>
    {
    public:
        PropertyRecordPtr() : Ogre::SharedPtr<PropertyRecord>() {}
        explicit PropertyRecordPtr(PropertyRecord* rep) : Ogre::SharedPtr<PropertyRecord>(rep) {}
        PropertyRecordPtr(const PropertyRecordPtr& res) : Ogre::SharedPtr<PropertyRecord>(res) {}
    
    protected:
        void destroy() { Ogre::SharedPtr<PropertyRecord>::destroy(); }
    };

}

#endif //__Properties_H__
