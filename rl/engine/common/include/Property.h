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
#ifndef __Property_H__
#define __Property_H__

#include "CommonPrerequisites.h"

#include <boost/any.hpp>
#include <map>
#include <set>

#include "Exception.h"
#include "Tripel.h"

namespace rl
{

#define PropertyMethod(Name, Type)                                                                                     \
    Property(Type value)                                                                                               \
    {                                                                                                                  \
        setValue(value);                                                                                               \
        mName = Ogre::String(#Name);                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    bool is##Name() const                                                                                              \
    {                                                                                                                  \
        try                                                                                                            \
        {                                                                                                              \
            boost::any_cast<Type>(mValue);                                                                             \
            return true;                                                                                               \
        }                                                                                                              \
        catch (boost::bad_any_cast)                                                                                    \
        {                                                                                                              \
            return false;                                                                                              \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    void setValue(Type value)                                                                                          \
    {                                                                                                                  \
        mValue = value;                                                                                                \
        mName = #Name;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    Type to##Name() const                                                                                              \
    {                                                                                                                  \
        try                                                                                                            \
        {                                                                                                              \
            return boost::any_cast<Type>(mValue);                                                                      \
        }                                                                                                              \
        catch (boost::bad_any_cast)                                                                                    \
        {                                                                                                              \
            Throw(rl::WrongFormatException,                                                                            \
                "Wrong property type for to" + Ogre::String(#Name) + "() (type is "                                    \
                    + Ogre::String(mValue.type().name()) + ") " + (mValue.empty() ? "EMPTY!" : "not empty"));          \
        }                                                                                                              \
    }                                                                                                                  \
    operator Type() const                                                                                              \
    {                                                                                                                  \
        return to##Name();                                                                                             \
    }

    class Property;

    typedef std::vector<Property> PropertyArray;
    typedef std::map<CeGuiString, Property> PropertyMap;

    class _RlCommonExport Property
    {
    public:
        Property();

        PropertyMethod(Bool, const bool&);
        PropertyMethod(String, CEGUI::String);
        PropertyMethod(Int, const int&);
        PropertyMethod(Long, const long&);
        PropertyMethod(LongLong, const RL_LONGLONG&);
        PropertyMethod(Real, const Ogre::Real&);
        PropertyMethod(Vector3, const Ogre::Vector3&);
        PropertyMethod(Quaternion, const Ogre::Quaternion&);
        PropertyMethod(IntTriple, const Tripel<int>);
        PropertyMethod(IntPair, const IntPair);
        PropertyMethod(Array, const PropertyArray);
        PropertyMethod(Map, const PropertyMap);

        Ogre::String getTypeName() const;
        Ogre::String getName() const;
        CeGuiString getAsString() const;
        Ogre::Real getAsNumber() const;
        void getFromString(const CeGuiString& string);

        bool operator==(const Property& other) const;
        bool operator!=(const Property& other) const;

        bool isEmpty() const;

    private:
        boost::any mValue;
        Ogre::String mName;
    };

    using PropertyPtr = std::shared_ptr<Property>;

#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
    template <typename T> PropertyMap convertToPropertyMap(const std::map<const CeGuiString, T>& input)
    {
        PropertyMap map;
        for (typename std::map<const CeGuiString, T>::const_iterator it = input.begin(); it != input.end(); ++it)
        {
            map[it->first] = Property(it->second);
        }
        return map;
    }
#endif

    template <typename T> PropertyMap convertToPropertyMap(const std::map<const Ogre::String, T>& input)
    {
        PropertyMap map;
        for (typename std::map<const Ogre::String, T>::const_iterator it = input.begin(); it != input.end(); ++it)
        {
            map[it->first] = Property(it->second);
        }
        return map;
    }

    template <typename T> PropertyArray convertToPropertyArray(const std::set<T>& input)
    {
        PropertyArray vec;
        for (typename std::set<T>::const_iterator it = input.begin(); it != input.end(); ++it)
        {
            vec.push_back(Property(*it));
        }
        return vec;
    }

    template <typename T> void convertToMap(const PropertyMap& propmap, std::map<const Ogre::String, T>& output)
    {
        for (PropertyMap::const_iterator it = propmap.begin(); it != propmap.end(); ++it)
        {
            T temp = it->second;
            output[it->first.c_str()] = temp;
        }
    }

#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
    template <typename T> void convertToMap(const PropertyMap& propmap, std::map<const CeGuiString, T>& output)
    {
        for (PropertyMap::const_iterator it = propmap.begin(); it != propmap.end(); ++it)
        {
            T temp = it->second; // needed for GCC, else there is ambiguous operator CEGUI::String::=
            output[it->first] = temp;

            // This solution doesn't work. Why?
            // output.insert(typename std::map<const CeGuiString, T>::value_type(it->first, it->second));
        }
    }
#endif

    template <typename T> void convertToSet(const PropertyArray& propvec, std::set<T>& output)
    {
        for (PropertyArray::const_iterator it = propvec.begin(); it != propvec.end(); ++it)
        {
            output.insert(*it);
        }
    }

} // namespace rl

#endif //__Property_H__
