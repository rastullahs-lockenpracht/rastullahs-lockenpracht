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
#ifndef __Properties_H__
#define __Properties_H__

#include "CommonPrerequisites.h"
#include "Property.h"

namespace rl {

    class PropertySet;

    class _RlCommonExport PropertyHolder
    {
    public:
        virtual const Property getProperty(const Ogre::String& key) const = 0;
        virtual void setProperty(const Ogre::String& key, const Property& value) = 0;
        virtual const PropertySet getAllProperties() const = 0;
    };

    class _RlCommonExport PropertySet : public PropertyHolder
    {
	public:
        virtual const Property getProperty(const Ogre::String& key) const;
        virtual void setProperty(const Ogre::String& key, const Property& value);
        virtual const PropertySet getAllProperties() const;

    private:
        typedef std::map<const Ogre::String, Property> PropertyMap;

        PropertyMap mProperties;
	};
}

#endif //__Properties_H__
