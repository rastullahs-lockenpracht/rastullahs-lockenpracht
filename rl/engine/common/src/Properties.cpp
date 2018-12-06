/* This source file is part of Rastullahs Lockenpracht.
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

#include "stdinc.h" //precompiled header

#include "Properties.h"

#include "Exception.h"
#include "Property.h"

using namespace std;

namespace rl
{

    void PropertyHolder::setProperties(const PropertyRecordPtr props)
    {
        for (PropertyRecord::PropertyRecordMap::const_iterator it = props->begin(); it != props->end(); it++)
        {
            setProperty((*it).first, (*it).second);
        }
    }

    void PropertyHolder::setProperties(const PropertyMap& propmap)
    {
        for (PropertyMap::const_iterator it = propmap.begin(); it != propmap.end(); it++)
        {
            setProperty((*it).first.c_str(), (*it).second);
        }
    }

    PropertyRecordPtr PropertyHolder::getAllProperties() const
    {
        PropertyRecordPtr record(new PropertyRecord);

        PropertyKeys keys = getAllPropertyKeys();
        PropertyKeys::const_iterator end = keys.end();
        for (PropertyKeys::const_iterator it = keys.begin(); it != end; ++it)
        {
            CeGuiString key = *it;
            record->setProperty(key, getProperty(key));
        }

        return record;
    }

    void PropertyHolder::setPropertyAsString(const Ogre::String& key, const CeGuiString& value)
    {
        Property prop = getProperty(key);
        prop.getFromString(value);
        setProperty(key, prop);
    }

    PropertyRecord::PropertyRecord()
        : PropertyHolder()
    {
        mProperties.clear();
    }

    PropertyRecord::PropertyRecord(const PropertyRecordPtr ps)
        : PropertyHolder()
    {
        mProperties.clear();
        setProperties(ps);
    }

    PropertyRecord::PropertyRecord(const PropertyMap& propmap)
        : PropertyHolder()
    {
        mProperties.clear();
        convertToMap(propmap, mProperties);
    }

    const Property PropertyRecord::getProperty(const CeGuiString& key) const
    {
        PropertyRecordMap::const_iterator it = mProperties.find(key);
        if (it == mProperties.end())
        {
            Throw(rl::RuntimeException, "Property " + key + " not found.");
        }

        return (*it).second;
    }

    bool PropertyRecord::hasProperty(const CeGuiString& key) const
    {
        return mProperties.find(key) != mProperties.end();
    }

    void PropertyRecord::setProperty(const CeGuiString& key, const Property& value)
    {
        mProperties[key] = value;
    }

    PropertyKeys PropertyRecord::getAllPropertyKeys() const
    {
        PropertyKeys keys;
        for (PropertyRecordMap::const_iterator it = mProperties.begin(); it != mProperties.end(); ++it)
        {
            keys.insert(it->first);
        }
        return keys;
    }

    const PropertyRecord::PropertyRecordMap::const_iterator PropertyRecord::begin() const
    {
        return mProperties.begin();
    }

    const PropertyRecord::PropertyRecordMap::const_iterator PropertyRecord::end() const
    {
        return mProperties.end();
    }

    PropertyMap PropertyRecord::toPropertyMap() const
    {
        return convertToPropertyMap(mProperties);
    }

    PropertyRecordPtr PropertyRecord::getDifference(const PropertyRecordPtr differenceTo) const
    {
        PropertyRecordPtr record(new PropertyRecord());
        for (PropertyRecordMap::const_iterator it = begin(); it != end(); ++it)
        {
            if (!differenceTo->hasProperty(it->first))
            {
                record->setProperty(it->first, it->second);
            }
            else
            {
                Property prop = differenceTo->getProperty(it->first);
                if (prop != it->second)
                    record->setProperty(it->first, it->second);
            }
        }
        return record;
    }
}
