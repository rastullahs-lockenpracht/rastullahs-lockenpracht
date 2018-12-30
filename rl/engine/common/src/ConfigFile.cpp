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

#include "ConfigFile.h"
#include "Exception.h"

#ifdef __APPLE__
#include <Ogre/OgreMemoryAllocatorConfig.h>
#else
#include <OgreMemoryAllocatorConfig.h>
#endif

using namespace Ogre;
using namespace std;

namespace rl
{

    ConfigFile::ConfigFile()
        : Ogre::ConfigFile()
    {
    }

    void ConfigFile::save(const String& filename, const String& separators, bool trimWhitespace)
    {
        saveDirect(filename, separators, trimWhitespace);
    }

    void ConfigFile::save(
        const String& filename, const String& resourceGroup, const String& separators, bool trimWhitespace)
    {
        saveToResourceSystem(filename, resourceGroup, separators, trimWhitespace);
    }

    void ConfigFile::saveDirect(const String& filename, const String& separators, bool trimWhitespace)
    {
        /* Open the configuration file */
        ofstream fp;
        fp.open(filename.c_str());
        if (!fp)
        {
            Throw(FileNotFoundException, "'" + filename + "' file not found!");
        }

        save(fp, separators, trimWhitespace);
    }

    void ConfigFile::saveToResourceSystem(
        const String& filename, const String& resourceGroup, const String& separators, bool trimWhitespace)
    {
        ///@todo ResourceGroup zum Dateiname erstellen.
        ofstream fp;
        fp.open(filename.c_str());
        if (!fp)
        {
            Throw(FileNotFoundException, "'" + filename + "' file not found!");
        }

        save(fp, separators, trimWhitespace);
    }

    void ConfigFile::save(ofstream& stream, const String& separators, bool trimWhitespace)
    {
        for (const auto& section : mSettings)
        {
            if (section.first != Ogre::BLANKSTRING)
            {
                stream << "[" << section.first << "]" << endl;
            }
            else
            {
                /// Keine Sektion
            }

            for (const auto& setting : section.second)
            {
                stream << setting.first << "=" << setting.second << endl;
            }

            // Insert a linebreak
            stream << endl;
        }
    }

    void ConfigFile::addSection(const Ogre::String& section, const Ogre::NameValuePairList& settings)
    {
        // Create new section
        mSettings[section] = {};
        // Insert values from the settings list
        mSettings[section].insert(settings.begin(), settings.end());
    }
}
