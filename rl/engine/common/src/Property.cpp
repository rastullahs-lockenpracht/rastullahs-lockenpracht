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

#include "stdinc.h" //precompiled header

#include "Property.h"

#include "Exception.h"
#include <CEGUI/PropertyHelper.h>

using namespace boost;
using namespace Ogre;

namespace rl
{

    Property::Property()
    {
    }

    Ogre::String Property::getTypeName() const
    {
        return mValue.type().name();
    }

    Ogre::String Property::getName() const
    {
        return mName;
    }

    bool Property::isEmpty() const
    {
        return mValue.empty();
    }

    bool comparePropertyArrays(PropertyArray array1, PropertyArray array2);

    /// @return: will return true if the arrays are equal
    bool comparePropertyMaps(PropertyMap map1, PropertyMap map2)
    {
        if (map1.size() != map2.size())
            return false;
        for (PropertyMap::const_iterator it = map1.begin(); it != map1.end(); ++it)
        {
            if (map2.find(it->first) == map2.end())
                return false;
            else if (it->second.getTypeName() == map2[it->first].getTypeName())
            {
                if (it->second.isArray())
                {
                    if (!comparePropertyArrays(it->second, map2[it->first]))
                        return false;
                }
                else if (it->second.isMap())
                {
                    if (!comparePropertyMaps(it->second, map2[it->first]))
                        return false;
                }
                else if (it->second != map2[it->first])
                    return false;
            }
        }

        return true;
    }

    /// @return: will return true if the arrays are equal
    bool comparePropertyArrays(PropertyArray array1, PropertyArray array2)
    {
        if (array1.size() != array2.size())
            return false;
        for (unsigned int i = 0; i < array1.size(); i++)
        {
            if (array1[i].getTypeName() == array2[i].getTypeName())
            {
                if (array1[i].isArray())
                {
                    if (!comparePropertyArrays(array1[i], array2[i]))
                        return false;
                }
                else if (array1[i].isMap())
                {
                    if (!comparePropertyMaps(array1[i], array2[i]))
                        return false;
                }
                else if (array1[i] != array2[i])
                    return false;
            }
            else
                return false;
        }
        return true;
    }

    bool Property::operator==(const rl::Property& other) const
    {
        if (other.getTypeName() == getTypeName())
        {
            if (isArray())
                return comparePropertyArrays(*this, other);
            else if (isMap())
                return comparePropertyMaps(*this, other);
            return other.getAsString() == getAsString();
        }

        return false;
    }

    bool Property::operator!=(const rl::Property& other) const
    {
        if (other.getTypeName() == getTypeName())
        {
            if (isArray())
                return !comparePropertyArrays(*this, other);
            else if (isMap())
                return !comparePropertyMaps(*this, other);
            return other.getAsString() != getAsString();
        }

        return true;
    }

    Ogre::Real Property::getAsNumber() const
    {
        if (isInt())
        {
            return toInt();
        }
        else if (isReal())
        {
            return toReal();
        }
        else
        {
            return 0;
        }
    }

    CeGuiString Property::getAsString() const
    {
        if (isBool())
        {
            return StringConverter::toString(toBool());
        }
        else if (isString())
        {
            return toString();
        }
        else if (isInt())
        {
            return StringConverter::toString(toInt());
        }
        else if (isReal())
        {
            return StringConverter::toString(toReal());
        }
        else if (isVector3())
        {
            return StringConverter::toString(toVector3());
        }
        else if (isQuaternion())
        {
            return StringConverter::toString(toQuaternion());
        }
        else if (isIntTriple())
        {
            Tripel<int> triple = toIntTriple();
            return StringConverter::toString(triple.first) + "," + StringConverter::toString(triple.second) + ","
                + StringConverter::toString(triple.third);
        }
        else if (isIntPair())
        {
            IntPair pair = toIntPair();
            return StringConverter::toString(pair.first) + "," + StringConverter::toString(pair.second);
        }
        ///@todo
        // else if (isArray())
        //      {
        //          return StringConverter::toString(toBool());
        //      }
        // else if (isMap())
        //      {
        //          return StringConverter::toString(toBool());
        //      }
        return "";
    }

    void Property::getFromString(const CeGuiString& str)
    {
        if (isBool())
        {
            mValue = CEGUI::PropertyHelper<bool>::fromString(str);
        }
        else if (isString())
        {
            mValue = str;
        }
        else if (isInt())
        {
            mValue = CEGUI::PropertyHelper<int>::fromString(str);
        }
        else if (isReal())
        {
            mValue = CEGUI::PropertyHelper<float>::fromString(str);
        }
        else if (isVector3())
        {
            mValue = StringConverter::parseVector3(str.c_str());
        }
        else if (isQuaternion())
        {
            mValue = StringConverter::parseQuaternion(str.c_str());
        }
        ///@todo
        // else if (isIntTriple())
        //{
        //    Tripel<int> triple = toIntTriple();
        //    mValue = StringConverter::toString(triple.first)
        //        + "," + StringConverter::toString(triple.second)
        //        + "," + StringConverter::toString(triple.third);
        //}
        // else if (isIntPair())
        //{
        //    IntPair pair = toIntPair();
        //    mValue = StringConverter::toString(pair.first)
        //        + "," + StringConverter::toString(pair.second);
        //}
        // else if (isArray())
        //      {
        //          mValue = StringConverter::toString(toBool());
        //      }
        // else if (isMap())
        //      {
        //          mValue = StringConverter::toString(toBool());
        //      }
        else
        {
            mValue = str;
        }
    }
}
