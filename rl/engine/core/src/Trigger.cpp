/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */
#include "stdinc.h" //precompiled header

#include "Trigger.h"

namespace rl
{
    Trigger::Trigger(const Ogre::String& classname, const Ogre::String& name, bool needsToBeSaved)
        : mName(name)
        , mClassName(classname)
        , mNeedsToBeSaved(needsToBeSaved)
    {
    }

    const Property Trigger::getProperty(const CeGuiString& key) const
    {
        Property prop;
        if (key == "name")
            prop.setValue(mName);
        else if (key == "classname")
            prop.setValue(mClassName);
        else
        {
            Throw(IllegalArgumentException, key + " is not a property of this Trigger (" + mName + ").");
        }

        return prop;
    }

    void Trigger::setProperty(const CeGuiString& key, const Property& value)
    {
        try
        {
            if (key == "name")
                mName = value.toString().c_str();
            else if (key == "classname")
                mClassName = value.toString().c_str();
            else
            {
                LOG_WARNING(Logger::CORE, key + " is not a property of this Trigger (" + mName + ").");
            }
        }
        catch (WrongFormatException ex)
        {
            LOG_ERROR(Logger::CORE, "property " + key + " has the wrong format");
        }
    }

    PropertyKeys Trigger::getAllPropertyKeys() const
    {
        PropertyKeys keys;
        keys.insert("name");
        keys.insert("classname");
        return keys;
    }
}
