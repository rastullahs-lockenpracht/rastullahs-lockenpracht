/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
 * Definiert die abstrakte Basisklasse für das Konfigurationsmanagement.
 */

#ifndef __RL_CONFIGURATIONMANAGER_H__
#define __RL_CONFIGURATIONMANAGER_H__

#include "CorePrerequisites.h"
#include "ConfigFile.h"

#include <OgreSingleton.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

namespace fs = boost::filesystem;

namespace rl
{
    class _RlCoreExport ConfigurationManager : protected Ogre::Singleton<ConfigurationManager>
    {
        public:
            static Ogre::String PathDelimiter;

            /// Sections where settings can be queried from
            enum ConfigurationSection
            {
                CS_GENERAL = 1,
                CS_GRAPHICS,
                CS_SOUND,
                CS_INPUT
            };

            ~ConfigurationManager();

            /**
             * Returns a pointer to the single instance of the
             * ConfigurationManager.
             *
             * The ConfigurationManager object is created, if it does not
             * exist.
             *
             * @return Pointer to ConfigurationManager object
             */
            static ConfigurationManager* getSingletonPtr();

            /**
             * Returns a reference to the single instance of the
             * ConfigurationManager.
             *
             * The ConfigurationManager object is created, if it does not
             * exist.
             *
             * @return Reference to ConfigurationManager object
             */
            static ConfigurationManager& getSingleton();

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
             * Returns the filename of the configuration file for the modules
             *
             * @return filename of module's configuration file
             */
            Ogre::String getModulesCfgFile() const;

            /**
             * Returns the filename of the configuration file for the OS
             * specific plugins
             *
             * @return filename of the plugin's configuration file
             */
            Ogre::String getPluginCfgFile() const;

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
             * Returns the directory where OGRE stores its plugins
             *
             * @return directory where OGRE plugins are stored
             */
            Ogre::String getOgrePluginDirectory() const;

            /**
             * Adds a key-value pair to the list specified by section
             *
             * If the key already exists in the list, its value will be updated
             *
             * @param section The section where the key-value pair will be inserted
             * @param key The key of the key-value pair
             * @param value The value of the key-value pair
             */
            void addSetting(ConfigurationSection section, Ogre::String key, Ogre::String value);

            /**
             * Returns the string value for a specific key in a specific section
             *
             * @param section The sections key-value list, which is searched
             * @param key The key, for which a value will be returned
             * @return string value for the specific key in the specific section
             */
            Ogre::String getStringSetting(ConfigurationSection section, Ogre::String key);

            /**
             * Returns the int value for a specific key in a specific section
             *
             * @param section The sections key-value list, which is searched
             * @param key The key, for which a value will be returned
             * @return int value for the specific key in the specific section
             */
            int getIntSetting(ConfigurationSection section, Ogre::String key);

            /**
             * Returns the boolean value for a specific key in a specific section
             *
             * @param section The sections key-value list, which is searched
             * @param key The key, for which a value will be returned
             * @return boolean value for the specific key in the specific section
             */
            bool getBoolSetting(ConfigurationSection section, Ogre::String key);

            /**
             * Returns the real value for a specific key in a specific section
             *
             * @param section The sections key-value list, which is searched
             * @param key The key, for which a value will be returned
             * @return real value for the specific key in the specific section
             */
            Ogre::Real getRealSetting(ConfigurationSection section, Ogre::String key);

            /**
             * Returns a pointer to the graphical settings for the OGRE renderer
             *
             * @return key-value list of graphic settings
             */
            Ogre::NameValuePairList getGraphicSettings() const;

            /**
             * Returns a pointer to the sound settings for the Rl::SoundManager
             *
             * @return key-value list of sound settings
             */
            Ogre::NameValuePairList getSoundSettings() const;

            /**
             * Returns the filename of the current keymap file used by
             * Rastullah
             *
             * @return filename of keymap file
             */
            Ogre::String getKeymap() const;

            /**
             * Returns the input settings for the CommandMapper
             *
             * @return key-value list of input settings
             */
            Ogre::NameValuePairList getInputSettings() const;

            /**
             * Returns the log level specifying how verbose the created
             * logfiles are
             *
             * @return verbosity level of logfiles
             */
            Logger::LogLevel getLogLevel();

            Ogre::String getEngineVersionString() const;
            Ogre::String getEngineVersionName() const;
            long         getEngineBuildNumber() const;

            /**
             * Returns the text displayed in the "About Application" window
             *
             * @return text displayed in "About Application" window
             */
            const rl::CeGuiString& getAboutText() const;

            /**
             * Loads configuration files from the configuration directories
             *
             * Loads up to four configuration files (systemwide, user, current
             * directory, module) and sets certain settings for the game
             */
            void loadConfig();

            /**
             * Saves a configuration file in OS specific directory
             */
            void saveConfig();

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
            Ogre::String findSetting(ConfigurationSection section, Ogre::String key) const;

            Ogre::String mRastullahCfgFile;           //!< Filename of the Rastullah configuration file
            Ogre::String mOgreLogFile;                //!< Filename of the logfile written by OGRE
            Ogre::String mRastullahLogFile;           //!< Filename of the logfile written by Rastullah
            Ogre::String mCeguiLogFile;               //!< Filename of the logfile written by CEGUI
            Ogre::String mModulesCfgFile;             //!< Filename of the Module configuration file
            Ogre::String mModulesRootDirectory;       //!< Directory where Rastullah's modules reside
            Ogre::String mRastullahLogDirectory;      //!< Directory where all logfiles are stored
            Ogre::String mRastullahCfgPath;           //!< List of directories containing Rastullah configuration files
            Ogre::String mOgrePluginDirectory;        //!< Directory where OGRE stores its plugins
            Ogre::NameValuePairList mGeneralSettings; //!< General settings for Rastullah
            Ogre::NameValuePairList mGraphicSettings; //!< Graphic settings for the OGRE Renderer
            Ogre::NameValuePairList mSoundSettings;   //!< Sound settings for the Rastullah soundsystem
            Ogre::NameValuePairList mInputSettings;   //!< Input settings for the Rastullah inputsystem
    };
}

#endif
