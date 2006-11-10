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

#include "Property.h"

#include "Exception.h"

using namespace boost;
using namespace Ogre;

namespace rl {

    Property::Property()
    {
    }

    void Property::toGameObjectData(Ogre::String& classname, unsigned int& goid) const
    {
        Ogre::String sval = toString().c_str();
        int pos = sval.find("/");
        classname.assign(sval.substr(0, pos - 1));
        goid = Ogre::StringConverter::parseInt(sval.substr(pos));
    }

    void Property::setValue(Ogre::String& classname, unsigned int& goid)
    {
        mValue = classname + "/" + Ogre::StringConverter::toString(goid);
    }
}
