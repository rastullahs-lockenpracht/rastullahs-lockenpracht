/* 
 * (C) 2003-2007. Team Pantheon. www.team-pantheon.de
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
 *  along with this program; if not you can get it here:
 *  http://www.perldoc.com/perl5.6/Artistic.html
 */
#ifndef __Properties_H__
#define __Properties_H__

#include "CommonPrerequisites.h"
#include "Property.h"
 
namespace rl {

    class PropertySet;
    class PropertySetPtr;

    typedef std::map<const Ogre::String, Property> PropertySetMap;

    class _RlCommonExport PropertyHolder
    {
    public:
        virtual const Property getProperty(const Ogre::String& key) const = 0;
        virtual void setProperty(const Ogre::String& key, const Property& value) = 0;
        virtual PropertySet* getAllProperties() const = 0;
        virtual void setProperties(const PropertySet* props);
    };

    class _RlCommonExport PropertySet : public PropertyHolder
    {
	public:
        PropertySet();
        PropertySet(const PropertySet* ps);

        virtual const Property getProperty(const Ogre::String& key) const;
        virtual void setProperty(const Ogre::String& key, const Property& value);
        virtual PropertySet* getAllProperties() const;
        const PropertySetMap::const_iterator begin() const;
        const PropertySetMap::const_iterator end() const;
        
    private:
        PropertySetMap mProperties;
	};

    class _RlCommonExport PropertySetPtr :
        public Ogre::SharedPtr<PropertySet>
    {
    public:
        PropertySetPtr() : Ogre::SharedPtr<PropertySet>() {}
        explicit PropertySetPtr(PropertySet* rep) : Ogre::SharedPtr<PropertySet>(rep) {}
        PropertySetPtr(const PropertySetPtr& res) : Ogre::SharedPtr<PropertySet>(res) {}
    
    protected:
        void destroy() { Ogre::SharedPtr<PropertySet>::destroy(); }
    };

}

#endif //__Properties_H__
