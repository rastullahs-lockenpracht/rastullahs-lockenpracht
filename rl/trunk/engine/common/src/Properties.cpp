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

#include "stdinc.h" //precompiled header

#include "Properties.h"

#include "Exception.h"
#include "Property.h"

using namespace std;

namespace rl {

    void PropertyHolder::setProperties(const PropertySet* props)
    {
        for (PropertySetMap::const_iterator it = props->begin();
            it != props->end(); it++)
        {
            setProperty((*it).first, (*it).second);
        }
    }

    void PropertyHolder::setProperties(const PropertyMap& propmap)
    {
        for (PropertyMap::const_iterator it = propmap.begin();
            it != propmap.end(); it++)
        {
            setProperty((*it).first.c_str(), (*it).second);
        }
    }

    PropertySet::PropertySet()
        : PropertyHolder()
    {
        mProperties.clear();
    }

    PropertySet::PropertySet(const PropertySet* ps)
        : PropertyHolder()
    {
        mProperties.clear();
        setProperties(ps);
    }

    PropertySet::PropertySet(const PropertyMap& propmap)
        : PropertyHolder()
    {
        mProperties.clear();
        convertToMap(propmap, mProperties);
    }

    const Property PropertySet::getProperty(const Ogre::String& key) const
    {
        PropertySetMap::const_iterator it = mProperties.find(key);
        if (it == mProperties.end())
        {
            Throw(rl::RuntimeException, "Property "+key+" not found.");
        }

        return (*it).second;
    }

    void PropertySet::setProperty(const Ogre::String& key, const Property& value)
    {
        mProperties[key] = value;
    }

    PropertySet* PropertySet::getAllProperties() const
    {
        return new PropertySet(this);
    }

    const PropertySetMap::const_iterator PropertySet::begin() const
    {
        return mProperties.begin();
    }

    const PropertySetMap::const_iterator PropertySet::end() const
    {
        return mProperties.end();
    }

    PropertyMap PropertySet::toPropertyMap() const
    {
        return convertToPropertyMap(mProperties);
    }
}
