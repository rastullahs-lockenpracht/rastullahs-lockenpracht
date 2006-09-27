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
#ifndef __Property_H__
#define __Property_H__

#include "CommonPrerequisites.h"

#include <boost/any.hpp>

namespace rl {

#define PropertyMethod(Name, Type)\
    Property(Type value) \
    { \
        setValue(value); \
    } \
    \
    bool is##Name() const\
    { \
        try \
        { \
            boost::any_cast<Type>(mValue); \
            return true; \
        } \
        catch (boost::bad_any_cast) \
        { \
        return false; \
        } \
    } \
    \
    void setValue(Type value) \
    { \
        mValue = value; \
    } \
    \
    Type to##Name() const \
    { \
        return boost::any_cast<Type>(mValue); \
    }


    class _RlCommonExport Property
    {
    public:
        Property();

        PropertyMethod(Bool, const bool);
        PropertyMethod(String, const CeGuiString&);
        PropertyMethod(Int, const int);
        PropertyMethod(Real, const Ogre::Real);
        PropertyMethod(Vector, const Ogre::Vector3&);
        PropertyMethod(Quaternion, const Ogre::Quaternion&);
        
        void toGameObjectData(Ogre::String& classname, unsigned int& goid) const;
        void setValue(Ogre::String& classname, unsigned int& goid);

    private:
        boost::any mValue;
    };
} // namespace rl

#endif //__Property_H__
