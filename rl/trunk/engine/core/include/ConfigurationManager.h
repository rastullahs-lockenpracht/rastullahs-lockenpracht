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

#ifndef __RL_CONFIGURATIONMANAGER_H__
#define __RL_CONFIGURATIONMANAGER_H__

#include "CorePrerequisites.h"
/**
 * \file ConfigurationManager.h
 * Definiert die abstrakte Basisklasse für das Konfigurationsmanagement.
 */

#include <OgreConfigFile.h>
#include <OgreSingleton.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

namespace fs = boost::filesystem;

namespace rl
{
	class _RlCoreExport ConfigurationManager : protected Ogre::Singleton<ConfigurationManager>
	{
		public:
			/**
			 * Constructs a new ConfigurationManager object.
			 *
			 * Sets the name for the Ogre and Rastullah logfiles
			 * and the Rastullah configuration file. The path
			 * where configuration files might be reside is also set.
			 */
            ConfigurationManager();

			~ConfigurationManager();

			/**
			 * Returns a pointer to the single instance of the
			 * ConfigurationManager.
			 *
			 * The ConfigurationManager object is created, if it does not
			 * exist.
			 * @return Pointer to ConfigurationManager object
			 */
			static ConfigurationManager* getSingletonPtr();

			/**
			 * Returns a reference to the single instance of the
			 * ConfigurationManager.
			 *
			 * The ConfigurationManager object is created, if it does not
			 * exist.
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
			 * Returns the filename of the current keymap file used by
			 * Rastullah
			 *
			 * @return filename of keymap file
			 */
			Ogre::String getKeymap() const;

			/**
			 * @todo Remove, after other subsystems were changed to
			 *       new ConfigurationManager style
			 */
			Ogre::String getPluginCfgPath() const;
			Ogre::String getRastullahLogPath() const;
			Ogre::String getModulesCfgPath() const;
			Ogre::String getConfigModulePath() const;
            Ogre::String getRastullahSystemCfgPath() const;
			Ogre::String getSoundCfgPath() const;
			Ogre::String getInputConfigPath() const;

			Ogre::String getStringSetting(const Ogre::String& name, const Ogre::String& _default = Ogre::StringUtil::BLANK) const;
			Ogre::Real getRealSetting(const Ogre::String& name, Ogre::Real _default = 0.0f) const;
			int getIntSetting(const Ogre::String& name, int _default = 0) const;
			bool getBoolSetting(const Ogre::String& name, bool _default = false) const;

            Logger::LogLevel getLogLevel() const;

			Ogre::String getEngineVersionString() const;
			Ogre::String getEngineVersionName() const;
			long         getEngineBuildNumber() const;
			const rl::CeGuiString&  getAboutText() const;

			bool shouldUseStaticGeometry() const;
			Ogre::String getTextureUnitScheme() const;

			/**
			 * Loads configuration files from the configuration directories
			 *
			 * Loads up to four configuration files (systemwide, user, current
			 * directory, module) and sets certain settings for the game
			 */
            void loadConfig();

		protected:
			Ogre::String mRastullahCfgFile;
			Ogre::String mOgreLogFile;
			Ogre::String mRastullahLogFile;
			Ogre::String mCeguiLogFile;
			Ogre::String mSoundCfgFile;
			Ogre::String mModulesCfgFile;
			Ogre::String mPluginCfgFile;

			Ogre::String mModulesRootDirectory;
			Ogre::String mRastullahLogDirectory;
			Ogre::String mModulesCfgDirectory;
			Ogre::String mRastullahCfgPath;

			/**
			 * @todo Remove, after other subsystems were changed to
			 *       new ConfigurationManager style
			 */
			Ogre::String mPluginCfgPath;
			Ogre::String mModulesCfgPath;
			Ogre::String mConfigModuleDirectory;
            Ogre::String mRastullahSystemCfgPath;
			Ogre::String mSoundCfgPath;
			Ogre::String mRastullahOgreCfgPath;

        private:
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

            Ogre::ConfigFile* mSystemConfig;
	};
}

#endif
