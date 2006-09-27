/* 
 * (C) 2003-2005. Team Pantheon. www.team-pantheon.de
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

#include "Properties.h"

#include "Exception.h"
#include "Property.h"

namespace rl {

    const Property PropertySet::getProperty(const Ogre::String& key) const
    {
        PropertyMap::const_iterator it = mProperties.find(key);
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

    const PropertySet PropertySet::getAllProperties() const
    {
        return *this;
    }
}
