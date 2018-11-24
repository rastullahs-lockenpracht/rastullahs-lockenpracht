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

/**
 * \file ConfigurationManager.h
 *
 * Declaration of the ConfigurationManager subsystem
 */

#ifndef __RL_CONFIGURATIONMANAGER_H__
#define __RL_CONFIGURATIONMANAGER_H__

#include "ConfigFile.h"
#include "CorePrerequisites.h"

#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

namespace rl
{
    /**
     * The ConfigurationManager subsystem class
     *
     * It provides a convenient way for other subsystems to access values read from the
     * configuration files. If no configuration file is available, sane default values are
     * provided so that Rastullah will work properly on most systems.
     */
    class _RlCoreExport ConfigurationManager : public Ogre::Singleton<ConfigurationManager>
    {
    public:
        ~ConfigurationManager();

        /**
         * Returns the filename for the log written by Ogre
         *
         * @return filename of Ogre's logfile
         */
        Ogre::String getOgreLogFile() const;

        /**
         * Returns the filename for the log written by Rastullah
         *
         * @return filename of Rastullah's logfile
         */
        Ogre::String getRastullahLogFile() const;

        /**
         * Returns the filename for the log written by CEGUI
         *
         * @return filename of CEGUI's logfile
         */
        Ogre::String getCeguiLogFile() const;

        /**
         * Returns a list of paths, were Rastullah configuration files
         * reside
         *
         * The list is seperated via the : character and has to be
         * processed, before usage
         *
         * @return colon seperated path of Rastullah configuration
         *         directories
         */
        Ogre::String getRastullahCfgPath() const;

        /**
         * Returns the root of the Rastullah modules directory
         *
         * @return directory where Rastullah modules reside
         */
        Ogre::String getModulesRootDirectory() const;

        /**
         * Returns the directory where Rastullah stores all its log files
         *
         * @return directoy where Rastullah logfiles are stored
         */
        Ogre::String getRastullahLogDirectory() const;

        /**
         * Adds a key-value pair to the list specified by section
         *
         * If the key already exists in the list, its value will be updated
         *
         * @param section The section where the key-value pair will be inserted
         * @param key The key of the key-value pair
         * @param value The value of the key-value pair
         */
        void addSetting(const Ogre::String& section, const Ogre::String& key, const Ogre::String& value);

        /**
         * Returns the string value for a specific key in a specific section
         *
         * @param section The sections key-value list, which is searched
         * @param key The key, for which a value will be returned
         * @return string value for the specific key in the specific section
         */
        Ogre::String getStringSetting(const Ogre::String& section, const Ogre::String& key) const;

        /**
         * Returns the int value for a specific key in a specific section
         *
         * @param section The sections key-value list, which is searched
         * @param key The key, for which a value will be returned
         * @return int value for the specific key in the specific section
         */
        int getIntSetting(const Ogre::String& section, const Ogre::String& key) const;

        /**
         * Returns the boolean value for a specific key in a specific section
         *
         * @param section The sections key-value list, which is searched
         * @param key The key, for which a value will be returned
         * @return boolean value for the specific key in the specific section
         */
        bool getBoolSetting(const Ogre::String& section, const Ogre::String& key) const;

        /**
         * Returns the real value for a specific key in a specific section
         *
         * @param section The sections key-value list, which is searched
         * @param key The key, for which a value will be returned
         * @return real value for the specific key in the specific section
         */
        Ogre::Real getRealSetting(const Ogre::String& section, const Ogre::String& key) const;

        /**
         * Returns a pointer to the settings of the named section.
         * @param section name of the section to get settings of.
         * @return key-value list of graphic settings
         */
        Ogre::NameValuePairList getSettings(const Ogre::String& section) const;

        /**
         * Returns a pointer to the video settings
         * @return key-value list of graphic settings
         */
        Ogre::NameValuePairList* getVideoSettings() const;

        /**
         * Returns a list of Ogre plugins specific to the OS
         *
         * @return vector of plugin names
         */
        Ogre::StringVector getPluginList() const;

        /**
         * Returns the list of Rastullah game modules
         *
         * @return vector of module names
         */
        Ogre::StringVector getModuleList() const;

        /**
         * Remove modules from the module list (e.g. because they are broken)
         *
         * @param modules the modules to remove
         */
        void removeModules(const Ogre::StringVector& modules);

        /**
         * Returns the filename of the current keymap file used by
         * Rastullah
         *
         * @return filename of keymap file
         */
        Ogre::String getKeymap() const;

        /**
         * Returns the log level specifying how verbose the created
         * logfiles are
         *
         * @return verbosity level of logfiles
         */
        Logger::LogLevel getLogLevel() const;

        /**
         * Loads configuration files from the configuration directories
         *
         * Loads up to four configuration files (systemwide, user, current
         * directory) and sets certain settings for the game
         */
        void loadConfig();

        /**
         * Loads module configuration files
         *
         * NOTE: run loadConfig() beforehand to get the path to that configuration file
         */
        void loadModulesConfig();

        /**
         * Saves a configuration file in OS specific directory
         */
        void saveConfig() const;

        /**
         * Extract the path of the started RL executable
         */
        void setExecutable(const Ogre::String& executableFile);

        /**
         * Set the path of the started RL executable
         */
        const Ogre::String& getExecutablePath() const;

    private:
        /**
         * Constructs a new ConfigurationManager object.
         *
         * Sets the name for the Ogre, Rastullah and CEGUI logfiles
         * and the Rastullah configuration file. The path
         * where configuration files might be reside is also set.
         */
        ConfigurationManager();

        /**
         * Sets the path where configuration files may reside
         * based on the operating system.
         */
        void setRastullahCfgPath();

        /**
         * Adds the specified path to the Rastullah configuration path
         * variable.
         *
         * All values are seperated via a colon.
         *
         * @param cfg_dir Directory where a configuration file may reside
         */
        void addToCfgPath(const std::string& cfg_dir);

        /**
         * Adds a plugin to the plugin list, correctly adds _d for Windows if compiled
         * in debug mode
         *
         * @param plugin the plugin to add (name without _d or file extension)
         * @param pluginDir the directory in which the plugin dll is located
         */
        void addPlugin(const Ogre::String& plugin, const Ogre::String& pluginDir);

        /**
         * Check if a specified file exists.
         *
         * @param filename File being checked for existence
         * @return true if file exists, false otherwise
         */
        bool checkForFile(const std::string& filename);

        /**
         * Searches for a specific key in a specific section
         *
         * @param section The sections key-value list, which is searched
         * @param key The key, for which a value will be returned
         * @return value if key exits, otherwise NULL
         */
        Ogre::String findSetting(const Ogre::String& section, const Ogre::String& key) const;

        Ogre::String mExecutablePath; //!< Filename of the running RL executable
        Ogre::String mRastullahCfgFile; //!< Filename of the Rastullah configuration file
        Ogre::String mOgreLogFile; //!< Filename of the logfile written by OGRE
        Ogre::String mRastullahLogFile; //!< Filename of the logfile written by Rastullah
        Ogre::String mCeguiLogFile; //!< Filename of the logfile written by CEGUI
        Ogre::String mModulesCfgFile; //!< Filename of the Module configuration file
        Ogre::String mModulesRootDirectory; //!< Directory where Rastullah's modules reside
        Ogre::String mRastullahLogDirectory; //!< Directory where all logfiles are stored
        Ogre::String mRastullahCfgPath; //!< List of directories containing Rastullah configuration files
        Ogre::String mOgrePluginDirectory; //!< Directory where OGRE stores its plugins
        Ogre::StringVector mModuleList; //!< List of Rastullah game modules
        Ogre::StringVector mPluginList; //!< List of OGRE Plugins, which will be loaded by CoreSubsystem

        typedef std::map<Ogre::String, Ogre::NameValuePairList> SectionMap;
        /// Stores settings per section in a NameValuePairList
        SectionMap mSettings;
    };
}

#endif
