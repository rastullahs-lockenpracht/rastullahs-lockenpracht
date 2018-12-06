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

#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include "CommonPrerequisites.h"

namespace rl
{
    class _RlCommonExport ConfigFile : public Ogre::ConfigFile
    {
    public:
        /**
         * Default constructor
         *
         * Calls parent constructor
         */
        ConfigFile();

        /**
         * Wrapper function for saveDirect
         *
         * @param filename Filename of the configuration file
         * @param separators Inserted between key and value
         * @param trimWhitespace Should leading and trailing whitespace be removed?
         */
        void save(const Ogre::String& filename, const Ogre::String& separators = "\t:=", bool trimWhitespace = true);

        /**
         * Wrapper function for saveToResourceSystem
         *
         * @param filename Filename of the configuration file
         * @param separators Inserted between key and value
         * @param trimWhitespace Should leading and trailing whitespace be removed?
         */
        void save(const Ogre::String& filename, const Ogre::String& resourceGroup,
            const Ogre::String& separators = "\t:=", bool trimWhitespace = true);

        /**
         * Save to a stream
         *
         * The function, which actually save the contents of the ConfigFile
         * object.
         *
         * @param stream Data stream
         * @param separators Inserted between key and value
         * @param trimWhitespace Should leading and trailing whitespace be removed?
         */
        void save(std::ofstream& stream, const Ogre::String& separators = "\t:=", bool trimWhitespace = true);

        /**
         * Save to a filename (not using resource group locations)
         *
         * Opens a filehandle to filename and calls the stream save function.
         *
         * @param filename Filename of the configuration file
         * @param separators Inserted between key and value
         * @param trimWhitespace Should leading and trailing whitespace be removed?
         */
        void saveDirect(
            const Ogre::String& filename, const Ogre::String& separators = "\t:=", bool trimWhitespace = true);

        /**
         * Save to a filename (using resource group locations)
         *
         * Creates the resource group if needed, opens a filehandle to filename
         * and calls the stream save function.
         *
         * @param filename Filename of the configuration file
         * @param separators Inserted between key and value
         * @param trimWhitespace Should leading and trailing whitespace be removed?
         */
        void saveToResourceSystem(const Ogre::String& filename, const Ogre::String& resourceGroup,
            const Ogre::String& separators = "\t:=", bool trimWhitespace = true);

        /**
         * Adds a section to the config file and adds a list of key-value pairs
         *
         * @param section The name of the section, where the key-value pairs are written to
         * @param settings The list of key-value pairs
         */
        void addSection(const Ogre::String& section, const Ogre::NameValuePairList& settings);
    };
}

#endif /*CONFIGFILE_H_*/
